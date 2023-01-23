#include "list.h"
#include "csv.h"
#include "userinput.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Error: expected 1 argument, but got %d\n", argc-1);
        return 1;
    }

    Customer *head = NULL;
    process_file(argv[1], &head);
    print_list(head);
    while (1) 
    {   
        handle_input(head, argv[1]);
    }
    free_list(head);

    return 0;
}