#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "userinput.h"
#include "list.h"
#include "utils.h"

#define INPUT_BUFFER_SIZE 256
#define ARR_LEN(_arr) (sizeof(_arr)/sizeof(_arr[0]))

void handle_select(char *parameters, Customer *head) 
{
    char field[32], operator[4], value[64];
    int ret = 1;
    extract_field_operator_value(parameters, field, operator, value, &ret);
    if (ret)
    {   
        int match_found = 0;
        Customer *current = head;
        while (current != NULL) {
            if (match_customer(current, field, operator, value)) {
                match_found++;
                print_customer(current);
            }
            current = current->next;
        }
        printf("%d results found.\n",match_found);
        if (!match_found) {
            printf("No customer matching the query\n");
        }
    }  
}

void handle_input(Customer *head, char *file_name) 
{
    char input[INPUT_BUFFER_SIZE];
    char command[16], *parameters=NULL;
    char *cursor;

    // get input from the user
    printf("Enter a command { > select >set >print >quit }\n > ");
    fgets(input, INPUT_BUFFER_SIZE, stdin);
    input[strcspn(input, "\n")] = 0;
    cursor = input;

    // extract the command
    int len = strcspn(cursor, " ");
    strncpy(command, cursor, len);
    command[len] = '\0';
    len++;

    // move the cursor to the start of the parameters
    cursor += len;
    cursor += strspn(cursor, "\n");

    // extract the parameters
    len = strlen(cursor);
    parameters = (char*)malloc(len);    //len +1
    strncpy(parameters, cursor, len);
    parameters[len] = '\0';

    if (strcmp(command, "select") == 0) {
        handle_select(parameters, head);
    } else if (strcmp(command, "set") == 0) {
        handle_set(parameters, &head, file_name);
    } else if (strcmp(command, "print") == 0) {
        print_list(head);
    } else if (strcmp(command, "quit") == 0) {
        exit(0);
    } else {
        printf("Error: '%s' is not a valid command. Allowed commands are select, set, print, quit.\n", command);
    }
    printf("parameters:%s\n", parameters);
    free(parameters);
}