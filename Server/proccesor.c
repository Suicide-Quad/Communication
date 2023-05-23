#include "proccesor.h" 
#include "utils.h" 
#include "main.h" 
#include "send.h"
#include "udp.h" 
#include <stdio.h>

void processMessage(TypeRequest actualType, uint8_t* payload)
{
    char buffer[BUFF_SIZE];
    double z;
    switch (actualType)
    {
        case ASK_POSITION: 
            printf("Ask Position\n");
			PositionCommande relativeCommande = {1,2};
			uint8_t idAr = 3;
			sendResponsePosition(relativeCommande,idAr);
            break;
        case DEBUG_INT:
            uint32_t number = 0;
            number += payload[3] << 24;
            number += payload[2] << 16;
            number += payload[1] << 8;
            number += payload[0] ;

            uint8_t name = payload[4];
            sprintf(buffer, "%c:%d",name, number); 
            sendUdp((uint8_t*)buffer);
        case DEBUG_FLOAT:
            double y = 0;
            y = payload[3] << 24 ;
            y += payload[2] << 16;
            y += payload[1] << 8;
            y += payload[0];
            z = payload[7] << 24 ;
            z += payload[6] << 16;
            z += payload[5] << 8;
            z += payload[4];
            y += z / 1000000;
            name = payload[8];
            sprintf(buffer, "%c:%f",name, y);
            sendUdp((uint8_t*)buffer);            
            break;
        case DEBUG_POSITION:
            double coo[2] = {0, 0};
            for(int i = 0; i < 2; i++)
            {
                z = 0;
                coo[i] = payload[3 + i * 8] << 24 ;
                coo[i] += payload[2 + i * 8] << 16;
                coo[i] += payload[1 + i * 8] << 8;
                coo[i] += payload[0 + i * 8];
                z = payload[7 + i * 8] << 24 ;
                z += payload[6 + i * 8] << 16;
                z += payload[5 + i * 8] << 8;
                z += payload[4 + i * 8];
                coo[i] += z / 1000000;
            }
            name = payload[16];
            sprintf(buffer, "%c:%f:%f|xy",name, coo[0], coo[1]);
            sendUdp((uint8_t*)buffer);
            break;
        default: 
            break;
    }
}

