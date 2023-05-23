#include "decoder.h" 
#include <string.h>
#include <stdio.h>
#include "proccesor.h" 
#include "utils.h" 

StateReceive actualStatement = WAITING;
uint8_t payload[BUFF_SIZE];
TypeRequest actualType;
uint32_t sizePayload;
uint32_t indexPayload = 0;

TypeRequest receivedToEnum(uint8_t received)
{
    if (received < MAX_TYPE_REQUEST)
        return received;
    printf("Wrong type\n");
    return 0;
}

void receiveByte(uint8_t received)
{
    if (actualStatement == WAITING && received == START_REQUEST)
    {
        actualStatement = TYPE;
        return ;
    }
    if (actualStatement == TYPE)
    {
        actualType = receivedToEnum(received);
        actualStatement = PAYLOAD;
        sizePayload = getSizeType(actualType);
        memset(payload, 0, BUFF_SIZE);
        return ;
    }
    if (actualStatement == PAYLOAD && sizePayload > indexPayload)
    {
        payload[indexPayload] = received;
        indexPayload++;
        return ;
    }
    else if (actualStatement == PAYLOAD)
    {
        // Actual statement == SUM
        if (received == computeCheckSum(payload, sizePayload))
        {
            processMessage(actualType, payload);
        }
        else 
        {
            printf("Wrong checksum %d %d\n",received, computeCheckSum(payload, sizePayload));
        }
        indexPayload = 0;
        actualType = NONE;
        actualStatement = WAITING;
    }
}
