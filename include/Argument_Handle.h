#ifndef ARGUMENT_HANDLE_H
#define ARGUMENT_HANDLE_H

#include <unistd.h>  	// UNIX standard function definitions
#include <fcntl.h>   	// File control definitions
#include <termios.h> 	// For baud rate symbols
#include <stdio.h>		// Standard input/output definitions
#include <stdlib.h> 	// For NULL symbol
#include <stdbool.h> 	// For boolean variable
#include <string.h> 	// For strstr
#include <errno.h>

typedef struct
{
	char *configurationFileName;

	char *ubloxSerialPortName;
	int   ubloxBaudRate;

	char *ntripCasterHostName;
	int   ntripCasterPort;
	char *ntripPassword;
	char *ntripMountPoint;
	char *ntripProductName;
	char *ntripUserComment;

	bool  sendRTCM;
	bool  sendUBX;
} Argument_Handle;

bool kbhit_linux();
bool HandleConf(Argument_Handle *hArg, int argc, char **argv);
void HandleArguments(char *portName, int *baudRate, int argc, char **argv);
void printHelpConf();
void printHelp();

#endif
