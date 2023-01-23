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




void print_customer(Customer *c) 
{
    printf("+------------------------------------------------------+\n");
    printf("| Name: %-5s %-5s | ID Number: %-20s  |\n", c->first_name, c->last_name, c->id_number);
    printf("| Phone: %-5s | Debt: %-5.2lf | Date: %-10s  |\n", c->phone, c->debt, c->date);
    printf("+------------------------------------------------------+\n");
}

void handle_set(char *input, Customer **head, char *file_name) 
{
    char *token;
    Customer new_customer;
    int fields_count = 0;
    int first_name_set, last_name_set, id_set, phone_set, debt_set, date_set = 0;

    while ((token = strtok(input, ",")) != NULL) 
    {
        input = NULL;
        // check if there is a space before the comma
        if (token[0] == ' ') {
            printf("Error: Invalid format. No spaces after comma.\n");
            return;
        }
        // check if there is a space before the equals sign
        if (strchr(token, ' ') != strrchr(token, ' ')) {
            printf("Error: Invalid format. No spaces before equals sign.\n");
            return;
        }
        if(strstr(token,"first name=")){
            strcpy(new_customer.first_name, token + strlen("first name="));
            first_name_set = 1;
            fields_count++;
        }else if(strstr(token,"last name=")){
            strcpy(new_customer.last_name, token + strlen("last name="));
            last_name_set = 1;
            fields_count++;
        }else if(strstr(token,"id=")){
            strcpy(new_customer.id_number, token + strlen("id="));
            id_set = 1;
            fields_count++;
        }else if(strstr(token,"phone=")){
            strcpy(new_customer.phone, token + strlen("phone="));
            phone_set = 1;
            fields_count++;
        }else if(strstr(token,"date=")){
            strcpy(new_customer.date, token + strlen("date="));
            fields_count++;
            date_set = 1;
        }else if(strstr(token,"debt=")){
            sscanf(token + strlen("debt="),"%f",&new_customer.debt);
            fields_count++;
            debt_set = 1;
        }
    }

    if(fields_count != 6){
        printf("\nError: expected 6 fields, but got %d .\nCheck that you fill up all the fields\n {first name=,second name=,id=,date=,debt=}\n" , fields_count);
        return;
    }

    if(!first_name_set){
        printf("Error: Missing first name field.\n");
        return;
    }
    if(!last_name_set){
        printf("Error: Missing last name field.\n");
        return;
    }
    if(!id_set){
        printf("Error: Missing id field.\n");
        return;
    }
    if(!phone_set){
        printf("Error: Missing phone field.\n");
        return;
    }
    if(!date_set){
        printf("Error: Missing date field.\n");
        return;
    }
    if(!debt_set){
        printf("Error: Missing debt field.\n");
        return;
    }

    if(!validate_first_name(new_customer.first_name)){
        printf("Wrong input value for first name field.\n");
        return;
    }
    if(!validate_last_name(new_customer.last_name)){
        printf("Wrong input value for last name field.\n");
        return;
    }
    if(!validate_id_number(new_customer.id_number)){
        printf("Wrong input value for id field.\n");
        return;
    }
    if(!validate_telephone(new_customer.phone)){
        printf("Wrong input value for phone field.\n");
        return;
    }
    if(!validate_date(new_customer.date)){
        printf("Wrong input value for date field.\n");
        return;
    }
    // char debt_str[20];
    // snprintf(debt_str, sizeof(debt_str), "%f", new_customer.debt);
    if(!validate_debt_float(new_customer.debt)){
        printf("Wrong input value for debt field.\n");
        return;
    }

    Customer *temp = *head;
    Customer *prev = NULL;
    int found = 0;
    while (temp != NULL) {
        if (strcasecmp(temp->id_number, new_customer.id_number) == 0) {
            found = 1;
            if(strcasecmp(temp->first_name, new_customer.first_name) != 0 || strcasecmp(temp->last_name, new_customer.last_name) != 0){
                printf("This id already exist for a different customer.\n");
                return;
            }else if((strcasecmp(temp->phone, new_customer.phone) != 0)){
                save_to_csv(&new_customer, file_name);
                strcpy(temp->phone, new_customer.phone);
                printf("User exist with different phone. debt was updated in the list and added to file.");
                return;
            }
            float temp_s = temp->debt;
            temp->debt -= new_customer.debt;
            if (compare_date(new_customer.date, ">", temp->date))
            {
                strcpy(temp->date, new_customer.date);
            }
            if (prev == NULL) {
                *head = temp->next;
            } else {
                prev->next = temp->next;
            }
            save_to_csv(temp, file_name);
            insert_in_order(temp, head);
            printf("Debt was updated for customer: %s %s, from %.2f to %.2f\n", temp->first_name, temp->last_name, temp_s, temp->debt);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (!found) {
        insert_in_order(&new_customer, head);
        save_to_csv(&new_customer, file_name);
        printf("Added new customer to the list!");
    }  
}

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