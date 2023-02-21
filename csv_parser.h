#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "customer_manager.h"

typedef struct {
    int error;
    char message[256];
} Customer_error;

void process_file(char *file_name, Customer **head, int server_flag);

#endif