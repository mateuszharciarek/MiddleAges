/** @file
   Interface of parser.
   Parse lines from standard input and check if are they correct.
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

const int MAX_LINE_LENGTH = INT_MAX;

const char * INIT = "INIT";
const char *MOVE = "MOVE";
const char *PRODUCE_KNIGHT = "PRODUCE_KNIGHT";
const char *PRODUCE_PEASANT = "PRODUCE_PEASANT";
const char *END_TURN = "END_TURN";


typedef struct def_command {
    char name[16];
    int data[7];
} command;


command *parse_command(command *new_command,bool *is_no_inputerror) {

    ssize_t read;
    size_t len = MAX_LINE_LENGTH;
    char *line = NULL;
    char *tmp;
    bool isnumber = true;

    for (int i = 0; i < 7; i++) {
        new_command->data[i] = 0;
    }

    strcpy(new_command->name, "ERROR");

    read = getline(&line, &len, stdin);
    char *line2 = line;

    if (read != -1 && read <= 100 && line[0] != '\n' && line[0] != ' ') {

        tmp = strsep(&line2, " \n");

        if (strlen(tmp) <= 16)
            strcpy(new_command->name, tmp);
        else {
            *is_no_inputerror = false;
        }

        int i = 0;

        int number_of_arguments = -1;
        if (strcmp(new_command->name, INIT) == 0)
            number_of_arguments = 7;
        if (strcmp(new_command->name, MOVE) == 0)
            number_of_arguments = 4;
        if (strcmp(new_command->name, PRODUCE_KNIGHT) == 0)
            number_of_arguments = 4;
        if (strcmp(new_command->name, PRODUCE_PEASANT) == 0)
            number_of_arguments = 4;
        if (strcmp(new_command->name, END_TURN) == 0)
            number_of_arguments = 0;

        tmp = strsep(&line2, " \n");

        while (tmp != NULL && i < number_of_arguments && isnumber) {

            for (int j = 0; j < strlen(tmp); ++j) {
                if (!isdigit(tmp[j])) {
                    isnumber = false;
                }
            }
            if (strcmp(tmp, "") == 0)
                isnumber = false;
            else {
                new_command->data[i] = atoi(tmp);
                if (i == number_of_arguments - 1)
                    tmp = strsep(&line2, "\0\n");
                else
                    tmp = strsep(&line2, " \n");
                i++;
            }
        }
        if (i != number_of_arguments) {
            *is_no_inputerror = false;
        }

        if (strcmp(tmp, "") != 0) {
            *is_no_inputerror = false;
        }

    }
    else {
        *is_no_inputerror = false;
    }
    free(line);

    if (!isnumber) {
        *is_no_inputerror = false;
    }
    return new_command;
}

int find_function(char *name) {

    if (strcmp(name, INIT) == 0)
        return 1;
    if (strcmp(name, MOVE) == 0)
        return 2;
    if (strcmp(name, PRODUCE_KNIGHT) == 0)
        return 3;
    if (strcmp(name, PRODUCE_PEASANT) == 0)
        return 4;
    if (strcmp(name, END_TURN) == 0)
        return 5;

    return 0;
}