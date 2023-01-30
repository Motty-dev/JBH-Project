#ifndef USERINPUT_H
#define USERINPUT_H

#include "list.h"

void menu_screen(Customer *head, char *file_name);
void handle_input(Customer *head, char *input, char *file_name, void(*cb)(char*, int), int server_mode);

#endif