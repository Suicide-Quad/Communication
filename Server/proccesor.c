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
            uint8_t number = 0;
            number += payload[0] << 24;
            number += payload[0] << 16;
            number += payload[0] << 8;
            number += payload[0] ;

            uint8_t name = payload[4];
            sprintf(buffer, "%d:%c", number, name); 
            sendUdp(buffer);
        default: 
            break;
    }
}

