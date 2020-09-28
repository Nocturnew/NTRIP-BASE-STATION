#ifndef UBLOX_PARSER_H
#define UBLOX_PARSER_H

#include "uBlox.h"

void ParseMessageUblox		(Ublox *ublox);
void PrintAvailableSVUblox	(Ublox *ublox);
void PrintUTCTimeUblox		(Ublox *ublox);
void PrintRTCMStatusUblox	(Ublox *ublox);

#endif
