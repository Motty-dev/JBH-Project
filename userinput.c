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

void save_to_csv(Customer *new_customer, char *file_name, void(*cb)(char *, int), int server_mode) 
{
    FILE *file;
    char inner_buffer[1024];
    file = fopen(file_name, "a");
    if (file == NULL)
    {
        strcpy(inner_buffer, "Error opening file\n");
        cb(inner_buffer,server_mode);
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

int validate_input(const char* input, const char* field_name, void(*cb)(char*, int), int server_mode) 
{
    char inner_buffer[1024];
    if(input == NULL || input[0] == '\0')
    {
       strcpy(inner_buffer, "Error: Empty Input\n");
       cb(inner_buffer,server_mode);
       return 0;
    }
    if (strchr(input, ' ') != strrchr(input, ' ')) {
        snprintf(inner_buffer, 1024, "Error: Invalid format. No spaces before or after '=' in %s field.\n", field_name);
        cb(inner_buffer,server_mode);
        return 0;
    }
    if (strchr(input, '=') != strrchr(input, '=')) {
        snprintf(inner_buffer, 1024, "Error: Invalid format. Only one '=' is allowed in %s field.\n", field_name);
        cb(inner_buffer,server_mode);
        return 0;
    }
    if (*(strchr(input, '=') + 1) == ' ') {
        snprintf(inner_buffer, 1024, "Error: Invalid format. No spaces after '=' in %s field.\n", field_name);
        cb(inner_buffer,server_mode);
        return 0;
    }
    return 1;
}

int handle_field(char *token, char *field_name, Customer *new_customer, void(*cb)(char*, int), int server_mdoe)
{
    char inner_buffer[1024];
    if(!validate_input(token, field_name, cb, server_mdoe)) return 0;

    char *eq_sign = strchr(token, '=');
    if(eq_sign == NULL) {
        snprintf(inner_buffer, 1024, "Error: Invalid format. Missing '=' in %s field.\n", field_name);
        cb(inner_buffer, server_mdoe);
        return 0;
    }
    if (*(eq_sign + 1) == '\0') {
        snprintf(inner_buffer, 1024, "Error: Invalid format. No value for %s field.\n", field_name);
        cb(inner_buffer, server_mdoe);
        return 0;
    }
    // copy value to correct field into customer struct
    if(strstr(token,"first name=")){
        toLower(eq_sign + 1);
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
            strcpy(inner_buffer, "Error: Invalid value for debt field.\n");
            cb(inner_buffer, server_mdoe);
            return 0;
        }
        float debt = strtof(eq_sign + 1, NULL);
        new_customer->debt = debt;      
    }
    return 1;
}

void handle_set(char *input, Customer **head, char *file_name, void(*cb)(char*, int), int server_mode) 
{
    char inner_buffer[1024];
    char *token;
    Customer new_customer = {0};
    int fields_count = 0;
    int missing_fields = 0;

    if(input == NULL || input[0] == '\0')
    {
       strcpy(inner_buffer, "Error: Empty Input\n");
       cb(inner_buffer, server_mode);
       return;
    }

    missing_fields = check_missing_fields(input);
    if (missing_fields > 0) {
        snprintf(inner_buffer, 1024, "Error: Missing %d required fields.\n", missing_fields);
        cb(inner_buffer, server_mode);
        return;
    }

    while ((token = strtok_r(input, ",", &input)) != NULL) {
        if (!handle_field(token, "", &new_customer, cb, server_mode)) {
            return;
        }
        fields_count++;
    }

    if (fields_count != 6) {
        snprintf(inner_buffer, 1024, "\nError: expected 6 fields, but got %d .\nCheck that you fill up all the fields\n {first name=,last name=,id=,phone=,date=,debt=}\n" , fields_count);
        cb(inner_buffer, server_mode);
        return;
    }

    if(!validate_first_name(new_customer.first_name)){
        strcpy(inner_buffer, "Wrong input value for first name field.\n");
        cb(inner_buffer, server_mode);
        return;
    }
    if(!validate_last_name(new_customer.last_name)){
        strcpy(inner_buffer, "Wrong input value for second name field.\n");
        cb(inner_buffer, server_mode);
        return;
    }
    if(!validate_telephone(new_customer.phone)){
        strcpy(inner_buffer, "Wrong input value for phone field.\n");
        cb(inner_buffer, server_mode);
        return;
    }
    if(!validate_date(new_customer.date)){
        strcpy(inner_buffer, "Wrong input value for date field.\n");
        cb(inner_buffer, server_mode);
        return;
    }
    if(!validate_id_number(new_customer.id_number)){
        strcpy(inner_buffer, "Wrong input value for id field.\n");
        cb(inner_buffer, server_mode);
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
                strcpy(inner_buffer, "This id already exist for a different customer.\n");
                cb(inner_buffer, server_mode);
                return;
            }else if((strcasecmp(temp->phone, new_customer.phone) != 0)){
                //save_to_csv(&new_customer, file_name);
                strcpy(temp->phone, new_customer.phone);
                strcpy(inner_buffer, "User exist with different phone. debt was updated in the list and added to file.\n");
                cb(inner_buffer, server_mode);
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
            snprintf(inner_buffer, 1024, "Debt was updated for customer: %s %s, from %.2f to %.2f\n", temp->first_name, temp->last_name, temp_s, temp->debt);
            cb(inner_buffer, server_mode);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (!found) {
        build_list(new_customer, head);
        save_to_csv(&new_customer, file_name, cb, server_mode);
        puts("Added to list!");
    }
}

void handle_select(char *parameters, Customer *head, void(*cb)(char *, int), int server_mode) 
{
    char field[32], operator[4], value[64], inner_buffer[1024];
    int ret = 1;
    extract_field_operator_value(parameters, field, operator, value, &ret, cb, server_mode);
    if (ret)
    {   
        int match_found = 0;
        Customer *current = head;
        while (current != NULL) {
            if (match_customer(current, field, operator, value)) {
                match_found++;
                snprintf(inner_buffer, 1024, "+------------------------------------------------------+\n| Name: %-5s %-5s | ID Number: %-20s  |\n| Phone: %-5s | Debt: %-5.2lf | Date: %-10s  |\n+------------------------------------------------------+\n" ,current->first_name, current->last_name, current->id_number, current->phone, current->debt, current->date);
                cb(inner_buffer, server_mode);
            }
            current = current->next;
        }
        printf("%d results found.\n",match_found);
        if (!match_found) {
            strcpy(inner_buffer,"No match found\n");
            cb(inner_buffer, server_mode);
        }
    }  
}

void menu_screen(Customer *head, char *file_name)
{
    char input[INPUT_BUFFER_SIZE];

    //set the server to send localy if menu_screen is called
    int server_mode = 0;

    // get input from the user
    printf("Enter a command { > select > set > print > quit }\n > ");
    fgets(input, INPUT_BUFFER_SIZE, stdin);
    input[strcspn(input, "\n")] = 0;

    handle_input(head, input, file_name, &print_cb, server_mode);
}

void handle_input(Customer *head, char *input, char *file_name, void(*cb)(char*, int), int server_mode) 
{
    char command[16], *parameters=NULL;
    char inner_buffer[1024];
    char *cursor;

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
    parameters = (char*)malloc(len);    //len +1(to include the space before or not)
    strncpy(parameters, cursor, len);
    parameters[len] = '\0';

    if (strcmp(command, "select") == 0) {
        handle_select(parameters, head, cb, server_mode);
    } else if (strcmp(command, "set") == 0) {
        handle_set(parameters, &head, file_name, cb, server_mode);
    } else if (strcmp(command, "print") == 0) {
        print_list(head, cb, server_mode);
    } else if (strcmp(command, "quit") == 0) {
        exit(0);
    } else {
        snprintf(inner_buffer,1024, "Error: '%s' is not a valid command. Allowed commands are select, set, print, quit.\n", command);
        cb(inner_buffer, server_mode);
    }

    free(parameters);
}