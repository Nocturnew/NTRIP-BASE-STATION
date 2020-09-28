#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>   /* Standard input/output definitions 		*/
#include <stdbool.h> /* Boolean definitions						*/
#include <string.h>  /* String function definitions 			*/
#include <unistd.h>  /* UNIX standard function definitions 		*/
#include <fcntl.h>   /* File control definitions 				*/
#include <errno.h>   /* Error number definitions 				*/
#include <termios.h> /* POSIX terminal control definitions 		*/

int openSerialPort	 (int *spDescriptor, char *portName, int baudRate);
int writeSerialPort	 (int spDescriptor, unsigned char *string, int size);
int readSerialPort	 (int spDescriptor, unsigned char *string, int size);
bool closeSerialPort (int spDescriptor);

#endif
