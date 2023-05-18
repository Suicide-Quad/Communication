#include "communication.h"
#include <stdio.h>

int SizeTypeFrame [6] = {131072,8,24,16,32,64};
/*___GLOBAL-VAR___*/

//ID of the ArUco
uint8_t IDArUco;

//Local Position of ArUco (from the robot)
struct PositionCommand PositionArUco;

int getSizeTypeFrame (enum TypeFrame type)
{
	if (type == NONE)
		return 0;
	return SizeTypeFrame[type];
}

// TODO : Merge with utils 
uint8_t computeCheckSum(int size, char* data)
{
	uint8_t sum = 0;
	for (int i = 0; i < size ; i ++)
	{
		sum += data[i];
	}
	return sum % 256;
}

void receiveStartBlock(uint8_t* start, char buffer[], uint8_t* pointBuffer, int buff_size)
{
	while (!*start && *pointBuffer < buff_size)
	{
		if ((enum TypeFrame)buffer[*pointBuffer] == START_REQUEST)
		{
			*start = 1;
		}
		(*pointBuffer) ++;
	}
}

void receiveType(char* data, enum TypeFrame type)
{
	if (type == RESPONSE_POSITION)
	{
		IDArUco = data[0];
		PositionArUco.x = data[1];
		PositionArUco.y = data[2];
	}
	else if (type == ASK_POSITION)
	{
		printf("image\n");
	}
    // TODO : Add picture
}

//catch data and compute things to do
void receiveData(enum TypeFrame type, uint8_t* pointBuffer, uint8_t* sizeReadBuffer, char* buffer, int buff_size)
{
	buffer += 2;
	int size = getSizeTypeFrame(type);
	size /= 8;
	if (computeCheckSum(size, buffer) == *(buffer + size))
	{
		receiveType(buffer, type);
	}

}

void receiveRequest(char buffer[], int buff_size)

{
	printf("%s",buffer);
	static uint8_t start = 0;
	static enum TypeFrame type = NONE;
	static uint8_t sizeReadData = 0; //in bytes calculted in the start of data


	uint8_t pointBuffer = 0;
	receiveStartBlock(&start, buffer, &pointBuffer, buff_size);
	if (type == NONE && pointBuffer < buff_size)
	{
		type = buffer[pointBuffer];
		pointBuffer ++;
	}
	if (pointBuffer < buff_size)
	{
		receiveData(type, &pointBuffer, &sizeReadData, buffer, buff_size);
	}
}


uint8_t getId()
{
	return IDArUco;
}

struct PositionCommand getPositionArUco()
{
	return PositionArUco;
}
