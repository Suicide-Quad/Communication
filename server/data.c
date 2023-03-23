#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include "data.h"

struct info *data(int fd, const void *buf, size_t count){

    size_t nbyte = 0;
    size_t i = 0;
    char *toadd = "";
    struct info datatosend = malloc(sizeof(struct info));
    datatosend->test = "";
    datatosend->test2 = "";
    while(count > nbyte){
        // on va check si on a un ":" dans le buffer
        if (buf[i] == ':'){
            if (i == 0){
                datatosend->test = toadd;
                toadd = "";
                i+= 1;
            }
            if (i == 1){
                datatosend->test2 = toadd;
                toadd = "";
                i+= 1;
            }
        }
        else{
            toadd += buf[i];
        }
    }
    return datatosend;
}