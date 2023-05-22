#include "decoder.h" 
#include "proccesor.h" 
#include "utils.h" 

StateReceive actualStatement = WAITING;
uint8_t payload[BUFF_SIZE];
TypeRequest actualType;
uint32_t sizePayload;
uint32_t indexPayload;

TypeRequest receivedToEnum(uint8_t received)
{
    if (received < MAX_TYPE_REQUEST)
        return received;
    return 0;
}

void receiveByte(uint8_t received)
{
    if (actualStatement == WAITING && received == START_REQUEST)
    {
        actualStatement = SOF;
        return ;
    }
    if (actualStatement == TYPE)
    {
        actualType = receivedToEnum(received);
        actualStatement = PAYLOAD;
        sizePayload = getSizeType(actualType);
        return ;
    }
    if (actualStatement == PAYLOAD && sizePayload > indexPayload)
    {
        payload[indexPayload] = received;
        received++;
        return ;
    }
    else if (actualStatement == PAYLOAD)
    {
        // Actual statement == SUM
        if (received == computeCheckSum(payload, sizePayload))
        {
            processMessage(actualType, payload);
        }
        indexPayload = 0;
        actualType = NONE;
        actualStatement = WAITING;
    }
}
