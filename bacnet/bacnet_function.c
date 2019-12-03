#include "stdint.h"
#include "types.h"
#include "define.h"
#include "usart.h"
#include "rs485.h"
#include "bacnet.h"
#include "eepDefine.h"
#include "24cxx.h"
#include "modbus.h"
#include "inputs.h"
#include "TstatFunctions.h"
#include "store.h"
#include "rtc.h"
#include "revision.h"
#include "control_logic.h"
#include "setpoint.h"
//#include "Pid.h"
#include "LCD.h"
#include "stmflash.h"
#include "Constcode.h"
#include "bactimevalue.h"
#include "bsp_esp8266.h"

#define BACNET_VENDOR_NETIX 	"NETIX Controls"
#define BACNET_VENDOR_JET			"JetControls"
#define BACNET_VENDOR_TEMCO	 	"TemcoControls"

#define BACNET_PRODUCT_NETIX 	"NCCB"
#define BACNET_PRODUCT_JET		"Jet Product"
#define BACNET_PRODUCT_TEMCO	"Temco Product"
		
#define BACNET_VENDOR_ID_NETIX 1007
#define BACNET_VENDOR_ID_JET 997
#define BACNET_VENDOR_ID_TEMCO 148


typedef	union
	{
		U8_T all[10];
		struct 
		{
			U8_T sec;				/* 0-59	*/
			U8_T min;    		/* 0-59	*/
			U8_T hour;      		/* 0-23	*/
			U8_T day;       		/* 1-31	*/
			U8_T week;  		/* 0-6, 0=Sunday	*/
			U8_T mon;     		/* 1-12	*/
			U8_T year;      		/* 0-99	*/
			U16_T day_of_year; 	/* 0-365	*/
			S8_T is_dst;        /* daylight saving time on / off */		
				
		}Clk;
		struct
    {
        U32_T timestamp;
        S8_T time_zone;
        U8_T daylight_saving_time;
        U8_T reserved[3];
    }NEW;
	}UN_Time;
	
UN_Time Rtc;	
BACNET_DATE Local_Date;
BACNET_TIME Local_Time;
uint8 flag_Updata_Clock;
S16_T far timezone;	

uint8 far wr_time_on_off[MAX_WR][MAX_SCHEDULES_PER_WEEK][6];
float output_priority[MAX_OUTS][16]; 
uint8 piority_flag = 0;

char* bacnet_vendor_name = BACNET_VENDOR_TEMCO;
char* bacnet_vendor_product = BACNET_PRODUCT_TEMCO;
U8_T far Bacnet_Vendor_ID;



extern bool Send_I_Am_Flag;	
extern uint8 init_PID_flag;
//extern int16 ctest[10];
extern void Inital_Bacnet_Server(void);
extern u32 Instance;
int32 av[20] = {0,1,2,3,4,5,6,7,8,9};

extern int16_t valve[3];
extern PTBDSTR RELAY_1TO5;
extern uint16 today_dayofyear;
U8_T ar_dates[MAX_AR][AR_DATES_SIZE];	
Wr_one_day wr_times[MAX_WR][MAX_SCHEDULES_PER_WEEK];

Str_weekly_routine_point weekly_routines[MAX_WR];
Str_annual_routine_point annual_routines[MAX_AR];

void switch_to_modbus(void)
{

//	modbus.com_config[0] = MODBUS;
//		modbus.com_config[0] = BAC_MSTP;
//	if(EEP_Baudrate == BAUDRATE_19200)
//		uart1_init(19200);
//	else if(EEP_Baudrate == BAUDRATE_9600)
//		uart1_init(9600);
//	else if(EEP_Baudrate == BAUDRATE_38400)
//		uart1_init(38400);
//	else if(EEP_Baudrate == BAUDRATE_57600)
//		uart1_init(57600);
//	else if(EEP_Baudrate == BAUDRATE_115200)
//		uart1_init(115200);
//	else if(EEP_Baudrate == BAUDRATE_76800)
//		uart1_init(76800);
}

uint16_t send_count;
u16 far Test[50];

uint8_t RS485_Get_Baudrate()
{
 if(Modbus.baudrate == BAUDRATE_9600)
  return 5;
 else if(Modbus.baudrate == BAUDRATE_19200)
  return 6;
 else if(Modbus.baudrate == BAUDRATE_38400)
  return 7;
 else if(Modbus.baudrate == BAUDRATE_57600)
  return 8;
 else if(Modbus.baudrate == BAUDRATE_115200)
  return 9;
 else if(Modbus.baudrate == BAUDRATE_76800)
  return 10; 
 else 
  return 6;// default is 19200
}

//----------------------------------------------------------
void Get_AVS(void)
{
	switch(EEP_Baudrate)
	{
		case  BAUDRATE_9600:
			av[0] = baudrate[0];
		break;
		case  BAUDRATE_19200:
			av[0] = baudrate[1];
		break;
		case  BAUDRATE_38400:
			av[0] = baudrate[2];
		break;
		case  BAUDRATE_57600:
			av[0] = baudrate[3];
		break;
		case  BAUDRATE_115200:
			av[0] = baudrate[4];
		break;	
		case  BAUDRATE_76800:
			av[0] = baudrate[5];
		break;
		case  BAUDRATE_1200:
			av[0] = baudrate[6];
		break;
		case  BAUDRATE_4800:
			av[0] = baudrate[7];
		break;
		case  BAUDRATE_14400:
			av[0] = baudrate[8];
		break;	
			
		default:
			break;
	}
	
		av[1] = Station_NUM;
		av[2] = protocol_select;
	  av[3] = (uint32)Instance;
	  av[4] = schedule_on_off;
	  av[5] = EEP_DaySpHi * 256 + EEP_DaySpLo;//day setpoint
	 // av[5] = (float)av[5]/10;
		av[6] = EEP_NightSpHi * 256 + EEP_NightSpLo;//night setpoint
		//av[6] = (float)av[6]/10;
	  av[7] = fan_speed_user;//fan mode 
	  av[8] = EEPROM_VERSION;//software version
	  //av[9] = 0;//cuurent mode of operation  
		av[9] = get_bit(COOLING_MODE,0);
		if(EEP_COOL_TABLE1 == 0)
			av[9] = 0;	
		if(get_bit(HEATING_MODE,0))
			{	
			av[9] += 2;
			if(EEP_HEAT_TABLE1 == 0)
				av[9] = 0;
			}	
		av[10] = EEP_DEGCorF;//degree C/f
	  av[11] = heat_cool_user;//EEP_HeatCoolConfig;//system mode auto/heat/cool
		av[12] = EEP_SpecialMenuLock;//keypad lock/unlock
		av[13] = EEP_OverrideTimer;//override timer
	  av[14] = EEP_UniversalSetpointHi * 256 + EEP_UniversalSetpointLo;//pid2 day setpoint
		//av[14] = (float)av[14]/10;	
	  av[15] = EEP_UniversalNightSetpointHi * 256 + EEP_UniversalNightSetpointLo;//pid2 night setpoint
		//av[15] = (float)av[15]/10;
		av[16] = EEP_OutputManuEnable;//output auto manual
		if(override_timer_time > 0)
			av[17] = 1 + override_timer_time/61;
		else
     	av[17] = 0;
		
		av[18] = EEP_DayCoolingDeadband;//cooling deadband
		av[19] = EEP_DayHeatingDeadband;//heating deadband

//    if(fan_speed_user == 0)
//			av[18] = 1;
//		else
//		{
//			if(temperature - loop_setpoint[PID_LOOP1] < 0)
//				av[18] = 5;
//			else if(temperature - loop_setpoint[PID_LOOP1] <= 5)
//				av[18] = 7;
//			else if(temperature - loop_setpoint[PID_LOOP1] <= 10)
//				av[18] = 8;
//			else if(temperature - loop_setpoint[PID_LOOP1] <= 15)
//				av[18] = 9;	
//		}
	  
}
//----------------------------------------------
float Get_bacnet_value_from_buf(uint8_t type,uint8_t priority,uint8_t i)
{	
	uint8 temp;
	switch(type)
	{
		case AV:
			
			Get_AVS();
		  if(i==5 || i==6 || i==14 || i==15)
				return (float)av[i]/10;
			else
				return av[i];//bacnet_AV.avs[i];
		break;
		
		case AI:
				//			if(inputs[i].range > 0)
				//				return swap_double(inputs[i].value) / 1000;
				//			else
				//				return input_raw[i];
		if(i == 8)
			return (float)temperature/10;
		else if(i == 9)
			return (float)humidity/10;
		else if(i == 10)//co2 
			return co2_data;
		else
		{
			if((AI_Range(i) == AI_RANGE_10K_THERMISTOR_TYPE2) || (AI_Range(i) == AI_RANGE_10K_THERMISTOR_TYPE3) || (AI_Range(i) == AI_RANGE_USER) ||(AI_Range(i) == AI_RANGE_USER_10V)|| (AI_Range(i) == AI_RANGE_CUSTOMER_SENSOR)||(AI_Range(i) == AI_RANGE_CUSTOMER_SENSOR_10V)||(AI_Range(i) == AI_RANGE_MA)||(AI_Range(i) == AI_RANGE_VOLTAGE_5V)||(AI_Range(i) == AI_RANGE_VOLTAGE_10V))
				return (float)mul_analog_input[i]/10;
			else
				return mul_analog_input[i];
		}
		break;
		case AO:
				//				if(outputs[i + max_dos].auto_manual == 0)
				//					priority = 15; // AUTO				//				else
				//					priority = 7; // manual

//				if(outputs[i + max_dos].range > 0)
//					return swap_double(outputs[i + max_dos].value)/ 1000;
//				else
//					return output_raw[i + max_dos];
		//valve[i]
		//if(priority)
		if(output_priority[i+5][priority] == 0xff)
			return 0xff;
		else
			return output_priority[i+5][priority]/100;
    //return Analog_Output_Present_Value1(i);
		//return  valve[i];
		break;
			
		case BO:
			if(output_priority[i][priority] == 0xff)
				return 0xff;
			else
				return output_priority[i][priority];
			//return Binary_Output_Present_Value1(i);

//		if(!GetByteBit(&RELAY_1TO5.BYTE,i) == 1)
//			temp = 1;
//		else
//			temp = 0;
//		return temp;
		
		break;
		
    case SCHEDULE:
			//return weekly_routines[i].value;
//			if(fan_speed_user == 0)//unoccupied
//				return 0;
//			else
//				return 1;
			return fan_speed_user;
		break;
		
		case CALENDAR:
			return annual_routines[i].value;
		break;
    		
		default:
			return 0;
			break;
				
	}	

}
//------------------------------------------------------------
//float Get_bacnet_value_from_buf(uint8_t type,uint8_t priority,uint8_t i)
//{	
//	uint8_t out_index;
//	switch(type)
//	{
//		case AO:
//		// find output index by AOx
//		{
//				
//				if(output_priority[out_index][priority] == 0xff)
//					return 0xff;
//				
//				if(outputs[out_index].digital_analog == 0)
//				{					
//					temp = output_priority[out_index][priority] ? 1 : 0;
//					if((outputs[out_index].range >= ON_OFF)  && (outputs[out_index].range <= HIGH_LOW))
//					{  // inverse logic
//						
//						if(temp == 1)
//						{
//							return 0;
//						}
//						else
//						{
//							return 1;
//						}
//					}	
//					else
//					{
//						
//						if(temp == 1)
//						{
//							return 1;
//						}
//						else
//						{
//							return 0;
//						}
//					}
//				}
//				else	
//				{
//					return output_priority[out_index][priority];
//				}

//			}	
//			
//		case BO:	
//		{
//			
//			if(output_priority[out_index][priority] == 0xff)
//				return 0xff;	

//			if(outputs[out_index].digital_analog == 0)
//			{  // digital
//				temp = output_priority[out_index][priority] ? 1 : 0;
//				if(outputs[out_index].range >= ON_OFF  && outputs[out_index].range <= HIGH_LOW)
//				{  // inverse logic
//					if(temp == 1)
//					{
//						return 0;
//					}
//					else
//					{
//						return 1;
//					}
//				}	
//				else
//				{
//					if(temp == 1)
//					{
//						return 1;
//					}
//					else
//					{
//						return 0;
//					}
//				}
//			}
//			else
//			{ // range is analog
//				return output_priority[out_index][priority];
//			}
//		}
//				
//	}	
//	return 0;
//}





//-----------------------------------------------------------
void wirte_bacnet_value_to_buf(uint8_t type,uint8_t priority,uint8_t i,float value)
{
	switch(type)
	{
		case AV:
			av[i] = value;
			if(i == 0)
			{
				switch(av[0])
				{
					case BAC_BAUDRATE_9600:
						update_flag = 4;
						EEP_Baudrate = 0;
					  
					break;

					case BAC_BAUDRATE_19200:
						update_flag = 4;
						EEP_Baudrate = 1;
					break;

					case BAC_BAUDRATE_38400:
						update_flag = 4;
						EEP_Baudrate = 2;
					break;

					case BAC_BAUDRATE_57600:
						update_flag = 4;
						EEP_Baudrate = 3;
					break;

					case BAC_BAUDRATE_115200:
						update_flag = 4;
						EEP_Baudrate = 4;
					break;
					
					case BAC_BAUDRATE_76800:
						update_flag = 4;
						EEP_Baudrate = 5;
					break;
					
					case BAC_BAUDRATE_1200:
						update_flag = 4;
						EEP_Baudrate = 6;
					break;
					
					case BAC_BAUDRATE_4800:
						update_flag = 4;
						EEP_Baudrate = 7;
					break;
					
					case BAC_BAUDRATE_14400:
					update_flag = 4;
						EEP_Baudrate = 5;
					break;
					
					
					default:
					break;
				}
				write_eeprom(EEP_BAUDRATE, EEP_Baudrate);
			}
			else if(i == 1)//station number
			{
				Station_NUM = value;
				laddress = Station_NUM;
				write_eeprom(EEP_ADDRESS, Station_NUM);
				write_eeprom(EEP_STATION_NUMBER, Station_NUM);
				flash_buf[0] = laddress;	
				STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);
				Inital_Bacnet_Server();
				//laddress = value;
			}
			
			else if(i == 2)//modbus/bacnet protocol switch
			{
				protocol_select = value;
				if(value == 0)//modbus
				{
					Modbus.com_config[0] = MODBUS;
					write_eeprom(EEP_PROTOCOL_SEL, 0);
				}
			}
			
			else if(i == 3)//instance number
			{
				Instance = (u32)value;
				write_eeprom(EEP_INSTANCE1, (u32)value & 0xff);
				write_eeprom(EEP_INSTANCE2, ((u32)value>>8) & 0xff);
				write_eeprom(EEP_INSTANCE3, ((u32)value>>16) & 0xff);
				write_eeprom(EEP_INSTANCE4, ((u32)value>>24) & 0xff);
				Device_Set_Object_Instance_Number(Instance);  
			}
			
		  
			else if(i == 4)//schedule function enable/disable
			{
				schedule_on_off = (uint8)value;
				write_eeprom(EEP_SCHEDULE_ON_OFF, schedule_on_off);
				if(schedule_on_off)
				{				
					EEP_OverrideTimer = read_eeprom(EEP_OVERRIDE_TIMER);	
					//write_eeprom(EEP_OVERRIDE_TIMER,60);
				}
			}
			
			else if(i == 5)//pid loop1 day setpoint
			{
				EEP_DaySpLo = (int16)(value*10) & 0xff;
				EEP_DaySpHi = ((int16)(value*10) >> 8) & 0xff;
				Set_day_setpoint(EEP_DaySpHi, EEP_DaySpLo);
				init_PID_flag |= 0x01;
				refresh_setpoint(DAY_MODE);			
			}
			
			else if(i == 6)//pid loop1 night setpoint
			{
				EEP_NightSpLo = (int16)(value*10) & 0xff;
				EEP_NightSpHi = ((int16)(value*10) >> 8) & 0xff;
				Set_night_setpoint(EEP_NightSpHi,EEP_NightSpLo);
				init_PID_flag |= 0x01;
				refresh_setpoint(NIGHT_MODE);
			}
			
			else if(i == 7)//fan mode could be auto/off/speed1/spped2/speed2
			{
			if(value> EEP_FanMode)
				value = FAN_AUTO;	
			EEP_FanSpeed = value;
			fan_speed_user = value;
			fanspeedbuf = value;
			write_eeprom(EEP_FAN_SPEED,value);
			if(fan_speed_user == FAN_OFF)
				setpoint_select = NIGHT_SP;
			else	
				setpoint_select = DHOME_SP;	
      #ifndef TSTAT7_ARM			
			icon.fan = 1;
			icon.setpoint = 1;
			icon.occ_unocc = 1;
			icon.fanspeed = 1;
			#endif			
			}
			
		
			else if(i == 10)//system unit
			{
				if(EEP_DEGCorF != value)
				{
					EEP_DEGCorF = value;
					write_eeprom(EEP_DEGC_OR_F, EEP_DEGCorF);
					#ifndef TSTAT7_ARM
					icon.unit = 1;	
					#endif
					ConverteCF();
				}			
			}
			
			else if(i == 11)//system mode could be HEAT/COOL/AUTO
			{
//				write_eeprom(EEP_HEAT_COOL_CONFIG,HC_CTL_USER);
//				EEP_HeatCoolConfig = HC_CTL_USER;
//				heat_cool_user = value;
//				write_eeprom(EEP_HC_USER, value);
				
					heat_cool_user = value;
					write_eeprom(EEP_HC_USER, heat_cool_user);
					EEP_HeatCoolConfig = HC_CTL_USER;
					write_eeprom(EEP_HEAT_COOL_CONFIG, HC_CTL_USER);
				#ifndef TSTAT7_ARM
					icon.heatcool = 1;
					icon.sysmode = 1;
				#endif
			}
			
			else if(i == 12)//
			{
				EEP_SpecialMenuLock = av[12];
				write_eeprom(EEP_SPECIAL_MENU_LOCK, EEP_SpecialMenuLock);
			}
			
			else if(i == 13)//override timer
			{
			EEP_OverrideTimer = value;
			write_eeprom(EEP_OVERRIDE_TIMER,EEP_OverrideTimer);				
			override_timer_time = (uint32)60*value;			
			}
			
			else if(i == 14)//pid loop2 day setpoint
			{
				EEP_UniversalSetpointHi = (int16)(value*10)>>8;
				EEP_UniversalSetpointLo = (int16)(value*10) & 0xff;
				write_eeprom(EEP_UNIVERSAL_SET, EEP_UniversalSetpointLo);
				write_eeprom(EEP_UNIVERSAL_SET + 1,EEP_UniversalSetpointHi);				
			}
			
			else if(i == 15)//pid loop2 night setpoint
			{
				EEP_UniversalNightSetpointHi = (int16)(value*10)>>8;
				EEP_UniversalNightSetpointLo = (int16)(value*10) & 0xff;
				write_eeprom(EEP_UNIVERSAL_NIGHTSET,EEP_UniversalNightSetpointLo);
				write_eeprom(EEP_UNIVERSAL_NIGHTSET + 1,EEP_UniversalNightSetpointHi);				
			}
			

			
			else if(i == 16)//output manual control
			{
				EEP_OutputManuEnable = value;
				write_eeprom(EEP_OUTPUT_MANU_ENABLE,value);
			}
			
			else if(i == 18)//cooling deadband
			{
				EEP_DayCoolingDeadband = value;
				change_setpoint_deadband(DAY_COOLING_DB, EEP_DayCoolingDeadband);
				init_PID_flag |= 0x01;
				refresh_setpoint(DAY_MODE);
			}			
			else if(i == 19) //heating deadband
			{
				EEP_DayHeatingDeadband = value;
				change_setpoint_deadband(DAY_HEATING_DB, EEP_DayHeatingDeadband);
				init_PID_flag |= 0x01;
				refresh_setpoint(DAY_MODE);
			}	
			
			break;
		
		case AI:
			if((i<8)&&(i>=0))//AI1-AI8
			{
				if(GetByteBit(&EEP_InputManuEnable,i))//manual mode
				{
					write_eeprom(MANUAL_ANALOG1_HI, ((uint16)value>>8)&0xff);
					write_eeprom(MANUAL_ANALOG1_LO, ((uint16)value&0xff));
					ManualAI_HI(i-1) = ((uint16)value>>8) & 0xff;	
					ManualAI_LO(i-1) =  (uint16)value & 0xff;				
				}
				else
				{
					mul_analog_cal[i-1] =  value - mul_analog_input[i-1];
					write_eeprom(EEP_CALIBRATION_ANALOG1 + (i-1)*2, mul_analog_cal[i-1]&0xFF );
					write_eeprom(EEP_CALIBRATION_ANALOG1 + (i-1)*2 + 1, (mul_analog_cal[i-1]>> 8) & 0xff );
				}
			}
			else if(i==8)//internal temperature
			{
					if(GetByteBit(&b.eeprom[EEP_INTERNAL_SENSOR_MANUAL],0))
					{
						write_eeprom(MANUAL_INTERNAL_HI, ((uint16)value>>8)&0xff);
						write_eeprom(MANUAL_INTERNAL_LO, (uint16)value&0xff);
					}
					else
					{
						if((EEP_TempSelect == THERMISTER_TYPE2)||(EEP_TempSelect == THERMISTER_TYPE3))
						{
							internal_calibration = value - temperature_org;
							write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR , internal_calibration&0xFF ) ;
							write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR + 1, internal_calibration>>8 ) ;
							Calibration_Internal_LO = internal_calibration&0xFF ;
							Calibration_Internal_HI = internal_calibration>>8; 
						}
					}		
			}
			else if(i == 9)//humidity
			{
				;
			}
			else if(i == 10)
			{
				;
			}

			else
			{
				;
			}
		break;
			
		case AO:
      output_priority[i+5][priority] = value*100;
			
//				if(i==0)
//				{
//					ManualAO1_HI = (uint16)value >> 8 & 0xff;
//					ManualAO1_LO = (uint16)value & 0xff;					
//					write_eeprom(MANUAL_COOL_VALVE_HI,ManualAO1_HI);				
//					write_eeprom(MANUAL_COOL_VALVE_LO,ManualAO1_LO);
//				  set_output(TIM4,2, value);					
//				}
//				if(i==1)
//				{
//					ManualAO2_HI = (uint16)value >> 8 & 0xff;
//					ManualAO2_LO = (uint16)value & 0xff;					
//					write_eeprom(MANUAL_HEAT_VALVE_HI,ManualAO2_HI);				
//					write_eeprom(MANUAL_HEAT_VALVE_LO,ManualAO2_LO);
//					set_output(TIM4,3, value);							
//				}

		break;

		case BO:
			output_priority[i][priority] = value;
			if(output_priority[i][7] == 0xff)
				SetByteBit(&EEP_OutputManuEnable,i,0);
			
//				ManualRelay(i) = (uint8)value;
//				write_eeprom(EEP_MANU_RELAY1 + i, ManualRelay(i));	
			
//			SetByteBit(&ManualRelayAll,i,(uint8)value);
//		  write_eeprom(MANUAL_RELAY,ManualRelayAll);
		break;

		 case SCHEDULE:
				//if(weekly_routines[i].auto_manual == 1)
//				{
//					weekly_routines[i].value = swap_double(value * 1000);
//				}
				break;
			case CALENDAR:
				//if(annual_routines[i].auto_manual == 1)
//				{
//					annual_routines[i].value = swap_double(value * 1000);
//				}
				break;		
		
		
		default:
		break;		
	}
	//if((AV_Present_Value[0] == Modbus.address) ||(A V_Present_Value[0] == 0) ) // internal AV, DI ,AV ...
//	{
//		switch(type)
//		{
//			case AV:
////			//				if(priority <= 8)	// manual
////			//					vars[i].auto_manual = 1;	
////			//				else 	// auto
////			//					vars[i].auto_manual = 0;
////			//												
////			//				if(vars[i].auto_manual == 1)
////				bacnet_AV.avs[i] = value;
////			//   store AVS			
////				Store_AVS(i);			
//				break;
//			case AI:
////				if(priority <= 8)	// manual
////					inputs[i].auto_manual = 1;	
////				else // auto
////					inputs[i].auto_manual = 0;
////						
////				if(inputs[i].auto_manual == 1)
////					inputs[i].value = swap_double(value);

//				break;
//			case BO:
////			//				if(priority <= 8)	// manual
////			//					outputs[i].auto_manual = 1;	
////			//				else 	// auto
////			//					outputs[i].auto_manual = 0;
////					
////				if(outputs[i].auto_manual == 1)
////					outputs[i].control = value;
//				break;
//			case AO:				
////			//				if(priority <= 8)	// manual
////			//					outputs[i + max_dos].auto_manual = 1;	
////			//				else 	// auto
////			//					outputs[i + max_dos].auto_manual = 0;
////				
////				if(outputs[i + max_dos].auto_manual == 1)
////				{
////					outputs[i + max_dos].value = swap_double(value * 1000);
////				}
//				break;
//	
//			default:
//			break;
//		}			
//	}
}
//-------------------------------------------------
void write_bacnet_name_to_buf(uint8_t type,uint8_t priority,uint8_t i,char* str)
{

	//if((AV_Present_Value[0] == Modbus.address) ||(AV_Present_Value[0] == 0) ) // internal AV, DI ,AV ...
	//{
		switch(type)
		{
			case AI:
				if(i < MAX_AIS)
				{
					memcpy(inputs[i].label,str,9);
					inputs[i].label[8] = 0;
					write_page_en[IN_TYPE] = 1;
				}
				break;
			case AO:
				if(i < MAX_AOS)
				{
					memcpy(outputs[i].label,str,9); 
					outputs[i].label[8] = 0;
					write_page_en[OUT_TYPE] = 1;
				} 
				break;
				
			case BO:
				if(i < MAX_BOS)
				{
					memcpy(outputs[i+2].label,str,9); 
					outputs[i+2].label[8] = 0;
					write_page_en[OUT_TYPE] = 1;
				} 
				break;				
				
				
			case AV:
				if(i < MAX_AVS) 
				{
					memcpy(var[i].label,str,9); 
					var[i].label[8] = 0;
					 write_page_en[VAR_TYPE] = 1;
				}
				break;
				
			case SCHEDULE:
				if(i < MAX_WR) 
				{
					memcpy(weekly_routines[i].label,str,9); 
					weekly_routines[i].label[8] = 0;
					 write_page_en[WR_TYPE] = 1;
				}
				break;

			case CALENDAR:
				if(i < MAX_AR) 
				{
					memcpy(annual_routines[i].label,str,9); 
					annual_routines[i].label[8] = 0;
					 write_page_en[AR_TYPE] = 1;
				}
				break;				
				
			default:
			break;
		}
			
//	}		
}
//---------------------------------------------------
void write_bacnet_unit_to_buf(uint8_t type,uint8_t priority,uint8_t i,uint8_t unit)
{
	U8_T temp;
	
	//if((AV_Present_Value[0] == Modbus.address) ||(AV_Present_Value[0] == 0) ) // internal AV, DI ,AV ...
	{
		switch(type)
		{
//			case AI:
//			//				if(priority <= 8)	// manual
//			//					inputs[i].auto_manual = 1;	
//			//				else // auto
//			//					inputs[i].auto_manual = 0;
//			//						
//			//				if(inputs[i].auto_manual == 1)
//			if(unit == UNITS_NO_UNITS)
//			{
//				inputs[i].range = not_used_input;			
//			}
//			if(unit == UNITS_DEGREES_CELSIUS)
//			{
//				inputs[i].range = R10K_40_120DegC;
//			}
//			if(unit == UNITS_DEGREES_FAHRENHEIT)
//			{
//				inputs[i].range = R10K_40_250DegF;	
//			}				
//			if(unit == UNITS_AMPERES)
//			{
//					inputs[i].range = I0_20ma;
//				// software jumper 
//				temp = inputs[i].decom;
//				temp &= 0x0f;
//				temp |= (INPUT_I0_20ma << 4);
//				inputs[i].decom = temp;
//			}
//			if(unit == UNITS_VOLTS)
//			{
//					inputs[i].range = V0_10_IN;
//				// software jumper 
//				temp = inputs[i].decom;
//				temp &= 0x0f;
//				temp |= (INPUT_0_10V << 4);
//				inputs[i].decom = temp;
//			}
//			
//			if( (unit != UNITS_VOLTS) && (unit != UNITS_AMPERES) )
//			{
//					// software jumper 
//				temp = inputs[i].decom;
//				temp &= 0x0f;
//				temp |= (INPUT_THERM << 4);
//				inputs[i].decom = temp;
//			}
//				break;
//			case BO:
//				outputs[i].digital_analog = 0;
//				if(unit == UNITS_NO_UNITS)
//					outputs[i].range = 0;
//				else
//					outputs[i].range = OFF_ON;
//				break;
//			case AO:
//				outputs[i + max_dos].digital_analog = 1;
//				if(unit == UNITS_NO_UNITS)
//					outputs[i + max_dos].range = 0;
//				else
//					outputs[i + max_dos].range = V0_10;			
//				break;
	
			default:
			break;
		}
			
	}	
}
//------------------------------------------------------------
char get_AM_Status(uint8_t type,uint8_t num)
{	
		return 0;
}
//------------------------------------------------------------
void write_bacent_AM_to_buf(uint8_t type,uint8_t i,uint8_t am)
{
//	if(type == BO)
//	{
//		outputs[i].auto_manual = am;		
//	}
//	if(type == AO)
//	{
//		outputs[i + max_dos].auto_manual = am;		
//	}
}
//------------------------------------------------------------
void add_remote_panel_db(uint32_t device_id,uint8_t panel,uint8_t protocal)
{				
}
//------------------------------------------------------------

extern const uint8 Var_label[MAX_AVS][9];
char* get_label(uint8_t type,uint8_t num)
{
//if(num == 0) return "null";    //start from var1.
//	else num -= 1;
	
	switch(type)
	{
	 case AV: 
		if(num < MAX_AVS)
		{
			return (char *)var[num].label;
			//return (char *)Var_label[num]; 			
		}  
		break;
		
	 case AI:
		 if(num < MAX_AIS)
			return (char *)inputs[num].label;
		break;
		 
	 case AO: 
		 if(num < MAX_AOS)
			return (char *)outputs[num].label; 
		break;
		 
	  case BO:		  
			if(num < MAX_BOS)
			return (char *)outputs[num+2].label; 	 
		break;
			
	  case SCHEDULE:		  
			if(num < MAX_WR)
			return (char *)weekly_routines[num].label; 	 
		break;			

	  case CALENDAR:		  
			if(num < MAX_AR)
			return (char *)annual_routines[num].label; 	 
		break;	
			
	 default:
	 break;
	}
	return "null";
}
//------------------------------------------------------------
char* get_description(uint8_t type,uint8_t num)
{
	switch(type)
				{
					case AV: 
						if(num < MAX_AVS) 
						return (char *)var[num].description;   
					break;
						
					case AI: 
						if(num < MAX_AIS)
						return (char *)inputs[num].description;
					break;
						
					case AO: 
						if(num < MAX_AOS)
						return (char *)outputs[num].description;  
					break;
						
					case BO: 
						if(num < MAX_BOS)
						return (char *)outputs[num+2].description;  
					break;
						
					case SCHEDULE: 
						if(num < MAX_WR)
						return (char *)weekly_routines[num].description;  
					break;	
						
					case CALENDAR: 
						if(num < MAX_AR)
						return (char *)annual_routines[num].description;  
					break;	
						
					default:
					break;
				}
			return "null";
}
//------------------------------------------------------------
//UNITS_VOLTS
//UNITS_MILLIAMPERES
//UNITS_DEGREES_CELSIUS
//UNITS_DEGREES_FAHRENHEIT
//UNITS_NO_UNITS
//UNITS_PERCENT
char get_range(uint8_t type,uint8_t num)
{
	switch(type)
	{
		case AV: 
			return UNITS_NO_UNITS;
		break;
		
	 case AI:
		 if(num < 8)
		 {
			if((AI_Range(num) == AI_RANGE_UNUSED) || (AI_Range(num) == AI_RANGE_VOLTAGE_5V) || (AI_Range(num) == AI_RANGE_UNUSED_10V )||(AI_Range(num) == AI_RANGE_VOLTAGE_10V))
				return UNITS_VOLTS;
			
			else if((AI_Range(num) == AI_RANGE_10K_THERMISTOR_TYPE2) || (AI_Range(num) == AI_RANGE_10K_THERMISTOR_TYPE3)|| (AI_Range(num) == AI_RANGE_50K_THERMISTOR))
			{
				if(EEP_DEGCorF == 0)//degree C
				 return UNITS_DEGREES_CELSIUS;
				else
				 return UNITS_DEGREES_FAHRENHEIT;
			}
			
			else if((AI_Range(num)== AI_RANGE_PERCNETAGE)|| (AI_Range(num)== AI_RANGE_PERCNETAGE_10V))
				return UNITS_PERCENT;
			
			else if(AI_Range(num) == AI_RANGE_MA)
				return UNITS_MILLIAMPERES;
			
			else 
				return UNITS_NO_UNITS;
		 }
		else if(num == 8)//internal temperature
		{
			if(EEP_DEGCorF == 0)//degree C
				return UNITS_DEGREES_CELSIUS;
			else
				return UNITS_DEGREES_FAHRENHEIT;	
		}
		else if(num == 9)
			return UNITS_PERCENT;
		 
		 break;
		 
	 case AO: 
		 if(num == 0)//ao1
		 {
			 if(EEP_Output1Scale == OUTPUT_ANALOG_RANGE_ONOFF)
				 return UNITS_NO_UNITS ;
			 else
				 return UNITS_VOLTS;
		 }
		 else if(num == 1)
		 {			 
			 if(EEP_Output2Scale == OUTPUT_ANALOG_RANGE_ONOFF)
				 return UNITS_NO_UNITS ;
			 else
				 return UNITS_VOLTS;
		 }
		break;
		 
	  case BO:		  
			return UNITS_NO_UNITS; 	 
		break;
			
	 default:
	 break;
	
	}
}
//------------------------------------------------------------
void Set_Object_Name(char * name)
{
	uint8 i;
	for(i=0;i<20;i++)
	{
		UserInfo_Name(i) = name[i];
		write_eeprom((EEP_TSTAT_NAME1 + i),name[i]);
	}				
}

//------------------------------------------------------------
void write_bacnet_description_to_buf(uint8_t type,uint8_t priority,uint8_t i,char* str)
{
switch(type)
		{ 
			case AO:
				if(i < MAX_AOS)
				{
					memcpy(outputs[i].description,str,20); 
					write_page_en[OUT_TYPE] = 1;
				} 
				break;

			case BO:
				if(i < MAX_BOS)
				{
					memcpy(outputs[i+2].description,str,20); 
					write_page_en[OUT_TYPE] = 1;
				} 
				break;
				
			case AI:
				if(i < MAX_AIS)
				{
					memcpy(inputs[i].description,str,20); 
					write_page_en[IN_TYPE] = 1;
				} 
				break;
				
			case AV:
				if(i < MAX_AVS) 
				{
					memcpy(var[i].description,str,20); 
					var[i].description[20] = 0;
					write_page_en[VAR_TYPE] = 1; 
				}
				break;
	
			default:
			break;
		} 
}
//------------------------------------------------------------
char* Get_Object_Name(void)
{
 return (void *)&(UserInfo_Name(0));
}
//------------------------------------------------------------
void Set_TXEN(U8_T dir)
{
if(dir == 1)//send
	TXEN = SEND;
else
	TXEN = RECEIVE;
}
//------------------------------------------------------------
u8 	UART_Get_SendCount(void)
{
return 1;
}
//------------------------------------------------------------
void uart_send_string(U8_T *p, U16_T length,U8_T port)
{
//				TXEN = SEND;
//			uart_send[0] = 0xaa;
//			USART_SendDataString(1);
	memcpy(uart_send, p, length);
	USART_SendDataString(length);
}
//------------------------------------------------------------

//------------------------------------------------------------
#if BAC_SCHEDULE

BACNET_TIME_VALUE Get_Time_Value(uint8_t object_index,uint8_t day,uint8_t i)
{
	BACNET_TIME_VALUE far array;
	array.Time.hour = wr_times[object_index][day].time[i].hours;
	array.Time.min = wr_times[object_index][day].time[i].minutes;
	array.Time.sec = 0;
	array.Time.hundredths = 0;
	array.Value.type.Enumerated = wr_time_on_off[object_index][day][i];//(i + 1) % 2;
	//array.Value.type.Enumerated = (i + 1) % 2;
	array.Value.tag = BACNET_APPLICATION_TAG_ENUMERATED;
	return array;

}

//------------------------------------------------------------
uint8_t Get_TV_count(uint8_t object_index,uint8_t day)
{
	char count;
	char i;
	count = 0;
	for(i = 0;i < 6;i++)
	{
		if(wr_time_on_off[object_index][day][i] != 0xff)
			count++;
		//if((wr_times[object_index][day].time[i].hours == 0) && (wr_times[object_index][day].time[i].minutes == 0))
			//return i;
	}
	return count;
}
//------------------------------------------------------------
BACNET_DEVICE_OBJECT_PROPERTY_REFERENCE * Get_Object_Property_References(uint8_t i)
{
	return NULL;
}
//------------------------------------------------------------

//void map_schedule_flag(uint8 day, uint8 event, uint8 value)
//{
//  uint8 flag_temp=0;

//	flag_temp = ScheduleMondayFlag(event/2 + day*3);
//	
//	if(event%2 == 0)//low event
//	{
//		if(value == 1)
//			flag_temp |= EVENT_DHOME;
//		else if(value == 0)
//			flag_temp |= EVENT_WORK;
//		else if(value == 0xff)
//			flag_temp |= EVENT_NULL;
//	}
//	else
//	{
//		if(value == 1)
//			flag_temp |= (EVENT_DHOME<<3);
//		else if(value == 0)
//			flag_temp |= (EVENT_WORK<<3);
//		else if(value == 0xff)
//			flag_temp |= (EVENT_NULL<<3);	
//	}
// 		
//	
//}

//void write_Time_Value(uint8_t index,uint8_t day,uint8_t i,uint8_t hour,uint8_t min/*BACNET_TIME_VALUE time_value*/)
void write_Time_Value(uint8_t index,uint8_t day,uint8_t i,uint8_t hour,uint8_t min/*BACNET_TIME_VALUE time_value*/,uint8_t value)
{	
	uint8 flag_temp=0;
	if(i<6)
	{
		if(value == 0xff)
		{
			wr_times[index][day].time[i].hours = 0;//time_value.Time.hour;
			wr_times[index][day].time[i].minutes = 0;//time_value.Time.min;
			
			ScheduleMondayEvent1(day*12+i*2) = 0xff;
			ScheduleMondayEvent1(day*12+i*2+1) = 0xff;
			
			write_eeprom(EEP_SCHEDULE_MONDAY_EVENT1_H+day*12+i*2, 0xff);
			write_eeprom(EEP_SCHEDULE_MONDAY_EVENT1_H+day*12+i*2+1, 0xff);
		}
		else
		{
			wr_times[index][day].time[i].hours = hour;//time_value.Time.hour;
			wr_times[index][day].time[i].minutes = min;//time_value.Time.min;
			
			ScheduleMondayEvent1(day*12+i*2) = hour;
			ScheduleMondayEvent1(day*12+i*2+1) = min;
			
			write_eeprom(EEP_SCHEDULE_MONDAY_EVENT1_H+day*12+i*2, hour);
			write_eeprom(EEP_SCHEDULE_MONDAY_EVENT1_H+day*12+i*2+1, min);		
		}
		
		wr_time_on_off[index][day][i] = value;

		flag_temp = ScheduleMondayFlag(i/2 + day*3);
		
		if(i%2 == 0)//low event
		{
			if(value == 1)
			{
				flag_temp &= 0xf8;
				flag_temp |= EVENT_DHOME;
			}
			else if(value == 0)
			{
				flag_temp &= 0xf8;
				flag_temp |= EVENT_WORK;
			}
			else if(value == 0xff)
			{
				flag_temp &= 0xf8;
				flag_temp |= EVENT_NULL;
			}
		}
		else
		{
			if(value == 1)
			{
				flag_temp &= 0xc7;
				flag_temp |= (EVENT_DHOME<<3);
			}
			else if(value == 0)
			{	
				flag_temp &= 0xc7;
				flag_temp |= (EVENT_WORK<<3);
			}
			else if(value == 0xff)
			{
				flag_temp &= 0xc7;
				flag_temp |= (EVENT_NULL<<3);	
			}
		}
		
		ScheduleMondayFlag(i/2 + day*3) = flag_temp;
		write_eeprom(EEP_SCHEDULE_MONDAY_FLAG+(i/2) + day*3, flag_temp);

	}
	schedule_change = 1;
}

//void write_Time_Value(uint8_t index,uint8_t day,uint8_t i,uint8_t hour,uint8_t min/*BACNET_TIME_VALUE time_value*/,uint8_t value)
//{	
//	ChangeFlash = 1;
//	write_page_en[WR_TIME] = 1;
//	wr_times[index][day].time[i].hours = hour;//time_value.Time.hour;
//	wr_times[index][day].time[i].minutes = min;//time_value.Time.min;
//	wr_time_on_off[index][day][i] = value;	
//}

//------------------------------------------------------------
uint8_t Get_CALENDAR_count(uint8_t object_index)
{
	uint16_t i,j;
	uint16_t count;
	count = 0;
	//test
	//for(i= 5;i<25;i++)
	//	ar_dates[0][i] = 0x55;
	//test end
	for(i = 0;i < AR_DATES_SIZE;i++)
	{
		for(j = 0;j < 8;j++)
		{
			if(ar_dates[object_index][i] & (0x01 << j)) 
			{
				count++;
			}
		}
	}
	return count;
}
//-------------------------------------------------------------	

U8_T far month_length[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


void get_bacnet_date_by_dayofyear(uint16_t dayofyear,BACNET_DATE * array)
{
	uint16_t far day;
	uint8_t far i;//,j,k;

	day = dayofyear;

	//if( ( Rtc.Clk.year & '\x03' ) == '\x0' )
	if( Is_Leap_Year == 0)
		month_length[1] = 29;
	else
		month_length[1] = 28;
	
	for(i = 0;i < 12;i++)
	{
		Test[10 + i] = month_length[i];
		if(day > month_length[i])
		{
			day -= month_length[i];
		}
		else
		{
			array->month = i + 1;
			array->day = day + 1;
			i = 12;
		}
	}
	
	//if(Rtc.Clk.day_of_year > dayofyear)
	if(today_dayofyear > dayofyear)
	{
	//	day = Rtc.Clk.week + 7 - (today_dayofyear - dayofyear) % 7;
		day = calendar.week + 7 - (today_dayofyear - dayofyear) % 7;
	}
	else
		//day = Rtc.Clk.week + (dayofyear - today_dayofyear) % 7;
		day = calendar.week + (dayofyear - today_dayofyear) % 7;
	
	array->wday = day % 7;
	//array->year = 2000 + Rtc.Clk.year;
	array->year = calendar.w_year;
}
//-------------------------------------------------------------	
BACNET_DATE Get_Calendar_Date(uint8_t object_index,uint8_t k)
{
	BACNET_DATE far array;
	
	uint16_t i,j;
	uint16_t dayofyear;
	uint16_t count;
	count = 0;

	for(i = 0;i < AR_DATES_SIZE;i++)
	{	
		for(j = 0;j < 8;j++)
		{
			if(ar_dates[object_index][i] & (0x01 << j)) 
			{
				if(k == count) // find current count
				{
					dayofyear = i * 8 + j;
					// map to BACNET_DATE
					get_bacnet_date_by_dayofyear(dayofyear,&array);
				}
				count++;	
			}					
		}
	}
	//av[5] = array.year;
	//av[6] = array.month;
	//av[7] = array.day;
	return array;
}
//-------------------------------------------------------------	
void write_annual_date(uint8_t index,BACNET_DATE date)
{
uint16_t day;
	char j;

	//if(date.year != 2000 + Rtc.Clk.year) return;
	if(date.year != calendar.w_year) return;
	
	//if( ( Rtc.Clk.year & '\x03' ) == '\x0' )
	if( Is_Leap_Year == 0)
		month_length[1] = 29;
	else
		month_length[1] = 28;
	
	day = 0;
	if(date.month >=1 && date.month <= 12)
	{
	for(j = 0;j < date.month - 1;j++)
		day += month_length[j];
	}
	day += date.day;

	ar_dates[index][(day - 1) / 8] |= (0x01 << ((day - 1) % 8));

	
	ScheduleDay((day - 1) / 8) = ar_dates[index][(day - 1) / 8];
	write_eeprom(EEP_SCHEDULE_DAY_BEGAIN +(day - 1) / 8, ScheduleDay((day - 1) / 8));
//ScheduleDay();
	
}
	
#endif
//------------------------------------------------------------


void check_weekly_routines(void)
{
 /* Override points can only be local to the panel even though the
		structure would allow for points from the network segment to which
		the local panel is connected */
	S8_T w, i, j;
	S32_T value;
	Str_weekly_routine_point *pw;
	Time_on_off *pt;
	
#if 1
	pw = &weekly_routines[0];
	for( i=0; i< MAX_WR; pw++, i++ )
	{		
		//w = Rtc.Clk.week - 1; //calendar.
		w = calendar.week - 1;
		if( w < 0 ) w = 6;
		if( pw->auto_manual == 0 )  // auto
		{		
			if( pw->override_2.point_type )
			{				
				//TBD get_point_value( (Point*)&pw->override_2, &value );//weekly_routines[point->number].value
				value = weekly_routines[i].value;
				pw->override_2_value = value?1:0;
				if( value )		w = 8;
			}
			else
			 	pw->override_2_value = 0;
		
			if(pw->override_1.point_type)
			{				
				//TBD get_point_value( (Point*)&pw->override_1, &value );
				value = weekly_routines[i].value;
				pw->override_1_value = value?1:0;
				if( value )
					w = 7;
			}
			else
			 	pw->override_1_value = 0;
		
		
			pt = &wr_times[i][w].time[2*MAX_INTERVALS_PER_DAY-1];

			j = 2 * MAX_INTERVALS_PER_DAY - 1;
		/*		for( j=2*MAX_INTERVALS_PER_DAY-1; j>=0; j-- )*/
		/*	do
			{				
				pt->hours = 10 + j;
				pt->minutes = 25 + j;
				pt--;
				
			}
			while( --j >= 0 );
			pt = &wr_times[i][w].time[2*MAX_INTERVALS_PER_DAY-1];
			j = 2 * MAX_INTERVALS_PER_DAY - 1; 
		 */

			do
			{				
				if( pt->hours || pt->minutes )
				{	
					if(calendar.hour == pt->hours)//if( Rtc.Clk.hour > pt->hours ) break;
						if(calendar.hour == pt->hours)//if( Rtc.Clk.hour == pt->hours )
							if(calendar.min >= pt->minutes)//if( Rtc.Clk.min >= pt->minutes )
							break;
				}
				pt--;
				
			}
			while( --j >= 0 );
			
			if( j < 0)		
				pw->value = 0;
			else
			{ 				
				if( j & 1 ) /* j % 2 */
				{
					pw->value = 0;//SetByteBit(&pw->flag,0,weekly_value,1);
				}
				else
				{
					pw->value = 1;//SetByteBit(&pw->flag,1,weekly_value,1);
				}    
			}
		}
	}
#endif
}
//------------------------------------------------------------


//-------------------------------------------------------------

void check_annual_routines( void )
{
	S8_T i;
	S8_T mask;
	S8_T octet_index;
	Str_annual_routine_point *pr;

	pr = &annual_routines[0];
	for( i=0; i<MAX_AR; i++, pr++ )
	{
   	if( pr->auto_manual == 0 )
	 	{
			mask = 0x01;
			/* Assume bit0 from octet0 = Jan 1st */
			/* octet_index = ora_current.day_of_year / 8;*/
			octet_index = today_dayofyear>>3;//octet_index = (Rtc.Clk.day_of_year) >> 3;//Rtc.Clk.day_of_year: which day of the whole year. e.g:the first day of the year
			/* bit_index = ora_current.day_of_year % 8;*/    // ????????????????????????????
	/*		bit_index = ora_current.day_of_year & 0x07;*/
			mask = mask << (today_dayofyear & 0x07);//mask = mask << ((Rtc.Clk.day_of_year) & 0x07 );
			
			if(ar_dates[i][octet_index] & mask)
			{
				pr->value = 1;
			}
			else
				pr->value = 0;
	   	}
	}
  //	misc_flags.check_ar=0;
}
//------------------------------------------------------------
char Get_Out_Of_Service(uint8_t type,uint8_t num)//0: disable outside control output  1: enable outside control output 
{
  if(type == AO)
		return outputs[num].out_of_service;
	else if(type == BO)
		return outputs[num].out_of_service;
}


void write_Out_Of_Service(uint8_t type,uint8_t i,uint8_t am)
{
	

}


U16_T Get_Vendor_ID(void)
{
 return BACNET_VENDOR_ID_TEMCO;
}

const char*  Get_Vendor_Name(void)
{
  return BACNET_VENDOR_TEMCO;
}

const char*  Get_Vendor_Product(void)
{
 return BACNET_PRODUCT_TEMCO;
}


float Get_Output_Relinguish(uint8_t type,uint8_t i)
{
	return 0;
}	

void write_Output_Relinguish(uint8_t type,uint8_t i,float value)
{
	
}




void Check_wr_time_on_off(uint8_t i,uint8_t j,uint8_t mode)
{
	U8_T k;
	U8_T k1,k2;
	U16_T tmp1,tmp2,tmp;
	U8_T tmp_onoff;
//	Time_on_off tmp;
//	ChangeFlash = 1;
//#if ARM	
//	write_page_en[WR_TIME] = 1;
//#endif	
	
	for(k = 0;k < 6;k++)
	{
		if(mode == 0)  // T3000 write weekly roution
		{
			if(wr_times[i][j].time[k].hours == 0 && wr_times[i][j].time[k].minutes == 0 )
				wr_time_on_off[i][j][k] = 0xff;
			else
				wr_time_on_off[i][j][k] = (k + 1) % 2;
		}

		if(wr_time_on_off[i][j][k] == 0xff)
		{
			if(wr_times[i][j].time[k].hours == 0 && wr_times[i][j].time[k].minutes == 0 )
			{
				if(k == 0)  // first one is 0
				{
					if(wr_times[i][j].time[k + 1].hours == 0 && wr_times[i][j].time[k + 1].minutes == 0	)
						wr_time_on_off[i][j][k] = 0xff;
					else
						wr_time_on_off[i][j][k] = 1;
				}
				else
					wr_time_on_off[i][j][k] = 0xff;
			}
			else
				wr_time_on_off[i][j][k] = (k + 1) % 2;
		}
		
	}
	
	// sort
	for(k1 = 0;k1 < 6;k1++)
	{
		if(wr_time_on_off[i][j][k1] == 0xff)
			continue;

		tmp1 = 60L * wr_times[i][j].time[k1].hours + wr_times[i][j].time[k1].minutes;

		for(k2 = k1 + 1;k2 < 6;k2++)
		{			
			if(wr_time_on_off[i][j][k2] == 0xff)
				continue;

			tmp2 = 60L *  wr_times[i][j].time[k2].hours + wr_times[i][j].time[k2].minutes;

			if(tmp1 > tmp2)
			{
				tmp = tmp1;
				tmp1 = tmp2;
				tmp2 = tmp;
				
				wr_times[i][j].time[k1].hours = tmp1 / 60;
				wr_times[i][j].time[k1].minutes = tmp1 % 60;
				
				wr_times[i][j].time[k2].hours = tmp2 / 60;
				wr_times[i][j].time[k2].minutes = tmp2 % 60;
				
				
				tmp_onoff = wr_time_on_off[i][j][k1];
				wr_time_on_off[i][j][k1] = wr_time_on_off[i][j][k2]; 
				wr_time_on_off[i][j][k2] = tmp_onoff;
				
			}
		}
	}
}


void Clear_Time_Value(uint8_t index,uint8_t day)
{
	char i;
	
	for(i = 0;i < 6;i++)
	{
		wr_times[index][day].time[i].hours = 0;
		wr_times[index][day].time[i].minutes = 0;
		wr_time_on_off[index][day][i] = 0xff;
	}
}

U8_T Get_WR_ON_OFF(uint8_t object_index,uint8_t day,uint8_t i)
{
	return wr_time_on_off[object_index][day][i];
}

uint8_t Get_modbus_address(void)
{
	return Modbus.address;
}

uint8 get_schedule_flag(uint8 day, uint8 event)
{
  uint8 flag_temp=0;

	flag_temp = ScheduleMondayFlag(event/2 + day*3);
	
	if(event%2 == 0)//low event
	{
		if((flag_temp & 0x03) == EVENT_DHOME)
			return 1;
		else if((flag_temp & 0x03) == EVENT_WORK)
			return 0;
		else if((flag_temp & 0x03) == EVENT_NULL)
		{
//			ScheduleMondayEvent1(day*12 + event*2) = 0;
//			ScheduleMondayEvent1(day*12 + event*2 + 1) = 0;
			return 0xff;
		}
	}
	else//high event
	{
		if(((flag_temp>>3) & 0x03) == EVENT_DHOME)
			return 1;
		else if(((flag_temp>>3) & 0x03) == EVENT_WORK)
			return 0;
		else if(((flag_temp>>3) & 0x03) == EVENT_NULL)
		{
//			ScheduleMondayEvent1(day*12 + event*2) = 0;
//			ScheduleMondayEvent1(day*12 + event*2 + 1) = 0;			
			return 0xff;
		}
	}		
}

uint8 get_schedule_flag_2(uint8 day, uint8 event)
{
  uint8 flag_temp=0;

	flag_temp = Schedule2MondayFlag(event/2 + day*3);
	
	if(event%2 == 0)//low event
	{
		if((flag_temp & 0x03) == EVENT_DHOME)
			return 1;
		else if((flag_temp & 0x03) == EVENT_WORK)
			return 0;
		else if((flag_temp & 0x03) == EVENT_NULL)
		{
//			ScheduleMondayEvent1(day*12 + event*2) = 0;
//			ScheduleMondayEvent1(day*12 + event*2 + 1) = 0;
			return 0xff;
		}
	}
	else//high event
	{
		if(((flag_temp>>3) & 0x03) == EVENT_DHOME)
			return 1;
		else if(((flag_temp>>3) & 0x03) == EVENT_WORK)
			return 0;
		else if(((flag_temp>>3) & 0x03) == EVENT_NULL)
		{
//			ScheduleMondayEvent1(day*12 + event*2) = 0;
//			ScheduleMondayEvent1(day*12 + event*2 + 1) = 0;			
			return 0xff;
		}
	}		
}

uint8 get_current_event_flag(uint8 day, uint8 event)//day 0-7  event  0-5
{
 uint8 flag_temp,flag_temp1;

 flag_temp = ScheduleMondayFlag(day*3 + event/2);
 if((event % 2) == 0)
 {
	flag_temp1 = flag_temp & 0x03;
	return flag_temp1;
 }
 else
 {
	flag_temp1 = (flag_temp>>3) & 0x03;
	return flag_temp1;	 
 }	 
	
}

void Check_All_WR(void)//need intial this at first run time
{
	U8_T i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<6;j++)
		{
			wr_time_on_off[0][i][j] = get_schedule_flag(i,j);
		}
	}
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<6;j++)
		{
			wr_time_on_off[1][i][j] = get_schedule_flag_2(i,j);
		}
	}
	
	
//	for(i = 0;i < MAX_WR;i++)
//		for(j = 0;j < 8;j++)
//			Check_wr_time_on_off(i,j,1);
}


void Set_Daylight_Saving_Status(bool status)
{
	Rtc.Clk.is_dst = status;
}

bool Get_Daylight_Savings_Status(void)
{
	return Rtc.Clk.is_dst;
}

bool write_Local_Date(BACNET_DATE* array)
{
	memcpy(&Local_Date,array,sizeof(BACNET_DATE));
	Rtc.Clk.day = array->day;
	Rtc.Clk.mon = array->month;
	Rtc.Clk.week = array->wday;
	Rtc.Clk.year = array->year;
	
	 
//	calendar.w_date = array->day;
//	calendar.w_month = array->month;
//	calendar.week = array->wday;
//	calendar.w_year = array->year;	
	
	flag_Updata_Clock = 1;
	Rtc_Set(Rtc.Clk.year,Rtc.Clk.mon,Rtc.Clk.day,calendar.hour,calendar.min,calendar.sec,0);
	return 1;

}


bool write_Local_Time(BACNET_TIME* array)
{
	memcpy(&Local_Time,array,sizeof(BACNET_TIME));
	Rtc.Clk.hour = array->hour;
	Rtc.Clk.min = array->min;
	Rtc.Clk.sec = array->sec;
	
//	calendar.hour= array->hour;
//	calendar.min = array->min;
//	calendar.sec = array->sec;
	flag_Updata_Clock = 1;
	Rtc_Set(calendar.w_year-2000,calendar.w_month,calendar.w_date,Rtc.Clk.hour,Rtc.Clk.min,Rtc.Clk.sec,0);
	return 1;
}



S16_T Get_UTC_Offset(void)
{
	return timezone;
}


void Set_UTC_OFFset(S16_T tz)
{
	timezone = tz;
}

u32 Rtc_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec, u8 flag)
{

  RTC_Set(syear+2000, smon,  sday,  hour,  min,  sec);
}


void chech_mstp_collision(void)
{
}

void check_mstp_packet_error(void)
{
}

void check_mstp_timeout(void)
{}

	
bool Analog_Input_Change_Of_Value(unsigned int instance)
{
}
bool Analog_Value_Change_Of_Value(unsigned int instance)
{	
}
void Store_Instance_To_Eeprom(uint32_t Instance)
{
	write_eeprom(EEP_INSTANCE1, Instance);
	write_eeprom(EEP_INSTANCE2, (U8_T)(Instance >> 8));
	write_eeprom(EEP_INSTANCE3, (U8_T)(Instance >> 16));
	write_eeprom(EEP_INSTANCE4, (U8_T)(Instance >> 24));

}

void Store_MASTER_To_Eeprom(uint8_t master)
{
	write_eeprom(EEP_MAX_MASTER,master);
}
	
char get_current_mstp_port(void)
{
	return 2;
}

BACNET_POLARITY Binary_Output_Polarity(
		uint32_t instance)
{}

bool Binary_Output_Polarity_Set(
	uint32_t object_instance,
	BACNET_POLARITY polarity)
{}
	
U8_T Get_current_panel(void)
{}

U8_T MAX_MASTER;
//void handler_timesync(void)
//{

//}

//void handler_timesync_utc(void)
//{

//}







//------------------------------------------------------------