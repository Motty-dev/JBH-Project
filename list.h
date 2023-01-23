#ifndef LIST_H
#define LIST_H

typedef struct customer {
    char first_name[30];
    char last_name[30];
    char id_number[10];
    char phone[11];
    float debt;
    char date[11];
    struct customer *next;
} Customer;

void free_list(Customer *head);
void print_list(Customer *head);
void build_list(Customer c, Customer **head);
void insert_in_order(Customer *c, Customer **head);

#endif