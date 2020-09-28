#include "uBlox_Settings.h"
// For more information: ZED-F9P Interface description [1]

// [1] pg. 39
void CalculateChecksum(byte *buffer, int length)
{
	byte CK_A = 0, CK_B = 0;
	for (int i = 0; i < length; i++)
	{
		CK_A = (CK_A + buffer[i]) & 0xFF;
		CK_B = (CK_B + CK_A) & 0xFF;
	}

	buffer[length] 		= CK_A;
	buffer[length + 1] 	= CK_B;
}

void PollInformationUblox(Ublox *ublox, byte classID, byte msgID)
{
	// Poll messages have headers, IDs, zero length and CS only.
	byte pollRequest[8] = {0xB5, 0x62, classID, msgID, 0x00, 0x00, 0x00, 0x00};
	CalculateChecksum(pollRequest + 2, 4);
	TransmitMessageUBlox(ublox, pollRequest, 8);
	ReceiveMessageUblox(ublox);
}

// [1] pg. 152
void GetVersionUblox(Ublox_Settings *ublox_conf, Ublox *ublox)
{
	PollInformationUblox(ublox, (byte)0x0A, (byte)0x04);
	for(int i = 0; i < ublox->rxCount; i++)
	{
		if (ublox->receiveBuffer[i] == 0xB5 && ublox->receiveBuffer[i+1] == 0x62)
		{
			if (ublox->receiveBuffer[i+2] == 0x0A && ublox->receiveBuffer[i+3] == 0x04)
			{
				int idxPl = i + 6;
				int extensionLen = (int)ublox->receiveBuffer[i+4] + (int)(ublox->receiveBuffer[i+5] << 8) - 40;
				strncpy(ublox_conf->sw_version, ublox->receiveBuffer + idxPl, 30);
				strncpy(ublox_conf->hw_version, ublox->receiveBuffer + idxPl + 30, 10);
				strncpy(ublox_conf->extension,  ublox->receiveBuffer + idxPl + 40, extensionLen);
			}
		}
	}
}
