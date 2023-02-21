#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "csv.h"
#include "utils.h"

#define MAX_ERROR_LINES 100
#define MAX_LEN 1024

typedef int (*validation_func)(char *field);

char incorrect_lines[MAX_ERROR_LINES][MAX_LEN];
int incorrect_lines_count = 0, correct_lines_count = 0, total_lines = 0;

void print_error_messages(char incorrect_lines[][MAX_LEN], int num_errors) 
{
    int i;
    for (i = 0; i < num_errors; i++) {
        printf("\n\tError %d: %s\n", i+1, incorrect_lines[i]);
    }
}

Customer parse_line(char *line_, Customer_error *err) 
{
    char *line = line_;
    Customer c = {0};
    int field_num = 0;
    validation_func func[] = {validate_first_name, validate_last_name, validate_id_number, validate_telephone, validate_date, validate_debt};
    char *field_names[] = {"first name", "last name", "ID number", "phone", "date", "debt"};
    char *token = strtok(line, ",");
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Empty line");
        return c;
    }
    while (token != NULL)
    {
        switch (field_num)
        {
            case 0:
                strncpy(c.first_name, token, sizeof(c.first_name) - 1);
                for (int i = 0; c.first_name[i]; i++) {
                    c.first_name[i] = tolower(c.first_name[i]);
                }
                break;
            case 1:
                strncpy(c.last_name, token, sizeof(c.last_name) - 1);
                for (int i = 0; c.last_name[i]; i++) {
                    c.last_name[i] = tolower(c.last_name[i]);
                }
                break;
            case 2:
                strncpy(c.id_number, token, sizeof(c.id_number) - 1);
                break;
            case 3:
                strncpy(c.phone, token, sizeof(c.phone) - 1);
                break;
            case 4:
                strncpy(c.date, token, sizeof(c.date) - 1);
                break;
            case 5:
                if (token[0] == '-') {
                    c.debt = -1 * strtof(token + 1, NULL);
                } else {
                    c.debt = strtof(token, NULL);
                }
                break;
        }
        if (!func[field_num](token))
        {
            err->error = 1;
            sprintf(err->message, "Invalid %s format for customer %s %s, ID: %s", field_names[field_num], c.first_name, c.last_name, c.id_number);
            return c;
        }
        token = strtok(NULL, ",");
        field_num++;
    }
    err->error = 0;
    return c;
}

void process_file(char *file_name, Customer **head, int server_flag) 
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        puts("Error opening file\n");
        return;
    }

    char line[MAX_LEN];
    while (fgets(line, sizeof(line), file)) 
    {
        Customer_error err = {0, ""};
        Customer curr = parse_line(line, &err);
        if (!err.error) {
            build_list(curr, head);
            correct_lines_count++;
        } else {
            strcpy(incorrect_lines[incorrect_lines_count], err.message);
            incorrect_lines_count++;
        }
        total_lines++;
    }

    if (server_flag)
    {
        if(incorrect_lines_count == 0) {
            printf("\n\nProcessed %d out of %d lines successfully.\n", correct_lines_count, total_lines);
        }
        else {
            printf("\nProcessed %d out of %d lines successfully, and %d lines with the following errors:\n", correct_lines_count, total_lines, incorrect_lines_count);
            print_error_messages(incorrect_lines, incorrect_lines_count);
        }
    }
    
    fclose(file);
}