#include "Message/proccesor.h" 
#include "utils.h" 
#include "main.h" 

//fonction pour avoir taille getSize()

void processMessage(TypeRequest actualType, uint8_t* payload)
{
    switch (actualType)
    {
        case RESPONSE_POSITION: 
            uint8_t id = payload[2];
            SwitchId(id); 
            break;
        default: 
            break;
    }
}

