#ifndef _UDP_H
#define _UDP_H
#include <stdint.h>
#define HOSTNAME "127.0.0.1" 
#define PORT 47269 // port teleplot 

void sendUdp(uint8_t* request);
#endif 
