#ifndef _ENCODER_MESSAGE_H
#define _ENCODER_MESSAGE_H
#include "utils.h" 

void initCommunication(UART_HandleTypeDef* uartESP, UART_HandleTypeDef* uartUSB);
void sendData(TypeRequest actualType, uint8_t*  payload);
#endif 
