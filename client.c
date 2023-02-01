#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_LEN 1024
#define MAX_QUERIES 5

int main(int argc, char **argv)
{
    int sockfd[MAX_QUERIES];
    struct sockaddr_in servaddr;
    char buffer[MAX_QUERIES][MAX_LEN];
    int i, n, num_queries = 0;

    if (argc < 3)
    {
        printf("Usage: %s %s %s <port> <ip address>\n", argv[0], argv[1], argv[2]);
        return 1;
    }

    // connect to the server *
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[2]);
    servaddr.sin_port = htons(atoi(argv[1]));

    // send data to the server 
    while (1)
    {

        printf("Enter up to 5 queries, separated by newline (press send or quit to terminate):\n");
        for (i = 0; i < 6; i++) 
        {
            char tmp_buf[1024];
            fgets(tmp_buf, MAX_LEN, stdin);
            if (strncmp(tmp_buf, "send", 4) == 0) {
                break;
            } else if (strncmp(tmp_buf, "quit", 4) == 0) {
                return 0;
            } else if (i == 5){
                puts("Error: expected send after 5 queries");
                return 0;
            }
            strcpy(buffer[i], tmp_buf);
            
        }
        num_queries = i;
        
        if (i == 0) {
            printf("Error: You must enter at least one query\n");
            continue;
        }

        // create the sockets 
        for (i = 0; i < num_queries; i++)
        {
            sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd[i] < 0)
            {
                perror("Error creating socket");
                return 1;
            }
            if (connect(sockfd[i], (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
            {
                perror("Error connecting");
                return 1;
            }
        }

        for (i = 0; i < num_queries; i++)
        {
            int r = 0;
            strtok(buffer[i], "\n");
            n = send(sockfd[i], buffer[i]+r, strlen(buffer[i]), 0);
            if (n < 0)
            {
                perror("Client error sending data");
                return 1;
            }
        }

        for (i = 0; i < num_queries; i++)
        {
            while (1)
            {
                n = recv(sockfd[i], buffer[i], MAX_LEN, 0);
                if (n < 0)
                {
                    perror("Client error receiving data");
                    return 1;
                }else if (n == 0)
                {
                    break;
                }
                
                buffer[i][n] = '\0';
                printf("%s\n", buffer[i]);  
            }

            close(sockfd[i]);
        }
    }    
    return 0;
}