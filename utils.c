#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

#define ARR_LEN(_arr) (sizeof(_arr)/sizeof(_arr[0]))



int check_value(char *field, char *value) 
{
    char *field_lower = toLower(field);
    if (strcmp(field_lower, "first name") == 0) {
        return validate_first_name(value);
    } else if (strcmp(field_lower, "last name") == 0) {
        return validate_last_name(value);
    } else if (strcmp(field_lower, "id") == 0) {
        return validate_id_number(value);
    } else if (strcmp(field_lower, "telephone") == 0) {
        return validate_telephone(value);
    } else if (strcmp(field_lower, "date") == 0) {
        return validate_date(value);
    } else if (strcmp(field_lower, "debt") == 0) {
        return validate_debt(value);
    }
    return 0;
}

int check_field(char *input, char *field) 
{
    char *input_ = input;
    char *pos = strstr(input_, field);
    if (pos != NULL) {
        return 1;
    } else {
        return 0;
    }
}

int compare_float(float a, char *operator, char *value) 
{
    float b = atof(value);
    if (strcmp(operator, "=") == 0) {
        return a == b;
    } else if (strcmp(operator, "!=") == 0) {
        return a != b;
    } else if (strcmp(operator, ">") == 0) {
        return a > b;
    } else if (strcmp(operator, "<") == 0) {
        return a < b;
    }else {
        return 0;
    }
}

int compare_string(char *field, char *operator, char *value) 
{
    if (strcmp(operator, "=") == 0) {
        return strcmp(field, value) == 0;
    } else if (strcmp(operator, "!=") == 0) {
        return strcmp(field, value) != 0;
    } else if (strcmp(operator, ">") == 0) {
        return strcmp(field, value) > 0;
    } else if (strcmp(operator, "<") == 0) {
        return strcmp(field, value) < 0;
    }else {
        return 0;
    }
}

int compare_date(const char *date1, char *operator, const char *date2) 
{
    struct tm tm1, tm2;
    strptime(date1, "%d/%m/%Y", &tm1);
    strptime(date2, "%d/%m/%Y", &tm2);

    if (strcmp(operator, "=") == 0) {
        return (tm1.tm_year == tm2.tm_year) && (tm1.tm_mon == tm2.tm_mon) && (tm1.tm_mday == tm2.tm_mday);
    } else if (strcmp(operator, "!=") == 0) {
        return !((tm1.tm_year == tm2.tm_year) && (tm1.tm_mon == tm2.tm_mon) && (tm1.tm_mday == tm2.tm_mday));
    } else if (strcmp(operator, ">") == 0) {
        if (tm1.tm_year > tm2.tm_year) return 1;
        if (tm1.tm_year < tm2.tm_year) return 0;
        if (tm1.tm_mon > tm2.tm_mon) return 1;
        if (tm1.tm_mon < tm2.tm_mon) return 0;
        return tm1.tm_mday > tm2.tm_mday;
    } else if (strcmp(operator, "<") == 0) {
        if (tm1.tm_year < tm2.tm_year) return 1;
        if (tm1.tm_year > tm2.tm_year) return 0;
        if (tm1.tm_mon < tm2.tm_mon) return 1;
        if (tm1.tm_mon > tm2.tm_mon) return 0;
        return tm1.tm_mday < tm2.tm_mday;
    }else {
        return 0;
    }
}

void extract_field_operator_value(char *input, char *field, char *operator, char *value, int *ret) 
{
    // Allowed fields array
    char *allowed_fields[] = {"first name", "last name", "telephone", "id", "debt", "date"};
    // Copy the input string to a temporary buffer
    char temp[strlen(input) + 1];
    strcpy(temp, input);

    // Extracting the field
    char *op = strpbrk(temp, "<>!=");
    if (op) {
        int field_len = op - temp;
        strncpy(field, temp, field_len);
        field[field_len] = '\0';

        int operator_len = strspn(op, "<>!=");
        strncpy(operator, op, operator_len);
        operator[operator_len] = '\0';

        strcpy(value, toLower(op + operator_len));

        int valid_field = 0;
        for (int i = 0; i < ARR_LEN(allowed_fields); i++) {
            if (strcmp(field, allowed_fields[i]) == 0) {
                valid_field = 1;
                break;
            }
        }
        if (!valid_field) {
            printf("Error: Check if field is correct and without spaces before and after operator\n");
            strcpy(field, "");
            strcpy(operator, "");
            strcpy(value, "");
            *ret = 0;
            return;
        }
    } else {
        printf("Error: Invalid format. use the format <field><operator><value> without spaces.\n");
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    // check if the operator and value contain any spaces
    if (strchr(operator, ' ') != NULL || strchr(value, ' ') != NULL) {
        printf("Error: Invalid format. use the format <field><operator><value> without spaces.\n");
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    if(strlen(operator)==0 || strlen(value)==0)
    {
        printf("Error: Missing operator or value.\n");
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    // check if the operator is in the allowed array
    int valid_operator = 0;
    char *allowed_operators[] = {"<", ">", "!=", "="};
    for (int i = 0; i < ARR_LEN(allowed_operators); i++) {
        if (strcmp(operator, allowed_operators[i]) == 0) {
            valid_operator = 1;
            break;
        }
    }
    if (!valid_operator) {
        printf("Error: Invalid operator. Allowed operators are <, >, !=, =.\n");
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    if (!check_value(field, value)) {
        printf("Error: Invalid value for field '%s'.\n", field);
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    *ret = 1;
}