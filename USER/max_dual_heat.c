

#include "define.h"
#include "eepDefine.h"

#ifdef TSTAT_ARM
#include "24cxx.h"
#else
#include "th_ee.h"
#endif //TSTAT_ARM

//The standard VAV strategy uses cool air to cool the room and reduces down to minimum ai flow for coasting.
//In heating the air flow is fixed at the minumum value, same as coasting.
//We can improve on this by letting the air flow increase in the heating mode. The supply air setpoint is also increased
//as the air flow increases. This strategy is known as 'dual maximum VAV reheat' and it allows
//more heating into the space for extreme cold weather conditions as well as reduces
//the overall energy required to heat the space. The air flow strategy requires an air flow sensor and the
//supply air adjustment strategy requires a supply air temp sensor.
//The supply air setpoints will not go to negative values below 0C or 0F
//The air flow setpoint will be in the same units as the air flow sensor, cfm usually for US and liters per second everywhere else.
//Defaults can be set up as 1000cfm, 300cfm, 500 cfm for max cool, min and heat air flow setpoints respectively.
//Defaults for the supply temperature setpoint can be 35C and 18C.

//AIRFLOW SETPOINT
//IF COOLING_MODE
//THEN AIRFLOW = PID1 * MAX_AIRFLOW_COOLING
//IF HEATING_MODE
//THEN AIRFLOW = PID2 * MAX_AIRFLOW_HEATING
//AIRFLOW = MAX( MIN_AIRFLOW , AIRFLOW) //MAINTAIN MIN AIRFLOW TO ROOM
//IF FAN = OFF
//THEN AIRFLOW = 0 //STOP RESET WINDUP
////SUPPLY SETPOINT
//IF HEATING_MODE
//THEN SUPPLY_SETPOINT = ( MAX_SUPPLY_SETPT - MIN_SUPPLY_SETPT) * PID2 + MIN_SUPPLY_SETPT
//ELSE
//SUPPLY_SETPOINT = MIN_SUPPLY_SETPT
#ifdef VAV
uint8 xdata airflow_sensor_option = NO;

uint8 xdata vav_pid = 0;
uint8 xdata max_dual_vav_enable;//MAX_DUAL_VAV_ENABLE
uint16 xdata max_airflow_cooling = 1000;
uint16 xdata max_airflow_heating = 500;
uint16 xdata occ_min_airflow = 400;
uint16 xdata unocc_min_airflow = 400;
uint16 xdata airflow_setpoint = 200;
uint8 xdata pid1_active_stage = 50;//if heating stage = cooling stage, pid1_active_stage = 50, if 2 cooling 1 heating,  pid1_active_stage = 66
int16 xdata max_supply_setpoint = 350;
int16 xdata min_supply_setpoint = 180;
int16 xdata supply_setpoint = 0;
//uint8 xdata heat_pid3_table = 3;
//uint8 xdata cool_pid3_table = 3;
//uint8 xdata pid3_heat_db = 10;
//uint8 xdata pid3_cool_db = 10;
//uint8 xdata pid3_pterm = 60;
//uint8 xdata pid3_iterm = 0;
int16 xdata airflow = 220;
//uint8  xdata pid3_input_select = 0;
//extern uint8 xdata pid1_active_stage; 
//extern uint8 xdata max_dual_vav_enable;//MAX_DUAL_VAV_ENABLE
//extern uint16 xdata max_airflow_cooling;
//extern uint16 xdata max_airflow_heating;
//extern uint16 xdata min_airflow;
//extern uint16 xdata airflow_setpoint;
//extern int16 xdata max_supply_setpoint;
//extern int16 xdata min_supply_setpoint;
//extern int16 xdata supply_setpoint;
//uint8 xdata vav_pid = 0;
//uint8 xdata max_dual_vav_enable;//MAX_DUAL_VAV_ENABLE
//uint16 xdata max_airflow_cooling = 1000;
//uint16 xdata max_airflow_heating = 500;
//uint16 xdata min_airflow = 400;
//uint16 xdata airflow_setpoint = 200;
//uint8 xdata pid1_active_stage = 50;//if heating stage = cooling stage, pid1_active_stage = 50, if 2 cooling 1 heating,  pid1_active_stage = 66
//int16 xdata max_supply_setpoint = 350;
//int16 xdata min_supply_setpoint = 180;
//int16 xdata supply_setpoint = 0;
//uint8 xdata heat_pid3_table = 3;
//uint8 xdata cool_pid3_table = 3;
//uint8 xdata pid3_heat_db = 10;
//uint8 xdata pid3_cool_db = 10;
//uint8 xdata pid3_pterm = 60;
//uint8 xdata pid3_iterm = 0;
//int16 xdata airflow = 220; 
//uint16 xdata airflow;

void dual_max_vav_ini(void)
{
	max_dual_vav_enable = read_eeprom(EEP_VAV_CONTROL) & 0x01;
	airflow_sensor_option = (read_eeprom(EEP_VAV_CONTROL) >> 1) & 0x01;
//	if(max_dual_vav_enable > 1)	 //illegal value	
//		{
//		max_dual_vav_enable = MAX_DUAL_VAV_ENABLE;
//		write_eeprom(EEP_MAX_DUAL_VAV_ENABLE, MAX_DUAL_VAV_ENABLE);
//		}
	max_airflow_cooling = read_eeprom(EEP_MAX_AIRFLOW_COOLING + 1);
	max_airflow_cooling = max_airflow_cooling << 8;
	max_airflow_cooling += read_eeprom(EEP_MAX_AIRFLOW_COOLING);
//	if(max_airflow_cooling > 100) //illegal value
//		{
//		max_airflow_cooling = MAX_AIRFLOW_COOL_DEFAULT;
//	    write_eeprom(EEP_MAX_AIRFLOW_COOLING, MAX_AIRFLOW_COOL_DEFAULT);
//		}
	max_airflow_heating = read_eeprom(EEP_MAX_AIRFLOW_HEATING + 1);
    max_airflow_heating = max_airflow_heating << 8;
	max_airflow_heating += read_eeprom(EEP_MAX_AIRFLOW_HEATING);
//	if(max_airflow_heating > 100)  //illegal value
//		{
//		max_airflow_heating = MAX_AIRFLOW_HEAT_DEFAULT;
//	    write_eeprom(EEP_MAX_AIRFLOW_HEATING, MAX_AIRFLOW_HEAT_DEFAULT);
//		}
	occ_min_airflow = read_eeprom(EEP_OCC_MIN_AIRFLOW + 1);
	occ_min_airflow = occ_min_airflow << 8;
	occ_min_airflow += read_eeprom(EEP_OCC_MIN_AIRFLOW);
	
	unocc_min_airflow = read_eeprom(EEP_UNOCC_MIN_AIRFLOW + 1);
	unocc_min_airflow = unocc_min_airflow << 8;
	unocc_min_airflow += read_eeprom(EEP_UNOCC_MIN_AIRFLOW);
//	if(min_airflow > 100)  //illegal value
//		{
//		min_airflow = MIN_AIRFLOW_DEFAULT;
//	    write_eeprom(EEP_MIN_AIRFLOW, MIN_AIRFLOW_DEFAULT);
//		}
	pid1_active_stage = (int16)read_eeprom(EEP_HEAT_ORIGINAL_TABLE)*100 /(read_eeprom(EEP_HEAT_ORIGINAL_TABLE) + read_eeprom(EEP_COOL_ORIGINAL_TABLE));
//	pid1_active_stage = (int16)read_eeprom(EEP_HEAT_UNIVERSAL_TABLE)*100 /(read_eeprom(EEP_HEAT_UNIVERSAL_TABLE) + read_eeprom(EEP_COOL_UNIVERSAL_TABLE));

	max_supply_setpoint = read_eeprom(EEP_MAX_SUPPLY_SETPOINT + 1) ;
	max_supply_setpoint = max_supply_setpoint << 8;
	max_supply_setpoint += read_eeprom(EEP_MAX_SUPPLY_SETPOINT);
//	if(max_supply_setpoint > 1000) //illegal value
//		{
//		max_supply_setpoint = MAX_SUPPLY_SETPOINT_DEFAULT; //default maximum supply setpoint 35C
//		write_eeprom(EEP_MAX_SUPPLY_SETPOINT + 1, (MAX_SUPPLY_SETPOINT_DEFAULT>>8));
//		write_eeprom(EEP_MAX_SUPPLY_SETPOINT , (MAX_SUPPLY_SETPOINT_DEFAULT&0xff));
//		}
//	min_supply_setpoint = (((int16)read_eeprom(EEP_MIN_SUPPLY_SETPOINT + 1) << 8) + read_eeprom(EEP_MIN_SUPPLY_SETPOINT));
	min_supply_setpoint = read_eeprom(EEP_MIN_SUPPLY_SETPOINT + 1) ;
	min_supply_setpoint = min_supply_setpoint << 8;
	min_supply_setpoint += read_eeprom(EEP_MIN_SUPPLY_SETPOINT);

}
#endif













