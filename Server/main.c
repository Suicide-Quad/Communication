#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "communication.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define PORT_TCP 8080
#define SA struct sockaddr

#define BUFFER_SIZE 0x14 
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente
#define DELIMITER ":"
#define DELIMITER_END ";"

#define HOSTNAME "127.0.0.1" 
#define PORT 42769 // port teleplot 

int send_Udp(char* request)
{
    char dest[strlen(request) + 1];
    memset(&dest,0,sizeof(dest));
    strcat(dest, request);
    struct sockaddr_in servaddr;
    int fd = socket(AF_INET,SOCK_DGRAM,0);
    if(fd<0){
        perror("cannot open socket");
        return 1;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOSTNAME);
    servaddr.sin_port = htons(PORT);
    if (sendto(fd, dest , strlen(dest) + 1, 0, // +1 to include terminator
               (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("cannot send message");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

void func(int connfd)
{
    char buff[BUFFER_SIZE];
    // infinite loop for chat
    for (;;) {
        bzero(buff, BUFFER_SIZE);
   
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        if (buff[0] != 0)
        {
        // print buffer which contains the client contents
            printf("From client\n");
            for(int i = 0; buff[i] != 0; i++)
            {
                printf("%.6d: %hhx\n", i, buff[i]);
            } 
            write(connfd, buff, sizeof(buff));
            bzero(buff, BUFFER_SIZE);
        }
    }
}


// Driver function
int main()
{
    int sockfd, connfd;
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
