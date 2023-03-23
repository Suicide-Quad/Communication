#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <netdb.h> 
#include <unistd.h> 
#include <err.h> 
#include "data.h"

#define BUFFER_SIZE 512

void rewrite(int fd, const void *buf, size_t count)
{
    size_t nbyte = 0;
    while(count > nbyte)
    {
        int temp;
        temp = write(fd, buf, count);
        if (temp == -1)
            errx(1, "Error while writing the file");
        buf += temp;
        nbyte += temp;
    }
}

struct info *echo(int fd_in, int fd_out)
{
    char buffer[BUFFER_SIZE];
    int nbyte;
    while(1)
    {
        nbyte = read(fd_in, buffer, BUFFER_SIZE);
        if (nbyte == 0)
           break;
        if (nbyte == -1)
            errx(1, "Error while reading the file");
        return data(fd_out, buffer, nbyte);
    }
}

int main(int argc, char** argv) 
{     
    if (argc != 2)         
        errx(EXIT_FAILURE, "Usage:\n"             
                "Arg 1 = Port number (e.g. 2048)");     

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* results;
    int err = getaddrinfo(NULL, argv[1], &hints, &results);
    int sockfd;
    struct addrinfo *p;
    int option = 1;
    for(p = results; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
            continue;
            
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)))
            errx(1, "Error while reuse addr");
        err = bind(sockfd, p->ai_addr, p->ai_addrlen);

        if (err == 0)
            break;
        else 
            close(sockfd);
    }
    freeaddrinfo(results);
    if (p == NULL)
        errx(1, "p NULL");
    err = listen(sockfd, 5);
    if (err == -1)
    {
        close(sockfd);
        errx(1, "Listen failed");
    }
    printf("Waiting for connections...\n");
    while (1)
    {
        int clientfd = accept(sockfd, NULL, NULL);

        if (clientfd == -1)
        {
            errx(1, "Client doesnt connect");
        }
        printf("Connection successful!\n");
        struct info *data = echo(clientfd, clientfd);
        printf("Data received: %s %s\n", data->test, data->test2);
        close(clientfd);
        printf("Connection finish\n");
    }
    return 0;
}