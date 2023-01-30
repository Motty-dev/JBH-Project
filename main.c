#include "list.h"
#include "csv.h"
#include "userinput.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Error: expected 1 argument, but got %d\n", argc-1);
        return 1;
    }

    Customer *head = NULL;
    char buffer[1024];

    process_file(argv[1], &head);
    print_list(head);

    while (1) 
    {   
        menu_screen(head, argv[1], buffer);
        printf("%s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    free_list(head);

    return 0;
}