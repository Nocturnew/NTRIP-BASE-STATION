#include "Argument_Handle.h"

extern int errno;

// Obtained from: https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
bool kbhit_linux()
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return true;
	}

	return false;
}

bool HandleConf(Argument_Handle *hArg, int argc, char **argv)
{
	if (argc != 3)
	{
		return false;
	}
	else
	{
		if (strcmp(argv[1], "-f") != 0)
		{
			return false;
		}
		else
		{
			if (argv[2] == NULL)
			{
				return false;
			}
			else
			{
				hArg->configurationFileName = (char*) malloc(strlen(argv[2]) * sizeof(char));
				strncpy(hArg->configurationFileName, argv[2], strlen(argv[2]));
			}
		}
	}

	FILE *fp;
	fp = fopen(hArg->configurationFileName, "r");

	if (fp == NULL)
	{
		printf("%s could not be opened.\r\n", hArg->configurationFileName);
		printf("Error description: %s\r\n", strerror(errno));
		return false;
	}

	int lineLength = 0;
	size_t lineSize = 0;
	char *lineBuffer;
	char *lptr;
	int lineCnt = 1;

	// Get serial port name from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "SerialPortName=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			hArg->ubloxSerialPortName = (char *) malloc((strLength+1) * sizeof(char));
			memset(hArg->ubloxSerialPortName, 0, strLength+1);
			strncpy(hArg->ubloxSerialPortName, lptr+descStrLen, strLength);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get serial port baud rate from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "BaudRate=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			char *baudRateStr;
			baudRateStr = (char *) malloc((strLength+1) * sizeof(char));
			memset(baudRateStr, 0, strLength+1);
			strncpy(baudRateStr, lptr+descStrLen, strLength);
			hArg->ubloxBaudRate = atoi(baudRateStr);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get NTRIP Caster host name from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "CasterHostName=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			hArg->ntripCasterHostName = (char *) malloc((strLength+1) * sizeof(char));
			memset(hArg->ntripCasterHostName, 0, strLength+1);
			strncpy(hArg->ntripCasterHostName, lptr+descStrLen, strLength);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get NTRIP Caster port number conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "CasterPort=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			char *casterPortStr;
			casterPortStr = (char *) malloc((strLength+1) * sizeof(char));
			memset(casterPortStr, 0, strLength+1);
			strncpy(casterPortStr, lptr+descStrLen, strLength);
			hArg->ntripCasterPort = atoi(casterPortStr);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get NTRIP Server password to access NTRIP Caster from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "ServerPassword=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			hArg->ntripPassword = (char *) malloc((strLength+1) * sizeof(char));
			memset(hArg->ntripPassword, 0, strLength+1);
			strncpy(hArg->ntripPassword, lptr+descStrLen, strLength);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get NTRIP Source mount point name from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "MountPoint=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			hArg->ntripMountPoint = (char *) malloc((strLength+1) * sizeof(char));
			memset(hArg->ntripMountPoint, 0, strLength+1);
			strncpy(hArg->ntripMountPoint, lptr+descStrLen, strLength);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get product name from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "ProductName=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			hArg->ntripProductName = (char *) malloc((strLength+1) * sizeof(char));
			memset(hArg->ntripProductName, 0, strLength+1);
			strncpy(hArg->ntripProductName, lptr+descStrLen, strLength);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get user comment from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "UserComment=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			int strLength = lineLength-descStrLen-1;
			hArg->ntripUserComment = (char *) malloc((strLength+1) * sizeof(char));
			memset(hArg->ntripUserComment, 0, strLength+1);
			strncpy(hArg->ntripUserComment, lptr+descStrLen, strLength);
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get send RTCM flag from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "SendRTCM=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			if (*(lptr + descStrLen) == '1')
			{
				hArg->sendRTCM = true;
			}
			else if (*(lptr + descStrLen) == '0')
			{
				hArg->sendRTCM = false;
			}
			else
			{
				printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
				return false;
			}
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	// Get send UBX messages flag from conf file
	if ((lineLength = getline(&lineBuffer, &lineSize, fp)) > 0)
	{
		char strLook[] = "SendUBX=";
		int descStrLen = strlen(strLook);

		if ((lptr = strstr(lineBuffer, strLook)) == NULL)
		{
			printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
			return false;
		}
		else
		{
			if (*(lptr + descStrLen) == '1')
			{
				hArg->sendUBX = true;
			}
			else if (*(lptr + descStrLen) == '0')
			{
				hArg->sendUBX = false;
			}
			else
			{
				printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
				return false;
			}
		}
	}
	else
	{
		printf("Some of data in conf has been changed or deleted: Line %d", lineCnt);
		return false;
	}

	lineCnt++;

	fclose(fp);

	return true;
}

void printHelpConf()
{
	puts("Usage:");
	puts("./Rtk_Box -f <Conf file path>");
	puts("Example:");
	puts("./Rtk_Box -f ../Config/Settings.conf");
}

void HandleArguments(char *portName, int *baudRate, int argc, char **argv)
{
	if (argc != 5)
	{
		printHelp();
		sprintf(portName, "/dev/ttyACM0");
		*baudRate = B115200;
	}
	else
	{
		if (strstr(argv[1], "--port") == NULL && strstr(argv[3], "--port") == NULL)
		{
			printHelp();
			sprintf(portName, "/dev/ttyACM0");
			*baudRate = B115200;
		}
		else if(strstr(argv[1], "--baud") == NULL && strstr(argv[3], "--baud") == NULL)
		{
			printHelp();
			sprintf(portName, "/dev/ttyACM0");
			*baudRate = B115200;
		}
		else
		{
			if (strstr(argv[1], "--port") != NULL)
			{
				sprintf(portName, "%s", argv[2]);
			}
			else if (strstr(argv[3], "--port") != NULL)
			{
				sprintf(portName, "%s", argv[4]);
			}

			if (strstr(argv[1], "--baud") != NULL)
			{
				int baud = atoi(argv[2]);
				switch (baud)
				{
					case 9600:
					{
						*baudRate = B9600;
						break;
					}
					case 19200:
					{
						*baudRate = B19200;
						break;
					}
					case 38400:
					{
						*baudRate = B38400;
						break;
					}
					case 57600:
					{
						*baudRate = B57600;
						break;
					}
					case 115200:
					{
						*baudRate = B115200;
						break;
					}
					case 230400:
					{
						*baudRate = B230400;
						break;
					}
					case 460800:
					{
						*baudRate = B460800;
						break;
					}
					default:
					{
						*baudRate = B115200;
						break;
					}
				}
			}
			else if (strstr(argv[3], "--baud") != NULL)
			{
				int baud = atoi(argv[4]);
				switch (baud)
				{
					case 9600:
					{
						*baudRate = B9600;
						break;
					}
					case 19200:
					{
						*baudRate = B19200;
						break;
					}
					case 38400:
					{
						*baudRate = B38400;
						break;
					}
					case 57600:
					{
						*baudRate = B57600;
						break;
					}
					case 115200:
					{
						*baudRate = B115200;
						break;
					}
					case 230400:
					{
						*baudRate = B230400;
						break;
					}
					case 460800:
					{
						*baudRate = B460800;
						break;
					}
					default:
					{
						*baudRate = B115200;
						break;
					}
				}
			}
		}
	}
}

void printHelp()
{
	printf("Usage:\r\n");
	printf("./Rtk_Box --port <Serial Port> --baud <Baud Rate>\r\n");
	printf("Example: ./Rtk_Box --port /dev/ttyACM0 --baud 115200\r\n");
}
