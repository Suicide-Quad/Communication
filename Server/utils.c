#include "utils.h"
#include <stdio.h>

uint8_t computeCheckSum(uint8_t* data, uint32_t size)
{
	uint8_t sum = 0;
	for (int i = 0; i < size ; i ++)
	{
		sum += data[i];
	}
	return sum % 255 ; 
}


uint8_t getSizeType(TypeRequest actualType)
{
    switch (actualType)
    {
        case NONE:
            return 0;
        case ACK:
            return 1;
        case RESPONSE_POSITION:
            return 3;
        case DEBUG_POSITION:
            return 17;
        case DEBUG_INT:
            return 5;
        case DEBUG_FLOAT:
            return 9;
        case ASK_POSITION:
            return 1;
        default: 
            return -1;
    }
}

