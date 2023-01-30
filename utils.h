#ifndef UTILS_H
#define UTILS_H


char* toLower(char* s);
int validate_date(char* date);
int validate_debt(const char* debt);
int validate_first_name(char* name);
int validate_last_name(char* last_name);
int validate_id_number(char* id_number);
int validate_telephone(char* telephone);
void print_cb(char *string, int ignore);
int check_value(char *field, char *value);
int check_field(char *input, char *field); 
int validate_debt_float(const char *debt_str);
int compare_float(float a, char *operator, char *value);
int compare_string(char *field, char *operator, char *value);
int compare_date(const char *date1, char *operator, const char *date2);
void extract_field_operator_value(char *input, char *field, char *operator, char *value, int *ret, void(*cb)(char *, int), int server_mode);

#endif