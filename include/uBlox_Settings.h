#ifndef UBLOX_SETTINGS_H
#define UBLOX_SETTINGS_H

#include "uBlox.h"

typedef struct
{
	char hw_version[10];
	char sw_version[30];
	char extension[200];

} Ublox_Settings;

void GetVersionUblox(Ublox_Settings *ublox_conf, Ublox *ublox);

#endif
