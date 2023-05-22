#include "communication.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "udp.h" 
#include "main.h"

int SizeTypeFrame [7] = {0,8,24,136,40,72,32};
/*___GLOBAL-VAR___*/

//ID of the ArUco
uint8_t IDArUco;

//Local Position of ArUco (from the robot)
struct PositionCommand PositionArUco;

//for now if image was send
int receivedImage = 0;
int sizeImage = 0;
FILE* fptr = NULL;

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

void receiveImage(uint8_t*data)
{
    /*
    if (receivedImage)
    {
        char* path = "file.jpg";
        if (fptr == NULL)
        {
            fptr = fopen(path,"w");
        }
        for(int i = 0; i < PACKET_MAX && sizeImage > 0; i ++)
        {
            fputc(data[i],fptr);
            sizeImage --;
        }
        if (sizeImage <= 0)
        {
            receivedImage = 0;
            sizeImage = 0;
            printf("Save image to %s\n",path);
            fclose(fptr);
            fptr = NULL;*/
            struct PositionCommand response = {12, 14};
            // call Davi
            // int id = 
            uint8_t id = 1;
            uint8_t request[6] = {START_REQUEST, 0x2, id, response.x, response.y, 0};
            request[5] = computeCheckSum(3, &request[2]);
            int connfd = getFD();
            write(connfd, request, sizeof(request));
/*
        }
    }
    else
    {
        receivedImage = 1;
        sizeImage = data[3] << 24 ;
        sizeImage += data[2] << 16;
        sizeImage += data[1] << 8;
        sizeImage += data[0];
        printf("next send was image of size : %d\n",sizeImage);
    }*/
}

void receiveType(uint8_t* data, enum TypeFrame type)
{
	double z = 0;
    char* udpRequest = malloc(sizeof(char) * 32);
    uint8_t name;
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
            printf("x = %f\n", coo[0]);
            printf("y = %f\n", coo[1]);
            printf("name = %c\n", name); 
            sprintf(udpRequest, "%c:%f:%f|xy",name, coo[0], coo[1]);
            sendUdp(udpRequest);
            break;
        case ASK_POSITION:
            receiveImage(data);
            break;
        case DEBUG_INT: 
            int x = 0;
            x = data[3] << 24 ;
            x += data[2] << 16;
            x += data[1] << 8;
            x += data[0]; 
            name = data[4];
            printf("x = %d\n", x);
            printf("name = %c\n", name); 
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
            printf("name = %c\n", name); 
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
	if (receivedImage)
	{
		receiveType(buffer, type);
	}
	else
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
}

void receiveRequest(uint8_t buffer[])
{
	if (receivedImage)
	{
		receiveData(ASK_POSITION, buffer);
	}
	else
	{
		uint8_t start = 0;
		enum TypeFrame type = NONE;
		uint8_t pointBuffer = 0;
		receiveStartBlock(&start, buffer, &pointBuffer);
		type = buffer[pointBuffer];
		printf("Type %d\n", type);
		if (start)
		{
			buffer+= (receivedImage ? 0 : 2);
			receiveData(type, buffer);
		}
		else 
		{
			printf("No start\n");
		}
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
