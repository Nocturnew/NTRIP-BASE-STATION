#ifndef UBLOX_H
#define UBLOX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "SerialPort.h"

typedef unsigned char byte;
typedef struct tm Datetime;

typedef struct
{
	byte svID;
	byte gnssID;
	byte sigID;
} SV;

typedef struct
{
	int 		spDesc;
	char 		*portName;
	int 		baudRate;

	byte 		*receiveBuffer;
	int 		receiveBufferLength;
	int 		rxCount;
	int 		txCount;

	Datetime 	utcTime;
	SV 			*svList;
	int			svCount;

	byte 		rtcm1005Message[1024];
	byte 		rtcm1005MessageLength;
	bool 		rtcm1005Arrived;
	byte 		rtcm1077Message[1024];
	byte 		rtcm1077MessageLength;
	bool 		rtcm1077Arrived;
} Ublox;

bool InitializeUblox(Ublox *ublox, int receiveBufferLen, char *prtName, int baud);
void ReceiveMessageUblox(Ublox *ublox);
void TransmitMessageUBlox(Ublox *ublox, byte *message, int len);
bool CloseUblox(Ublox *ublox);
#endif
