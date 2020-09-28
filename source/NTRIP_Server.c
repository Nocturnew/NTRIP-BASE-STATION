#include "NTRIP_Server.h"

bool InitializeSocketNtrip(NTRIP_Server *ntripServer, char *hostName, int port)
{
	if ((ntripServer->he = gethostbyname(hostName)) == NULL)
	{
		perror("Error when resolving host name.");
		return false;
	}

	if ((ntripServer->connSocketDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error when creating socket.");
		return false;
	}

	memset((char *) &(ntripServer->caster), 0x00, sizeof(ntripServer->caster));
	memcpy(&(ntripServer->caster.sin_addr), ntripServer->he->h_addr, (size_t)ntripServer->he->h_length);
	ntripServer->caster.sin_family = AF_INET;
	ntripServer->caster.sin_port = htons(port);

	if (connect(ntripServer->connSocketDesc, (struct sockaddr *)&(ntripServer->caster), sizeof(ntripServer->caster)) < 0)
	{
		perror("Error when connecting to the caster.");
		return false;
	}
	char sourceMessage[1024];
	memset(sourceMessage, 0, 1024);
	sprintf(sourceMessage, "SOURCE %s /%s\r\nSource-Agent: %s/%s\r\n\r\n", ntripServer->password, ntripServer->mountPoint, ntripServer->productName, ntripServer->comment);

	char receivedMessage[1024];
	memset(receivedMessage, 0, 1024);

	int txCount = send(ntripServer->connSocketDesc, sourceMessage, strlen(sourceMessage), MSG_DONTWAIT);
	int rxCount = recv(ntripServer->connSocketDesc, receivedMessage, 1024, 0);

	if (strstr(receivedMessage, "ICY 200 OK") == NULL)
	{
		printf("Connection refused by the caster. Returned message:\r\n");
		printf("%s\r\n", receivedMessage);

		return false;
	}

	return true;
}

void DescriptionServerNtrip(NTRIP_Server *ntripServer, const char *pw, const char *mp, const char *pn, const char *cmnt)
{
	if (pw != NULL)
	{
		ntripServer->password = (char*) malloc((strlen(pw)+1) * sizeof(char));
		memset(ntripServer->password, 0, (strlen(pw)+1));
		strncpy(ntripServer->password, pw, strlen(pw)+1);
	}

	if (mp != NULL)
	{
		ntripServer->mountPoint = (char*) malloc((strlen(mp)+1) * sizeof(char));
		memset(ntripServer->mountPoint, 0, (strlen(mp)+1));
		strncpy(ntripServer->mountPoint, mp, strlen(mp)+1);
	}

	if (pn != NULL)
	{
		ntripServer->productName = (char*) malloc((strlen(pn)+1) * sizeof(char));
		memset(ntripServer->productName, 0, (strlen(pn)+1));
		strncpy(ntripServer->productName, pn, strlen(pn)+1);
	}

	if (cmnt != NULL)
	{
		ntripServer->comment = (char*) malloc((strlen(cmnt)+1) * sizeof(char));
		memset(ntripServer->comment, 0, (strlen(cmnt)+1));
		strncpy(ntripServer->comment, cmnt, strlen(cmnt)+1);
	}

}

bool SendCasterNtrip(NTRIP_Server *ntripServer, unsigned char *message, int messageLength)
{
	int txCount = 0;
	if ((txCount = send(ntripServer->connSocketDesc, message, messageLength, MSG_DONTWAIT)) < 0)
	{
		perror("Error while sending to the caster.");
		return false;
	}
	else
	{
		return true;
	}
}

bool CloseSocketNtrip(NTRIP_Server *ntripServer)
{
	if (close(ntripServer->socketDesc) < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
