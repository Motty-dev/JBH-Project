#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "utils.h"

void insert_in_order(Customer *c, Customer **head) 
{
    Customer *temp = *head;
    Customer *prev = NULL;

    while (temp != NULL && temp->debt < c->debt) {
        prev = temp;
        temp = temp->next;
    }

    if (prev == NULL) {
        c->next = *head;
        *head = c;
    } else {
        c->next = prev->next;
        prev->next = c;
    }
}

void build_list(Customer c, Customer **head) 
{
    Customer *temp = *head;
    Customer *prev = NULL;
    int found = 0;
    // check if customer is already in the list (with id and name)
    while (temp != NULL) {

        if (strcasecmp(temp->id_number, c.id_number) == 0 && strcasecmp(temp->first_name, c.first_name) == 0 && strcasecmp(temp->last_name, c.last_name) == 0) {
            found = 1;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (found) {
        // update customer's debt and date 
        temp->debt += c.debt;
        if (compare_date(c.date, ">", temp->date))
        {
            strcpy(temp->date, c.date);
        }
        // remove node from list and re-insert in correct position
        if (prev == NULL) {
            *head = temp->next;
        } else {
            prev->next = temp->next;
        }
        insert_in_order(temp, head);
    } else {
        // create new node and insert in correct position (by debt) 
        // NOTE: casting is not a must. i used it for safty.
        Customer *new_node = (Customer*)malloc(sizeof(Customer));
        *new_node = c;
        insert_in_order(new_node, head);
    }
}

void print_list(Customer *head, void(*cb)(char *, int), int server_mode) 
{
    cb("\n******************************************************************************\n", server_mode);
    cb("***************                                                ***************\n", server_mode);
    cb("************              List of customers's debts               ************\n", server_mode);
    cb("*********                                                            *********\n", server_mode);
    cb("+-----+----------------+------------+----------------+----------+------------+\n", server_mode);
    cb("| No. | Name           | ID         | Phone          | Debt     | Date       |\n", server_mode);
    cb("+-----+----------------+------------+----------------+----------+------------+\n", server_mode);

    int i = 1;
    Customer *temp = head;

    while (temp != NULL) {
        printf("| %-3d | %-5s %-8s | %-10s | %-12s   | %-8.2lf | %-10s |\n", i++, temp->first_name, temp->last_name, temp->id_number, temp->phone, temp->debt,temp->date);
        printf("+-----+----------------+------------+----------------+----------+------------+\n");
        temp = temp->next;
    }
    puts("");
}

void free_list(Customer *head)
{
    while (head)
    {
        Customer *temp = head->next;
        free(head);
        head = temp;
    }
}