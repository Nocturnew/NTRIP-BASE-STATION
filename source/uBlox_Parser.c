#include "uBlox_Parser.h"
// For more information: ZED-F9P Interface description [1]

// [1] pg. 39
bool CorrectChecksum(byte *buffer, int length)
{
	byte CK_A = 0, CK_B = 0;
	for (int i = 0; i < length; i++)
	{
		CK_A = (CK_A + buffer[i]) & 0xFF;
		CK_B = (CK_B + CK_A) & 0xFF;
	}
	if (CK_A == buffer[length] && CK_B == buffer[length+1])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ParseMessageUblox(Ublox *ublox)
{
	int idx = 0;
	// Minimum UBX message length = 8
	for(idx = 0; idx < ublox->rxCount - 8;)
	{
		// If UBX message header came upon
		if (ublox->receiveBuffer[idx] == 0xB5 && ublox->receiveBuffer[idx+1] == 0x62)
		{
			int payloadLength 	= (int)(ublox->receiveBuffer[idx+4]) + (int)(ublox->receiveBuffer[idx+5] << 8);
			bool csCorrect 		= CorrectChecksum(ublox->receiveBuffer + (idx + 2) * sizeof(byte), payloadLength + 4);
			int idxPl = idx + 6; // Index started from payload
			if (csCorrect == true)
			{
				switch(ublox->receiveBuffer[idx+2])
				{
					// UBX-NAV
					case 0x01:
					{
						switch(ublox->receiveBuffer[idx+3])
						{
							// UBX-NAV-TIMEUTC
							case 0x21:
							{
								// If validity flag is set,
								if (ublox->receiveBuffer[idxPl+12] & 0x04)
								{
									ublox->utcTime.tm_year 	= (int)ublox->receiveBuffer[idxPl+12] + (int)(ublox->receiveBuffer[idxPl+13] << 8);
									ublox->utcTime.tm_mon 	= (int)ublox->receiveBuffer[idxPl+14];
									ublox->utcTime.tm_mday 	= (int)ublox->receiveBuffer[idxPl+15];
									ublox->utcTime.tm_hour 	= (int)ublox->receiveBuffer[idxPl+16];
									ublox->utcTime.tm_min 	= (int)ublox->receiveBuffer[idxPl+17];
									ublox->utcTime.tm_sec 	= (int)ublox->receiveBuffer[idxPl+18];
								}
								break;
							}
							default:
							{
								break;
							}
						}
						break;
					}
					// UBX-RXM
					case 0x02:
					{
						switch(ublox->receiveBuffer[idx+3])
						{
							// UBX-RXM-RAWX
							case 0x15:
							{
								ublox->svCount = ublox->receiveBuffer[idxPl+11];

								// WARNING: This array should be freed in every second
								ublox->svList = (SV*)malloc(ublox->svCount * sizeof(SV));

								for(int j = 0; j < ublox->svCount; j++)
								{
									ublox->svList[j].gnssID = ublox->receiveBuffer[idxPl+32*j+36];
									ublox->svList[j].svID 	= ublox->receiveBuffer[idxPl+32*j+37];
									ublox->svList[j].sigID 	= ublox->receiveBuffer[idxPl+32*j+38];
								}
								break;
							}
							default:
							{
								break;
							}
						}
						break;
					}
					default:
					{
						break;
					}
				}
			}

			// Otherwise just increase index
			else
			{

			}
			idx = idx + payloadLength + 8; // Payload + 6 header + 2 checksum
		}
		// If RTCM message header came upon
		// Note that to obtain RTCM message format
		else if (ublox->receiveBuffer[idx] == 0xD3)
		{
			int payloadLength = (int)ublox->receiveBuffer[idx+2];
			int messageType = ((int)(ublox->receiveBuffer[idx+3] << 8) + (int)ublox->receiveBuffer[idx+4]) >> 4;

			switch (messageType)
			{
				case 1005:
				{
					ublox->rtcm1005Arrived 			= true;
					ublox->rtcm1005MessageLength 	= payloadLength + 6;

					if (idx + ublox->rtcm1005MessageLength < ublox->rxCount)
					{
						memset(ublox->rtcm1005Message, 0, 1024);
						strncpy(ublox->rtcm1005Message,  ublox->receiveBuffer + idx, ublox->rtcm1005MessageLength);
					}
					break;
				}
				case 1077:
				{
					ublox->rtcm1077Arrived 			= true;
					ublox->rtcm1077MessageLength 	= payloadLength + 6;

					if (idx + ublox->rtcm1077MessageLength < ublox->rxCount)
					{
						memset(ublox->rtcm1077Message, 0, 1024);
						strncpy(ublox->rtcm1077Message,  ublox->receiveBuffer + idx, ublox->rtcm1077MessageLength);
					}

					break;
				}
				default:
				{
					break;
				}
			}
			idx = idx + payloadLength + 6; // Payload + 6 header
		}
		else
		{
			idx++;
		}
	}
}

void PrintAvailableSVUblox(Ublox *ublox)
{
	char svStr[500];
	char tempStr[10];

	memset(svStr, 0, 500);
	memset(tempStr, 0, 10);

	for(int j = 0; j < ublox->svCount; j++)
	{
		switch (ublox->svList[j].gnssID)
		{
			// GPS SV
			case 0:
			{
				sprintf(tempStr,"GPSL%d_%d\r\n", ublox->svList[j].sigID == 0 ? 1 : 2, ublox->svList[j].svID);
				break;
			}

			// GALILEO SV
			case 2:
			{
				sprintf(tempStr,"GALE%d_%d\r\n", ublox->svList[j].sigID < 5 ? 1 : 5, ublox->svList[j].svID);
				break;
			}

			// BEIDOU SV
			case 3:
			{
				sprintf(tempStr,"BEIB%d_%d\r\n", ublox->svList[j].sigID < 2 ? 1 : 2, ublox->svList[j].svID);
				break;
			}

			// GLONASS SV
			case 6:
			{
				sprintf(tempStr,"GLOL%d_%d\r\n", ublox->svList[j].sigID == 0 ? 1 : 2, ublox->svList[j].svID);
				break;
			}

			default:
			{
				break;
			}
		}

		strcat(svStr, tempStr);
	}

	puts(svStr);
	free(ublox->svList);
}

void PrintUTCTimeUblox(Ublox *ublox)
{
	printf("UTC Time: %02d-%02d-%04d %02d:%02d:%02d\r\n", ublox->utcTime.tm_mday, ublox->utcTime.tm_mon, ublox->utcTime.tm_year, ublox->utcTime.tm_hour, ublox->utcTime.tm_min, ublox->utcTime.tm_sec);
}

void PrintRTCMStatusUblox(Ublox *ublox)
{
	if (ublox->rtcm1005Arrived == true)
	{
		printf("RTCM1005\r\n");
	}

	if (ublox->rtcm1077Arrived == true)
	{
		printf("RTCM1077\r\n");
	}

}
