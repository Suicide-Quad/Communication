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

void print_page(const char *host)
{
    char buffer[BUFFER_SIZE];

    //char buf[] = "Coucou" ;
    //rewrite(STDIN_FILENO, buf, 6);
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *res;
    int gai = getaddrinfo(host, "80", &hints, &res);
    if (gai != 0 && res == NULL){
        errx(3, "error on getaddrinfo");
    }

    int fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd == -1){
        errx(3, "error on socket");
    }

    int con = connect(fd, res->ai_addr, res->ai_addrlen);
    if (con == -1){
        errx(3, "error on connect");
    }

    size_t len = strlen(host);
    char *query = build_query(host, &len);
    rewrite(fd, query, len);

    int rd = read(fd, buffer, BUFFER_SIZE);

    while (rd > 0){
        write(1, buffer, rd);
        rd = read(fd, buffer, BUFFER_SIZE);
    }

    freeaddrinfo(res);
    close(fd);
}