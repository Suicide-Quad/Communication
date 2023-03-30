#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 32;

void rewrite(int fd, const void *buf, size_t count)
{
    int wr = write(fd, buf, count);
    if (wr == -1){
	    err (3, "error on writing");
    }
    else if (wr == 0){
	    err(3, "nothing to write");
    }
    else if (wr < (int)count){

	    while (wr < (int)count){
		    wr += write(fd, buf, count);
	    }
    }
}

char *build_query(const char *host, size_t *len)
{
    char *query;
    int asp = asprintf(&query, "GET http://www.%s/ HTTP/1.0\r\n\r\n", host);
    if (asp == -1){
        err(3, "error on asprintf");
    }
    *len = asp;
    return query;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>

#define SERVER_PORT "2048" // port utilisé pour la connexion

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* server_hostname = argv[1];
    struct addrinfo hints, *server_info, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(server_hostname, SERVER_PORT, &hints, &server_info)) != 0) {
        errx(EXIT_FAILURE, "%s", gai_strerror(status));
    }

    int client_socket;
    for (p = server_info; p != NULL; p = p->ai_next) {
        if ((client_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (connect(client_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(client_socket);
            continue;
        }

        break;
    }

    if (p == NULL) {
        errx(EXIT_FAILURE, "Failed to connect to server");
    }

    freeaddrinfo(server_info);

    char* message = "Hello, world!";
    if (send(client_socket, message, strlen(message), 0) == -1) {
        err(EXIT_FAILURE, "Failed to send message to server");
    }

    close(client_socket);
    exit(EXIT_SUCCESS);
}
