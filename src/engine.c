/** @file
    Interface of game engine.
    File makes biggest work in program. Is's responsible to do function from input, start, control and end game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "parse.h"
#include "engine.h"

/**
 * Metrics of distance using as condition.
 */
const int metrics = 8;
/**
 * Maximum possible board size.
 */
const int MAX_BOARD_SIZE = INT_MAX;

bool start_game(player *p1, player *p2, int *number_of_rounds, int *board_size, char board[10][10], int *ai_number) {

    bool tmp_bool = true;
    command *init1;
    init1 = malloc(sizeof(command));

    init1 = parse_command(init1, &tmp_bool);
    if (find_function(init1->name) != 1)
        tmp_bool = false;

    if (!(init1->data[0] > 8 && init1->data[0] <= MAX_BOARD_SIZE &&
          init1->data[1] >= 1 && init1->data[1] <= MAX_BOARD_SIZE &&
          (init1->data[2] == 1 || init1->data[2] == 2) &&
          init1->data[3] > 0 && init1->data[3] <= init1->data[0] - 3 &&
          init1->data[4] > 0 && init1->data[4] <= init1->data[0] &&
          init1->data[5] > 0 && init1->data[5] <= init1->data[0] - 3 &&
          init1->data[6] > 0 && init1->data[6] <= init1->data[0]) && tmp_bool
            )
        tmp_bool = false;

    /*wrong kings positions*/;
    if (abs(init1->data[3] - init1->data[5]) < metrics && abs(init1->data[4] - init1->data[6]) < metrics){
        tmp_bool = false;
    }

    if (tmp_bool) {
        init_topleft(init1->data[3], init1->data[4], init1->data[5], init1->data[6], board);

        if(init1->data[2] == 1)
            *ai_number = 1;
        else
            *ai_number = 2;

        /*we checked that init1->data[2] != init2->data[2] && init{1,2}->data[2] = {1,2}*/
        p1->number = first;
        p2->number = second;

        *board_size = init1->data[0];
        *number_of_rounds = init1->data[1];


        make_player(p1, init1->data[3], init1->data[4]);
        make_player(p2, init1->data[5], init1->data[6]);

    }
    else {
        tmp_bool = false;
    }

    free(init1);
    return tmp_bool;
}

void make_player(player *p, int x, int y) {

    p->list = NULL;

    /*dummy usefull to delete last element from list without information about previous*/
    add_figure(p, KING, 0, 0, 0);
    add_figure(p, KNIGHT, x + 3, y, 0);
    add_figure(p, KNIGHT, x + 2, y, 0);
    add_figure(p, PEASANT, x + 1, y, 0);
    add_figure(p, KING, x, y, 0);
    add_figure(p, KING, 0, 0, 0);/*sentinel*/
}

void init_topleft(int x1, int y1, int x2, int y2, char board[10][10]) {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = '.';
        }
    }

    if (x1 <= 10 && y1 <= 10) {
        board[x1 - 1][y1 - 1] = 'K';
        if (x1 + 1 <= 10)
            board[x1][y1 - 1] = 'C';
        if (x1 + 2 <= 10)
            board[x1 + 1][y1 - 1] = 'R';
        if (x1 + 3 <= 10)
            board[x1 + 2][y1 - 1] = 'R';
    }

    if (x2 <= 10 && y2 <= 10) {
        board[x2 - 1][y2 - 1] = 'k';
        if (x2 + 1 <= 10)
            board[x2][y2 - 1] = 'c';
        if (x2 + 2 <= 10)
            board[x2 + 1][y2 - 1] = 'r';
        if (x2 + 3 <= 10)
            board[x2 + 2][y2 - 1] = 'r';
    }
}

void add_to_topleft(int x, int y, char c, char top_left[10][10]) {
    if (x <= 10 && y <= 10) {
        top_left[x - 1][y - 1] = c;
    }
}

void remove_from_topleft(int x, int y, char top_left[10][10]) {
    if (x <= 10 && y <= 10) {
        top_left[x - 1][y - 1] = '.';
    }
}

char find_symbol(enum player_number number, enum figure_type f) {
    if (number == first) {
        switch (f) {
            case PEASANT:
                return 'C';
            case KNIGHT:
                return 'R';
            case KING:
                return 'K';
        }
    }
    else {
        switch (f) {
            case PEASANT:
                return 'c';
            case KNIGHT:
                return 'r';
            case KING:
                return 'k';
        }
    }

    return '.';
}

void print_topleft(char board[10][10], int board_size) {

    for (int i = 0; i < board_size && i < 10; i++) {
        for (int j = 0; j < board_size && j < 10; j++) {
            printf("%c", board[j][i]);
        }
        printf("\n");
    }
}

void init(game *g, player *player1, player *player2, int number_of_rounds, int board_size) {
    g->player1 = *player1;
    g->player2 = *player2;
    g->board_size = board_size;
    g->current_round = 1;
    g->number_of_rounds = number_of_rounds;
    g->current_palyer = first;
    g->game_state = during_game;
}

int is_field_empty(player player1, player player2, int x, int y) {

    figures_list *tmp = player1.list;
    while (tmp != NULL) {
        if (tmp->our_figure.x == x && tmp->our_figure.y == y) {
            return 1;
        }
        tmp = tmp->next;
    }

    tmp = player2.list;
    while (tmp != NULL) {
        if (tmp->our_figure.x == x && tmp->our_figure.y == y) {
            return -1;
        }
        tmp = tmp->next;
    }
    return 0;
}

void remove_figure_from_list(figures_list *list, player p) {

    figures_list *tmp = p.list;
    while (tmp->next != NULL && tmp->next != list)
        tmp = tmp->next;

    if (tmp->next != NULL) {
        tmp->next = list->next;
        free(list);
    }
    else {
        printf("cos zle\n");
    }
}

int fight(enum figure_type figure1, enum figure_type figure2) {

    if (figure1 == KING && figure2 == KING)
        return 1;
    if (figure1 == KNIGHT && figure2 == KING)
        return 2;
    if (figure1 == KING && figure2 == KNIGHT)
        return 3;
    if (figure1 == KNIGHT && figure2 == KNIGHT)
        return 4;
    if (figure1 == PEASANT && figure2 == PEASANT)
        return 5;
    if (figure1 == KNIGHT && figure2 == PEASANT)
        return 6;
    if (figure1 == KING && figure2 == PEASANT)
        return 7;
    if (figure1 == PEASANT && figure2 == KING)
        return 8;
    if (figure1 == PEASANT && figure2 == KNIGHT)
        return 9;

    return 0;
}

bool move(int x1, int y1, int x2, int y2, game *g, char top_left[10][10]) {

    figures_list *tmp;
    if (g->current_palyer == first) {
        tmp = g->player1.list;

        while (tmp != NULL && (tmp->our_figure.x != x1 || tmp->our_figure.y != y1)) {
            tmp = tmp->next;
        }

        int field_value = is_field_empty(g->player1, g->player2, x2, y2);
        if (tmp != NULL && tmp->our_figure.last_move_in_round < g->current_round && field_value <= 0 &&
            abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && x1 <= g->board_size &&
            y1 <= g->board_size && x2 <= g->board_size && y2 <= g->board_size) {
            if (field_value == 0) {
                tmp->our_figure.x = x2;
                tmp->our_figure.y = y2;
                tmp->our_figure.last_move_in_round = g->current_round;

                remove_from_topleft(x1, y1, top_left);
                char c = find_symbol(tmp->our_figure.player, tmp->our_figure.type);
                add_to_topleft(x2, y2, c, top_left);

            }
                /*else field isn't empty*/
            else {
                figures_list *tmp2 = g->player2.list;

                while (tmp2 != NULL && (tmp2->our_figure.x != x2 || tmp2->our_figure.y != y2)) {
                    tmp2 = tmp2->next;
                }

                if (tmp2 != NULL) {

                    int f = fight(tmp->our_figure.type, tmp2->our_figure.type);
                    char c;
                    switch (f) {
                        case 1:
                            remove_from_topleft(x1, y1, top_left);
                            remove_from_topleft(x2, y2, top_left);
                            remove_figure_from_list(tmp, g->player1);
                            remove_figure_from_list(tmp2, g->player2);
                            g->game_state = draw;
                            break;
                        case 2:
                            remove_from_topleft(x1, y1, top_left);
                            add_to_topleft(x2, y2, 'R', top_left);
                            remove_figure_from_list(tmp2, g->player2);
                            tmp->our_figure.x = x2;
                            tmp->our_figure.y = y2;
                            tmp->our_figure.last_move_in_round = g->current_round;
                            g->game_state = won1;
                            break;
                        case 3:
                            remove_from_topleft(x1, y1, top_left);
                            remove_figure_from_list(tmp, g->player1);
                            g->game_state = won2;
                            break;
                        case 4:
                        case 5:
                            remove_from_topleft(x1, y1, top_left);
                            remove_from_topleft(x2, y2, top_left);
                            remove_figure_from_list(tmp, g->player1);
                            remove_figure_from_list(tmp2, g->player2);
                            break;
                        case 6:
                        case 7:
                            remove_from_topleft(x1, y1, top_left);
                            c = find_symbol(first, tmp->our_figure.type);
                            add_to_topleft(x2, y2, c, top_left);
                            tmp->our_figure.x = x2;
                            tmp->our_figure.y = y2;
                            tmp->our_figure.last_move_in_round = g->current_round;
                            remove_figure_from_list(tmp2, g->player2);
                            break;
                        case 8:
                        case 9:
                            remove_from_topleft(x1, y1, top_left);
                            remove_figure_from_list(tmp, g->player1);
                            break;
                        default:
                            printf("move error\n");
                    }
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    else {/*current_player == second*/
        tmp = g->player2.list;

        while (tmp != NULL && (tmp->our_figure.x != x1 || tmp->our_figure.y != y1)) {
            tmp = tmp->next;
        }

        int field_value = is_field_empty(g->player2, g->player1, x2, y2);
        if (tmp != NULL && tmp->our_figure.last_move_in_round < g->current_round && field_value <= 0 &&
            abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && x1 <= g->board_size &&
            y1 <= g->board_size && x2 <= g->board_size && y2 <= g->board_size) {
            if (field_value == 0) {
                tmp->our_figure.x = x2;
                tmp->our_figure.y = y2;
                tmp->our_figure.last_move_in_round = g->current_round;

                remove_from_topleft(x1, y1, top_left);
                char c = find_symbol(tmp->our_figure.player, tmp->our_figure.type);
                add_to_topleft(x2, y2, c, top_left);

            }
                /*else field isn't empty*/
            else {
                figures_list *tmp2 = g->player1.list;

                while (tmp2 != NULL && (tmp2->our_figure.x != x2 || tmp2->our_figure.y != y2)) {
                    tmp2 = tmp2->next;
                }

                if (tmp2 != NULL) {

                    int f = fight(tmp->our_figure.type, tmp2->our_figure.type);
                    char c;
                    switch (f) {
                        case 1:
                            remove_from_topleft(x1, y1, top_left);
                            remove_from_topleft(x2, y2, top_left);
                            remove_figure_from_list(tmp, g->player2);
                            remove_figure_from_list(tmp2, g->player1);
                            g->game_state = draw;
                            break;
                        case 2:
                            remove_from_topleft(x1, y1, top_left);
                            add_to_topleft(x2, y2, 'r', top_left);
                            remove_figure_from_list(tmp2, g->player1);
                            tmp->our_figure.x = x2;
                            tmp->our_figure.y = y2;
                            tmp->our_figure.last_move_in_round = g->current_round;
                            g->game_state = won2;
                            break;
                        case 3:
                            remove_from_topleft(x1, y1, top_left);
                            remove_figure_from_list(tmp, g->player2);
                            g->game_state = won1;
                            break;
                        case 4:
                        case 5:
                            remove_from_topleft(x1, y1, top_left);
                            remove_from_topleft(x2, y2, top_left);
                            remove_figure_from_list(tmp, g->player2);
                            remove_figure_from_list(tmp2, g->player1);
                            break;
                        case 6:
                        case 7:
                            remove_from_topleft(x1, y1, top_left);
                            c = find_symbol(second, tmp->our_figure.type);
                            add_to_topleft(x2, y2, c, top_left);
                            tmp->our_figure.x = x2;
                            tmp->our_figure.y = y2;
                            tmp->our_figure.last_move_in_round = g->current_round;
                            remove_figure_from_list(tmp2, g->player1);
                            break;
                        case 8:
                        case 9:
                            remove_from_topleft(x1, y1, top_left);
                            remove_figure_from_list(tmp, g->player2);
                            break;
                        default:
                            printf("move error\n");
                    }
                }
                else {
                    return false;
                }
            }
        }
        else {
            return false;
        }
    }
    return true;
}

bool produce_knight(int x1, int y1, int x2, int y2, game *g, char top_left[10][10]) {

    figures_list *tmp;
    int i;

    if (g->current_palyer == first) {
        tmp = g->player1.list;

        while (tmp != NULL && tmp->our_figure.x != x1 &&
               tmp->our_figure.y != y1) {
            tmp = tmp->next;
        }

        i = is_field_empty(g->player1, g->player2, x2, y2);

        if (tmp != NULL && abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && i == 0 &&
            (g->current_round - tmp->our_figure.last_move_in_round) > 2 && x2 > 0 && x2 <= g->board_size && y2 > 0 &&
            y2 <= g->board_size) {
            add_figure_after_dummy(&g->player1, KNIGHT, x2, y2, g->current_round - 1);
            add_to_topleft(x2, y2, 'R', top_left);
        }
        else {
            return false;
        }
    }
    else {
        tmp = g->player2.list;

        while (tmp != NULL && tmp->our_figure.x != x1 && tmp->our_figure.y != y1) {
            tmp = tmp->next;
        }

        i = is_field_empty(g->player2, g->player1, x2, y2);

        if (tmp != NULL && abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && i == 0 &&
            (g->current_round - tmp->our_figure.last_move_in_round) > 2 &&
            (g->current_round - tmp->our_figure.last_move_in_round) > 2 && x2 > 0 && x2 <= g->board_size && y2 > 0 &&
            y2 <= g->board_size) {
            add_figure_after_dummy(&g->player2, KNIGHT, x2, y2, g->current_round - 1);
            add_to_topleft(x2, y2, 'r', top_left);
        }
        else {
            return false;
        }
    }
    return true;
}

bool produce_peasant(int x1, int y1, int x2, int y2, game *g, char top_left[10][10]) {

    figures_list *tmp;
    int i;

    if (g->current_palyer == first) {
        tmp = g->player1.list;

        while (tmp != NULL && tmp->our_figure.x != x1 && tmp->our_figure.y != y1) {
            tmp = tmp->next;
        }

        i = is_field_empty(g->player1, g->player2, x2, y2);

        if (tmp != NULL && abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && i == 0 &&
            (g->current_round - tmp->our_figure.last_move_in_round) > 2) {
            add_figure_after_dummy(&g->player1, PEASANT, x2, y2, g->current_round - 1);
            add_to_topleft(x2, y2, 'C', top_left);
        }
        else {
            return false;
        }
    }
    else {
        tmp = g->player2.list;

        while (tmp != NULL && tmp->our_figure.x != x1 && tmp->our_figure.y != y1) {
            tmp = tmp->next;
        }

        i = is_field_empty(g->player2, g->player1, x2, y2);

        if (tmp != NULL && abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1 && i == 0 &&
            (g->current_round - tmp->our_figure.last_move_in_round) > 2) {
            add_figure_after_dummy(&g->player2, PEASANT, x2, y2, g->current_round - 1);
            add_to_topleft(x2, y2, 'c', top_left);
        }
        else {
            return false;
        }
    }
    return true;
}

void end_turn(game *g) {
    if (g->current_palyer == first)
        g->current_palyer = second;
    else
        g->current_palyer = first;
}

void add_figure(player *p, enum figure_type type, int x, int y, int current_round) {
    figures_list *tmp;
    tmp = malloc(sizeof(figures_list));

    tmp->our_figure.type = type;
    tmp->our_figure.x = x;
    tmp->our_figure.y = y;
    tmp->our_figure.player = p->number;
    tmp->our_figure.last_move_in_round = current_round;

    tmp->next = p->list;
    p->list = tmp;
}

void add_figure_after_dummy(player *p, enum figure_type type, int x, int y, int current_round) {
    figures_list *tmp;
    tmp = malloc(sizeof(figures_list));

    tmp->our_figure.type = type;
    tmp->our_figure.x = x;
    tmp->our_figure.y = y;
    tmp->our_figure.player = p->number;
    tmp->our_figure.last_move_in_round = current_round;

    tmp->next = p->list->next;
    p->list->next = tmp;
}

void free_figure_list(figures_list *l) {

    figures_list *delete;
    while (l != NULL) {
        delete = l;
        l = l->next;
        free(delete);
    }
}

void end_game(game *g) {
    free_figure_list(g->player1.list);
    free_figure_list(g->player2.list);
}