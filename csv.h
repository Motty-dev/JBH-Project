#ifndef CSV_H
#define CSV_H

#include "list.h"

typedef struct {
    int error;
    char message[256];
} Customer_error;

void process_file(char *file_name, Customer **head);

#endif