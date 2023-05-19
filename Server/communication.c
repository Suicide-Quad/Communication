#include "communication.h"
#include <stdio.h>
#include <stdlib.h>
#include "udp.h" 

int SizeTypeFrame [7] = {0,8,24,136,40,72,131072};
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

uint8_t computeCheckSum(int size, uint8_t* data)
{
	uint8_t sum = 0;
	for (int i = 0; i < size ; i ++)
	{
		sum += data[i];
	}
	return sum % 256;
}

void receiveStartBlock(uint8_t* start, uint8_t buffer[], uint8_t* pointBuffer)
{
	if ((uint8_t)buffer[*pointBuffer] == 254)
	{
		*start = 1;
	}
	(*pointBuffer) ++;
}

void receiveType(uint8_t* data, enum TypeFrame type)
{
	double z = 0;
    char* udpRequest = malloc(sizeof(char) * 32);
    char name;
	switch (type)
	{ 
        case DEBUG_POSITION:
            double coo[2] = {0, 0};
            for(int i = 0; i < 2; i++)
            {
                z = 0;
                coo[i] = data[3 + i * 8] << 24 ;
                coo[i] += data[2 + i * 8] << 16;
                coo[i] += data[1 + i * 8] << 8;
                coo[i] += data[0 + i * 8];
                z = data[7 + i * 8] << 24 ;
                z += data[6 + i * 8] << 16;
                z += data[5 + i * 8] << 8;
                z += data[4 + i * 8];
                coo[i] += z / 1000000;
            }
            name = data[16];
            printf("f = %f\n", coo[0]);
            printf("f = %f\n", coo[1]);
            printf("f = %c\n", name); 
            sprintf(udpRequest, "%c:%f:%f|xy",name, coo[0], coo[1]);
            sendUdp(udpRequest);
            break;
        case ASK_POSITION: 
            printf("image\n");
            FILE* fptr = fopen("file.test","w");
            fwrite(&data, sizeof(char), getSizeTypeFrame(type) / 8, fptr);
            fclose(fptr); 
            break;
        case DEBUG_INT: 
            int x = 0;
            x = data[3] << 24 ;
            x += data[2] << 16;
            x += data[1] << 8;
            x += data[0]; 
            name = data[5];
            printf("x = %d\n", x);
            printf("f = %c\n", name); 
            sprintf(udpRequest, "%c:%d",name, x);
            sendUdp(udpRequest);
            break;
        case DEBUG_FLOAT:
            double y = 0;
            y = data[3] << 24 ;
            y += data[2] << 16;
            y += data[1] << 8;
            y += data[0];
            z = data[7] << 24 ;
            z += data[6] << 16;
            z += data[5] << 8;
            z += data[4];
            y += z / 1000000;
            name = data[8];
            printf("f = %f\n", y);
            printf("f = %f\n", z);
            printf("f = %c\n", name); 
            sprintf(udpRequest, "%c:%f",name, y);
            sendUdp(udpRequest);
            break;
        default:
            break;
    }
}

//catch data and compute things to do
void receiveData(enum TypeFrame type, uint8_t* buffer)
{
	int size = getSizeTypeFrame(type);
	size /= 8;
	for(int i = 0; i < size; i++)
	{
		printf("%.6d : %hhx\n",i, buffer[i]);
	} 
	if (computeCheckSum(size, buffer) == *(buffer + size))
	{
		receiveType(buffer, type);
	}
	else 
	{
		printf("Checksum Invalid\n");
	}

}

void receiveRequest(uint8_t buffer[])
{
	uint8_t start = 0;
	enum TypeFrame type = NONE;
	uint8_t pointBuffer = 0;
	receiveStartBlock(&start, buffer, &pointBuffer);
	type = buffer[pointBuffer];
	printf("Type %d\n", type);
	if (start)
	{
		buffer+=2;
		receiveData(type, buffer);
	}
	else 
	{
		printf("No start\n");
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
