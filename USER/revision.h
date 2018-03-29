#ifndef _REVISION_H_
#define _REVISION_H_
//Tstat ARM base product revision file

#define EEPROM_VERSION	  62   // universl table, custom sensors, two PIDs
#define FW_VER_LOW  	EEPROM_VERSION & 0xff
#define FW_VER_HIGH  	(EEPROM_VERSION >> 8) & 0xff

#endif // _REVISION_H_











