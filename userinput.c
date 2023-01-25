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

void save_to_csv(Customer *new_customer, char *file_name) 
{
    FILE *file;
    file = fopen(file_name, "a");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    fprintf(file, "\n%s,%s,%s,%s,%s,%.2f", new_customer->first_name, new_customer->last_name, new_customer->id_number, new_customer->phone, new_customer->date, new_customer->debt);
    fclose(file);
}

int match_customer(Customer *c, char *field, char *operator, char *value) 
{
    if (strcmp(field, "first name") == 0) {
        return compare_string(c->first_name, operator, value);
    } else if (strcmp(field, "second name") == 0) {
        return compare_string(c->last_name, operator, value);
    } else if (strcmp(field, "phone") == 0) {
        return compare_string(c->phone, operator, value);
    } else if (strcmp(field, "debt") == 0) {
        return compare_float(c->debt, operator, value);
    } else if (strcmp(field, "date") == 0) {
        return compare_date(c->date, operator, value);
    } else if (strcmp(field, "id") == 0) {
        return compare_string(c->id_number, operator, value);
    } else {
        return 0;
    }
    return 0;
}

void print_customer(Customer *c) 
{
    printf("+------------------------------------------------------+\n");
    printf("| Name: %-5s %-5s | ID Number: %-20s  |\n", c->first_name, c->last_name, c->id_number);
    printf("| Phone: %-5s | Debt: %-5.2lf | Date: %-10s  |\n", c->phone, c->debt, c->date);
    printf("+------------------------------------------------------+\n");
}

int check_missing_fields(const char* input) 
{
    int missing_fields = 0;
    if (strstr(input, "first name=") == NULL) {
        missing_fields++;
    }
    if (strstr(input, "second name=") == NULL) {
        missing_fields++;
    }
    if (strstr(input, "id=") == NULL) {
        missing_fields++;
    }
    if (strstr(input, "phone=") == NULL) {
        missing_fields++;
    }
    if (strstr(input, "date=") == NULL) {
        missing_fields++;
    }
    if (strstr(input, "debt=") == NULL) {
        missing_fields++;
    }
    return missing_fields;
}

int validate_input(const char* input, const char* field_name) 
{
    if(input == NULL || input[0] == '\0')
    {
       printf("Error: Empty Input\n");
       return 0;
    }
    if (strchr(input, ' ') != strrchr(input, ' ')) {
        printf("Error: Invalid format. No spaces before or after '=' in %s field.\n", field_name);
        return 0;
    }
    if (strchr(input, '=') != strrchr(input, '=')) {
        printf("Error: Invalid format. Only one '=' is allowed in %s field.\n", field_name);
        return 0;
    }
    if (*(strchr(input, '=') + 1) == ' ') {
        printf("Error: Invalid format. No spaces after '=' in %s field.\n", field_name);
        return 0;
    }
    return 1;
}

int handle_field(char *token, char *field_name, Customer *new_customer)
{
    if(!validate_input(token, field_name)) return 0;

    char *eq_sign = strchr(token, '=');
    if(eq_sign == NULL) {
        printf("Error: Invalid format. Missing '=' in %s field.\n", field_name);
        return 0;
    }
    if (*(eq_sign + 1) == '\0') {
        printf("Error: Invalid format. No value for %s field.\n", field_name);
        return 0;
    }
    // copy value to correct field in Customer struct
    if(strstr(token,"first name=")){
        snprintf(new_customer->first_name, sizeof(new_customer->first_name), "%s", eq_sign + 1);
    }else if(strstr(token,"second name=")){
        snprintf(new_customer->last_name, sizeof(new_customer->last_name), "%s", eq_sign + 1);
    }else if(strstr(token,"id=")){
        snprintf(new_customer->id_number, sizeof(new_customer->id_number), "%s", eq_sign + 1);
    }else if(strstr(token,"phone=")){
        snprintf(new_customer->phone, sizeof(new_customer->phone), "%s", eq_sign + 1);
    }else if(strstr(token,"date=")){
        snprintf(new_customer->date, sizeof(new_customer->date), "%s", eq_sign + 1);
    }else if(strstr(token,"debt=")){
        if(eq_sign[1] == '\0' || !validate_debt_float(eq_sign + 1)){
            printf("Error: Invalid value for debt field.\n");
            return 0;
        }
        float debt = strtof(eq_sign + 1, NULL);
        new_customer->debt = debt;      
    }
    return 1;
}

void handle_set(char *input, Customer **head, char *file_name) 
{
    char *token;
    Customer new_customer = {0};
    int fields_count = 0;
    int missing_fields = 0;

    if(input == NULL || input[0] == '\0')
    {
       printf("Error: Empty Input\n");
       return;
    }

    missing_fields = check_missing_fields(input);
    if (missing_fields > 0) {
        printf("Error: Missing %d required fields.\n", missing_fields);
        return;
    }

    while ((token = strtok_r(input, ",", &input)) != NULL) {
        if (!handle_field(token, "", &new_customer)) {
            return;
        }
        fields_count++;
    }

    if (fields_count != 6) {
        printf("\nError: expected 6 fields, but got %d .\nCheck that you fill up all the fields\n {first name=,last name=,id=,phone=,date=,debt=}\n" , fields_count);
        return;
    }

    if(!validate_first_name(new_customer.first_name)){
        printf("Wrong input value for first name field.\n");
        return;
    }
    if(!validate_last_name(new_customer.last_name)){
        printf("Wrong input value for second name field.\n");
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
    if(!validate_id_number(new_customer.id_number)){
        printf("Wrong input value for id field.\n");
        return;
    }
    // add new customer to the list logic( after missing fields check )
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
                //save_to_csv(&new_customer, file_name);
                strcpy(temp->phone, new_customer.phone);
                printf("User exist with different phone. debt was updated in the list and added to file.\n");
            }
            float temp_s = temp->debt;
            temp->debt += new_customer.debt;
            if (compare_date(new_customer.date, ">", temp->date))
            {
                strcpy(temp->date, new_customer.date);
            }
            if (prev == NULL) {
                *head = temp->next;
            } else {
                prev->next = temp->next;
            }
            
            insert_in_order(temp, head);
            printf("Debt was updated for customer: %s %s, from %.2f to %.2f\n", temp->first_name, temp->last_name, temp_s, temp->debt);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (!found) {
        build_list(new_customer, head);
        save_to_csv(&new_customer, file_name);
        printf("Added new customer to the list!\n");
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
            printf("No match found\n");
        }
    }  
}

void handle_input(Customer *head, char *file_name) 
{
    char input[INPUT_BUFFER_SIZE];
    char command[16], *parameters=NULL;
    char *cursor;

    // get input from the user
    printf("Enter a command ( select / set / print / quit ) :\n> ");
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
        printf("Error: '%s' is not a valid command. Allowed commands are select, set, print, quit, (witout spaces before).\n", command);
    }

    free(parameters);
}