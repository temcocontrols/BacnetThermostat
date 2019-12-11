#ifndef _REVISION_H_
#define _REVISION_H_
//Tstat ARM base product revision file

#define EEPROM_VERSION	  92   // universl table, custom sensors, two PIDs
#define FW_VER_LOW  	EEPROM_VERSION & 0xff
#define FW_VER_HIGH  	(EEPROM_VERSION >> 8) & 0xff

#endif // _REVISION_H_
//rev8_3
//fix when use register 101 & 691 to change the cool/heat mode, it will write wrong value to register 113(cool/heat config)

//rev8_2
//fixed cycle delay and change over delay 
//modify co2 filter, before it dose not work.
//modify complier level to solve rev3 relays problem(relay4 will be on when power up and relay1 to 3 can not be controled)
//add new register 693 to clear all eeprom except serial number and ID and set to factory default settings.


//rev8_1
//modify clock/occupancy-sensor funtions for inputs, any channel of input is trigged, system will go to occupied mode.   
//T7: fixed fan mode when press decrese key, it not go to off mode
//T7: fixed cool/heat mode when switch mode the display always show heat.
//redefine keypad function for fan mode setpoint and sys mode
//left key: selec items up way
//right key: select items down way
//up and down key: adjust current selected item.

//rev8_0
//add new humidity sensor sht3x
//rec 7_9
//1.add ptransfer function so can read/write registers under MSTP protocol by T3000.
//rev7_8
//tstat7 fixed canot read temperature from hum sensor bug
//rev7_7
//bacnet lib fixed station number can not bigger than 127 issue.
//rev7_6
//fix when deadmaster is set, after power on, the timer is not initialized bug.
// rev7_5
// for tstst7:
// change refresh outputs task to 50ms peroid
// add ID check before deal with serial data to make system faster, when too many polling data on the bus, this can save time.
//rev7_4
//fixed Tstat7 keypad no response bug. the inputs initial routine is overflow.
//rev7_3
//modify schedule so can match for yabe and T3000
//add time synchronization 

//rev71
//fixed icon control when only set one of heat or cool icon relative with output, another one will not work at normal state.
//add color indicator for Vicente

//rev7_0
//modify 10V input formular, previous it can not up to 10V.
//REV6_9
//fixed customer sensor 0-10V
//REV6_8
//fixed instance number for bacnet protocol, before only take low 16 bits of serial number as default value.
//fixed clock function for AI, and for occupancy sensor function, when singal is on, it will trigger override timer.  
//REV6_7
//add voltage check function for rs485 lins
//remap GPIO pins for AI
//rev6_6
//fixed occupancy sensor function for AI, before only AI1 and AI2 can work.
//add priority array for AO and BO 
//rev 6_5
//add float value for temperature humidity and setpoint for bacnet









