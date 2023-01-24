#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "csv.h"
#include "utils.h"

#define MAX_ERROR_LINES 100

char incorrect_lines[MAX_ERROR_LINES][1024];
int incorrect_lines_count = 0, correct_lines_count = 0, total_lines = 0;

void print_error_messages(char incorrect_lines[][1024], int num_errors) 
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
    char *token = strtok_r(line, ",", &line);
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Empty line");
        return c;
    }
    // first Name
    strncpy(c.first_name, token, sizeof(c.first_name) - 1);
    for (int i = 0; c.first_name[i]; i++) {
        c.first_name[i] = tolower(c.first_name[i]);
    }
    if(!validate_first_name(c.first_name)) {
        err->error = 1;
        sprintf(err->message, "Invalid first name format for customer %s", c.first_name);
        return c;
    }
    // last Name
    token = strtok_r(NULL, ",", &line);
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Missing last name field");
        return c;
    }
    strncpy(c.last_name, token, sizeof(c.last_name) - 1);
    for (int i = 0; c.last_name[i]; i++) {
        c.last_name[i] = tolower(c.last_name[i]);
    }
    if(!validate_last_name(c.last_name)) {
        err->error = 1;
        sprintf(err->message, "Invalid last name format for customer %s", c.last_name);
        return c;
    }
    // id Number
    token = strtok_r(NULL, ",", &line);
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Missing ID Number field");
        return c;
    }
    strncpy(c.id_number, token, sizeof(c.id_number) - 1);
    if(!validate_id_number(c.id_number)) {
        err->error = 1;
        sprintf(err->message, "Invalid ID number format for customer %s %s, ID: %s", c.first_name, c.last_name, c.id_number);
        return c;
    }
    // phone
    token = strtok_r(NULL, ",", &line);
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Missing Phone field");
        return c;
    }
    strncpy(c.phone, token, sizeof(c.phone) - 1);
    if(!validate_telephone(c.phone)) {
        err->error = 1;
        sprintf(err->message, "Invalid phone number format for customer %s %s, ID: %s", c.first_name, c.last_name, c.id_number);
        return c;
    }
    // date
    token = strtok_r(NULL, ",", &line);
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Missing date field");
        return c;
    }
    strncpy(c.date, token, sizeof(c.date) - 1);
    if(!validate_date(c.date)) {
        err->error = 1;
        sprintf(err->message, "Invalid date format for customer %s %s, ID: %s", c.first_name, c.last_name, c.id_number);
        return c;
    }
    // debt
    token = strtok_r(NULL, ",", &line);
    if (token == NULL) {
        err->error = 1;
        sprintf(err->message, "Invalid line format: Missing debt field");
        return c;
    }
    if (token[0] == '-') {
        c.debt = -1 * strtof(token + 1, NULL);
    } else {
        c.debt = strtof(token, NULL);
    }
    if (!validate_debt(token)) {
        err->error = 1;
        sprintf(err->message, "Invalid debt format for customer %s %s, ID: %s", c.first_name, c.last_name, c.id_number);
        return c;
    }
    err->error = 0;
    return c;
}

void process_file(char *file_name, Customer **head) 
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[1024];
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
    if(incorrect_lines_count == 0) {
        printf("\n\nProcessed %d out of %d lines successfully.\n", correct_lines_count, total_lines);
    }
    else {
        printf("\n\nProcessed %d out of %d lines successfully, and %d lines with the following errors:\n", correct_lines_count, total_lines, incorrect_lines_count);
        print_error_messages(incorrect_lines, incorrect_lines_count);
    }

    fclose(file);
}