#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "customer_manager.h"
#include "csv_parser.h"
#include "queries.h"

typedef struct thread_args {
    Customer **head; 
    char* file_name; 
    int new_sock; 
} thread_args;

#define MAX_LEN 1024

void send_cb(char *buffer, int new_sock) 
{
    // send string through socket
    if (send(new_sock, buffer, strlen(buffer), 0) < 0) 
    {
        perror("Send failed");
        return;
    }
}

void *conn_handler(void *args)
{
    char buffer[MAX_LEN];
    int n;
    thread_args *targs = args;
    
    n = recv(targs->new_sock, buffer, MAX_LEN, 0);
    if (n < 0)
    {
        perror("Server error receiving data");
        goto exit;
    }
    buffer[n] = '\0';
    printf("Server received: %s\n", buffer);
    
    //buffer logic here - string from client
    handle_input(*targs->head, buffer, targs->file_name, &send_cb, targs->new_sock);
exit:
    close(targs->new_sock);
    return NULL;
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    if (argc < 3)
    {
        puts("Usage: ./<file_name> <port> <file_name.txt>\n");
        return 1;
    }
    // process file and create the list 
    Customer *head = NULL;
    int server_flag = 0;
    process_file(argv[2], &head, server_flag);

    // create a socket 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    // bind the socket to a specific port 
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error binding socket");
        return 1;
    }

    if (listen(sockfd, 5) < 0)
    {
        perror("Error listenning");
        return 1;
    }

    // receive data from clients
    while (1)
    {
        pthread_t tid;
        int new_sock = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t*)&len);
        if (new_sock < 0)
        {
            perror("accept failed");
            return 1;
        }

        thread_args args = {
            .head = &head,
            .file_name = argv[1],
            .new_sock = new_sock
        };

        pthread_create(&tid, NULL, conn_handler, &args);
        pthread_join(tid, NULL);
    }

    return 0;
}