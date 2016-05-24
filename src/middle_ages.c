/** @file
   Main file in program. Combines all files and make game symulation.
   @author Mateusz Harciarek
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "engine.h"

int main() {

    bool is_no_inputerror = true;
    bool is_game_init_correct;
    int number_of_rounds, board_size;

    command *new_command;
    new_command = malloc(sizeof(command));


    player *player1;
    player *player2;
    player1 = malloc(sizeof(player));
    player2 = malloc(sizeof(player));

    game *g;
    g = malloc(sizeof(game));

    char top_left[10][10];
    is_no_inputerror = start_game(player1, player2, &number_of_rounds, &board_size, top_left);
    init(g, player1, player2, number_of_rounds, board_size);

    is_game_init_correct = is_no_inputerror;

    int function;

    while (is_no_inputerror && g->current_round <= g->number_of_rounds && g->game_state == during_game) {

        while (g->current_palyer == first && g->game_state == during_game && is_no_inputerror) {

            new_command = parse_command(new_command, &is_no_inputerror);
            function = find_function(new_command->name);

            if (is_no_inputerror) {
                switch (function) {
                    case 1:
                        is_no_inputerror = false;
                        break;
                    case 2:
                        is_no_inputerror = move(new_command->data[0], new_command->data[1], new_command->data[2],
                                                new_command->data[3], g,
                                                top_left);
                        if (is_no_inputerror) {
                            print_topleft(top_left, g->board_size);
                            printf("\n");
                        }
                        break;
                    case 3:
                        is_no_inputerror = produce_knight(new_command->data[0], new_command->data[1],
                                                          new_command->data[2],
                                                          new_command->data[3], g, top_left);
                        if (is_no_inputerror) {
                            print_topleft(top_left, g->board_size);
                            printf("\n");
                        }
                        break;
                    case 4:
                        is_no_inputerror = produce_peasant(new_command->data[0], new_command->data[1],
                                                           new_command->data[2],
                                                           new_command->data[3], g, top_left);
                        if (is_no_inputerror) {
                            print_topleft(top_left, g->board_size);
                            printf("\n");
                        }
                        break;
                    case 5:
                        end_turn(g);
                        break;
                    default:
                        is_no_inputerror = false;
                }
            }
        }

        while (g->current_palyer == second && g->game_state == during_game && is_no_inputerror) {

            new_command = parse_command(new_command, &is_no_inputerror);
            function = find_function(new_command->name);

            if (is_no_inputerror) {
                switch (function) {
                    case 1:
                        is_no_inputerror = false;
                        break;
                    case 2:
                        is_no_inputerror = move(new_command->data[0], new_command->data[1], new_command->data[2],
                                                new_command->data[3], g,
                                                top_left);
                        if (is_no_inputerror) {
                            print_topleft(top_left, g->board_size);
                            printf("\n");
                        }
                        break;
                    case 3:
                        is_no_inputerror = produce_knight(new_command->data[0], new_command->data[1],
                                                          new_command->data[2],
                                                          new_command->data[3], g, top_left);
                        if (is_no_inputerror) {
                            print_topleft(top_left, g->board_size);
                            printf("\n");
                        }
                        break;
                    case 4:
                        is_no_inputerror = produce_peasant(new_command->data[0], new_command->data[1],
                                                           new_command->data[2],
                                                           new_command->data[3], g, top_left);
                        if (is_no_inputerror) {
                            print_topleft(top_left, g->board_size);
                            printf("\n");
                        }
                        break;
                    case 5:
                        end_turn(g);
                        break;
                    default:
                        is_no_inputerror = false;
                }
            }
        }
        g->current_round++;
    }
    if (is_no_inputerror && g->game_state == won1)
        fprintf(stderr, "player 1 won\n");
    if (is_no_inputerror && g->game_state == won2)
        fprintf(stderr, "player 2 won\n");
    if (is_no_inputerror && (g->game_state == draw || g->game_state == during_game))
        fprintf(stderr, "draw\n");


    free(new_command);

    free(player1);
    free(player2);

    if(is_game_init_correct)
        end_game(g);

    free(g);

    if (is_no_inputerror)
        return 0;
    else {
        fprintf(stderr, "input error\n");
        exit(42);
    }
}