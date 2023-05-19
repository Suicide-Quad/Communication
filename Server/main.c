#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include "communication.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/types.h>

#define PORT_TCP 2048
#define SA struct sockaddr

#define BUFFER_SIZE 131072 
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente
#define DELIMITER ":"
#define DELIMITER_END ";"

int connfd;

void CatchSignal(int sig)
{
    printf("Catch\n");
    close(connfd);
    exit(0);
}

void func(int connfd)
{
    printf("Infinite Loop\n");
    uint8_t buff[BUFFER_SIZE];
    // infinite loop for chat
    for (;;) {
        bzero(buff, BUFFER_SIZE);
   
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        if (buff[0] != 0)
        {
        // print buffer which contains the client contents

            receiveRequest(buff);
            //write(connfd, buff, sizeof(buff));
            bzero(buff, BUFFER_SIZE);
        }
    }
}


// Driver function
int main()
{
    signal(SIGINT, CatchSignal);
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT_TCP);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    addrlen = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &addrlen);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);
}
