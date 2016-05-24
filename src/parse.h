/** @file
   Interface of parser.
   Parse lines from standard input and check if are they correct.
*/

#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>

/**
 * Contain command responsible to go function witch the same name.
 */
extern const char *INIT;
/**
 * Contain command responsible to go function witch the same name.
 */
extern char *MOVE;
/**
 * Contain command responsible to go function witch the same name.
 */
extern char *PRODUCE_KNIGHT;
/**
 * Contain command responsible to go function witch the same name.
 */
extern char *PRODUCE_PEASANT;
/**
 * Contain command responsible to go function witch the same name.
 */
extern char *END_TURN;

/**
 * Contain parsed line from input.
 * @param name name of function
 * @param data function arguments (if function have less arguments, empty ones have 0 value)
 */
typedef struct def_command {
    char name[16];
    int data[7];
} command;


/**
 * Reads a command from input, if data are wrong function make parametr is_no_inputerror false.
 * No implement part of array in command structure get 0 value.
 * @param new_command is our overriding struct
 * @param is_no_inputerror give information if input data are correct
 * @return command with data points using command structure
 */
command *parse_command(command *new_command, bool *is_no_inputerror);

/**
 * Finding function initialized by command.
 * Functions have assigned numbers using in other part of program.
 * @param name contain word to interpret
 * @return number assigned to function
 */
int find_function(char *name);

#endif /* PARSE_H */