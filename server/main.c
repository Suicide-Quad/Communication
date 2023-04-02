#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>

#define BUFFER_SIZE 512
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente
#define DELIMITER ':'

void parsing(char* buffer, ssize_t bytes_received)
{ 
        buffer[bytes_received] = '\0';
        printf("Received message from client: %s\n", buffer);

        char *position1;
        char *position2;
        int flag = 0;
        for (int i = 0; i < bytes_received; i++) {
            if (buffer[i] == DELIMITER){
                flag = 1;
            }
            else
            {
                if (flag){
                    position1+=buffer[i];
                }
                else {
                    position2+=buffer[i];
                }
            }
        }
        printf("x = %d, y = %d\n", atoi(position1), atoi(position2));
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
