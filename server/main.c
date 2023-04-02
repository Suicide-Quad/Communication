#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 512
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente
#define DELIMITER ":"

int send_Udp()
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char client_message[] = "myserver:12.3";
    char server_message[2000];
    int server_struct_length = sizeof(server_addr);
    
    // Clean buffers:
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(47269);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    
    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0,
         (struct sockaddr*)&server_addr, server_struct_length) < 0){
        printf("Unable to send message\n");
        return -1;
    }

    if(recvfrom(socket_desc, server_message, sizeof(server_message), -1,
                (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        printf("Error while receiving server's msg\n");
        return -1;
    }
    close(socket_desc);
    return 0;
}

void parsing(char* buffer, ssize_t bytes_received)
{ 
        buffer[bytes_received] = '\0';
        buffer = strchr(buffer,'\n');
        buffer++;
        printf("Received message from client: %s\n", buffer);
        char *position1 = strtok(buffer, DELIMITER);
        char *position2 = strtok(buffer, DELIMITER);
        printf("x = %s y = %s\n", position1, position2);
        send_Udp();
}

void launch_socket()
{
    struct addrinfo hints, *server_info, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, SERVER_PORT, &hints, &server_info)) != 0) {
        errx(EXIT_FAILURE, "%s", gai_strerror(status));
    }

    int server_socket;
    for (p = server_info; p != NULL; p = p->ai_next) {
        if ((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        int yes = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            err(EXIT_FAILURE, "Failed to set socket options");
        }

        if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_socket);
            continue;
        }

        break;
    }

    if (p == NULL) {
        errx(EXIT_FAILURE, "Failed to bind server socket");
    }

    freeaddrinfo(server_info);

    if (listen(server_socket, BACKLOG) == -1) {
        err(EXIT_FAILURE, "Failed to listen for connections");
    }

    printf("Server waiting for connections on port %s...\n", SERVER_PORT);

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;

    int client_socket;
    char buffer[256];
    ssize_t bytes_received;

    while (1) 
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            err(EXIT_FAILURE, "Failed to accept connection");
        }

        bytes_received = recv(client_socket, buffer, sizeof buffer, 0);
        if (bytes_received == -1) {
            err(EXIT_FAILURE, "Failed to receive message from client");
        }
        parsing(buffer, bytes_received);
        close(client_socket);
    }
    close(server_socket);
}

int main(void){

    launch_socket();
}
