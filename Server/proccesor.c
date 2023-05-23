#include "proccesor.h" 
#include "utils.h" 
#include "main.h" 
#include "send.h"
#include "udp.h" 
#include <stdio.h>

void processMessage(TypeRequest actualType, uint8_t* payload)
{
    char buffer[BUFF_SIZE];
    switch (actualType)
    {
        case ASK_POSITION: 
            // TODO : Compute the coo 
            printf("Ask Position\n");
            break;
        case DEBUG_INT:
            uint32_t number = 0;
            number += payload[3] << 24;
            number += payload[2] << 16;
            number += payload[1] << 8;
            number += payload[0] ;

            uint8_t name = payload[4];
            sprintf(buffer, "%c:%d",name, number); 
            sendUdp(buffer);
        default: 
            break;
    }
}

