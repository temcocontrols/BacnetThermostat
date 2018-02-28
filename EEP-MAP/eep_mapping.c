//this .C file include functions which operate variables and eeprom  
#include "types.h"
#include "eep_mapping.h"
#include "define.h"
#include "eepdefine.h"
#define EEPROM    	0
#define RAM					1


LINK b;	

uint8 GetValue(uint32 address, uint8 eep_or_ram)
{
uint8 data_temp = 0;
	if(eep_or_ram == EEPROM)
		{
		;//data_temp = read_eeprom(address);
		}
	else
		{
		data_temp = b.eeprom[address];
		}
	
	return data_temp;
}

void SetValue(uint8 value, uint32 address, uint8 eep_or_ram)
{
uint8 data_temp = value;
	if(eep_or_ram == EEPROM)
		{
		;//write_eeprom(address, data_temp);
		}
	else
		{
		b.eeprom[address] = data_temp;
		}
}

