#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



void process_file(char *file_name, Customer **head) 
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) 
    {
        Customer_error err = {0, ""};
        Customer curr = parse_line(line, &err);
        if (!err.error) {
            build_list(curr, head);
            correct_lines_count++;
        } else {
            strcpy(incorrect_lines[incorrect_lines_count], err.message);
            incorrect_lines_count++;
        }
        total_lines++;
    }
    if(incorrect_lines_count == 0) {
        printf("\n\nProcessed %d out of %d lines successfully.\n", correct_lines_count, total_lines);
    }
    else {
        printf("\n\nProcessed %d out of %d lines successfully, and %d lines with the following errors:\n", correct_lines_count, total_lines, incorrect_lines_count);
        print_error_messages(incorrect_lines, incorrect_lines_count);
    }

    fclose(file);
}