#ifndef QUERIES_H
#define QUERIES_H

#include "customer_manager.h"

int menu_screen(Customer **head, char *file_name);
int handle_input(Customer **head, char *input, char *file_name, void(*cb)(char*, int), int server_mode);

#endif