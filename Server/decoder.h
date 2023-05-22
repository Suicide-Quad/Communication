#ifndef _DECODER_H
#define _DECODER_H
#include "main.h" 
#include "utils.h" 

TypeRequest receivedToEnum(uint8_t received);

void receiveByte(uint8_t received);

#endif
