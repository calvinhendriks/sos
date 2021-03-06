#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sortedcontainer.h"
#include "test.h"

// DO NOT change this value. It does not fix your problems
#define INPUT_INCREMENT 10

/**
 * @brief Prints the prompt to @c f
 * @param f The FILE to print the prompt to.
 */
void print_prompt(FILE* f) {
    fprintf(f, "\n> "); fflush(f);
}

/**
 * @brief Basic parser to read data from @c command
 * @param command The command string to read the data from
 * @return A new data object
 *
 * TO FIX:
 *   There are two serious problems in this function that are related
 */
data* read_data(char const* command) {
    int age;
    char name[NAME_LENGTH];
    char buff[30];
    char num[20];
    if(sscanf(command, "%*s %19s %29s", num, buff) != 2 || strlen(num) > 10 || strlen(buff) > 19){
        printf("%s\n", "Invalid input");
        return NULL;
    }
    if(strspn(num, "0123456789") != strlen(num)){
        printf("%s\n", "Invalid input");
        return NULL;
    }
    long err = 0;
    char *ptr;
    err = strtol(num, &ptr, 10);
    if(err > 2147483647 || err < 0){
        printf("%s\n", "Invalid input");
        return NULL;
    } else {
        age = err;
    }
    strncpy(name, buff, 20);
    name[19] = '\0';
    return data_new(age, name);
}

/**
 * @brief Handles @c command
 * @param printFile FILE to print messages to
 * @param sc The sortedcontainer to query or modify
 * @param command The command to handle
 * @return 1 iff the problem should quit, otherwise 0
 *
 * TO FIX:
 *   There are three problems in this function, two of which are related
 */
int handle_command(FILE* printFile, sortedcontainer* sc, char* command) {
    switch(*command) {
        case 'i':
            {
                data* d = read_data(command);
                sortedcontainer_insert(sc, d);
                break;
            }
        case 'e':
            {
                data* d = read_data(command);
                sortedcontainer_erase(sc, d);
                free(d);
                break;
            }
        case 'c':
            {
                data* d = read_data(command);
                if(sortedcontainer_contains(sc, d) == -1){
                    break;
                } else if(sortedcontainer_contains(sc, d)) {
                    fprintf(printFile, "y\n");
                } else {
                    fprintf(printFile, "n\n");
                }
                free(d);
                break;
            }
        case 'p':
            sortedcontainer_print(sc, printFile);
            break;
        case 'x':
            return 1;
        case 't':
            test(printFile);
            break;
        default: {
            fprintf(printFile, "No such command: ");
            fprintf(printFile, "%c", *command);
            fprintf(printFile, "\n");
            break;
        }
    }
    return 0;
}

/**
 * @brief Reads a command from the FILE @c in
 * @param in FILE to read a command from
 * @return The read command
 *
 * TO FIX:
 *   There are two separate problems in this function. Fix these problems
 *   by only changing TWO lines in total.
 *   Changed input because realloc might change the place of the memory address
 */
char* read_command(FILE* in) {
    int inputMaxLength = 0;
    char* input = NULL;
    char* inputAt = NULL;

    int incr = INPUT_INCREMENT;

    inputMaxLength = incr;
    input = (char*)malloc(sizeof(char) * incr);
    inputAt = input;
    do {
        inputAt[incr - 1] = 'e';
        if(fgets(inputAt, incr, in) == NULL) return NULL;
        if(inputAt[incr - 1] != '\0' || inputAt[incr - 2] == '\n') {
            break;
        }
        inputMaxLength += INPUT_INCREMENT;
        input = realloc(input, sizeof(char) * inputMaxLength);
        inputAt = input + inputMaxLength - INPUT_INCREMENT - 1;
        incr = INPUT_INCREMENT + 1;
    } while(1);
    if(strlen(input) != 0) {
        input[strlen(input)-1] = 0;
    }
    return input;
}

/**
 * @brief The main SINT loop
 * @param argc Argument count
 * @param argv Arguments
 * @return 0
 *
 * TO FIX:
 *   One issue needs to be fixed here. free(command) was het probleem
 */
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    sortedcontainer* sc = sortedcontainer_new();

    while(1) {
        print_prompt(stdout);

        char* command = read_command(stdin);
        if(command == NULL) {
            free(command);
            break;
        }

        if(handle_command(stdout, sc, command)) {
            free(command);
            break;
        }
        free(command);
    }

    sortedcontainer_delete(sc);

    fprintf(stdout, "\nBye.\n");

    return 0;
}

