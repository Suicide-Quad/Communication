#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include "decoder.h"
#include <sys/types.h>
#include "main.h"

#define PORT_TCP 2048
#define SA struct sockaddr
#define TIMEOUT 20 * 1000
#define BUFFER_SIZE (262144/8)
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

int getFD()
{
    return connfd;
}

void func(int connfd)
{
    printf("Reading the client\n");
    uint8_t buff[BUFFER_SIZE];
    // infinite loop for chat
    int begin = time(NULL);
    int sizeRead;
    for (;;) {
        bzero(buff, BUFFER_SIZE);
   
        // read the message from client and copy it in buffer
        sizeRead = read(connfd, buff, sizeof(buff));
        if (sizeRead != 0)
        {
        // print buffer which contains the client contents

            for(int i = 0; i < sizeRead; i++)
            {
                receiveByte(buff[i]);
            }
            //write(connfd, buff, sizeof(buff));
            bzero(buff, BUFFER_SIZE);
        }
        if (time(NULL) > TIMEOUT + begin)
        {
            printf("Refresh the client\n");
            return ;
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
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT_TCP);

	int optval = 1;
	setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
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

    for(;;)
    {
        // Accept the data packet from client and verification
        connfd = accept(sockfd, (SA*)&cli, &addrlen);
        if (connfd < 0) {
            printf("Server accept failed...\n");
            exit(0);
        }
        else
            printf("Server accept the client...\n");

        // Function for chatting between client and server
        func(connfd);

        // After chatting close the socket
        close(sockfd);
    }
}
