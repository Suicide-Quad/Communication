#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <stdint.h>

/*___INCLUDE___*/


/*___DEFINE___*/

#define START_REQUEST 0xFE
#define SIZE_START 8

#define SIZE_TYPEFRAME 8

#define SIZE_CHECKSUM 8

#define SIZE_REQUEST(data) (SIZE_START + SIZE_TYPEFRAME + (data) + SIZE_CHECKSUM )/8

#define FINISH 1

#define NOT_FINISH 0

#define FLOAT_PRECISION 100000

/*___Struct and Enum___*/

enum TypeFrame
{
	NONE = -1,
	ASK_POSITION = 0,
	ACK = 1,
	RESPONSE_POSITION = 2,
	DEBUG_POSITION = 3,
	DEBUG_INT = 4,
	DEBUG_FLOAT = 5,
};

typedef struct PositionCommand
{
	uint8_t x;
	uint8_t y;
}positionCommand;



/*___FUNCTION___*/

//receive request and manage all
void receiveData(enum TypeFrame type, uint8_t* pointBuffer, uint8_t* sizeReadBuffer, char buffer[], int buff_size);
void receiveRequest(char buffer[], int buff_size);

//get the ID of the sended image, 255 if not receive response, and 254 if is not found ArUco on image
uint8_t getId();
//get the local position of the ArUCo (from the robot)
struct PositionCommand getPositionArUco();

#endif 
