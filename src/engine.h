/** @file
    Interface of game engine.
    File makes biggest work in program. Is's responsible to do function from input, start, control and end game.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

/**
 * Represent type of figure.
 * */
enum figure_type {
    KING, /*!< represent KING */
            KNIGHT, /*!< represent KNIGHT */
            PEASANT/*!< represent PEASANT */
};

/**
 * Represent player number in game where there are two players.
 * */
enum player_number {
    first, /*!< player who start the game */
            second/*!< second player */
};

/**
 * State of a game.
 * */
enum state {
    during_game, /*!< the game goes on */
            won1, /*!< first player win the game */
            won2, /*!< second player win the game */
            draw/*!< game end by draw */
};

/**
 *Contain all information about figure.
 * */
typedef struct def_figure {
    enum figure_type type;
    /*!< what is figure type */
    enum player_number player;
    /*!< to whom figure belong */
    int last_move_in_round;
    /*!< when figure take move last */
    int x;
    /*!< current coordinate */
    int y;/*!< current coordinate */
} figure;

/**
 * List of all figures belongs to one player.
 * */
typedef struct def_figures_list {
    figure our_figure;
    struct def_figures_list *next;
} figures_list;

/**
 * Information about one of the players.
 * */
typedef struct def_player {
    enum player_number number;
    struct def_figures_list *list;
} player;

/**
 * Contains all information about game.
 * */
typedef struct def_game {
    struct def_player player1;
    /*!< first player in game */
    struct def_player player2;
    /*!< second player in game */
    int number_of_rounds;
    /*!< max quantity of rounds that game can play */
    int board_size;
    /*!< size of game board */
    int current_round;
    /*!< number of current pound play in game */
    enum player_number current_palyer;
    /*!< which player now doing turn */
    enum state game_state;/*!< current state of game */
} game;

/**
 * Initializes a game. Make initial state of game. Initializes two players and game parameters.
 * @param p1 not initialized struct of first player which is fill in that function
 * @param p2 not initialized struct of second player which is fill in that function
 * @param number_of_rounds value keeps number maximum number of round in game
 * @param board_size size of a board in game
 * @param board keep top left corner of the board (in a established characters)
 * @return true if function was executed without anny input errors\n false if function find input error
 */
bool start_game(player *p1, player *p2, int *number_of_rounds, int *board_size, char board[10][10]);

/**
 * We know figures position only by KING position so we can add figures to list of figures of given player.
 * @param p player which we adds figures
 * @param x coordinate of KING
 * @param y coordinate of KING
 */
void make_player(player *p, int x, int y);

/**
 * Initialize top left corner of board as a two-dimensional array
 * @param x1 coordinate of KING belongs to first player
 * @param y1 coordinate of KING belongs to first player
 * @param x2 coordinate of KING belongs to second player
 * @param y2 coordinate of KING belongs to second player
 * @param board empty array which is fills in function
 * @warning function don't check if parameters are correct
 */
void init_topleft(int x1, int y1, int x2, int y2, char board[10][10]);

/**
 * Change value of one cell of array.
 * @param x coordinate of cell to change
 * @param y coordinate of cell to change
 * @param c new character which is put on cell
 * @param top_left board where we do change
 * @warning function don't check if parameters are correct
 */
void add_to_topleft(int x, int y, char c, char top_left[10][10]);

/**
 * Remove figure from array. Only change value of cell to '.' which represent empty field.
 * @param x coordinate of cell to change
 * @param y coordinate of cell to change
 * @param top_left board where we do change
 * @warning function don't check if parameters are correct
 */
void remove_from_topleft(int x, int y, char top_left[10][10]);

/**
 * Looking for graphic representation of given figure type.
 * @param number first and second players' figures have different representation so this parameter os necessary
 * @param f what figures' representation we are looking for
 * @return symbol of figure if data are correct and symbol of empty field '.' when function can't find given figure
 */
char find_symbol(enum player_number number, enum figure_type f);

/**
 * Prints (into stdout) top-left corner of the game board.
 * @param board array to print
 * @param board_size size of the board. Board can be smallet than 10 so we print min(board_size,10)
 */
void print_topleft(char board[10][10], int board_size);

/**
 * Initializes all arguments to game. Arguments which are the same for any games are fill too.
 * @param g structure is fill by the rest of arguments
 * @param player1 first player
 * @param player2 second player
 * @param number_of_rounds maximum number of rounds
 * @param board_size size of a board
 */
void init(game *g, player *player1, player *player2, int number_of_rounds, int board_size);

/**
 * Check what is on specific field
 * @param player1 first player, what give as list of his figures
 * @param player2 second player, what give as list of his figures
 * @param x coordinate of cell to check
 * @param y coordinate of cell to check
 * @return value = 1 if on the field is first player figure \n value = -1 if no the field is second player figure \n value = 0 if field is empty
 */
int is_field_empty(player player1, player player2, int x, int y);

/**
 * If figure is in player list of figures then it is removed.
 * @param list element of list to remove
 * @param p player who should have figure to remove
 */
void remove_figure_from_list(figures_list *list, player p);

/**
 * Give a value of fight between two figures as integer.
 * @param figure1 1 fighting figure
 * @param figure2 2 fighting figure
 * @return values returns in all possible fights ():\n
 * K - 1\n
 * R K - 2\n
 * K R - 3\n
 * R R - 4\n
 * C C - 5\n
 * R C - 6\n
 * K C - 7\n
 * C K - 8\n
 * C R - 9\n
 * where K - KING, R - KNIGHT, C - PEASANT
 */
int fight(enum figure_type figure1, enum figure_type figure2);

/**
 * Makes a move. And change values on a board[10][10].
 * @param x1 column number before a move
 * @param y1 row number before a move
 * @param x2 column number after a move
 * @param y2 row number before a move
 * @param g played game
 * @param top_left board which is changed in case of move
 * @return true if move is make correct\n
 * false if parameters are wrong
 */
bool move(int x1, int y1, int x2, int y2, game *g, char top_left[10][10]);

/**
 * Function make new figure (knight). Add it to players' list and change corner array.
 * @param x1 coordinate of making figure
 * @param y1 coordinate of making figure
 * @param x2 coordinate of new figure
 * @param y2 coordinate of new figure
 * @param g played game
 * @param top_left board which is changed in case of produce
 * @return true if move is make correct\n
 * false if parameters are wrong
 */
bool produce_knight(int x1, int y1, int x2, int y2, game *g, char top_left[10][10]);

/**
 * Function make new figure (peasant). Add it to players' list and change corner array.
 * @param x1 coordinate of making figure
 * @param y1 coordinate of making figure
 * @param x2 coordinate of new figure
 * @param y2 coordinate of new figure
 * @param g played game
 * @param top_left board which is changed in case of produce
 * @return true if move is make correct\n
 * false if parameters are wrong
 */
bool produce_peasant(int x1, int y1, int x2, int y2, game *g, char top_left[10][10]);

/**
 * End turn of player (not turn of game)!
 * @param g value of current_player is change
 */
void end_turn(game *g);

/**
 * Add first figure to figure list.
 * @param p player who have new figure
 * @param type type of new figure
 * @param x coordinate of new figure
 * @param y coordinate of new figure
 * @param current_round represent current round what became the figure last move round - 1 (we are treat new figure like moved in previous turn)
 * @warning function don't check if data are correct
 */
void add_figure(player *p, enum figure_type type, int x, int y, int current_round);

/**
 * Add following figure to figure list after dummy which is create in player figure list.
 * @param p player who have new figure
 * @param type type of new figure
 * @param x coordinate of new figure
 * @param y coordinate of new figure
 * @param current_round represent current round what became the figure last move round - 1 (we are treat new figure like moved in previous turn)
 * @warning function don't check if data are correct
 */
void add_figure_after_dummy(player *p, enum figure_type type, int x, int y, int current_round);//???

/**
 * Release memory allocated to figure list.
 * @param l list from this specific structure is released
 */
void free_figure_list(figures_list *l);

/**
 * Release memory allocated to players in game structure.
 * @param g struct where memory is release
 */
void end_game(game *g);

#endif /* ENGINE_H */