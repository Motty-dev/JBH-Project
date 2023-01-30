#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"

#define ARR_LEN(_arr) (sizeof(_arr)/sizeof(_arr[0]))

char* toLower(char* s) 
{
    for(char *p=s; *p; p++) *p=tolower(*p);
    return s;
}

int validate_date(char* date) 
{
    int day, month, year;

    if (sscanf(date, "%d%*c%d%*c%d", &month, &day, &year) != 3) return 0;
    
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (year < 0) return 0;
    // mktime function to check if the extracted date is a valid date
    struct tm tm = {0};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    time_t t = mktime(&tm);
    if (t == -1) return 0;
    return 1;
}

int validate_debt(const char* debt_str) 
{
    char* end;
    strtod(debt_str, &end);
    if (debt_str == end) {  // input string is not a valid number
        return 0;
    }
    while (*end) {
        if (!isspace(*end)) {  // check if there are any non-whitespace characters after the number
            return 0;
        }
        end++;
    }
    return 1;
}

int validate_first_name(char* name) 
{
    if(strlen(name) < 2) return 0; //can be changed to more names
    if(!isalpha(name[0])) return 0;
    int word_count = 1;
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
        if (name[i] == ' ') {
            word_count++;
            if(!isalpha(name[i+1])) return 0;
        }
    }
    if(word_count>2) return 0;
    return 1;
}

int validate_debt_float(const char *debt_str) 
{
    int dot_count = 0;
    for (int i = 0; debt_str[i] != '\0'; i++) {
        if (!(isdigit(debt_str[i]) || debt_str[i] == '.' || debt_str[i] == '-')) {
            return 0;
        }
        if (debt_str[i] == '.') {
            dot_count++;
        }
    }
    if (dot_count > 1) {
        return 0;
    }
    return 1;
}

int validate_last_name(char* last_name) 
{
    for (int i = 0; last_name[i]; i++) {
        if (!isalpha(last_name[i]) && !(last_name[i] == ' ') && !(last_name[i] == '-') && !(last_name[i] == '.') && !(last_name[i] == '_') ) {
            return 0;
        }
    }
    return 1;
}

int validate_id_number(char* id_number) 
{
    if(strlen(id_number) != 9) return 0;
    for (int i = 0; id_number[i]; i++) {
        if (!isdigit(id_number[i])) return 0;
    }
    return 1;
}

int validate_telephone(char* telephone) 
{
    if(strlen(telephone) != 10) return 0;
    if(telephone[0] != '0') return 0;
    for (int i = 1; telephone[i]; i++) {
        if (!isdigit(telephone[i])) return 0;
    }
    return 1;
}

int check_value(char *field, char *value) 
{
    char *field_lower = toLower(field);
    if (strcmp(field_lower, "first name") == 0) {
        return validate_first_name(value);
    } else if (strcmp(field_lower, "second name") == 0) {
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

void extract_field_operator_value(char *input, char *field, char *operator, char *value, int *ret, void(*cb)(char *, int), int server_mode) 
{
    char inner_buffer[1024];

    char *allowed_fields[] = {"first name", "second name", "telephone", "id", "debt", "date"};
    // copy the input string to a temporary buffer
    char temp[strlen(input) + 1];
    strcpy(temp, input);

    // extracting the field
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
            strcpy(inner_buffer, "Error: Check if field is correct and without spaces before and after operator\n");
            cb(inner_buffer, server_mode);
            strcpy(field, "");
            strcpy(operator, "");
            strcpy(value, "");
            *ret = 0;
            return;
        }
    } else {
        strcpy(inner_buffer, "Error: Invalid format. use the format <field><operator><value> without spaces.\n");
        cb(inner_buffer, server_mode);
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    // check if the operator and value contain any spaces
    if (strchr(operator, ' ') != NULL || strchr(value, ' ') != NULL) {
        strcpy(inner_buffer, "Error: Invalid format. use the format <field><operator><value> without spaces.\n");
        cb(inner_buffer, server_mode);
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    if(strlen(operator)==0 || strlen(value)==0)
    {
        strcpy(inner_buffer, "Error: Missing operator or value.\n");
        cb(inner_buffer, server_mode);
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
        strcpy(inner_buffer, "Error: Invalid operator. Allowed operators are <, >, !=, =.\n");
        cb(inner_buffer, server_mode);
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    if (!check_value(field, value)) {
        snprintf(inner_buffer, 1024, "Error: Invalid value for field '%s'.\n", field);
        cb(inner_buffer, server_mode);
        strcpy(field, "");
        strcpy(operator, "");
        strcpy(value, "");
        *ret = 0;
        return;
    }
    *ret = 1;
}

void print_cb(char *string, int ignore) 
{
    printf("%s\n", string);
}