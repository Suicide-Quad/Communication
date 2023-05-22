#ifndef _GENERATOR_H
#define _GENERATOR_H
#include "utils.h"

#define FLOAT_PRECISION 1000000
void sendDebugInt(int val, char id);
void sendDebugDouble(double val, char id);
void sendPosition(double x, double y, char id);
void sendAskPosition();
void sendResponsePosition(PositionCommande commande, uint8_t id);
void sendAck();

#endif 
