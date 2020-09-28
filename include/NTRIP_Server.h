#ifndef NTRIP_SERVER_H
#define NTRIP_SERVER_H

// For more information:
// https://www.binarytides.com/socket-programming-c-linux-tutorial/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>		//hostent
#include <arpa/inet.h>	//inet_addr

typedef struct sockaddr_in Socket;
typedef struct hostent Hostent;
typedef struct in_addr In_addr;

static const char *casterOK 		= "ICY 200 OK";
static const char *badPassword 		= "Error - Bad Password";
static const char *clientMessage 	= "GET /ANKR_TEST1 HTTP/1.0\r\n";
static const char *sourceTableOK 	= "SOURCETABLE 200 OK";
static const char *unauthorized 	= "HTTP/1.0 401 Unauthorized";
//static char *sourceMessage  		= "SOURCE UMfA4r4G /ANKR_TEST1\r\nSource-Agent: NTRIP Ankara_Test/1.0\r\n\r\n";

typedef struct
{
	int socketDesc;
	int connSocketDesc;
	Socket caster;
	Hostent *he;
	In_addr **addr_list;
	char *ipAddress;
	char *hostAddress;
	char *password;
	char *mountPoint;
	char *productName;
	char *comment;
	char sendBuffer[1024];
	char receiveBuffer[1024];
	int txCount;
	int rxCount;
	int portNumber;
} NTRIP_Server;

bool InitializeSocketNtrip(NTRIP_Server *ntripServer, char *hostAddress, int port);
void DescriptionServerNtrip(NTRIP_Server *ntripServer, const char *pw, const char *mp, const char *pn, const char *cmnt);
bool SendCasterNtrip(NTRIP_Server *ntripServer, unsigned char *message, int messageLength);
bool CloseSocketNtrip(NTRIP_Server *ntripServer);
#endif
