#ifndef USERINPUT_H
#define USERINPUT_H

#include "list.h"

void menu_screen(Customer *head, char *file_name, char *buffer);
void handle_input(Customer *head, char *input, char *file_name, char *buffer);

#endif