#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 512
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente
#define DELIMITER ":"

#define HOSTNAME "127.0.0.1" 
#define PORT 47269

struct Data{
    char name[20];
    char value[20];
};

int send_Udp(char* name, char *position)
{
    printf("Sending UDP message to client\n");
    char dest[strlen(position)+strlen(name) + 1];
    memset(&dest,0,sizeof(dest));
    strcat(dest, name);
    dest[strlen(name)] = ':';
    strcat(dest,position);
    struct sockaddr_in servaddr;
    int fd = socket(AF_INET,SOCK_DGRAM,0);
    if(fd<0){
        perror("cannot open socket");
        return 1;
    }
    printf("%s\n", dest); 
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(HOSTNAME);
    servaddr.sin_port = htons(PORT);
    if (sendto(fd, dest , strlen(position)+strlen(name) + 1, 0, // +1 to include terminator
               (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("cannot send message");
        close(fd);
        return 1;
    }
    close(fd);
    //close(socket_desc);
    return 0;
}

void parsing(char* buffer, ssize_t bytes_received)
{ 
    printf("Parsing message from client\n");
    printf("Buffer in parsing : %s\n",buffer);

    buffer[bytes_received] = '\0';

    int i = 0;
    while(buffer[i] != 'n'){
        i++;
    }
    buffer[i] = '\0';
    buffer = buffer + i + 1;
    printf("Received message from client: %s\n", buffer);
    //char *position1 = strtok(buffer, DELIMITER);
    //buffer += strlen(position1) + 1;
    //char *position2 = strtok(buffer, DELIMITER);
    struct Data datalist[10];
    int datalen = 0;
    i = 0;

    while(buffer != NULL && buffer[0] != '\0'){
        char *name = strtok(buffer, DELIMITER);
        buffer += strlen(name) + 1;
        char *value = strtok(buffer, DELIMITER);
        buffer += strlen(value) + 1;

        printf("name = %s value = %s\n", name, value);

        //datalist[i] = malloc(sizeof(struct Data));
        strcpy(datalist[i].name, name);
        strcpy(datalist[i].value, value);
        i++;
        datalen++;
    }
    printf("datalen = %d\n", datalen);
    //printf("x = %s y = %s\n", position1, position2);

    for (int i = 0; i < datalen; i++)
    {
        printf("name = %s value = %s\n", datalist[i].name, datalist[i].value);
        send_Udp(datalist[i].name, datalist[i].value);
    }
    //send_Udp("test", position2);
}

void launch_socket()
{
    printf("Launching socket\n");
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
        printf("Received %zd bytes from client\n", bytes_received);
        printf("buffer = %s\n", buffer);
        parsing(buffer, bytes_received);
        close(client_socket);
    }
    close(server_socket);
}

int main(void){

    launch_socket();
}
