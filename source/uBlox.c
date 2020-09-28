#include "uBlox.h"

bool InitializeUblox(Ublox *ublox, int receiveBufferLen, char *prtName, int baud)
{
	if (ublox == NULL)
	{
		perror("ublox struct is null.\r\n");
		return false;
	}
	else
	{
		int result = 0;
		ublox->spDesc 				= 0;
		ublox->portName 			= prtName;
		ublox->baudRate				= baud;

		openSerialPort(&(ublox->spDesc), ublox->portName, ublox->baudRate);
		if (ublox->spDesc < 0)
		{
			perror("Error opening serial port.\r\n");
			return false;
		}
		else
		{
			ublox->receiveBufferLength 	= receiveBufferLen;
			ublox->receiveBuffer 		= (byte*) malloc(ublox->receiveBufferLength * sizeof(byte));
			ublox->rxCount 				= 0;
			ublox->txCount 				= 0;
		}
	}

	return true;
}

void ReceiveMessageUblox(Ublox *ublox)
{
	ublox->rxCount = readSerialPort(ublox->spDesc, ublox->receiveBuffer, ublox->receiveBufferLength);
	ublox->rtcm1005Arrived = false;
	ublox->rtcm1077Arrived = false;
}

void TransmitMessageUBlox(Ublox *ublox, byte *message, int len)
{
	ublox->txCount = writeSerialPort(ublox->spDesc, message, len);
	if (ublox->txCount != len)
	{
		perror("Fail on tranmission of message.");
	}
}

bool CloseUblox(Ublox *ublox)
{
	return closeSerialPort(ublox->spDesc);
}
