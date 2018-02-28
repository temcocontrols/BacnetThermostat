//#include "define.h"
//#include "eepDefine.h"
#include "FreeRTOS.h"
#include "task.h"
#include "control_logic.h"
#include "constcode.h"
#include "setpoint.h"
#include "lcd.h"
#include "TstatFunctions.h"
#include "eepdefine.h"
#include "24cxx.h"
#include "inputs.h"
#include "rtc.h"
//#include "task.h"

//uint8 OUTPUT_1TO5;
uint8 pwm1_percent[2];
uint8 pid_ctrl_bit[7];
extern int16 ctest[10];
extern void vStateSwitch( void *pvParameters );
extern void vCoolingLockOut( void *pvParameters );
extern void vHeatingLockOut( void *pvParameters );
uint8 stateswitchflag = 0;
uint8	init_delay_flag = 0;
uint16 xdata HEATING_LOCKOUT;
uint16 xdata COOLING_LOCKOUT;
uint8 xdata heating_lockout_time[3];
uint8 xdata cooling_lockout_time[3];
uint8 xdata heating_lock_10s_counter = 0;
uint8 xdata cooling_lock_10s_counter = 0;
uint16 xdata STATE_LOCK = 0;
uint16 idata   COOLING_MODE = 0;
uint16 idata   HEATING_MODE = 0;
uint16 xdata   COOLING_LOCKOUT = 0;
uint16 xdata   HEATING_LOCKOUT = 0;
uint16 xdata   output_buf[3];

uint8  xdata universal_pid[3][6];
//uint8	init_delay_flag;
uint8 heat_cool_flag;
int16 xdata valve_target_percent; //valve target position in percent
uint8 xdata pid[3] = {0,0,0};          //this should be changed to pid[2]
int16 valve[3];  	//this is an old variable name. It should be changed to analog_output[0]
et_mode_of_operation intended_mode_of_operation[3] ;  // global variable to keep track of 
et_mode_of_operation current_mode_of_operation[3] ;
et_sequence_of_operation   sequence_of_operation ; 
uint8 override_timer; 


//uint8 xdata transducer_max;
//uint8 xdata transducer_min;
extern uint16 xdata co2_data;
extern int16 xdata humidity;
int16 SetTransducerValue(uint8 output_function);

#ifdef STAGEDEBUG
uint8 xdata stagetest0 = 0;
uint8 xdata stagetest1 = 0;
uint8 xdata stagetest2 = 0;
#endif
int8 xdata pwm_number = 0;//how many outputs are set to PWM range 
//int16 ctestpid = 100;//ctest2,ctest3,ctest4,ctest5;
#ifdef VAV
extern uint8 xdata airflow_sensor_option;//
extern int16 xdata airflow;
extern uint8 xdata vav_pid;
extern uint8 xdata pid1_active_stage; 
extern uint8 xdata max_dual_vav_enable;//MAX_DUAL_VAV_ENABLE
extern uint16 xdata max_airflow_cooling;
extern uint16 xdata max_airflow_heating;
extern uint16 xdata occ_min_airflow;
extern uint16 xdata unocc_min_airflow;
extern uint16 xdata airflow_setpoint;
extern int16 xdata max_supply_setpoint;
extern int16 xdata min_supply_setpoint;
extern int16 xdata supply_setpoint;
#endif


//uint8 xdata heat_pid3_table;
//uint8 xdata cool_pid3_table;

uint8 output_floating_flag;
uint16  xdata  DELAY_FLAG = 0;
uint8 xdata  cool_pid[3], heat_pid[3];
uint8 xdata pidoutput[7];//confirm which pid control the output
uint16  xdata output_temp ;///////////////////////////
//extern uint8 OUT5;
 
uint8 timer_switch_status[2] = {0,0};
uint8 DIGITAL_IN;

uint8 timer_changed_status;
uint8 freeze_delay_off;
uint16 xdata freeze_timer_counter;

//uint16 xdata STATE_LOCK  ;
uint8 xdata lighting_stage;
 
extern void delay_us(u32 nus);
 
uint8 TIMER_FLAG;
 
uint8 xdata  mode_stage_changed[3];
uint8 xdata reset_poweron ;
uint8 timer_enable;
uint8 OVERRIDE_NEWDAY = 0;
uint8 ROTATION_NEWDAY = 0;
uint8 hour,minute,second;
typedef struct {
uint8 day;
uint8 hour;
uint8 minute;
uint8 second;
}CLOCK;

CLOCK desire_time[3] = {
{0},
{0},
{0}
};

 uint16 xdata time_counter;
void CalDesireTime( uint16 timevalue,uint8 units,uint8 type);

uint16 xdata giPwmTimeOn[2];
uint16 xdata giPwmTimeOff[2];
//uint8 xdata pwm[2];
/**************************LCD define start***********************************************/
//uint8 xdata gucHeatCool;
uint8 xdata gucManualValve;  
uint8 temp_bit;
//extern void vStateSwitch( void *pvParameters);

void CalDesireTime( uint16 timevalue,uint8 units,uint8 type)
{
	uint8 carry;
	uint8 temp;
	OVERRIDE_NEWDAY = 0;
	ROTATION_NEWDAY = 0;
	switch ( units )
	{
		case 0://second		
			temp = calendar.sec + timevalue % 60;
			carry = temp / 60;
			desire_time[type].second = temp % 60;
			timevalue -= timevalue % 60;
			timevalue /= 60;
			
			temp = calendar.min + timevalue % 60+(uint8)carry;
			carry = temp / 60;
			desire_time[type].minute = temp % 60;
			temp = calendar.hour +  (uint8)carry + timevalue/60;
			carry = temp / 24;
			desire_time[type].hour = temp % 24;
			desire_time[type].day = carry;
		break;
		case 1://minute
			desire_time[type].second = calendar.sec;
			temp = timevalue % 60 + calendar.min;
			carry = temp / 60;
			desire_time[type].minute = temp % 60;
			temp = calendar.hour + (uint8 )carry + timevalue/60;
			carry = temp / 24;
			desire_time[type].hour = temp % 24;
			desire_time[type].day = carry;
		
		break;
		case 2://hour
			desire_time[type].second = calendar.sec;
			desire_time[type].minute = calendar.min;
			temp = calendar.hour + timevalue;
			carry = temp / 24;
			desire_time[type].hour = temp % 24;
			
			desire_time[type].day = carry;
		break;
		default :
		break;
	}
	if(desire_time[type].second == 59)//make sure that the compare routine can detect rightly at critical time value
	desire_time[type].second = 58;

}
uint8 CompareTime( uint8 type )	
{
	if(!type)
	{
		if(OVERRIDE_NEWDAY == desire_time[type].day)
		{
			if(desire_time[type].hour == calendar.hour)
			{
				if(desire_time[type].minute == calendar.min)
				{
					if(desire_time[type].second <= calendar.sec)
						return TRUE;
					else 
						return FALSE;
				}
				else 
				return FALSE;
			}
			else
			return FALSE;
		}
		else 
		return FALSE;
	}
	else
	{
		if(ROTATION_NEWDAY == desire_time[type].day)
		{
			if(desire_time[type].hour == calendar.hour)
			{
				if(desire_time[type].minute == calendar.min)
				{
					if(desire_time[type].second <= calendar.sec)
						return TRUE;
					else 
						return FALSE;
				}
				else 
				return FALSE;
			}
			else
			return FALSE;
		}
		else 
		return FALSE;

	}
}
void  TimerSwitch(uint8 times)
{
	uint8 i ;
	uint8 temp_bit;
	if(timer_enable == 1)
	{
		for(i=0;i<times;i++)
		{
			temp_bit = CompareTime(i + 1);
//			if(i==0)
//			ctest[6] = temp_bit;
			if(temp_bit == 1 && timer_switch_status[i] == 0)
			{
				if(EEP_TimerSelect == 3)
				{
	   				 
					timer_switch_status[i] = 1;
					CalDesireTime(giPwmTimeOn[i],0,i + 1); 
					mode_stage_changed[i] = 1;
				}
				else
				{
	   				time_counter = (EEP_TimerOnHi << 8) + EEP_TimerOn;
					timer_switch_status[i] = 1;
					CalDesireTime(time_counter,EEP_TimerUnits,1);
				}
//				if(EEP_TimerSelect == 1)
//				{
//					timer_changed_status++;
//					i = CalOneNumber();
//					if(timer_changed_status > i)
//					timer_changed_status = 1;
//							 
//					rotate_left = time_counter;
//				}
				if(EEP_TimerSelect == 2 )
				{
					timer_switch_status[i] = 0;
					TIMER_FLAG = 0;
					timer_enable = 0;				 
					
				}
			}
			else if(temp_bit == 1 && timer_switch_status[i] == 1)
			{
				if(EEP_TimerSelect == 3)
				{	   				 
				
					if(giPwmTimeOff[i] ==0)
					{
						timer_switch_status[i] = 1;
						CalDesireTime(giPwmTimeOn[i],0,i + 1);
					}
					else
					{
						timer_switch_status[i] = 0;
//						if(i == 0)
//							ctest[7]++;
						CalDesireTime(giPwmTimeOff[i],0,i + 1);
						//mode_stage_changed[i] = 1;
					}
				}
				else
				{
					time_counter = (EEP_TimerOffHi << 8) + EEP_TimerOff;
					timer_switch_status[i] = 0;
					CalDesireTime(time_counter,EEP_TimerUnits,1);
				}
//				if(EEP_TimerSelect == 1)
//				{
//					timer_changed_status++;
//					i = CalOneNumber();
//					if(timer_changed_status > i)
//					timer_changed_status = 1;
//						 
//					rotate_left = time_counter;
//				}
			}
		}
		
	}
}













void JudgeDecrease(et_mode_of_operation current,et_mode_of_operation mode ,uint8 temp_pi)
{
	if(	current == mode)	
	{
		set_bit(&DELAY_FLAG,temp_pi + 3,0);						 
	}
	else
	{
		set_bit(&DELAY_FLAG,temp_pi + 3,1);
	}
}
//add this routine for reduce code space
void SetDelayFlag(uint8 i,uint8 setmode)
{
	if(setmode)
		current_mode_of_operation[i] = DAYTIME_COASTING ;
	set_bit(&DELAY_FLAG,i,1);
	set_bit(&DELAY_FLAG,i + 3,0);
}

void SetMode( uint8 temp )
{

				if (pid[temp] > pid_setpoint[temp] + 2 )     //....setpoints go to fixed unccupiued limits
				{	
					set_bit( &COOLING_MODE, temp ,FALSE );				 										 
					set_bit( &HEATING_MODE, temp ,TRUE );
				}
				else if (pid[temp] < pid_setpoint[temp] - 2) 
				{	
					set_bit( &COOLING_MODE, temp ,TRUE );
					set_bit( &HEATING_MODE, temp ,FALSE );
				}
				else
				{
					set_bit( &COOLING_MODE, temp ,FALSE );
					set_bit( &HEATING_MODE, temp ,FALSE );		
				}		 
}
//------------mode_operation --------------------
//Purpose:
//			1 the tstat is colling mode or heating mode.
//			2 calculate which operation mode  the tstat is .

void mode_operation( uint8 temp_pi )
{
	uint8 oldState[3];
	uint16  xdata tmp;
  uint8 xdata i ,j;
  uint8 xdata cool_heat_stage[6] ;
	uint8 temp_digital_in,temp_bit;
	//--------------FIRST DETERMINE THE HEATING COOLING MODE-------------------------------

	switch (EEP_HeatCoolConfig) 
	{		
		// Mode determined automatically.  I.E. HC > 0
		case HC_AUTOMATIC:  // Heating Cooling automatically determined by the PID.
			if ( sequence_of_operation == TRANSDUCER_MODE)  //switch based on the	current	sequence setting
			{
				//transducer mode als has one setpoint
				set_bit( &COOLING_MODE, 0 ,TRUE );
				set_bit( &HEATING_MODE, 0 ,FALSE );		//this type never goes into heat_mode
			}
			else
			{
				if((heat_cool_user == HC_CFG_HEAT) || (heat_cool_user == HC_CFG_COOL))//heat/cool mode is selected
					{
					SetMode(1);//pid2 mode still controled by pid

					if(heat_cool_user == HC_CFG_HEAT)
						{				 		
						set_bit( &COOLING_MODE, 0 ,FALSE );
						set_bit( &HEATING_MODE, 0 ,TRUE );

				 		}
					else
						{
						set_bit( &COOLING_MODE, 0 ,TRUE );
						set_bit( &HEATING_MODE, 0 ,FALSE );		
				 		}
					}
				 else				 	
					SetMode(temp_pi);
				SetMode(1);//pid2 mode still controled by pid
			} 
			break;

		case HC_KEYPAD:	//Heating Cooling determined by the keypad
			SetMode(1);//pid2 mode still controled by pid

			if(heat_cool_flag)
			{				 		
				set_bit( &COOLING_MODE, 0 ,FALSE );
				set_bit( &HEATING_MODE, 0 ,TRUE );
		 	}
			else
			{
				set_bit( &COOLING_MODE, 0 ,TRUE );
				set_bit( &HEATING_MODE, 0 ,FALSE );		
		 	}
//			if(reset_poweron > 0 )
//			{
				if(reset_poweron > 0)
				{
					reset_poweron--;
					if(reset_poweron == 0)
					{
						#ifdef GRIDPOINT
							TIMER_FLAG = 1;
							timer_enable = 1;
							CalDesireTime(time_counter,EEP_TimerUnits,1);
						#endif
					}
					set_bit( &COOLING_MODE, 0 ,FALSE );
					set_bit( &HEATING_MODE, 0 ,FALSE );	
				}

//			}
			break;

		// MDF 12/06/04 Added feature to tSS and DI1 to allow for 2-pipe operation.
		case HC_DI_ACTIVE_HIGH:	//Heating Cooling determined by the digital input

			if(AI_Function2 == 5)
			{
				if(analog_input[1] == 1)
					temp_bit = 1;
				else
					temp_bit =0;
			}
			else if(AI_Function1 == 5)
			{
				if(analog_input[0] == 1)
				temp_bit = 1;
				else
				temp_bit =0;
			}

			if (temp_bit)
			{
				set_bit( &COOLING_MODE, 0 ,FALSE );
				set_bit( &HEATING_MODE, 0 ,TRUE );
		 	}
			else
			{
				set_bit( &COOLING_MODE, 0 ,TRUE );
				set_bit( &HEATING_MODE, 0 ,FALSE );		
		 	}
			break;

		// MDF 12/06/04 Added feature to tSS and DI1 to allow for 2-pipe operation.
		case HC_DI_ACTIVE_LOW:	//Heating Cooling determined by the digital input
			if(GetByteBit(&EEP_InputManuEnable,2))
			{
				 //temp_digital_in = EEP_DigitalInput;
			}
			else
			{
				DIGITAL_IN = 1;
				temp_digital_in = DIGITAL_IN;
			}
			if(AI_Function2  == 5)
			{
				if(analog_input[1] == 1)
				temp_bit = 1;
				else
				temp_bit = 0;
			}
			else if(AI_Function1  == 5)
			{
				if(analog_input[0] == 1)
				temp_bit = 1;
				else
				temp_bit = 0;
			}

			if (!temp_bit)
			{
				set_bit( &COOLING_MODE, 0 ,FALSE );
				set_bit( &HEATING_MODE, 0 ,TRUE );
		 	}
			else
			{
				set_bit( &COOLING_MODE, 0 ,TRUE );
				set_bit( &HEATING_MODE, 0 ,FALSE );		
		 	}
			break;

//		case HC_ANALOG_IN1:	//Heating Cooling determined by the differnce between external sensor 1 and setpoint
//							// If the pipe is cooler than setpoint, we are in cooling mode. Else in heating mode.
//			tmp = analog_input[0];
//		//	if(EEP_DEGCorF)  
//			//	tmp = (tmp * 9)/5 +320;

//			if (tmp < temperature - 10)//((EEP_CoolingSpHi<<8)+ EEP_CoolingSp - 10))
//			{
//				set_bit( &COOLING_MODE, 0 ,TRUE );
//				set_bit( &HEATING_MODE, 0 ,FALSE );	
//				gucHeatCool = 2;
//		 	}
//			else if (tmp > temperature + 10)//((EEP_CoolingSpHi<<8)+ EEP_CoolingSp  + 10))
//			{
//				set_bit( &COOLING_MODE, 0 ,FALSE );
//				set_bit( &HEATING_MODE, 0 ,TRUE );	
//				gucHeatCool = 1;	
//		 	}
//			else
//				gucHeatCool = 0;
//			break;

//		case HC_ANALOG_IN2:	//Heating Cooling determined by the differnce between an external sensor 2 and setpoint

//			tmp = analog_input[1];
//			//if(EEP_DEGCorF)  
//				//tmp = (tmp * 9)/5 +320;

//			if (tmp < ((EEP_DayCoolingSpHi<<8)+ EEP_DayCoolingSpLo - 10))
//			{
//				set_bit( &COOLING_MODE, 0 ,TRUE );
//				set_bit( &HEATING_MODE, 0 ,FALSE );	
//		 	}
//			else if (tmp > ((EEP_DayCoolingSpHi<<8)+ EEP_DayCoolingSpLo + 10))
//			{
//				set_bit( &COOLING_MODE, 0 ,FALSE );
//				set_bit( &HEATING_MODE, 0 ,TRUE );		
//		 	}
//			break;
		
	} //----------------------END HEATING COOLING SWITCH-------------------------------


	

	//--------------NOW DETERMINE THE STAGE OF OPERATION-------------------------------						 
	        if(b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)] == 0)  //if there is no heat stage
	        {
	        	heat_pid[temp_pi] = 100 ;
	        	cool_pid[temp_pi] = universal_pid[temp_pi][0] ;
	        }
	        else if(b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)] == 6)
	        {
	        	cool_pid[temp_pi] = 0 ;
	        	heat_pid[temp_pi] = universal_pid[temp_pi][0] ;
	        }
	        else
	        {
	        	cool_pid[temp_pi] = universal_pid[temp_pi][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)]] ;
	        	heat_pid[temp_pi] = universal_pid[temp_pi][0] ;
	        }	   
		if (((pid[temp_pi] >= cool_pid[temp_pi]) && (pid[temp_pi] <= heat_pid[temp_pi])) || ((EEP_KeypadSelect==2||EEP_KeypadSelect<=4) && ((get_bit(HEATING_MODE,temp_pi) && pid[temp_pi] < heat_pid[temp_pi] ) || (get_bit(COOLING_MODE,temp_pi) && pid[temp_pi] > cool_pid[temp_pi]))))   
		{  
			intended_mode_of_operation[temp_pi] = DAYTIME_COASTING ;	
		}
		else		
		{
			if (get_bit(COOLING_MODE,temp_pi)) 
			{  					 
				for(i=0; i<b.eeprom[EEP_COOL_ORIGINAL_TABLE - (temp_pi<<1)]; i++)
				{
					cool_heat_stage[i] = universal_pid[temp_pi][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)] + i] ;
				}
				for(i=0; i<6 - b.eeprom[EEP_COOL_ORIGINAL_TABLE - (temp_pi<<1)]; i++)
		 
				{
					cool_heat_stage[i + b.eeprom[EEP_COOL_ORIGINAL_TABLE - (temp_pi<<1)]] =  2 ;
				}
				
				if(pid[temp_pi] <= 3)
				{
					i = pid[temp_pi] ;
					pid[temp_pi] = 3 ;//why here is three
				}
				if(pid[temp_pi] < cool_heat_stage[5]-2)
				{
					intended_mode_of_operation[temp_pi] = DAYTIME_COOLING6 ;
				}

				//else if (pid[temp_pi] > cool_heat_stage[5] && pid[temp_pi] <= cool_heat_stage[4]-2)						
				//	intended_mode_of_operation[temp_pi] = DAYTIME_COOLING5 ;
				//else if (pid[temp_pi] > cool_heat_stage[4] && pid[temp_pi] <= cool_heat_stage[3]-2)
				//	intended_mode_of_operation[temp_pi] = DAYTIME_COOLING4 ;
				//else if (pid[temp_pi] > cool_heat_stage[3] && pid[temp_pi] <= cool_heat_stage[2]-2)						
				//	intended_mode_of_operation[temp_pi] = DAYTIME_COOLING3 ;
				//else if (pid[temp_pi] > cool_heat_stage[2] && pid[temp_pi] <= cool_heat_stage[1]-2)
				//	intended_mode_of_operation[temp_pi] = DAYTIME_COOLING2 ;
				//else if (pid[temp_pi] > cool_heat_stage[1] && pid[temp_pi] <= cool_heat_stage[0]-2)						
				//	intended_mode_of_operation[temp_pi] = DAYTIME_COOLING1 ;
						 
				else if( pid[temp_pi] > cool_heat_stage[0])
					intended_mode_of_operation[temp_pi] = DAYTIME_COASTING ;
				else
				{
					for(j=0;j<4;j++)
					{						 
						if (pid[temp_pi] > cool_heat_stage[1 + j] && pid[temp_pi] < cool_heat_stage[j]-2)						
						intended_mode_of_operation[temp_pi] = DAYTIME_COOLING1 + j ;
					
					}		 
				}
					
				if (pid[temp_pi] >= cool_heat_stage[0]-2	 && pid[temp_pi] <= cool_heat_stage[0]	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING1 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COASTING )
					    intended_mode_of_operation[temp_pi] = DAYTIME_COOLING1 ;
				if (pid[temp_pi] >= cool_heat_stage[1]-2	 && pid[temp_pi] <= cool_heat_stage[1]	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING1 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING2 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_COOLING2 ;					
				if (pid[temp_pi] >= cool_heat_stage[2]-2	 && pid[temp_pi] <= cool_heat_stage[2]	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING2 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING3 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_COOLING3 ;
				if (pid[temp_pi] >= cool_heat_stage[3]-2	 && pid[temp_pi] <= cool_heat_stage[3]	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING3 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING4 )
				{
					    intended_mode_of_operation[temp_pi] = DAYTIME_COOLING4 ;
				}
				if (pid[temp_pi] >= cool_heat_stage[4]-2	 && pid[temp_pi] <= cool_heat_stage[4]	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING4 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING5 )
				{
					    intended_mode_of_operation[temp_pi] = DAYTIME_COOLING5 ;	
				}					
				if (pid[temp_pi] >= cool_heat_stage[5]-2	 && pid[temp_pi] <= cool_heat_stage[5]	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING5 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COOLING6 )
				{
					    intended_mode_of_operation[temp_pi] = DAYTIME_COOLING6 ;
				}
				if(pid[temp_pi] == 3)
					pid[temp_pi] = i ;
		 					
			}

  		if (get_bit(HEATING_MODE,temp_pi)) 
			{ 
			  for(i=0; i<b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)]; i++)
				{
				  cool_heat_stage[i] = universal_pid[temp_pi][i] ;
				}
				for(i=0; i<6 - b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)]; i++)
				{
					cool_heat_stage[i + b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (temp_pi<<1)]] = 102 ;
				}
				if(pid[temp_pi] > cool_heat_stage[5]+2)
					intended_mode_of_operation[temp_pi] = DAYTIME_HEATING6 ;

				//else if (pid[temp_pi] < cool_heat_stage[5] && pid[temp_pi] > cool_heat_stage[4]+2)						
				//  intended_mode_of_operation[temp_pi] = DAYTIME_HEATING5 ;
				//else if (pid[temp_pi] < cool_heat_stage[4] && pid[temp_pi] > cool_heat_stage[3]+2)
				//	intended_mode_of_operation[temp_pi] = DAYTIME_HEATING4 ;
				//else if (pid[temp_pi] < cool_heat_stage[3] && pid[temp_pi] > cool_heat_stage[2]+2)						
				//	intended_mode_of_operation[temp_pi] = DAYTIME_HEATING3 ;
				//else if (pid[temp_pi] < cool_heat_stage[2] && pid[temp_pi] > cool_heat_stage[1]+2)
				//	intended_mode_of_operation[temp_pi] = DAYTIME_HEATING2 ;
				//else if (pid[temp_pi] < cool_heat_stage[1] && pid[temp_pi] > cool_heat_stage[0]+2)						
				//	intended_mode_of_operation[temp_pi] = DAYTIME_HEATING1 ;

				else if( pid[temp_pi] < cool_heat_stage[0])
					intended_mode_of_operation[temp_pi] = DAYTIME_COASTING ;
				else
				{
					for(j=0;j<4;j++)
					{
						if(pid[temp_pi] < cool_heat_stage[j+1] && pid[temp_pi] > cool_heat_stage[j]+2)
						intended_mode_of_operation[temp_pi] = DAYTIME_HEATING1 + j;					
					}		 
				}
				 	
				if (pid[temp_pi] >= cool_heat_stage[0] 	 && pid[temp_pi] <= cool_heat_stage[0] + 2	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING1 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_COASTING )
					    intended_mode_of_operation[temp_pi] = DAYTIME_HEATING1 ;
				if (pid[temp_pi] >= cool_heat_stage[1] 	 && pid[temp_pi] <= cool_heat_stage[1] + 2	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING1 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING2 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_HEATING2 ;					
				if (pid[temp_pi] >= cool_heat_stage[2] 	 && pid[temp_pi] <= cool_heat_stage[2] + 2	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING2 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING3 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_HEATING3 ;
				if (pid[temp_pi] >= cool_heat_stage[3] 	 && pid[temp_pi] <= cool_heat_stage[3] + 2	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING3 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING4 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_HEATING4 ;
				if (pid[temp_pi] >= cool_heat_stage[4] 	 && pid[temp_pi] <= cool_heat_stage[4] + 2	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING4 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING5 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_HEATING5 ;					
				if (pid[temp_pi] >= cool_heat_stage[5] 	 && pid[temp_pi] <= cool_heat_stage[5] + 2	 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING5 
						&& intended_mode_of_operation[temp_pi] !=	DAYTIME_HEATING6 )
					    intended_mode_of_operation[temp_pi] = DAYTIME_HEATING6 ;
			 
			}   //if temperature < heating_setpoint

		}

// intended_mode_of_operation is the mode in which the tstat should be, based on the PID
// current_mode_of_operation is the current mode of the tstat.

// The follow system of delays gives a 10 second delay whenever the mode of operation is increased.
// i.e. going from coasting to C1 or from coasting to H1 or from C1 to C2 or C2 to C3.
// There is no delay when decreasing - i.e. going from H3 to coasting or from C3 to C1.

		oldState[temp_pi] = current_mode_of_operation[temp_pi] ;
		if( get_bit(DELAY_FLAG,temp_pi) )
			{
			if(!get_bit(STATE_LOCK,temp_pi))	
				{
					// If the mode is in coasting.... check the lockouts before changing mode
					if (current_mode_of_operation[temp_pi]==DAYTIME_COASTING)
					{
						
						// if switching into heating, check the heating lockout
						if(intended_mode_of_operation[temp_pi]>DAYTIME_COOLING6 && !get_bit(HEATING_LOCKOUT,temp_pi))
							current_mode_of_operation[temp_pi] = DAYTIME_HEATING1;
						// if switching into cooling, check the cooling lockout
						else if (intended_mode_of_operation[temp_pi]>DAYTIME_COASTING && intended_mode_of_operation[temp_pi]<DAYTIME_HEATING1 && !get_bit(COOLING_LOCKOUT,temp_pi))
							current_mode_of_operation[temp_pi]++ ;	
						 
					}

					else
					{
						
						current_mode_of_operation[temp_pi]++;				 
					}
					set_bit(&DELAY_FLAG,temp_pi,0);
				}
			
			}
			
		if( get_bit(DELAY_FLAG,temp_pi+3) )
			{
				if(!get_bit(STATE_LOCK,temp_pi))	
				{
					 
					if (current_mode_of_operation[temp_pi]==DAYTIME_HEATING1)
					{
					
						current_mode_of_operation[temp_pi] = DAYTIME_COASTING;
					}
					else
					{
						if(current_mode_of_operation[temp_pi] != DAYTIME_COASTING)
						current_mode_of_operation[temp_pi]--;
				 
					}
					set_bit(&DELAY_FLAG,temp_pi+3,0);
				}
			
			}
//		ctest[4] = intended_mode_of_operation[0];	
		switch ( intended_mode_of_operation[temp_pi] ) 
			{	
				case DAYTIME_COOLING1 :
					if((current_mode_of_operation[temp_pi]>DAYTIME_COASTING)&&(current_mode_of_operation[temp_pi]<DAYTIME_HEATING1))
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
							//current_mode_of_operation[temp_pi] = DAYTIME_COOLING1 ;
							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_COOLING1,temp_pi);
							#ifdef PIDMODETEST
//							if(temp_pi == 0)
//							ctestpid = 1;
							#endif
						}	
					else
						{
							SetDelayFlag(temp_pi,1);
//							if(temp_pi == 0)
//							ctestpid = 111;
						}	
				break;

				case DAYTIME_COOLING2 :
					if((current_mode_of_operation[temp_pi]>DAYTIME_COOLING1)&&(current_mode_of_operation[temp_pi]<DAYTIME_HEATING1))
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
							//current_mode_of_operation[temp_pi] = DAYTIME_COOLING2 ;
							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_COOLING2,temp_pi);
//							if(temp_pi == 0)
//							ctestpid = 2;
						}	
					else if(current_mode_of_operation[temp_pi] == DAYTIME_COOLING1)
						{
							SetDelayFlag(temp_pi,0);
//							if(temp_pi == 0)
//							ctestpid = 221;
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
//							if(temp_pi == 0)
//							ctestpid = 222;
						}	
				break;

				case DAYTIME_COOLING3 :
					if((current_mode_of_operation[temp_pi]>DAYTIME_COOLING2)&&(current_mode_of_operation[temp_pi]<DAYTIME_HEATING1))
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
							//current_mode_of_operation[temp_pi] = DAYTIME_COOLING3 ;
							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_COOLING3,temp_pi);
//							if(temp_pi == 0)
//							ctestpid = 3;
						}	
					else if((current_mode_of_operation[temp_pi]==DAYTIME_COOLING2)||(current_mode_of_operation[temp_pi]==DAYTIME_COOLING1))
						{
							SetDelayFlag(temp_pi,0);
//							if(temp_pi == 0)
//							ctestpid = 331;
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
//							if(temp_pi == 0)
//							ctestpid = 332;
						}	
				break;
				case DAYTIME_COOLING4 :
					if((current_mode_of_operation[temp_pi]>DAYTIME_COOLING3)&&(current_mode_of_operation[temp_pi]<DAYTIME_HEATING1))
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
						//	current_mode_of_operation[temp_pi] = DAYTIME_COOLING4 ;
						JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_COOLING4,temp_pi);
						}	
					else if((current_mode_of_operation[temp_pi]>=DAYTIME_COOLING3)&&(current_mode_of_operation[temp_pi]<=DAYTIME_COOLING1))
						{
							SetDelayFlag(temp_pi,0);
						}		
					else 
						{
							SetDelayFlag(temp_pi,1);
						}	
				break;
				case DAYTIME_COOLING5 :
					if((current_mode_of_operation[temp_pi]>DAYTIME_COOLING4)&&(current_mode_of_operation[temp_pi]<DAYTIME_HEATING1))
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
							//current_mode_of_operation[temp_pi] = DAYTIME_COOLING5 ;
							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_COOLING5,temp_pi);
						}	
					else if((current_mode_of_operation[temp_pi]<=DAYTIME_COOLING4)&&(current_mode_of_operation[temp_pi]>=DAYTIME_COOLING1))
						{
							SetDelayFlag(temp_pi,0);
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
						}	
				break;
				case DAYTIME_COOLING6 :
					if(current_mode_of_operation[temp_pi]==DAYTIME_COOLING6)
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
							current_mode_of_operation[temp_pi] = DAYTIME_COOLING6 ;
						}	
					else if((current_mode_of_operation[temp_pi]<=DAYTIME_COOLING5)&&(current_mode_of_operation[temp_pi]>=DAYTIME_COOLING1))
						{
							SetDelayFlag(temp_pi,0);
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
						}	
				break;
				case DAYTIME_HEATING1 :
					if(current_mode_of_operation[temp_pi]>DAYTIME_COOLING6)
						{
							set_bit(&DELAY_FLAG,temp_pi,0); 
						//	current_mode_of_operation[temp_pi] = DAYTIME_HEATING1 ; 
 							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_HEATING1,temp_pi);
//						if(temp_pi == 0)
//							ctestpid = 7;

						}	
					else
						{
							SetDelayFlag(temp_pi,1);
//							if(temp_pi == 0)
//							ctestpid = 771;
						}	
				break;

				case DAYTIME_HEATING2 :
					if(current_mode_of_operation[temp_pi]>DAYTIME_HEATING1)
						{
							set_bit(&DELAY_FLAG,temp_pi,0); 
							//current_mode_of_operation[temp_pi] = DAYTIME_HEATING2 ;
							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_HEATING2,temp_pi);
//							if(temp_pi == 0)
//							ctestpid = 8;
						}	
					else if(current_mode_of_operation[temp_pi] == DAYTIME_HEATING1)
						{
							SetDelayFlag(temp_pi,0);
//							if(temp_pi == 0)
//							ctestpid = 881;
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
//							if(temp_pi == 0)
//							ctestpid = 882;
						}	
				break;

				case DAYTIME_HEATING3 :
					if(current_mode_of_operation[temp_pi] > DAYTIME_HEATING2)
						{
							set_bit(&DELAY_FLAG,temp_pi,0); 
							//current_mode_of_operation[temp_pi] = DAYTIME_HEATING3 ;
							JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_HEATING3,temp_pi); 
 
						}	
					else if((current_mode_of_operation[temp_pi]==DAYTIME_HEATING2)||(current_mode_of_operation[temp_pi]==DAYTIME_HEATING1))
						{
							SetDelayFlag(temp_pi,0);
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
						}	
				break;
				case DAYTIME_HEATING4 :
					if(current_mode_of_operation[temp_pi]>DAYTIME_HEATING3)
						{
							set_bit(&DELAY_FLAG,temp_pi,0); 
							//current_mode_of_operation[temp_pi] = DAYTIME_HEATING4 ;
JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_HEATING4,temp_pi);
 
						}	
					else if((current_mode_of_operation[temp_pi]<=DAYTIME_HEATING3)||(current_mode_of_operation[temp_pi]>=DAYTIME_HEATING1))
						{
							SetDelayFlag(temp_pi,0);
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
						}	
				break;
				case DAYTIME_HEATING5 :
					if(current_mode_of_operation[temp_pi]>DAYTIME_HEATING4)
						{
							set_bit(&DELAY_FLAG,temp_pi,0); 
							//current_mode_of_operation[temp_pi] = DAYTIME_HEATING5 ;
 
JudgeDecrease(current_mode_of_operation[temp_pi],DAYTIME_HEATING5,temp_pi);
						}	
					else if((current_mode_of_operation[temp_pi]<=DAYTIME_HEATING4)||(current_mode_of_operation[temp_pi]>=DAYTIME_HEATING1))
						{
							SetDelayFlag(temp_pi,0);
						}	
					else 
						{
							SetDelayFlag(temp_pi,1);
						}	
				break;
				case DAYTIME_HEATING6 :
					if(current_mode_of_operation[temp_pi]>DAYTIME_HEATING5)
						{
							set_bit(&DELAY_FLAG,temp_pi,0);
							current_mode_of_operation[temp_pi] = DAYTIME_HEATING6 ;
						}	
					else if((current_mode_of_operation[temp_pi]<=DAYTIME_HEATING5)||(current_mode_of_operation[temp_pi]>=DAYTIME_HEATING1))
						{
							//set_bit(&DELAY_FLAG,temp_pi,1);
//set_bit(&DELAY_FLAG,temp_pi + 2,0);
SetDelayFlag(temp_pi,0);
						}	
					else 
						{
SetDelayFlag(temp_pi,1);
						//	current_mode_of_operation[temp_pi] = DAYTIME_COASTING ;
//set_bit(&DELAY_FLAG,temp_pi + 2,0);
							//set_bit(&DELAY_FLAG,temp_pi,1);
						}	
				break;
				
				default :  // Coasting mode
							set_bit(&DELAY_FLAG,temp_pi,0);
							if(current_mode_of_operation[temp_pi] >= DAYTIME_COOLING1)
								set_bit(&DELAY_FLAG,temp_pi + 3,1);
							else
							{
								current_mode_of_operation[temp_pi] = intended_mode_of_operation[temp_pi] ;
								set_bit(&DELAY_FLAG,temp_pi + 3,0);
							}
				break;		

			}

// --------------------------MODE DELAYS -------------------------

		// If the mode has changed, we want to start the short delay timer.
		if(oldState[temp_pi] != current_mode_of_operation[temp_pi])
		{
			mode_stage_changed[0] = 1;
			mode_stage_changed[1] = 1;
			mode_stage_changed[2] = 1;
			set_bit(&STATE_LOCK, temp_pi, 1);			
		  stateswitchflag = 10;
			xTaskCreate( vStateSwitch, ( signed portCHAR * ) "StateSwitch", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );
//			start_timer(STATE_SWITCH_TIMER, DEFAULT_TIMEOUT);
		}

		// if the new mode of operation is coasting, start up the appropriate lockouts
		if (current_mode_of_operation[temp_pi] == DAYTIME_COASTING)
		{
			// If we are entering coasting mode from cooling mode, we need to lock out cooling for the cycling delay
			// and lock out heating for the crossover delay
			if (oldState[temp_pi] > DAYTIME_COASTING && oldState[temp_pi] < DAYTIME_HEATING1)
			{
				if(CyclingDelay)//if(read_eeprom(EEP_CYCLING_DELAY))
				{
					set_bit(&COOLING_LOCKOUT,temp_pi,1) ; //cooling_lockout = 1;
					cooling_lockout_time[temp_pi] = CyclingDelay;//read_eeprom(EEP_CYCLING_DELAY);
					xTaskCreate( vCoolingLockOut, ( signed portCHAR * ) "CoolingLockOut", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	//				start_timer(COOLING_LOCKOUT_TIMER, DEFAULT_TIMEOUT);
				}
				if(ChangeOverDelay)
				{
					set_bit(&HEATING_LOCKOUT,temp_pi,1) ; //heating_lockout = 1;
					heating_lockout_time[temp_pi] = ChangeOverDelay;//read_eeprom(EEP_CHANGOVER_DELAY);
					xTaskCreate( vHeatingLockOut, ( signed portCHAR * ) "HeatingLockOut", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	//				start_timer(HEATING_LOCKOUT_TIMER, DEFAULT_TIMEOUT);
				}
			}
	
			// If we are entering coasting mode from heating mode, we need to lock out heating for the cycling delay
			// and lock out cooling for the crossover delay
			else if (oldState[temp_pi] >= DAYTIME_HEATING1)
			{
				if(CyclingDelay)//if(read_eeprom(EEP_CYCLING_DELAY))
				{
					set_bit(&HEATING_LOCKOUT,temp_pi,1) ; //heating_lockout = 1;
					heating_lockout_time[temp_pi] = CyclingDelay;//read_eeprom(EEP_CYCLING_DELAY);
					xTaskCreate( vHeatingLockOut, ( signed portCHAR * ) "HeatingLockOut", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	//				start_timer(HEATING_LOCKOUT_TIMER, DEFAULT_TIMEOUT);
				}
				if(ChangeOverDelay)
				{
					set_bit(&COOLING_LOCKOUT,temp_pi,1) ; //cooling_lockout = 1;
					cooling_lockout_time[temp_pi] = ChangeOverDelay;//read_eeprom(EEP_CHANGOVER_DELAY);
					xTaskCreate( vCoolingLockOut, ( signed portCHAR * ) "CoolingLockOut", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	//				start_timer(COOLING_LOCKOUT_TIMER, DEFAULT_TIMEOUT);
				}
			}
		}

		// This is to bypass the delay timers
		if(init_delay_flag==0)	
		{
			DELAY_FLAG = 0 ;
			COOLING_LOCKOUT = 0;
			HEATING_LOCKOUT = 0;
			if(intended_mode_of_operation[0]>=7)
				intended_mode_of_operation[0] = 0;
			current_mode_of_operation[temp_pi] = intended_mode_of_operation[temp_pi] ;
			init_delay_flag = 1 ;
		}
}
//
//CalPwmTime(temp_output,test_buf,temp_output,test_buf);//
void CalPwmTime(uint8 percent,uint8 range,uint8 percent1,uint8 range1)
{
	uint8 xdata ucPercent;
	uint32 xdata ulTemp;
	uint8 ucRange; 
	uint8 xdata i,j; 

	  
 	ucPercent = percent;
	for(i=0;i<2;i++)
	{
		if (i == 0)
		{
			ulTemp = (uint16)(EEP_TimerOnHi << 8) + EEP_TimerOn;
			ucRange = (range >> 4) & 0x0f;
			j = percent;

		}
		else
		{
			ulTemp = (uint16)(EEP_TimerOffHi << 8) + EEP_TimerOff;
			ucRange = range1 & 0x0f;
			j = percent1;
		}
	
		switch(ucRange)
		{
			case VALVE_CLOSED://0% = 0
				ucPercent = 0;
			break;
			case VALVE_OPEN: //100% = 1
				ucPercent = 100;
			break;
			case PID1: //0 - 100% = 2
				ucPercent = j;
			break;
			case PID2: //50 - 100% = 3
				ucPercent = 50 + (j >> 1);
			break;
			case PID3: //0 - 50% = 4
				ucPercent = j >> 1;
			break;
		
		}
    //if(i == 0)
		pwm1_percent[i] = ucPercent; 			
		
		if(EEP_TimerUnits == 1)//change the timer units to second to make sure the accuracy
		ulTemp = ulTemp * 60;
		else if(EEP_TimerUnits == 2)
		ulTemp = ulTemp * 3600;
		
		if(i==0)
		{
//			ctest[3] = ulTemp;
//			ctest[4] = ucPercent;
//			ctest[5] = mode_stage_changed[i];
		}
	
		giPwmTimeOn[i]  = ulTemp*ucPercent/100;
		giPwmTimeOff[i] = ulTemp*(100 - ucPercent)/100;
		
//		ctest[1] = giPwmTimeOn[0];
		
		//pwm[i] = ucPercent;

		if(giPwmTimeOn[i] > 0 && ucPercent > 0 && mode_stage_changed[i] == 1)
		{		 		
			timer_enable = 1;								
			CalDesireTime(giPwmTimeOn[i],0,i+1); 				
			timer_switch_status[i] = 1;
			mode_stage_changed[i] = 0;
		} 
		if(giPwmTimeOn[i] == 0 || ucPercent == 0) 
		{
			timer_switch_status[i] = 0;	   //if cool stage,timer_switch_status[0] = 1; timer_switch_status[1] = 0;  
		}
		//if(i == 0)
//			ctest[2] = timer_switch_status[0];
//			ctest[0] = timer_enable;
	}

}

void  OutputValue(uint8 testBUF,uint8 *ValveValue)
{

	uint8 xdata test_buf1;
	uint32 xdata tmp;
	uint8 xdata i,j;
	uint8 xdata ucHeatTableNumber; //0 to 6
	uint8 xdata ucCoolTableNumber; //0 to 6

  for(i=0;i<2;i++)
	{
		j = b.eeprom[EEP_PID_OUTPUT6 + i];// 0 or 1,pid0 or pid1
		if(b.eeprom[EEP_INTERLOCK_OUTPUT6 + i] == 7)//free cool
			test_buf1 = testBUF;
		else
			test_buf1 = (ValveValue[j] >> (i*2)) & 0x03;		
		
		ucHeatTableNumber = b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (j << 1)];//original table number(pid0) or universal table number(pid1)
		ucCoolTableNumber = b.eeprom[EEP_COOL_ORIGINAL_TABLE - (j << 1)];
		
		if(frc_mode == FRC_FUNCTION_START)
		{
			//if free cooling enable and use AO1, set AO1 range to 0-100%
			if((EEP_PidOutput6 == 1) && (i == 0)) 
				test_buf1 = PID1;
			//if free cooling enable and use AO2, set AO2 range to 0-100%
			if((EEP_PidOutput7 == 1) && (i == 1)) 
				test_buf1 = PID1;
		}
		

		switch(test_buf1)         // valve2
				{
					case VALVE_CLOSED:	//==0
						tmp = 0;
					break;

					case PID1:	 //==2	0-100%
					//pid_test4 = 91;
					 	if(pid[j] < cool_pid[j])
							{
							if((ucHeatTableNumber == 0)&&(current_mode_of_operation[j] == 0))
								tmp = mul(universal_pid[j][ucCoolTableNumber -1 + current_mode_of_operation[j]] - pid[j], 1000 )/universal_pid[j][ucHeatTableNumber + ucCoolTableNumber -1];								
							else
								tmp = mul(universal_pid[j][ucHeatTableNumber -1 + current_mode_of_operation[j]] - pid[j], 1000 )/universal_pid[j][ucHeatTableNumber + ucCoolTableNumber -1];																							 //universal_pid[j][ucCoolTableNumber + b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (EEP_PidOutput6 << 1)]-1];
							if(tmp >1000)
								tmp = 1000;
							}
						else if(pid[j] > heat_pid[j])
							{
							tmp = mul(pid[j] - universal_pid[j][current_mode_of_operation[j]-7], 1000 )/(100 - universal_pid[j][ucHeatTableNumber - 1]);
							if(tmp > 1000)
								tmp = 1000 ;
							}
     						 
						else
							tmp = 0;
					break;

					case PID2:   //valve opens in stage2 heating
					//pid_test4 = 92;
					 if(pid[j] < cool_pid[j])
						{
							if(ucHeatTableNumber == 0)
							 	tmp = mul(universal_pid[j][ucCoolTableNumber -1 + current_mode_of_operation[j]] - pid[j], 1000 )/universal_pid[j][ucHeatTableNumber + ucCoolTableNumber -1];
							else
								tmp = mul(universal_pid[j][ucHeatTableNumber -1 + current_mode_of_operation[j]] - pid[j], 1000 )/universal_pid[j][ucHeatTableNumber + ucCoolTableNumber -1];
							
							tmp += 500;
							if(tmp >1000)
								tmp = 1000;
						}
						else if(pid[j] > heat_pid[j])
						{
						tmp = mul(pid[j] - universal_pid[j][current_mode_of_operation[j]-7], 500 )/(100 - universal_pid[j][ucHeatTableNumber - 1]);
//   						tmp = mul(pid[j] - universal_pid[j][current_mode_of_operation[j]-7], 500 )/(100 - universal_pid[j][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (j << 1)] - 1]);
						tmp += 500;
							if(tmp >1000)
								tmp = 1000;
						}					
//								heating_valve[0] =	 mul (pid[temp_pi] - HEATING1_PID -12 , 100 )	;		//valve	acts from 0 to 100% over first stage		 
						else
							tmp = 0 ;
					break;
					case VALVE_OPEN:
							tmp = 1000;
					break;
				} 
    


				if(b.eeprom[EEP_INTERLOCK_OUTPUT6 + i] == 7)
					test_buf1 = 0;
				else
					test_buf1 = ValveValue[j] & (0x40<< i);

				if(test_buf1)
				{				       						
					if(pid[j] < cool_pid[j])
					{
						if(ucHeatTableNumber == 0)							
							 	tmp = mul(universal_pid[j][ucCoolTableNumber -1 + current_mode_of_operation[j]] - pid[j], 1000 )/universal_pid[j][ucHeatTableNumber + ucCoolTableNumber -1];
						else						 	 
							tmp = mul(universal_pid[j][ucHeatTableNumber -1 + current_mode_of_operation[j]]- pid[j] , 500 )/universal_pid[j][ucCoolTableNumber + ucHeatTableNumber-1];	
					}									  		 
					else if(pid[j] > heat_pid[j])//(pid[EEP_PidOutput6] > heat_pid[EEP_PidOutput6])
						//tmpv = mul(pid[EEP_PidOutput7] - universal_pid[EEP_PidOutput7][current_mode_of_operation[EEP_PidOutput7]-7], 500 )/(100 - universal_pid[EEP_PidOutput7][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (EEP_PidOutput7 << 1)] - 1]);
						tmp = mul(pid[j] - universal_pid[j][current_mode_of_operation[j]-7], 500 )/(100 - universal_pid[j][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - (j << 1)] - 1]);
					else 
						tmp = 0;
				} 
		 
 
				
#ifdef VAV
				if((airflow_sensor_option == NO) && (j == 2))//if no airflow sensor and outputs are set to PID3
					{
					if(occupied == 1)//occupied mode
						{
						if(current_mode_of_operation[0] <= DAYTIME_COOLING6)  //cooling stage
							{
							tmp = (max_airflow_cooling - max_airflow_heating); //
							tmp	*= (100 - pid[0]);
							tmp /= 100;
							tmp += occ_min_airflow;
							}
						else
							{
							tmp = (max_airflow_cooling - max_airflow_heating); //
							tmp	*= pid[0];
							tmp /= 100;
							tmp += occ_min_airflow;	
							}
						if(tmp < occ_min_airflow) 
							tmp = occ_min_airflow;
						}
					else  //unoccupied mode
						tmp = unocc_min_airflow;
					}
#endif				
				if(tmp >1000)
					tmp = 1000;
				


			    if(b.eeprom[EEP_OUTPUT1_SCALE + i] != OUTPUT_ANALOG_RANGE_PWM)
					{
					if(b.eeprom[EEP_OUTPUT1_SCALE + i] == ZERO_FIVEVOLTS)       // 0 - 5 v
					 	valve[i] = tmp / 2;	   // * 100 / 2
					else if(b.eeprom[EEP_OUTPUT1_SCALE + i] == TWO_TENVOLTS)  // this is for a 2-10v valve 
						{
						valve[i] = tmp * 5 / 6 + 200;   // * 5 / 6
						if(valve[i] >1000)
							valve[i] = 1000;
						}
					else if (b.eeprom[EEP_OUTPUT1_SCALE + i] == TEN_ZEROVOLTS)        					// reverse 
						valve[i] = 1000 - tmp; 	
					else
					 	valve[i] = tmp;	   			// * 100
					
					
					}
	}
		if((EEP_Interlock6 == 2 && mul_analog_input[0] == 0)  || (EEP_Interlock6 == 3 && mul_analog_input[1] == 0))
							valve[0] = 0;	
		if((EEP_Interlock7 == 2 && mul_analog_input[0] == 0)  || (EEP_Interlock7 == 3 && mul_analog_input[1] == 0))
						valve[1] = 0;		

}

// ---------------- control_logic ---------------------					 
//Purpose:	service	the valve, do control logic
//			Set OUTPUT_1TO5, heating_value and cooling_value			
//Called:   by control logic timer, do this roughly every one second
//Note:		the PID's should be ready before this is run
//			not critical though since eventually the PID's will kick in
//Note2: 	runs every one second, but not exactly since sometimes other routines will
//          refresh by calling this when the user presses the fan buttons for example
//Note3:    OK for managing timed pulses, as long as you call this when pulses timeout.
//          For example, if pulse manager shows a pulse is over, then call this routine to 
//          refresh the outputs conditions
//Note4:    Only this routine can actually turn an output on or off

 void control_logic( )  
{
    //uint16  output_temp ;
	uint8 test_buf ,valve_value[3],temp_output,test_buf1,temp_output1,i;
	#ifdef VAV
	uint8 xdata test_buf2;
	#endif
	uint8 relay_num,pwmindex[2];
	uint16 xdata  temp_output2;
	int16 temp ;
	uint8 float_buf = 0;
	uint8 temp_bit;
//	start_timer( CONTROL_LOGIC_TIMER, DEFAULT_TIMEOUT ) ;//	----- reset the	timer

	//-----store the state of the digital outputs to allow a short delay

	//FUTURE: allow	for more delay between speed changes, also inlude HEAT2
	//        each time one digital output changes state, a timer is started
	//        the digital outputs cannot change state again until
	//        the timer has expired
	//        the delay can be a new menu item "DEL" for delay

		//dont store the value of the valve, this is analog
		//only store digital output items
//	setpoint( ) ;
//	original_setpoint = Get_current_setpoint(occupied);
//	loop_setpoint[PID_LOOP1] = Get_current_setpoint(occupied);
	if(occupied)
		{
		cooling_db = EEP_DayCoolingDeadband;
		heating_db = EEP_DayHeatingDeadband;
		}
	else
		{
		cooling_db = EEP_NightCoolingDeadband;
		heating_db = EEP_NightHeatingDeadband;
		}
  for(test_buf=0; test_buf<3; test_buf++)
			mode_operation( test_buf );

	//make current mode is the highest heating mode in freeze protect 
	if(freeze_delay_off == 1)
	{
		current_mode_of_operation[0] = EEP_HEAT_TABLE1 + DAYTIME_COASTING;
		freeze_timer_counter = (uint16)EEP_FreezeDelayOff* 60;	
	}
	// Read the OUTPUT_1TO5 from the SOP table.
	//OUTPUT_1TO5 = read_eeprom(EEP_FAN0_OPER_TABLE_COAST + fan_speed_user * 7 + current_mode_of_operation[0] );
	for(test_buf=0; test_buf<4; test_buf++)
	{
		if(read_eeprom(EEP_PID_OUTPUT4 + test_buf) == 3)
		{
			if(pid[0] > pid[1])
				b.eeprom[EEP_PID_OUTPUT4 + test_buf] = 0;
			else
				b.eeprom[EEP_PID_OUTPUT4 + test_buf] = 1;
		}
		else if(read_eeprom(EEP_PID_OUTPUT4 + test_buf) == 4)
		{
			if(pid[0] < pid[1])
				b.eeprom[EEP_PID_OUTPUT4 + test_buf] = 0;
			else
				b.eeprom[EEP_PID_OUTPUT4 + test_buf] = 1;
		}
	}
//-------------------------------------------------------------PID1----------------------------------------------------------------------
	
	
	
	if(current_mode_of_operation[0] <= DAYTIME_COOLING6)//cooling mode
	{    
		// get output status from output table accordingto fan speed and current mode ,pid1 is working
		output_buf[0] = FanTable0(fan_speed_user * 7 + current_mode_of_operation[0]);//read_eeprom(EEP_FAN0_OPER_TABLE_COAST + fan_speed_user * 7 + current_mode_of_operation[0]);		
		// the following lines for OUT4&OUT5 PWM output,caculate which range be used ,read pwm table.Caculate the percentage in some stage
		
		if(fan_speed_user == 0)
			test_buf = Output_PWM_Off_Table(current_mode_of_operation[0]);
		else
			test_buf = Output_PWM_Table(current_mode_of_operation[0]);//read_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + current_mode_of_operation[0]); //PWM range
		temp_output = mul(universal_pid[0][EEP_HEAT_TABLE1 -1 + current_mode_of_operation[0]] - pid[0], 100 ) / universal_pid[0][EEP_COOL_TABLE1 + EEP_HEAT_TABLE1-1];
		//PWM percentage
		//temp_output = temp_output/universal_pid[0][EEP_COOL_TABLE1 + EEP_HEAT_TABLE1-1];
		if(	temp_output > 100)
		temp_output = 100;	
		 		
	}
	else  //heating mode
	{
	//  temp_output = EEP_FAN0_OPER_TABLE_COAST + fan_speed_user * 7 + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1); //6-x = 
	    output_buf[0] = FanTable0(fan_speed_user * 7 + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));//read_eeprom(EEP_FAN0_OPER_TABLE_COAST + fan_speed_user * 7 + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));
		//	output_buf[0] = read_eeprom(EEP_FAN0_OPER_TABLE_COAST + fan_speed_user * 7 + current_mode_of_operation[0] - 3);
		if(fan_speed_user == 0)
			test_buf = Output_PWM_Off_Table(current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));//read_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));
		else
			test_buf = Output_PWM_Table(current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));
		temp_output = mul(pid[0] - universal_pid[0][current_mode_of_operation[0]-7], 100 )/(100 - universal_pid[0][EEP_HEAT_TABLE1 - 1]);
//    ctest[1] = temp_output;
		if(	temp_output > 100) 
		temp_output = 100;
	 	
	}//
//-------------------------------------------------------------PID2----------------------------------------------------------------------

	if(current_mode_of_operation[1] <= DAYTIME_COOLING6)// cooling mode
	{ 
		if(fan_speed_user == 0)
		{
			output_buf[1] = PID2_Output_OFF_Table(current_mode_of_operation[1]);//read_eeprom(EEP_UNIVERSAL_OFF_TABLE_BEGIN + current_mode_of_operation[1]);
			test_buf1 = Output_PWM_Off_Table(current_mode_of_operation[1]);
		}
			else
			{
		// get output status from output table accordingto fan speed and current mode ,pid2 is working
			output_buf[1] = PID2_Output_Table(current_mode_of_operation[1]);//read_eeprom(EEP_UNIVERSAL_OUTPUT_BEGIN + current_mode_of_operation[1]);
			test_buf1 = Output_PWM_Table(current_mode_of_operation[1]);	
			}
		//read_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + current_mode_of_operation[1]);
		temp_output1 = mul(universal_pid[1][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - 2] -1 + current_mode_of_operation[1]] - pid[1], 100 )/universal_pid[1][b.eeprom[EEP_COOL_ORIGINAL_TABLE - 2] + b.eeprom[EEP_HEAT_ORIGINAL_TABLE - 2]-1];
		if(	temp_output1 > 100)
		temp_output1 = 100;			 
	}
	else  //heating mode
	{ 
		if(fan_speed_user == 0)
		{
			output_buf[1] = PID2_Output_OFF_Table(current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));//read_eeprom(EEP_UNIVERSAL_OFF_TABLE_BEGIN + current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2)); 
			test_buf1 = Output_PWM_Off_Table(current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2	));
		}
		else
		{
			output_buf[1] =  PID2_Output_Table(current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));//read_eeprom(EEP_UNIVERSAL_OUTPUT_BEGIN + current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));
			test_buf1 = Output_PWM_Table(current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2	));
			
		}
		//read_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2	));
		temp_output1 = mul(pid[1] - universal_pid[1][current_mode_of_operation[1]-7], 100) / (100 - universal_pid[1][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - 2] - 1]);
		//temp_output1 = temp_output1/(100 - universal_pid[1][b.eeprom[EEP_HEAT_ORIGINAL_TABLE - 2] - 1]);
		if(	temp_output1 > 100) 
		temp_output1 = 100;		 
	}
//-------------------------------------------------------------PID3----------------------------------------------------------------------
//#ifdef VAV
//	if((airflow_sensor_option == YES)) //if airflow sensor is present
//		{
		if(current_mode_of_operation[2] <= DAYTIME_COOLING6)//cooling mode
			{    
			if(fan_speed_user == 0)
			{
				output_buf[2] = PID3_Output_OFF_Table(current_mode_of_operation[2]);//read_eeprom(EEP_PID3_OFF_OUTPUT_BEGIN + current_mode_of_operation[2]);
				test_buf2 = Output_PWM_Off_Table(current_mode_of_operation[2]);
			}
			else 
			{				
				output_buf[2] = PID3_Output_Table(current_mode_of_operation[2]);//read_eeprom(EEP_PID3_OUTPUT_BEGIN + current_mode_of_operation[2]);
				test_buf2 = Output_PWM_Table(current_mode_of_operation[2]);
			}
			//read_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + current_mode_of_operation[2]);
	 
			temp_output2 = mul(universal_pid[2][EEP_HEAT_TABLE3 -1 + current_mode_of_operation[2]] - pid[2], 100 );
			temp_output2 = temp_output/universal_pid[2][EEP_HEAT_TABLE3 + EEP_COOL_TABLE3-1];
			if(	temp_output2 > 100)
			temp_output2 = 100;	
			 		
			}
		else  //heating mode  
			{
			if(fan_speed_user == 0)
				{
				temp_output2 = PID3_Output_OFF_Table(current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3));//EEP_PID3_OFF_OUTPUT_BEGIN + current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3);
		    	output_buf[2] = temp_output2;
				}
			else
				{
				temp_output2 = PID3_Output_Table(current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3));//EEP_PID3_OUTPUT_BEGIN + current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3);
		    	output_buf[2] = temp_output2;			
				}
	
			test_buf2 = Output_PWM_Table(current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3));//read_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3));
	
			temp_output2 = mul(pid[2] - universal_pid[2][current_mode_of_operation[2]-7], 100 )/(100 - universal_pid[2][EEP_HEAT_TABLE3 - 1]);
			if(	temp_output2 > 100) 
				temp_output2 = 100;	 	
			}

   		
//#endif
//----------------------------------------------------------------------------------------------------------------------------------------

	if(EEP_TimerSelect != 0)//output4 and output5 can be pwm output and floating vavle output
	{
		temp_bit = 1;
		for(temp=0;temp<7;temp++)
			{
		//if(b.eeprom[EEP_OUTPUT1_FUNCTION + temp] == 3)
			if(temp<5)
				{
				if(b.eeprom[EEP_RANGE_OUTPUT1 + temp] == OUTPUT_RANGE_PWM)
					{
						
					//temp_bit = !temp_bit;
					if(temp_bit == 1)
						temp_bit =0;
					else
						temp_bit =1;
					pwmindex[temp_bit] = temp;//there are maximum 2 pwm output,any output can be set as pwm output,but for hardware,the relay should be solid relay,otherwise the relay will be damaged very soon
					}
				}
			else
				{
				if(b.eeprom[EEP_OUTPUT1_SCALE + temp - 5] == OUTPUT_ANALOG_RANGE_PWM)
					{
					if(temp_bit == 1)
						temp_bit =0;
					else
						temp_bit =1;

					pwmindex[temp_bit] = temp;//there are maximum 2 pwm output,any output can be set as pwm output,but for hardware,the relay should be solid relay,otherwise the relay will be damaged very soon
					}
				}
			 }
		//if(b.eeprom[EEP_PID_OUTPUT1 + pwmindex[0]] == 0)
			if(pid_ctrl_bit[pwmindex[0]] == 0)
			{
//temp_output and test_buf for pid 1,temp_output1 and test_buf1 for pid 2
//temp_output is how much output in the current stage,test_buf is the output percentage range set up in PWM output table		
			if(pid_ctrl_bit[pwmindex[1]] == 0)
 
		 		CalPwmTime(temp_output,test_buf,temp_output,test_buf);//two OUTPUTs controled by pid1
 			else
				CalPwmTime(temp_output,test_buf,temp_output1,test_buf1);  //first pwm OUTPUT controld by pid1 and second OUTPUT controled by pid2                                                                                                               
			}
		else
			{
			if(pid_ctrl_bit[pwmindex[1]] == 0)
		 		CalPwmTime(temp_output1,test_buf1,temp_output,test_buf);//first OUTPUT controled by pid2 and second OUTPUT controled by pid1
			else
				CalPwmTime(temp_output1,test_buf1,temp_output1,test_buf1);//two OUTPUTs controled by pid2
			}
	}


	for(test_buf=0; test_buf<7; test_buf++)
	{
    	if(pid_ctrl_bit[test_buf] == 3)
    	{
    		if(pid[0] > pid[1])
    		pidoutput[test_buf] = 0;
    		else
    		pidoutput[test_buf] = 1;
    	}
    	else if(pid_ctrl_bit[test_buf] == 4)
    	{
    		if(pid[0] < pid[1])
    		pidoutput[test_buf] = 0;
    		else
    		pidoutput[test_buf] = 1;
    	}
		else 
			pidoutput[test_buf] = pid_ctrl_bit[test_buf];
		
		i = get_bit(output_buf[pidoutput[test_buf]], test_buf);
		
    	set_bit( &output_temp, test_buf, i);
	}
	
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	/////////////////////////RELAY STATE CONFIG///////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	
	//need check floating control bit to judge which output will be floating output, any two of outputs can be a pair
//	if(EEP_OutputRange4 == BINARY_OUTPUT)    //if OUTPUT4 = 0    ON_OFF MODE
//	{
//	 
//			OUTPUT_1TO5 = output_temp ;			
//	}
//	else
//	{
//	//cc 10-12-2012 extend out1 and ou2 t ofloating mode
//		if(EEP_OutputRange4 == FLOATING)
//			{	
//			for(i=0;i<3;i++)
//			SetByteBit(&OUTPUT_1TO5,i,(bool)((0x01 << i) & output_temp));
//			}
////		else if(EEP_OutputRange1 == FLOATING)
////			{			 
////			for(i=2;i<5;i++)
////			SetByteBit(&OUTPUT_1TO5,i,(bool)((0x01 << i) & output_temp));
////			}
//		///cc 2010/01/20 blank the code below for Roth Bros customer 
//	} 	   	
	
	//if(EEP_OutputRange4 == BINARY_OUTPUT)    //if OUTPUT4 = 0    ON_OFF MODE

//	floattest1 = output_temp;
	if(output_floating_flag == OUTPUT_NO_FLOATING)
		{	 
		OUTPUT_1TO5.BYTE  = output_temp & 0xff ;			
		}
	else
		{
		float_buf = (uint8)output_temp; 	
		for(i=0;i<5;i++)
			{
			if((b.eeprom[EEP_RANGE_OUTPUT1 + i] != FLOATING_COOLING) &&
				(b.eeprom[EEP_RANGE_OUTPUT1 + i] != FLOATING_HEATING))
				{
				SetByteBit(&OUTPUT_1TO5.BYTE,i,GetByteBit(&float_buf,i));

				}

			}
		} 
//    floattest2 = OUTPUT_1TO5;
   	temp_bit = 1;

//	test_buf = 0;
//	for(i=0;i<7;i++) //detect how many outputs are set to PWM mode
//		{
//		if((b.eeprom[EEP_RANGE_OUTPUT1 + i] == OUTPUT_RANGE_PWM)||(b.eeprom[EEP_RANGE_OUTPUT1 + i] == OUTPUT_ANALOG_RANGE_PWM))
//			test_buf++;	
//		}
	
		for(relay_num=0;relay_num<7;relay_num++)
			{
			if(relay_num < 5)
				{
				if(b.eeprom[EEP_RANGE_OUTPUT1 + relay_num] == OUTPUT_RANGE_PWM)//pwm	modify pwm way to mode select, not function 
					{
					if(temp_bit == 1)
						temp_bit =0;
					else
						temp_bit =1;		
				//	temp_bit = !temp_bit;//first PWM output ,index of timer switch status is 0,second pwm ouput,index is 1
		 			if(timer_switch_status[temp_bit] == 1)
						{
						SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,1);  
						}
					else
						{
						SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,0); 		
						}
					}
				}
			else
				{
				if(b.eeprom[EEP_OUTPUT1_SCALE + relay_num - 5] == OUTPUT_ANALOG_RANGE_PWM)
					{
					temp_bit = !temp_bit;//first PWM output ,index of timer switch status is 0,second pwm ouput,index is 1
					if(timer_switch_status[temp_bit] == 1)
						SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,1);
				  else
						SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,0);
					}
				}
			}

//	temp_bit = 1;
//	for(relay_num=0;relay_num<2;relay_num++)//for out6 and out7 PWM function
//		{
//		if(b.eeprom[EEP_OUTPUT1_SCALE + relay_num] == OUTPUT_ANALOG_RANGE_PWM)
//			{
//			temp_bit = !temp_bit;//first PWM output ,index of timer switch status is 0,second pwm ouput,index is 1
//			if(timer_switch_status[temp_bit] == 1)
//				SetByteBit(&OUTPUT_1TO5,(relay_num + 5),1);
//		   	else
//				SetByteBit(&OUTPUT_1TO5,(relay_num + 5),0);
//			}
//		}
	//analog outputs and two discreet led's
	//note: writing to the analog_output buffer here for analog items
	for(test_buf=0; test_buf<2; test_buf++)
	{
		if(read_eeprom(EEP_PID_OUTPUT6 + test_buf) == 3)
		{
			if(pid[0] > pid[1])
				b.eeprom[EEP_PID_OUTPUT6 + test_buf] = 0;
			else
				b.eeprom[EEP_PID_OUTPUT6 + test_buf] = 1;
		}
		else if(read_eeprom(EEP_PID_OUTPUT6 + test_buf) == 4)
		{
			if(pid[0] < pid[1])
				b.eeprom[EEP_PID_OUTPUT6 + test_buf] = 0;
			else
				b.eeprom[EEP_PID_OUTPUT6 + test_buf] = 1;
		}
	}

	switch (sequence_of_operation ) 
	{	
		case TEST_MODE :  //SOP=0   Test relays, analog outputs, internal temperature chip, external temp sensor
			valve[0] += 50 ;
			if(valve[0] > 1000)
				valve[0] =	0 ;
			valve[1] -= 50 ;
			if(valve[1] < 0)
				valve[1] =	1000 ;
//			test_buf = valve[0]/4;

//			write_eeprom( EEPROM_TEST_ADDRESS, test_buf);
//			if(test_buf!=read_eeprom( EEPROM_TEST_ADDRESS ))
//				{
//				while(1)
//					{		
//					disp_str(FORM15X30, 0,260,"Warning         ",0,0x07e0);
//					disp_str(FORM15X30, 0,290,"E2_error        ",0,0x07e0);							
//					}
//				}
			break;


//	    case THREE_WIRE_MODE:    //out1=room temp, setpt done by pulses
		case TRANSDUCER_MODE:    //out1=room temp, anout2=setpt
			// In transducer mode, cooling valve output should correspond to the temperature
			// and heating valve output should correspond to the setpoint.
			// Relay 1 corresponds to the occupied mode.
//			 tstest1 = 55;
			if(Out6_Function == OUTPUT_RANGE_TRANSDUCER)
				valve[0] = SetTransducerValue(ANALOG_OUTPUT6_TRANSDUCER);
			else
				valve[0] = 0;

			if(Out7_Function == OUTPUT_RANGE_TRANSDUCER)
				valve[1] = SetTransducerValue(ANALOG_OUTPUT7_TRANSDUCER);
			else 
				valve[1] = 0;

			if (valve[0] > 1000)
				valve[0] = 1000;
			if (valve[1] > 1000)
				valve[1] = 1000;

//			OUTPUT_1TO5 = OUTPUT_1TO5 & 0xF8; // Turn off relays 1, 2, 3   //cc 2010/01/18 operate relay
//			OUT1 = occupied | override_timer;




//			if(EEP_DEGCorF==1)
//				{
//					valve[0] =	((temperature - 320) *5)/9 ;   
//					valve[1] =	(((EEP_DayCoolingSpHi<<8)+ EEP_DayCoolingSpLo - 320) *5)/9 ;  
//				}
//			else
//				{
// 					valve[0] =	temperature ;  
// 					valve[1] =	(EEP_DayCoolingSpHi<<8)+ EEP_DayCoolingSpLo ;  
//				}
//
//		    if (valve[0] > 1000)
//				valve[0] = 1000;
//		    if (valve[1] > 1000)
//				valve[1] = 1000;
//
////CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
////CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
//			OUTPUT_1TO5 = OUTPUT_1TO5 & 0xF8; // Turn off relays 1, 2, 3   //cc 2010/01/18 operate relay
//			OUT1 = occupied | override_timer;
////CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
////CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
			break;
 
		default :	 
			{
				//----------------------------------------------PID1--------------------------------------------------
					if(current_mode_of_operation[0] <= DAYTIME_COOLING6)
					{
						//test_buf = read_eeprom(EEP_OUTPUT_FREE_COOL_COAST + current_mode_of_operation[0]) ;

						if(fan_speed_user == FAN_OFF)	//OFF
						{				 
							valve_value[0] = PID1_Valve_Off_Table( current_mode_of_operation[0]);//read_eeprom(EEP_VALVE_OFF_TABLE_BEGIN + current_mode_of_operation[0]) ;
							test_buf = test_buf >> 4;						
						//	test_valve = valve_value[0];						
						}
						else
						{
							valve_value[0] = PID1_Valve_Table(current_mode_of_operation[0]);//read_eeprom(EEP_VALVE_OPER_TABLE_BEGIN + current_mode_of_operation[0]) ;
							test_buf = test_buf & 0x0f;
						}				
					}
					else
					{
					//	test_buf = read_eeprom(EEP_OUTPUT_FREE_COOL_COAST + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));
						if(fan_speed_user == 0)
						{
							test_buf = test_buf >> 4;
							temp_output2 = 	PID1_Valve_Off_Table(current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));//EEP_VALVE_OFF_TABLE_BEGIN + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1);
							valve_value[0] = temp_output2;//read_eeprom(temp_output2);
						//	test_valve = valve_value[0];
						}					
						else
						{
							temp = PID1_Valve_Table(current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1));//EEP_VALVE_OPER_TABLE_BEGIN + current_mode_of_operation[0] - (DAYTIME_COOLING6 - EEP_COOL_TABLE1);
							valve_value[0] = temp;//read_eeprom(temp);					
							test_buf = test_buf & 0X0F;	
	
						}				
					}
					//----------------------------------------------PID2--------------------------------------------------
					temp = EEP_UNIVERSAL_VALVE_BEGIN + current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2);
					if(current_mode_of_operation[1] <= DAYTIME_COOLING6)
						{
						//valve_value[1] = read_eeprom(EEP_UNIVERSAL_VALVE_BEGIN + current_mode_of_operation[1]);
						if(fan_speed_user == 0)
						   	valve_value[1] = PID2_Valve_OFF_Table(current_mode_of_operation[1]);//read_eeprom(EEP_UNIVERSAL_OFF_VALVE_BEGIN + current_mode_of_operation[1]);
						else
							valve_value[1] = PID2_Valve_Table(current_mode_of_operation[1]);//read_eeprom(EEP_UNIVERSAL_VALVE_BEGIN + current_mode_of_operation[1]);
						}
					else
						{
						//valve_value[1] = read_eeprom(temp);
 						if(fan_speed_user == 0)
						   	valve_value[1] = PID2_Valve_OFF_Table(current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));//read_eeprom(EEP_UNIVERSAL_OFF_VALVE_BEGIN + current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));
						else
							valve_value[1] = PID2_Valve_Table(current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));//read_eeprom(EEP_UNIVERSAL_VALVE_BEGIN + current_mode_of_operation[1] - (DAYTIME_COOLING6 - EEP_COOL_TABLE2));
						}

				//----------------------------------------------PID3--------------------------------------------------
					//current_mode_of_operation[2] = 1;
//				#ifdef VAV
					if(current_mode_of_operation[2] <= DAYTIME_COOLING6)
						{
  						if(fan_speed_user == 0)	
				 
							valve_value[2] = PID3_Valve_Off_Table(current_mode_of_operation[2]);//read_eeprom(EEP_PID3_VALVE_OFF_TABLE_BEGIN + current_mode_of_operation[2]) ;					

						else
							valve_value[2] = PID3_Valve_Table(current_mode_of_operation[2]);//read_eeprom(EEP_PID3_VALVE_OPER_TABLE_BEGIN + current_mode_of_operation[2]) ;
							//valve_value[2] = read_eeprom(EEP_PID3_VALVE_OPER_TABLE_BEGIN + 1) ;				
						}
					else
						{
						if(fan_speed_user == 0)	
							{
							temp_output = PID3_Valve_Off_Table(current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3));//EEP_PID3_VALVE_OFF_TABLE_BEGIN + current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3);
							valve_value[2] = temp_output;
							}
						else
							{
							temp_output = PID3_Valve_Table(current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3));//EEP_PID3_VALVE_OPER_TABLE_BEGIN + current_mode_of_operation[2] - (DAYTIME_COOLING6 - EEP_COOL_TABLE3);
							valve_value[2] = temp_output;//read_eeprom(temp_output);
							}									
						}
//				#endif
				//-----------------------------------------------------------------------------------------------------

				  	OutputValue(test_buf,valve_value); 

				
			}// end of switch default case

 
			if(GetByteBit(&EEP_OutputManuEnable,5))
			{
				valve[0] = (int16)(ManualAO1_HI << 8) + ManualAO1_LO;	 
			}
			 
			if(GetByteBit(&EEP_OutputManuEnable,6))
			{
				valve[1] = (int16)(ManualAO2_HI << 8) + ManualAO2_LO;				 
			}
//			for(test_buf=0;test_buf<2;test_buf++)
//			{
//				if(valve[test_buf] >1000)
//				valve[test_buf] = 1000;
//				if(valve[test_buf] < 0)
//				valve[test_buf] = 0;
//	
//			}		

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////FLOATING MODE //////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
		
//	if((EEP_OutputRange4==FLOATING)	|| (EEP_OutputRange1==FLOATING)) //cc floating mode, add floating mode to RELAY1 & RELAY2 as REALY4 & RELAY5
	if(output_floating_flag == OUTPUT_FLOATING)
			{//START
//////cc 2010/01/20 blank the code below for Roth Bros customer 
//			    floating_test1 = pid[EEP_PidOutput6];
//				floating_test2 = valve_value[EEP_PidOutput6];
//				floating_test3 = valve_target_percent;
//				floating_test4 = current_mode_of_operation[0];

//valve action is stored in a 2 bool number, 4 possible actions so 2 bits can store this
//cc do not understand! why don't define 0 == 0x00 1 == 0x01 2 == 0x10 3 == 0x11???
//#define VALVE_CLOSED  0  //00		 
//#define PID1 			2  //01  valve opens in stage1 cooling or heating
//#define PID2 			3  //10  valve opens in stage2 cooling or heating
//#define VALVE_OPEN 	1  //11  valve is open
//use the MSB 4 bits at the same byte
//#define PID3			4  //01  0---50% valve open
				switch(valve_value[EEP_PidOutput6] >> 4 & 0x03)         // valve3
				{
					case VALVE_CLOSED:
						valve_target_percent = 0;
					break;

					case PID1:
						if(pid[EEP_PidOutput6] > heat_pid[EEP_PidOutput6])
						{	
							valve_target_percent = (pid[EEP_PidOutput6] - heat_pid[EEP_PidOutput6])*2;						 
						}
						else if(pid[EEP_PidOutput6] < cool_pid[EEP_PidOutput6])
						{
							valve_target_percent = (50 - pid[EEP_PidOutput6])*2;						
						}	
						else
							valve_target_percent = 0;
					break;

					case PID2:   //valve opens in stage2 heating
						if(pid[EEP_PidOutput6] > heat_pid[EEP_PidOutput6])
						{
							valve_target_percent = pid[EEP_PidOutput6];							
						}
						else if(pid[EEP_PidOutput6] < cool_pid[EEP_PidOutput6])
						{
							valve_target_percent = 100 - pid[EEP_PidOutput6];						
						}	
						else
							valve_target_percent= 0;
					break;

					case VALVE_OPEN:
						valve_target_percent = 100;
					break;
				}
				if(GetByteBit(&EEP_OutputManuEnable,7))
					valve_target_percent = gucManualValve;
				if(valve_target_percent<MIN_VALVE_POSITION)
					valve_target_percent=0;
				else if(valve_target_percent>MAX_VALVE_POSITION)
					valve_target_percent=100;

	  		}

			 } //END		      
	
///////////////////////////////////////////////////////////////////////////////////////
	i = 0;
	i = ManualRelayAll;//read_eeprom(MANUAL_RELAY);
	for(relay_num=0;relay_num<5;relay_num++)
	{

//		else
//		{
//			if(GetByteBit(&EEP_OutputManuEnable,relay_num))   //USER SET
//			{ 
//				SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,GetByteBit(&i,relay_num));//CC 2011/01/18 RELAY SET
//			//	OUTPUT_1TO5 = read_eeprom(MANUAL_RELAY)&0x1f;	 
//			}
//			else if(deadmaster_triggered == 1)
//			{
//				if(DeadMsater_AutoManual == 2)//deadmaster manual mode, outputs will follow demaster setting 
//					SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,GetByteBit(&DeadMsater_OutputState,relay_num));//CC 2011/01/18 RELAY SET
//				else if(DeadMsater_AutoManual == 1)//deadmaster auto mode, outputs will follow pid control
//				{
//					EEP_OutputManuEnable = 0;  //trun to auto mod
//					write_eeprom(EEP_OUTPUT_MANU_ENABLE, 0);
//					if(i == 4)
//						deadmaster_triggered = 3;
//				}
//			}
			
//			else
//			{
				if((b.eeprom[EEP_INTERLOCK_OUTPUT1 + relay_num] == 1 && !occupied) || (b.eeprom[EEP_INTERLOCK_OUTPUT1 + relay_num] == 2 && mul_analog_input[0] == 0)  || (b.eeprom[EEP_INTERLOCK_OUTPUT1 + relay_num] == 3 && mul_analog_input[1] == 0))
					SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,0); //CC RELAY SET
				else if(b.eeprom[EEP_INTERLOCK_OUTPUT1 + relay_num] == 4 && b.eeprom[EEP_TIMER_SELECT ] == 0)
				{
					if(timer_switch_status[0] == 1)
					SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,1);
				}
				else if(b.eeprom[EEP_INTERLOCK_OUTPUT1 + relay_num] == 5 && b.eeprom[EEP_TIMER_SELECT ] == 0)
				{
					SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,timer_switch_status[0] & GetByteBit(&OUTPUT_1TO5.BYTE,relay_num));
				}
				else if(b.eeprom[EEP_INTERLOCK_OUTPUT1 + relay_num] == 6 && b.eeprom[EEP_TIMER_SELECT ] == 2)
				{
					if(TIMER_FLAG == 1)
						SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,0);				 
				}
//			}
//		}
	
	}
	
	 temp_bit = 1;
	for(relay_num=0;relay_num<5;relay_num++)
		{
		if(b.eeprom[EEP_OUTPUT1_FUNCTION + relay_num] == OUTPUT_FUNCTION_LIGHTING)	 ///CC 2011/01/17 0 = normal,1 = rotation,2 = lighting 
			{
		///cc 2010/01/20 blank the code below for Roth Bros customer 	
			if(lighting_stage == 1)
				SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,1);
			else
				SetByteBit(&OUTPUT_1TO5.BYTE,relay_num,0);
			}
//	    else if(b.eeprom[EEP_OUTPUT1_FUNCTION + relay_num] == OUTPUT_RANGE_PWM)//pwm
//			if(b.eeprom[EEP_RANGE_OUTPUT1 + relay_num] == OUTPUT_RANGE_PWM)//pwm	modify pwm way to mode select, not function 
//			{	
//			temp_bit = !temp_bit;//first PWM output ,index of timer switch status is 0,second pwm ouput,index is 1
// 			if(timer_switch_status[temp_bit] == 1)
//				{
//				SetByteBit(&OUTPUT_1TO5,relay_num,1);
//				}
//			else
//				{
//				SetByteBit(&OUTPUT_1TO5,relay_num,0);	 
//				}			
//			}
		}
		
//		ctest[0] = EEP_PidOutput1;
//		ctest[1] = EEP_PidOutput2;
//		ctest[2] = EEP_PidOutput3;
//		ctest[3] = EEP_PidOutput4;
//		ctest[4] = EEP_PidOutput5;
//		ctest[5] = EEP_PidOutput6;
//		ctest[6] = EEP_PidOutput7;

	
}//end of control_logic() 

//void init_valve_operation_eeprom( )
//{
//	uint8 col, temp_char;
////	if(flash_or_eeprom)
////	{
//		for(col = 0; col < 21; col++)
//		{
//			write_eeprom(EEP_VALVE_OPER_TABLE_BEGIN + col,default_valve_table[ col ]);
//		}        
////	}
////	else
////	{
////		for(col = 0 ;col < 21 ;col++)
////		{
////			FlashRead_Absolute(DATA_FAC_START + EEP_VALVE_OPER_TABLE_BEGIN + col, &temp_char);
////			write_eeprom(EEP_VALVE_OPER_TABLE_BEGIN + col,temp_char);
////		}    
////	}
//     
//}
//void init_pwm_operation_eeprom( )
//{
//	uint8 xdata col, temp_char;
////	if(flash_or_eeprom)//cc do not need read data from flash, all info store in EEPROM as default
////	{
//		for(col = 0; col < 7; col++)
//		{
//			write_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + col,default_pwm_table[ col ]);
//		}   
//     
////	}
////	else
////	{
////		for(col = 0 ;col < 7 ;col++)
////		{
////			FlashRead_Absolute(DATA_FAC_START + EEP_OUTPUT_PWM_AUTO_BEGIN + col, &temp_char);
////			write_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + col,temp_char);
////		}    
////	}
//     
//}



int16 SetTransducerValue(uint8 output_function)//detect analog output function setting and write output value to ananlog output buffer
{
int16 idata analog_output_buffer;
int32 idata	transducer_buffer;
uint8 idata channel;
	
	if(output_function == ANALOG_OUTPUT6_TRANSDUCER)
		{
		channel = EEP_Output1Scale;
		}
	else
		{
		channel = EEP_Output2Scale;
		}

	switch(channel)
		{
		case TRANSDUCER_INTERNAL: //internal sensor
			analog_output_buffer =	temperature;
			transducer_buffer = analog_output_buffer * (MaxTransducerRange - MinTransducerRange)/100;
			analog_output_buffer = transducer_buffer;						 
		break;

		case TRANSDUCER_SETPOINT:
			analog_output_buffer =	loop_setpoint[0]; 
			transducer_buffer = analog_output_buffer * (MaxTransducerRange - MinTransducerRange)/100;
			analog_output_buffer = transducer_buffer;
		break;

		case TRANSDUCER_AI1:
		case TRANSDUCER_AI2:
		case TRANSDUCER_AI3:
		case TRANSDUCER_AI4:
		case TRANSDUCER_AI5:
		case TRANSDUCER_AI6:
		case TRANSDUCER_AI7:
		case TRANSDUCER_AI8:

			analog_output_buffer =	mul_analog_input[channel - TRANSDUCER_AI1];
		    transducer_buffer = analog_output_buffer * (MaxTransducerRange - MinTransducerRange)/100;
			analog_output_buffer = transducer_buffer;
			 
		break;

		case TRANSDUCER_HUM:
			analog_output_buffer = humidity; 
		break;
		case TRANSDUCER_CO2:
			analog_output_buffer =	co2_data; 
		break;

		
		default:
			analog_output_buffer =	0;
		break;
		}
   return analog_output_buffer;
}

uint8 get_cureent_stage(void)
{
uint8 stage_temp = 0;
	if(current_mode_of_operation[0] >=4 && current_mode_of_operation[0] <=6)
		stage_temp = current_mode_of_operation[0] + 10;
	
	else if(current_mode_of_operation[0] >=7 && current_mode_of_operation[0] <=9)
		stage_temp = current_mode_of_operation[0] - 3;

	else if(current_mode_of_operation[0] >=10 && current_mode_of_operation[0] <=12)
		stage_temp = current_mode_of_operation[0] + 7;

	else  
		stage_temp = current_mode_of_operation[0];

  return stage_temp;
}

uint8 get_current_mode(void)
{
	uint8 mode_temp = 0;
	mode_temp = get_bit(COOLING_MODE,0);
	if(EEP_COOL_TABLE1 == 0)
		mode_temp = 0;	
	if(get_bit(HEATING_MODE,0))
		{	
		mode_temp += 2;
		if(EEP_HEAT_TABLE1 == 0)
			mode_temp = 0;
		}
   return mode_temp;		
}


