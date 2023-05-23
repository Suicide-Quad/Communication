#include "generator.h"
#include <string.h>
#include "send.h"
#include <stdio.h>
#include <math.h>
#include "main.h"

void getBytesFromInt(uint8_t* payload, uint8_t index, int val)
{
    memcpy((void*)(payload+index),(void*) &val, sizeof(int));
}

void sendDebugInt(int val, char id)
{
	uint8_t payload [getSizeType(DEBUG_INT)];

	getBytesFromInt(payload, 0, val);
	payload[4] = id;

    sendData(DEBUG_INT, payload);
}

void sendDebugDouble(double val, char id)
{
	uint8_t payload [getSizeType(DEBUG_FLOAT)];

	int vp = trunc(val);
	int vn = trunc(val*FLOAT_PRECISION)-vp*FLOAT_PRECISION;

	getBytesFromInt(payload,0,vp);
	getBytesFromInt(payload,4,vn);

	payload[8] = id;

    sendData(DEBUG_FLOAT, payload);
}

void sendDebugPosition(double x, double y, char id)
{
	uint8_t payload [getSizeType(DEBUG_POSITION)];

	int vxp = trunc(x);
	int vxn = trunc(x*FLOAT_PRECISION)-vxp*FLOAT_PRECISION;

	getBytesFromInt(payload,0,vxp);
	getBytesFromInt(payload,4,vxn);

	int vyp = trunc(y);
	int vyn = trunc(y*FLOAT_PRECISION)-vyp*FLOAT_PRECISION;

	getBytesFromInt(payload,8,vyp);
	getBytesFromInt(payload,12,vyn);

	payload[16] = id;

    sendData(DEBUG_POSITION, payload);
}

void sendAskPosition()
{
	uint8_t payload [getSizeType(ASK_POSITION)];

	payload[0] = 0;

    sendData(ASK_POSITION, payload);
}

void sendResponsePosition(PositionCommande commande, uint8_t id)
{
	uint8_t payload [getSizeType(RESPONSE_POSITION)];

	//getBytesFromInt(payload, -1, commande.x);
	getBytesFromInt(payload, 0, commande.x);
	getBytesFromInt(payload, 4, commande.y);

	payload[8] = id;

    sendData(RESPONSE_POSITION, payload);
}

void sendAck()
{
	uint8_t payload [getSizeType(ACK)];

	payload[0] = 1;
    
    sendData(ACK, payload);
}
