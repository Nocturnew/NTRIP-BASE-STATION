#include "main.h"

extern sig_atomic_t gotAlarm; // Will be nonzero on reception of SIGALRM

int main(int argc, char **argv)
{
	TimerVal itv;
	Ublox ublox;
	Ublox_Settings ublox_conf;
	NTRIP_Server ntripServer;
	Argument_Handle hArg;
	char serialPortName[50];
	int baudRate;

	if (HandleConf(&hArg, argc, argv) == false)
	{
		printHelpConf();
		return EXIT_FAILURE;
	}

	if (InitializeUblox(&ublox, 2000, hArg.ubloxSerialPortName, hArg.ubloxBaudRate) == false) // Could not start serial port.
	{
		return EXIT_FAILURE;
	}

	GetVersionUblox(&ublox_conf, &ublox);

	DescriptionServerNtrip(&ntripServer, hArg.ntripPassword, hArg.ntripMountPoint, hArg.ntripProductName, hArg.ntripUserComment);

	if (InitializeSocketNtrip(&ntripServer, hArg.ntripCasterHostName, hArg.ntripCasterPort) == false)
	{
		return EXIT_FAILURE;
	}

	setTimer(&itv, 2, 0, 1, 0);

	while(kbhit_linux() == false)
	{
		if (gotAlarm == 1)
		{
			system("clear"); // Off in debugging mode
			gotAlarm = 0;
			ReceiveMessageUblox(&ublox);

			puts("*******************************");
			puts("*** Press any key for exit! ***");
			puts("*******************************");

			if ((ublox.receiveBuffer[0] == 0xB5 && ublox.receiveBuffer[1] == 0x62) || (ublox.receiveBuffer[0] == 0xD3))
			{
				printf("Message OK. Received %d bytes from u-blox.\r\n", ublox.rxCount);
				ParseMessageUblox(&ublox);
				PrintAvailableSVUblox(&ublox);
				PrintRTCMStatusUblox(&ublox);
				PrintUTCTimeUblox(&ublox);

				if (hArg.sendUBX == true)
				{
					if (SendCasterNtrip(&ntripServer, ublox.receiveBuffer, ublox.rxCount) == true)
					{
						puts("U-Blox message sent to the caster.");
					}
				}
				else if (hArg.sendRTCM == true)
				{
					if (ublox.rtcm1005Arrived == true)
					{
						if (SendCasterNtrip(&ntripServer, ublox.rtcm1005Message, ublox.rtcm1005MessageLength) == true)
						{
							puts("RTCM 1005 sent to the caster.");
						}
					}
					if (ublox.rtcm1077Arrived == true)
					{
						if (SendCasterNtrip(&ntripServer, ublox.rtcm1077Message, ublox.rtcm1077MessageLength) == true)
						{
							puts("RTCM 1077 sent to the caster.");
						}
					}
				}
			}
			else
			{
				printf("Message FAIL\r\n");
			}
		}
		fflush(stdout);
	}

	puts("Exit by user input.");

	if (CloseUblox(&ublox) == true)
	{
		puts("U-Blox has been successfully closed.");
	}

	if (CloseSocketNtrip(&ntripServer) == true)
	{
		puts("TCP socket has been successfully closed.");
	}

	return EXIT_SUCCESS;
}
