#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "24cxx.h"
#include "spi.h"
#include "lcd.h"
#include "touch.h"
#include "flash.h"
//#include "stmflash.h"
//#include "sdcard.h"
#include "mmc_sd.h"
#include "dma.h"
//#include "vmalloc.h"
#include "ff.h"
#include "fattester.h"
#include "exfuns.h"
#include "enc28j60.h"
#include "timerx.h"
//#include "uip.h"
//#include "uip_arp.h"
//#include "tapdev.h"
//#include "usb_app.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "modbus.h"
#include "define.h"
#include "inputs.h"
//#include "../output/output.h"
#include "dlmstp.h"
#include "rs485.h"
#include "datalink.h"
#include "config.h"
#include "handlers.h"
#include "device.h"	
#include "registerlist.h"
#include "types.h"
#include "modbus.h"
#include "eep_mapping.h"
#include "TstatFunctions.h"
#include "eepdefine.h"
#include "pid.h"
#include "setpoint.h"
#include "control_logic.h"
#include "inputs.h"
#include "output.h"
#include "development_note.h"
#include "Constcode.h"
#include "rtc.h"
#include "revision.h"
#include "stdlib.h"
#include "bacnet.h"
#include "ProductModel.h"
#include "stm32f10x_iwdg.h"
#include "store.h"
#include "stmflash.h"
#include "tstat7_lcd.h"
#include "tstat7_menu.h"


#ifdef TSTAT7_ARM
et_menu_parameter item_to_adjust;
#endif
/************charly's test code*********************/
extern int16 ctest[10];
//extern uint16 temperature;
//extern uint16 mul_analog_input[8];
/***************************************************/
extern uint16 today_dayofyear;
extern uint8 idlockcnt;
extern uint8 idlockflag;
uint8 pir_trigger = PIR_NOTTRIGGERED;
uint8 override_state = OVERRIDE_NOTTRIGGER;//indicate if override timer has been triggered
extern uint8 showidbit;
extern uint8 showid_cnt;
extern uint16 pir_value;
extern uint8 scheduleoff;
extern uint8 faninflag;
extern uint8 update_flag;
uint8 fan_flag = 0;
//extern uint8 dealwith_package;
extern uint16 scan_timer;
uint8 uartbuf2[9] = {0xff,0xfe,0x86,0x00,0x00,0x00,0x00,0x00,0x7c};
extern PTBDSTR OUTPUT_1TO5;
uint8 exist_menu = 0;
void disp_ch(uint8 form, uint16 x, uint16 y,uint8 value,uint16 dcolor,uint16 bgcolor);		
static void vInputTask( void *pvParameters );
static void vCOMMTask( void *pvParameters );
//static void vUpdatePID( void *pvParameters );
static void vControlLogic( void *pvParameters );
void vStateSwitch( void *pvParameters );
void vCoolingLockOut( void *pvParameters );
void vHeatingLockOut( void *pvParameters );
static void vKeypadsHandle( void *pvParameters );
//void vDealWithKey(void *pvParameters);
#ifndef TSTAT7_ARM
static void vDisplayRefresh( void *pvParameters );
#endif
static void vOutputTask( void *pvParameters );
static void vSoftwareTimer( void *pvParameters ); 
//static void vKEYTask( void *pvParameters );
//static void vUSBTask( void *pvParameters );
//static void vINPUTSTask( void *pvParameters );
//static void vNETTask( void *pvParameters );
//static void vOUTPUTSTask( void *pvParameters );
static void vMSTP_TASK(void *pvParameters ) ;
//static void vFAN_TASK(void *pvParameters ) ;
//void uip_polling(void);
//void EEP_Dat_Init(void) ;
#ifndef TSTAT7_ARM
extern void  LCDtest(void);
#endif //TSTAT7_ARM
//void GPIO_ini();

#define	BUF	((struct uip_eth_hdr *)&uip_buf[0])	

u32 Instance = 0;

void bacnettest(uint8 value)
{	
	TXEN = SEND;
	uart_send[0] = value;
	USART_SendDataString(1);
}

//uint8 SCHEDULES;
//uint8 CALENDARS;

void Inital_Bacnet_Server(void)
{
//	u32 Instance = 0x1d4c0;
	Device_Init();
	Device_Set_Object_Instance_Number(Instance); 
	SCHEDULES = 1;
	CALENDARS = 1;
	
}
	
void SoftReset(void)
{
 __set_FAULTMASK(1);      // 关闭所有中断
 NVIC_SystemReset();      // 复位
}

// --------------- update initialize --------------------
// if (update_flash_status == PROGRAM_START)//0X7F
// {
//  STMFLASH_Write(DATA_TO_ISP + EEP_UPDATE_STATUS*2+STM32_FLASH_BASE,&update_flash_status,1);
//  SoftReset(); 
// }

//#define DATA_TO_ISP   0x7800          //page 15 
//FLASH起始地址
#define STM32_FLASH_BASE    0x08000000  //STM32 FLASH的起始地址

void GPIO_init_LCD()
{
	GPIO_InitTypeDef DISP_RS;
	GPIO_InitTypeDef DISP_RES;
	GPIO_InitTypeDef DISP_SCL;
	GPIO_InitTypeDef DISP_SDA;
	GPIO_InitTypeDef DISP_CS;	
	GPIO_InitTypeDef DISP_BACKLIGHT; 
	
	GPIO_InitTypeDef RS458LED;
	GPIO_InitTypeDef R1LED;
	GPIO_InitTypeDef R2LED;
	GPIO_InitTypeDef R3LED;
	GPIO_InitTypeDef R4LED;
	GPIO_InitTypeDef R5LED;
	GPIO_InitTypeDef D1LED;
	GPIO_InitTypeDef D2LED;
	GPIO_InitTypeDef AI1LED;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);//enable GPIO clock
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//enable GPIO clock	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);//enable GPIO clock	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//enable GPIO clock	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//enable GPIO clock	
//#ifdef TSTAT_OCC
	if(HardwareVersion >= HW_VERSION)
	{
	DISP_RS.GPIO_Pin = GPIO_Pin_6;  
	DISP_RS.GPIO_Mode = GPIO_Mode_Out_PP; 
	DISP_RS.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOB, &DISP_RS);
	}
//#else	
	else
	{
	DISP_RS.GPIO_Pin = GPIO_Pin_2;  
	DISP_RS.GPIO_Mode = GPIO_Mode_Out_PP; 
	DISP_RS.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOD, &DISP_RS);
	}
//#endif
	
DISP_RES.GPIO_Pin = GPIO_Pin_15;  
DISP_RES.GPIO_Mode = GPIO_Mode_Out_PP; 
DISP_RES.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOD, &DISP_RES);	
	
DISP_SCL.GPIO_Pin = GPIO_Pin_1;  
DISP_SCL.GPIO_Mode = GPIO_Mode_Out_PP; 
DISP_SCL.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOD, &DISP_SCL); 
	
DISP_SDA.GPIO_Pin = GPIO_Pin_3;  
DISP_SDA.GPIO_Mode = GPIO_Mode_Out_PP; 
DISP_SDA.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOD, &DISP_SDA);	

DISP_CS.GPIO_Pin = GPIO_Pin_0;  
DISP_CS.GPIO_Mode = GPIO_Mode_Out_PP; 
DISP_CS.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOD, &DISP_CS);	

DISP_BACKLIGHT.GPIO_Pin = GPIO_Pin_0;  
DISP_BACKLIGHT.GPIO_Mode = GPIO_Mode_Out_PP; 
DISP_BACKLIGHT.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOA, &DISP_BACKLIGHT);
}

//void inputs_adc_init(void)
//{
//	ADC_InitTypeDef ADC_InitStructure;	
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//
//	ADC_InitStructure.ADC_NbrOfChannel = 1;//
//	ADC_Init(ADC1, &ADC_InitStructure);
//	ADC_Cmd(ADC1, ENABLE); 
//	ADC_ResetCalibration(ADC1);
//	while(ADC_GetResetCalibrationStatus(ADC1)== SET)
//	{
//		;
//	}		
//  ADC_StartCalibration(ADC1);
//  while(ADC_GetCalibrationStatus(ADC1) == SET);
//}
void watchdog_init(void)
{
		/* Enable write access to IWDG_PR and IWDG_RLR registers */ 
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
		/* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */ 
		IWDG_SetPrescaler(IWDG_Prescaler_128); 
		/* Set counter reload value to 4000 = 3200ms */ 
		IWDG_SetReload(4000); 
		IWDG_Enable();  			//enable the watchdog
		IWDG_ReloadCounter(); // reload the value
}
void watchdog(void)
{
	IWDG_ReloadCounter(); // reload the value
}


void Warmboot(void)
{
//	int16 temp_unsigned_int;
  uint8 i,j;

//  demand_response = read_eeprom(EEP_DEMAND_RESPONSE);

	pid3_input_select = read_eeprom(EEP_PID3_INPUT_SELECT); 

	if(read_eeprom(EEP_PIR_TIMER + 1) == 255)//not intialized
		{
		write_eeprom(EEP_PIR_TIMER+1, 0); 
		write_eeprom(EEP_PIR_TIMER, 100);				   
		}
//	pid3_cool_db = read_eeprom(EEP_PID3_COOLING_DB);
//	pid3_heat_db = read_eeprom(EEP_PID3_HEATING_DB);
//	pid3_pterm = read_eeprom(EEP_PID3_PTERM);
//	pid3_iterm = read_eeprom(EEP_PID3_ITERM);

	pir_timer = (uint16)read_eeprom(EEP_PIR_TIMER + 1) + read_eeprom(EEP_PIR_TIMER);
//	pir_timer = pir_timer * 60;
//	ID_Lock = read_eeprom(EEP_ID_WRITE_ENABLE);

	if(ID_Lock > 1)
		{
		ID_Lock = ID_WRITE_DISABLE;
		write_eeprom(EEP_ID_WRITE_ENABLE,ID_WRITE_DISABLE);
		}
//	sp_display_select = read_eeprom(EEP_SP_DISPLAY_SELECT);
//	if(sp_display_select > 3)
//		{
//		sp_display_select = 0;
//		write_eeprom(EEP_SP_DISPLAY_SELECT,sp_display_select);
//		}
	/* ----------------------------------------------
	- init current menu to first menu item
	------------------------------------------------ */
	item_to_adjust = MIN_MENU ;
	
//	#ifdef WIRELESS_PIR
//	CTEST = 1;
//	CTEST1 = 1;
//	keypad_read_flag = 0;
//	pir_end = 1;
//	SPI_initial();
//  	delay_us(5000);
//	pir_end = 0;
//	keypad_read_flag = 1;
//	#endif
	
	
	write_eeprom(EEP_FACTORY_DEFAULTS,0); // reset factory defaults to 0

	analog_calibration[4] = ((int16)read_eeprom(EEP_EXT_SENSOR_TEMP_CAL+1)<<8) + read_eeprom(EEP_EXT_SENSOR_TEMP_CAL);

  if((read_eeprom(EEP_PRODUCT_MODEL) == 255) || (read_eeprom(EEP_HARDWARE_REV) == 255))
       write_eeprom(EEP_PRODUCT_MODEL, PRODUCT_ID);

//	if(read_eeprom(EEP_LCD_ROTATE_ENABLE) == 255)
//	    write_eeprom(EEP_LCD_ROTATE_ENABLE, 0);




  // ------------- get_ee_parameters --------------
  	if(FirmwareVersion_HI == 0xff)
			{
				initialize_eeprom();
				write_eeprom(EEPROM_VERSION_NUMBER_HI,((EEPROM_VERSION>>8)&0xFF) ) ;
			  write_eeprom(EEPROM_VERSION_NUMBER_LO,(EEPROM_VERSION&0xFF) ) ;
				InitialRAM();

			  EEP_Configuration = 0;
			
	//		rs485_writebit = RS485_WRITE_ENABLE;
				info_byte &= 0x3f;	//cc change this so can change ID after version update
				write_eeprom(EEP_INFO_BYTE,info_byte);		 
				
				for(i=0;i<7;i++)
						write_eeprom((EEP_OUTPUT1_DELAY_OFF_TO_ON + i), 0);	  
				for(i=0;i<7;i++)
						write_eeprom((EEP_OUTPUT1_DELAY_ON_TO_OFF + i), 0);	
					
				if(EEP_DEGCorF>1)
					EEP_DEGCorF = 0 ;	
//				DisRestart( );			
			}

																				    	
	if((EEP_DaySpLo == 255) && (EEP_DaySpHi == 255))
		{
		EEP_DaySpLo = 200;
		EEP_DaySpHi = 0;
		}
	switch(EEP_PowerupOnOff)	
		{
			
			case 0:
				EEP_FanSpeed = FAN_OFF ;
		    write_eeprom(EEP_FAN_SPEED , EEP_FanSpeed );
				lighting_stage = 1;
				write_eeprom(LIGHTING_STAGE,lighting_stage);
				break ;

			case 1:
				if(EEP_FanSpeed)
					{
						;
						//b.eeprom[EEP_FAN_SPEED  - MAXEEPCONSTRANGE]=1 ;
						//write_eeprom(EEP_FAN_SPEED , b.eeprom[ EEP_FAN_SPEED - MAXEEPCONSTRANGE]) ;
					}
				 else
					{
					//b.eeprom[EEP_FAN_SPEED  - MAXEEPCONSTRANGE]=0 ;
						EEP_FanSpeed = FAN_AUTO	 ;
				    write_eeprom(EEP_FAN_SPEED , EEP_FanSpeed) ;
					}	
				lighting_stage = 0;
				write_eeprom(LIGHTING_STAGE,lighting_stage);		
				break ;

			case 2:
				lighting_stage = read_eeprom(LIGHTING_STAGE);
				break ;
			case 3:
				EEP_FanSpeed = FAN_AUTO ;
		        write_eeprom(EEP_FAN_SPEED , EEP_FanSpeed) ;
				break ;

		}

			serial_no_activity = 0;	 
			backup_fan_speed = 	EEP_FanSpeed;
  		fan_speed_user = EEP_FanSpeed; //the user selected fan speed
//			fanspeedbuf = fan_speed_user;

//			i = read_eeprom(EEP_INFO_BYTE);//use the fifth bool of info_byte
//			delay_us(10);
//			if((i & 0x10) != 0)
//				i = read_eeprom(EEP_INFO_BYTE);
//			if((i & 0x10) == 0)
//			{
//				if((EEP_Analog1Function == 2 || EEP_Analog2Function == 2 ) && (EEP_OverrideTimer > 0))
//				{
//					//start_timer( OVERRIDE_TIMER , DEFAULT_TIMEOUT ) ;
//					//override_timer = ON;						 
//					//override_timer_time = EEP_OverrideTimer;
//				  
//					//backup_fan_speed = 0;
//					fan_speed_user = FAN_OFF;
//					EEP_FanSpeed = FAN_OFF;
//					write_eeprom(EEP_FAN_SPEED,FAN_OFF);
//					occupied = 0 ;
//					info_byte &= 0x2E;
//					write_eeprom(EEP_INFO_BYTE,info_byte);
//				}
//			}						
			if(fan_speed_user > FAN_OFF)
			{
				occupancy = 1;					 
			//	ResetTimer();
			}	 
// 	setpoint() ;
			loop_setpoint[PID_LOOP1] = Get_current_setpoint(occupied);
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
	j = 0;
	for(i=0;i<5;i++)
		{
		if((0 < read_eeprom(EEP_RANGE_OUTPUT1 + i)) && (read_eeprom(EEP_RANGE_OUTPUT1 + i) < 3))
			{
			j = 1;
			}		
		}
	if(j == 1)
		output_floating_flag = OUTPUT_FLOATING;
	else
		output_floating_flag = OUTPUT_NO_FLOATING;

	if(output_floating_flag == OUTPUT_FLOATING) //cc if set outputs to floating range, start valve timer, no matter which outputs are set
		{
			;
			//TBD: initial valve control here
//		TR2=1;
//		daytimer_ticks_buf=0;
//		start_timer(REFRESH_VALVE_POSITION_TIMER,DEFAULT_TIMEOUT);
//		command=INITIALIZE;
//		OUT1 = STOP;
		}

//  if((b.eeprom[EEP_RANGE_OUTPUT4-MAXEEPCONSTRANGE]!=1) && (b.eeprom[EEP_RANGE_OUTPUT1-MAXEEPCONSTRANGE]!=1))
	if(output_floating_flag == OUTPUT_NO_FLOATING)//cc if no outputs are set to floating range, stop valve timer		
		{
			;
			//TBD stop valve control
//		TR2=0;
//		stop_timer(REFRESH_VALVE_POSITION_TIMER);
		}

	//initialize timer  MHF:2006-3-22
	if((EEP_TimerOnHi == 0 && EEP_TimerOn  == 0) && (EEP_TimerOffHi  == 0 && EEP_TimerOff  == 0))
		timer_enable = 0; 
  else
 	{
		
		timer_switch_status[0] = 0;
		time_counter = (uint16)(EEP_TimerOffHi  << 8) + EEP_TimerOff ;
		if(EEP_TimerSelect  != 3)
		{		
			timer_enable = 1;
			CalDesireTime(time_counter,EEP_TimerUnits ,1);
		}	
	}
//start_timer(REFRESH_VALVE_POSITION_TIMER,DEFAULT_TIMEOUT);


	// added by RL   12/14/04 ---------------------------------
	// RL also extract data from Flash and see if address has been changed during ISP

	// check if during ISP mode if the address has been changed
//	FlashRead_Absolute(DATA_TO_FIRMWARE + EEP_ADDRESS,&laddress);
	// if it has not been changed, check the eeprom
	if( (laddress == 255) || (laddress == 0))
	{
		laddress =  254;
		ID_Lock = ID_WRITE_ENABLE;
		flash_buf[0] = laddress;	
		STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);	
		write_eeprom(EEP_ADDRESS , laddress ); 
	}
	
	lcd_rotate_max = read_eeprom(EEP_LCD_ROTATE_ENABLE);
	if(lcd_rotate_max > 12)
		{
	   	lcd_rotate_max = 0;
		write_eeprom(EEP_LCD_ROTATE_ENABLE, 0);
		}

	if(hum_manual_enable > 1)
		{
		hum_manual_enable = 0; 
		write_eeprom(EEP_HUM_INPUT_MANUAL_ENABLE, 0);
		}

	if(co2_manual_enable > 1)
		{
		co2_manual_enable = 0; 
		write_eeprom(EEP_CO2_INPUT_MANUAL_ENABLE, 0);
		}	
//    schedule_on_off = read_eeprom(EEP_SCHEDULE_ON_OFF);

	  
	 for(i=0;i<MAX_INPUT_CHANNEL;i++)
	 	{
//		mul_input_filter[i] = read_eeprom(EEP_INPUT1_FILTER + i);
//		if(mul_input_filter[i] == 255)
		if(InputFilter(i) == 255)
			{
			InputFilter(i) = 3;
		  write_eeprom(EEP_INPUT1_FILTER + i, 3);
			}
		}
		
//	HUM_Filter = read_eeprom(EEP_HUM_FILTER);

//	CO2_Filter = read_eeprom(EEP_CO2_FILTER);

 	//lcd_rotate_max = read_eeprom(EEP_LCD_ROTATE_ENABLE);
	if((lcd_rotate_max > 25) || (lcd_rotate_max == 255))
		{
		lcd_rotate_max = 0;
		write_eeprom(EEP_LCD_ROTATE_ENABLE, 0);
		}
  	//disp_item_queue[0] = 0;
    for(i=1;i<(lcd_rotate_max+1);i++)
		{
 		//disp_item_queue[i] = disp_item_queue(i-1);//read_eeprom(EEP_DISP_ITEM_TEMPERATURE + i -1);
		if(disp_item_queue(i) == 255)
			disp_item_queue(i) = 0;		 
	 	}

	co2_calibration_data = Calibration_CO2_HI;//read_eeprom(EEP_CO2_CALIBRATION + 1);
  co2_calibration_data = co2_calibration_data << 8;
  co2_calibration_data += Calibration_CO2_LO;

//    hardware_detect();

//	if (pic_version < 3)
//		internal_thermistor = 0;
//	else 
//		internal_thermistor = 1;
//    read_analog_input( );
//	for(i=0;i<3;i++)
//	{
//		prestatus[i] = 0;
//		event_counter[i] = 0;
//	}
	if((read_eeprom(EEP_TABLE1_ZERO + 1) << 8) + read_eeprom(EEP_TABLE1_ZERO) < (read_eeprom(EEP_TABLE1_FIVE + 1) << 8) + read_eeprom(EEP_TABLE1_FIVE))
  		slope_type[0] = 1;
	else
		slope_type[0] = 0;
	if((read_eeprom(EEP_TABLE2_ZERO + 1) << 8) + read_eeprom(EEP_TABLE2_ZERO) < (read_eeprom(EEP_TABLE2_FIVE + 1) << 8) + read_eeprom(EEP_TABLE2_FIVE))
  		slope_type[1] = 1;
	else
		slope_type[1] = 0;

	lighting_stage = read_eeprom(LIGHTING_STAGE);
	if(lighting_stage > 1)
		lighting_stage = 1;

	if(read_eeprom(EEP_SETPOINT_UNLIMIT) == SPLIMIT)
	if(EEP_DefaultSetpoint < EEP_MinSetpoint || EEP_DefaultSetpoint > EEP_MaxSetpoint) 
		{
		EEP_DefaultSetpoint = 200;//((int16)(b.eeprom[EEP_DAY_SETPOINT - MAXEEPCONSTRANGE + 1] << 8)+EEP_DefaultSetpoint)/10;
		write_eeprom(EEP_DEFAULT_SETPOINT,200);//b.eeprom[EEP_COOLING_SETPOINT - MAXEEPCONSTRANGE]);
 		Set_day_setpoint(EEP_DaySpHi, EEP_DaySpLo);
		}
// 	 #ifdef RTC8563 
//	 b.jump_function[CHECK_SCHEDULE_TIMER] = RUN_SCHEDULE;
// 	 start_timer(CHECK_SCHEDULE_TIMER , DEFAULT_TIMEOUT) ; 
//	 #endif 
//		watchdog();


//	temp_unsigned_int = ((int16)EEP_DaySpHi << 8) + EEP_DaySpLo;

	EEP_WorkMode = read_eeprom(EEP_WORK_MODE);

//	if (E2_OK)
//  	{
//		
//	 	init_keypad()	;
//	
//	  	init_control() ;  // initialize	the control logic, pid and refresh outputs timers
//	
//	
//		temperature_filter_bypass = 200;
		init_PID_flag |= 0x07;
//  	}

	// Set tstat to occupied.only if pom = on
	if(fan_speed_user == 0)
	{

		info_byte &= 0xfe;
		occupied = 0;        
	}
	else
	{
		info_byte |= 0x01;
		occupied = 1;  
	}
		

	// Clear out reset inhibit bool and upper bits of info_byte
	info_byte &= 0xfb; 
	write_eeprom(EEP_INFO_BYTE,info_byte); 
										 
//  	b.jump_function[OVERRIDE_TIMER] = OVERRIDE_TIMER_ROUTINE;
	
	srand(123);	
//TBD	
//	if(read_eeprom(MANUAL_VALVE) > 100)
//	{
//		write_eeprom(MANUAL_VALVE,0);
//		gucManualValve  = 0;
//	}
//	gucManualValve = read_eeprom(MANUAL_VALVE);


//	#ifdef VAV
//	dual_max_vav_ini();
//	#endif

	if(read_eeprom(EEP_DEADMASTER_AUTO_MANUAL) > 2)
		write_eeprom(EEP_DEADMASTER_AUTO_MANUAL,0);	


/*********FOR CALIBRATE HUM SENSOR******************/
/******************add by ye************************/

//if(ICON_ManualMode == MANUAL_ICON_ENABLE)
//	manual_icon_control = MANUAL_ICON_ENABLE;
//else
//	manual_icon_control = MANUAL_ICON_DISABLE;
//manual_icon_value = read_eeprom(EEP_ICON_MANUAL_VALUE);

if(read_eeprom(EEP_PIR_SENSOR_SELECT) == 255)
	{
	write_eeprom(EEP_PIR_SENSOR_SELECT,0);
	}

//	output6_function = read_eeprom(EEP_OUTPUT6_FUNCTION);
// 	output7_function = read_eeprom(EEP_OUTPUT7_FUNCTION);

// MaxTransducerRange = read_eeprom(EEP_TRANSDUCER_RANGE_MAX);
// MinTransducerRange = read_eeprom(EEP_TRANSDUCER_RANGE_MIN);
	#ifndef TSTAT8_HUNTER
 RTC_Get();
 if(calendar.w_year == 0)	
	SetRTCTime(EEP_CLOCK_YEAR, 2017);
#endif 

/*********************end of warmboot***************************/  


}//end of warmboot

uint8_t  PDUBuffer[MAX_APDU];

//u8 global_key = KEY_NON;

static void debug_config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

}

int main(void)
{
	uint8 i=0;
	GPIO_InitTypeDef HUNTER_LED;
	uint16_t pdu_len = 0; 
	BACNET_ADDRESS  src;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8008000);
	debug_config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	delay_init(72);
	
	TIM6_Int_Init(100, 719);

	AT24CXX_Init();    
//  if(RTC_Init())
//	{ 
//	update_flag = 3;
//	}

#ifndef TSTAT7_ARM
	keypad_ini();
#endif	
	InitialRAM();
	GPIO_init_LCD();
	inputs_init();
	output_ini();
	relay_ctl_pwm(1,0);
	relay_ctl_pwm(2,0);
	relay_ctl_pwm(3,0);
	relay_ctl_pwm(4,0);
	relay_ctl_pwm(5,0);	
	sequence_of_operation = Sys_Sequence;
	Warmboot();	
		
#ifdef TSTAT7_ARM
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
 	Tstat7_InitScreen();
 	Tstat7_FullScreen();
#else
	#ifdef COLOR_TEST
	TSTAT8_MENU_COLOR2 = 256*(uint32)b.eeprom[TEST1] + b.eeprom[TEST2];
	TSTAT8_BACK_COLOR = 256*(uint32)b.eeprom[TEST3] + b.eeprom[TEST3+1];
	TANGLE_COLOR = 256*(uint32)b.eeprom[TEST4] + b.eeprom[TEST5];
	#endif
 	LCDtest();
#endif
	
	mul_analog_in_buffer[8] = 0;
	fan_speed_user = EEP_FanSpeed;
	occupied = 1;

	init_PID_flag = 0X07;
	ManualInternalSensor_HI = 0;
	ManualInternalSensor_LO = 200;
	
	if(EEP_SpecialMenuLock > 1)
		part_lock = 1;
	
	if(schedule_on_off == SCHEDULE_OFF)
	{
	if(fan_speed_user == FAN_OFF)
		setpoint_select = NIGHT_SP;
	else
		setpoint_select = DHOME_SP;
	}
	loop_setpoint[PID_LOOP1] = Get_current_setpoint(setpoint_select);
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
	 
	if(protocol_select == 1)//bacnet protocol	
		modbus.com_config[0] = BAC_MSTP;//;
	else
		modbus.com_config[0] = MODBUS;	

	if(EEP_DeadMaster == 0)
			deadmaster_triggered = 0;
	
	if(modbus.com_config[0] == BAC_MSTP)
		mass_flash_init();
	
	if(modbus.com_config[0] == BAC_MSTP)//if bacnet protocol, schedule only support occupied and unoccupied mode 
	{
//		schedule_on_off = SCHEDULE_ON;
		for(i=0;i<7;i++)
		{
			ScheduleMondayFlag(0+i*3) = 0x11;
			ScheduleMondayFlag(1+i*3) = 0x11;
			ScheduleMondayFlag(2+i*3) = 0x11;
		}
	}
	
	for(i=0;i<7;i++)
		pid_ctrl_bit[i] = b.eeprom[EEP_PID_OUTPUT1+i];
	

	#ifdef TSTAT_ZIGBEE
	if((RS485_Mode == 0)||(RS485_Mode == 2))//rs485 mode
	{
		#endif
		switch(EEP_Baudrate)
		{
			case BAUDRATE_9600:
				uart1_init(9600);
			break;
			
			case BAUDRATE_19200:
				uart1_init(19200);
			break;

			case BAUDRATE_38400:
				uart1_init(38400);
			break;

			case BAUDRATE_57600:
				uart1_init(57600);
			break;

			case BAUDRATE_115200:
				uart1_init(115200);
			break;		

			case BAUDRATE_76800:
				uart1_init(76800);
			break;
					
			case BAUDRATE_1200:
				uart1_init(1200);
			break;

			case BAUDRATE_4800:
				uart1_init(4800);
			break;

			case BAUDRATE_14400:
				uart1_init(14400);
			break;
		
			default:
				uart1_init(19200);
			break;	
		}
	#ifdef TSTAT_ZIGBEE		
	}
	
	if((RS485_Mode == 1)||(RS485_Mode == 2))
		uart4_init(115200);
	#endif//TSTAT_ZIGBEE
	
	#ifdef TSTAT_CO2
	uart3_init(9600);
	#endif
	

#ifdef TSTAT8_HUNTER
//	uart1_init(115200);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//enable GPIO clock	
	
	HUNTER_LED.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ; 	
	HUNTER_LED.GPIO_Mode = GPIO_Mode_Out_PP;
	HUNTER_LED.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &HUNTER_LED);

	RELAY1_LED_ON
	RELAY2_LED_ON
	RELAY3_LED_ON
	RELAY4_LED_ON
	RELAY5_LED_ON
	RS485_LED_ON
	AI1_LED_ON
	AI2_LED_ON
	AI3_LED_ON
	AI4_LED_ON
	AI5_LED_ON
	AI6_LED_ON
	AI7_LED_ON
	DO1_LED_ON
	DO2_LED_ON
	delay_ms(2000);
	RELAY1_LED_OFF
	RELAY2_LED_OFF
	RELAY3_LED_OFF
	RELAY4_LED_OFF
	RELAY5_LED_OFF
	RS485_LED_OFF
	AI1_LED_OFF
	AI2_LED_OFF
	AI3_LED_OFF
	AI4_LED_OFF
	AI5_LED_OFF
	AI6_LED_OFF
	AI7_LED_OFF
	DO1_LED_OFF
	DO2_LED_OFF
//	delay_ms(1000);
//	AI7_LED_ON 
//	delay_ms(1000);
//	AI7_LED_OFF 
//	delay_ms(1000);
//	AI7_LED_ON
//delay_ms(1000);
//	AI7_LED_OFF 	
#endif

#ifndef TSTAT8_HUNTER 
  while(RTC_Init())
	{ 
	if(i<5)
		i++;
	else
	{
		#ifndef TSTAT7_ARM 
		disp_str(FORM15X30, 0,  0,  "RTC Err",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);	
		#endif
		break;
	}
	update_flag = 3;
	delay_ms(1000);	
	}	
#endif	
	
//disp_str(FORM15X30, 0,  0,  "testing",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
//while(1)
//{
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);
//	delay_ms(1000);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
//	delay_ms(1000);
//}
	watchdog_init();
	xTaskCreate( vControlLogic, ( signed portCHAR * ) "ControlLogic", configMINIMAL_STACK_SIZE+500, NULL, tskIDLE_PRIORITY + 5, NULL );
	xTaskCreate( vInputTask, ( signed portCHAR * ) "Input", configMINIMAL_STACK_SIZE+300, NULL, tskIDLE_PRIORITY + 2, NULL );
 	xTaskCreate( vCOMMTask, ( signed portCHAR * ) "COMM", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );
//	xTaskCreate( vUpdatePID, ( signed portCHAR * ) "PID", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

	xTaskCreate( vKeypadsHandle, ( signed portCHAR * ) "KeypadsHandle", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );	
#ifndef TSTAT7_ARM
	xTaskCreate( vDisplayRefresh, ( signed portCHAR * ) "DispalyRefresh", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );	
#endif
	xTaskCreate( vOutputTask, ( signed portCHAR * ) "OutputTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL );		
	xTaskCreate( vSoftwareTimer, ( signed portCHAR * ) "Softwaretimer", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );					
	xTaskCreate( vMSTP_TASK, (signed portCHAR * ) "MSTP", configMINIMAL_STACK_SIZE+1000, NULL, tskIDLE_PRIORITY + 3, NULL );
#ifdef TSTAT7_ARM
	xTaskCreate( vTstat7_Menu, ( signed portCHAR * ) "Tstat7_Menu", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );	
#endif
	vTaskStartScheduler();

}

u8 dim_timer[14];

#define testlcd 0
#ifdef testlcd
static int8 f[4];
#endif



void vInputTask( void *pvParameters )
{
	static uint8 i = 0;
	uint8 co2_cnt = 0;
	uint16 temp;
//	uint8 pwmcnt = 0;
	#ifdef TSTAT_CO2
	uint8 const *p_co2_cmd = poll_co2_cmd;
	#endif


	
    mul_analog_cal[0] = ((int16)Calibration_AI1_HI<<8) + Calibration_AI1_LO;
		mul_analog_cal[1] = ((int16)Calibration_AI2_HI<<8) + Calibration_AI2_LO;	
		mul_analog_cal[2] = ((int16)Calibration_AI3_HI<<8) + Calibration_AI3_LO;	
		mul_analog_cal[3] = ((int16)Calibration_AI4_HI<<8) + Calibration_AI4_LO;	
		mul_analog_cal[4] = ((int16)Calibration_AI5_HI<<8) + Calibration_AI5_LO;	
		mul_analog_cal[5] = ((int16)Calibration_AI6_HI<<8) + Calibration_AI6_LO;	
		mul_analog_cal[6] = ((int16)Calibration_AI7_HI<<8) + Calibration_AI7_LO;	
		mul_analog_cal[7] = ((int16)Calibration_AI8_HI<<8) + Calibration_AI8_LO;

    GPIO_ResetBits(GPIOE, GPIO_Pin_2);	
//		if (mul_analog_cal[i] > 1000 || mul_analog_cal[i] < 0)
//			{
//			mul_analog_cal[i] = 500;
//			write_eeprom(EEP_CALIBRATION_ANALOG1 + i*2 , CALIBRATION_DEFAULT & 0xFF);
//			write_eeprom(EEP_CALIBRATION_ANALOG1 + i*2 + 1, CALIBRATION_DEFAULT >> 8);
//			}
//		}																						    
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	for(;;)
		{	
		power_up_timer++;
		CalTemperature();
			
//		if((EEP_PidOutput7 == 1)&&(ctest[0] == 0))
//			ctest[0] = 1;	
//		if(giPwmTimeOn[0] == 0)
//			ctest[2] = 1;	
		
		CalInput();
		
		for(i=0;i<8;i++)
		{			
			if(AI_Function(i) == 2)//occupancy sensor
			{
			 if(mul_analog_input[i] == UNOCCUPIED)
			 {
				 fan_speed_user = FAN_OFF;
				 setpoint_select = NIGHT_SP;
				 fanspeedbuf = FAN_OFF;
				 EEP_FanSpeed = FAN_OFF;
			 }
			 else
			 {
				 fan_speed_user = FAN_AUTO;
				 setpoint_select = DHOME_SP;
				 fanspeedbuf = FAN_AUTO;
				 EEP_FanSpeed = FAN_AUTO;
			 }
			}			
		}	
			
		frc_input(); //refresh free cooling local inputs			
  // #ifdef TSTAT_OCC
		if(PirSensorSelect == 1)
		{
			if(abs(pir_value - PirSensorZero )> Pir_Sensetivity) //occupied
			{
				//led_state = !led_state;
				pir_trigger = PIR_TRIGGERED;
				override_state = OVERRIDE_TRIGGERED;
				
				if(override_state == OVERRIDE_TRIGGERED)/*&&(fan_speed_user == FAN_OFF)*/
				{
					if(EEP_OverrideTimer == 0xff)
						EEP_OverrideTimer = 60;
					if(EEP_OverrideTimer != 0)
					{
						#ifdef OCCTEST
						override_timer_time = 10;//(uint32)60 * EEP_OverrideTimer;
						GPIO_SetBits(GPIOA, GPIO_Pin_0);
						#else
						override_timer_time = (uint32)60 * EEP_OverrideTimer;
						#endif
					occupied = 1;
					fan_speed_user = FAN_AUTO;
					fanspeedbuf = FAN_AUTO;
					info_byte |= 0x01;//
					write_eeprom(EEP_INFO_BYTE,info_byte);	
						
						
					setpoint_select = DHOME_SP;					
					override_state = OVERRIDE_WORKING;
					}
				}	
				#ifndef TSTAT7_ARM
       icon.fan = 1;
			 icon.setpoint = 1;
			 icon.occ_unocc = 1;
				#endif
			}
		}
	//	#endif
		
	 #ifdef TSTAT7_ARM
		if(PirSensorSelect == 1)
				{
					if(abs(pir_value - PirSensorZero )> Pir_Sensetivity) //occupied
					{
						//led_state = !led_state;
						pir_trigger = PIR_TRIGGERED;
						override_state = OVERRIDE_TRIGGERED;
						
						if(override_state == OVERRIDE_TRIGGERED)/*&&(fan_speed_user == FAN_OFF)*/
						{
							if(EEP_OverrideTimer == 0xff)
								EEP_OverrideTimer = 60;
							if(EEP_OverrideTimer != 0)
							{
								#ifdef OCCTEST
								override_timer_time = 10;//(uint32)60 * EEP_OverrideTimer;
								GPIO_SetBits(GPIOA, GPIO_Pin_0);
								#else
								override_timer_time = (uint32)60 * EEP_OverrideTimer;
								#endif
							occupied = 1;
							fan_speed_user = FAN_AUTO;
							fanspeedbuf = FAN_AUTO;
							info_byte |= 0x01;//
							write_eeprom(EEP_INFO_BYTE,info_byte);	
								
								
							setpoint_select = DHOME_SP;					
							override_state = OVERRIDE_WORKING;
							}
						}	
//					 icon.fan = 1;
//					 icon.setpoint = 1;
//					icon.occ_unocc = 1;
					}
				}

	#endif		

		
				
		//reg 107:ff 03 00 6b 00 01 e0 08
		//reg 108:ff 03 00 6c 00 01 31 c9
		//reg 109:ff 03 00 6d 00 01 00 09				
		//read_co2_sensor();
		#ifdef TSTAT_CO2		
		if(co2_cnt == 5)
		{
		co2_cnt = 0;			
		memcpy(uart_sendC,p_co2_cmd,9);
    USART_SendDataStringC(9);	
		}
		co2_cnt++;
		#endif
				
//		if((EEP_PidOutput7 == 1)&&(ctest[0] == 1))
//				ctest[0] = 2;//EEP_PidOutput7 changed before run this task

//		if((EEP_PidOutput7 == 1)&&(ctest[0] == 0))
//				ctest[0] = 3;//EEP_PidOutput7 changed during run this task
		
//		if((giPwmTimeOn[0] == 0) && (ctest[2] == 1))
//			ctest[2] = 2;
//		if((giPwmTimeOn[0] == 1) && (ctest[2] == 0))
//			ctest[2] = 3;	
		
//		pwmcnt++;
//		ctest[2] = pwmcnt;
//		if(pwmcnt < 20)	
//			pwmtest = 470;			
//			
//		if(pwmcnt>20)
//			pwmtest = 495;	
//		
//		if(pwmcnt >= 40)
//			pwmcnt = 0;
		
		delay_ms(300);
		}			
}


uint8 utest = 0;
uint8 utest2 = 0;
void vCOMMTask(void *pvParameters )
{
	static uint16 i = 0;
	modbus_init();
	#ifdef TSTAT_ZIGBEE
  modbus_initB();
	#endif 
	for( ;; )
	{
		if (dealwithTag)
		{ 	
		  dealwithTag--;
		  if(dealwithTag == 1)//&& !Serial_Master )
				{				
					dealwithData();				
				}
		}

		if(serial_receive_timeout_count>0)  
		{
				serial_receive_timeout_count -- ; 
				if(serial_receive_timeout_count == 0)
					serial_restart();
		}
#ifdef TSTAT_ZIGBEE
		if (dealwithTagB)
		{ 
		 dealwithTagB--;
		  if(dealwithTagB == 1)//&& !Serial_Master )
				{				
				dealwithDataB();		
				}
		}		
		
		if(serial_receive_timeout_countB>0)  
		{
				serial_receive_timeout_countB-- ; 
				if(serial_receive_timeout_countB == 0)
				{
					serial_restartB();
					//printf("serial restart\r\n");
				}
		}		
#endif//tstat_zigbee	
	  if(update_flag == 10)//update 
		{
		
		}
    if(i<250)
			i++;
		else
		{
			i = 0;
			RTC_Get();
		}
		delay_ms(2) ;
	}
	
}


#ifdef TSTAT7_ARM
uint8 setpoint_change_flag = 0;
#endif
int16 pre_setpoint = 0;
/*
void vUpdatePID( void *pvParameters )
{
	int16 temp;
	
	for(;;)
		{
			taskENTER_CRITICAL();
			temp = Get_current_setpoint(setpoint_select);
			#ifdef TSTAT7_ARM
			if(setpoint_change_flag == 0)
				loop_setpoint[PID_LOOP1] = temp;
			
			#else
			loop_setpoint[PID_LOOP1] = temp;
			if(pre_setpoint != loop_setpoint[PID_LOOP1])
			{
				pre_setpoint = loop_setpoint[PID_LOOP1];
			  icon.setpoint = 1;
			}
			#endif
			
		  taskEXIT_CRITICAL();
			update_pid();	
			delay_ms(1000);
		}			
}
*/

void vControlLogic( void *pvParameters )
{
	int16 temp;
	uint16 pid_cnt = 0;
	init_delay_flag = 0;
	
	//PID_init();
	for(;;)
		{

			temp = Get_current_setpoint(setpoint_select);
			#ifdef TSTAT7_ARM
			if(setpoint_change_flag == 0)
				loop_setpoint[PID_LOOP1] = temp;
			
			#else
			loop_setpoint[PID_LOOP1] = temp;
			if(pre_setpoint != loop_setpoint[PID_LOOP1])
			{
				pre_setpoint = loop_setpoint[PID_LOOP1];
			  icon.setpoint = 1;
			}
			#endif
			
		 // taskEXIT_CRITICAL();
			
			pid_cnt++;
			if(pid_cnt > PidSampleTime *5)
			{	
				update_pid(1);
			  pid_cnt = 0;
			}
			else
			{
				update_pid(0);
			}
//			ctest[1] = temperature;
//			ctest[2] = loop_setpoint[PID_LOOP1];			
//			ctest[0] = PID_realize(loop_setpoint[PID_LOOP1],temperature);
			
		  control_logic();		
		if(EEP_TimerSelect == 3)
			{
			TimerSwitch(2);
			}
		else
  		TimerSwitch(1);
		

		delay_ms(200);
		}			
}

void vStateSwitch( void *pvParameters )
{
	uint8 i;
	for(;;)
	{
		if(stateswitchflag > 0)
		{	
			stateswitchflag--;
			if(stateswitchflag == 0)
			{
				for(i=0; i<3; i++)
					set_bit(&STATE_LOCK, i, 0);
				stateswitchflag = 0;
			//	vTaskDelete(NULL);
			}
		}
			delay_ms(1000);
	}		
}

void vCoolingLockOut( void *pvParameters )
{
	static uint8 i;
	for(;;)
		{			
			cooling_lock_10s_counter++;
				
				if (cooling_lock_10s_counter >= 6)
				{
					cooling_lock_10s_counter = 0;
					for(i=0;i<3;i++)
					{	
						if(cooling_lockout_time[i]>0)
							cooling_lockout_time[i]--;
						if (cooling_lockout_time[i] == 0)
						{
							set_bit(&COOLING_LOCKOUT,i,0) ; //cooling_lockout = 0 ;
							vTaskDelete(NULL);
						}
					}
				}				
		delay_ms(10000);
		}			
}

void vHeatingLockOut( void *pvParameters )
{
	static uint8 i;
	for(;;)
		{			
			heating_lock_10s_counter++;
			if (heating_lock_10s_counter >= 6)
			{
				heating_lock_10s_counter = 0;
				for(i=0;i<3;i++)
				{
					if(heating_lockout_time[i] > 0)
						heating_lockout_time[i]--;
					if(heating_lockout_time[i] == 0)
					{
						set_bit(&HEATING_LOCKOUT,i,0) ; //heating_lockout = 0 ;
						vTaskDelete(NULL);
					}
				}
			}			
			delay_ms(10000);
		}			
}

void vKeypadsHandle(void *pvParameters)
{
	setpoint_adjust_flag = 0 ;
  prev_keypad_state = 0	;
	keypad_ini();
	KEY_TEMP.BYTE = 0;
	for(;;)
		{
		keypad_handler( );	
		delay_ms(20);
		}			
}

//void vDealWithKey(void *pvParameters)
//{
//	for(;;)
//	{
////	DealWithKey(menu_id);	
//	delay_ms(50);
//	}		
//}

static void vOutputTask( void *pvParameters)
{
	OUTPUT_1TO5.BYTE = 0x00;
	RELAY_1TO5.BYTE = 0xff;
	OFF_FIRST.BYTE = 0xff;
	//output_ini();
	for(;;)
		{
		frc_mode_check();
		refresh_output();			
		delay_ms(200);
		}			
}



#ifndef TSTAT7_ARM
static void vDisplayRefresh(void *pvParameters)
{
	uint8 menu_buf[9],i;
	static uint8 top_refresh = 0;
	 
  icon.unit = 1;
	icon.setpoint = 1;
	icon.fan = 1;
	icon.sysmode = 1;
	icon.occ_unocc = 1;
	icon.fanspeed = 1;
	
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
  scroll = &scroll_ram[0][0];
	fanspeedbuf = fan_speed_user;
	if(testlcd)//test interface
	{
		;//disp_str(FORM15X30, 0,  0,  "testing",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
	}	
	else
	{
		
		disp_null_icon(240, 36, 0, 0,TIME_POS,TSTAT8_CH_COLOR, TSTAT8_MENU_COLOR2);
    disp_icon(14, 14, degree_o, UNIT_POS - 14,56 ,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
		
    draw_tangle(102,105);
		draw_tangle(102,148);
		draw_tangle(102,191);
	
	}
	for(;;)
		{	
//		if((EEP_PidOutput7 == 1 )&&(ctest[0] == 0))
//			ctest[0] = 1;	

		
			top_refresh++;
			if(top_refresh > 5)
				top_refresh = 0;
			if(update_flag == 1)
			{
				ClearScreen(TSTAT8_BACK_COLOR); 
				disp_str(FORM15X30, 30,  30,  "Updating",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
				serial_restart();
				SoftReset();
			}
			else if(update_flag == 2)
			{
				update_flag = 0;
				ClearScreen(TSTAT8_BACK_COLOR); 
				disp_str(FORM15X30, 30,  30,  "Restart",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
				initialize_eeprom( ) ; // set default parameters
				DisRestart( );
			}
			else if(update_flag == 4)//switch baudrate
			{
			//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				update_flag = 0;
			if(EEP_Baudrate == BAUDRATE_19200)
				{
				uart1_init(19200);
				modbus.baudrate = 19200;
				}
			else if(EEP_Baudrate == BAUDRATE_9600)
				{
				uart1_init(9600);
				modbus.baudrate = 9600;
				}

			else if(EEP_Baudrate == BAUDRATE_38400)
				{
				uart1_init(38400);
				modbus.baudrate = 38400;
				}
			else if(EEP_Baudrate == BAUDRATE_57600)
				{
				uart1_init(57600);
				modbus.baudrate = 57600;
				}
			else if(EEP_Baudrate == BAUDRATE_115200)
				{
				uart1_init(115200);
				modbus.baudrate = 115200;
				}
			else if(EEP_Baudrate == BAUDRATE_76800)
				{
				uart1_init(76800);
				modbus.baudrate = 76800;
				}	
			else if(EEP_Baudrate == BAUDRATE_1200)
				{
				uart1_init(1200);
				modbus.baudrate = 1200;
				}
			else if(EEP_Baudrate == BAUDRATE_4800)
				{
				uart1_init(4800);
				modbus.baudrate = 4800;
				}
			else if(EEP_Baudrate == BAUDRATE_14400)
				{
				uart1_init(14400);
				modbus.baudrate = 14400;
				}
			if(modbus.com_config[0] == MODBUS)	
				serial_restart();				
			}
			
			else if((update_flag == 5)||(update_flag == 7))
			{
				Set_night_setpoint(((setpoint_buf >> 8)&0xff), (setpoint_buf & 0xff));
				refresh_setpoint(NIGHT_MODE);
			  update_flag = 0;
			}
			else if((update_flag == 6)||(update_flag == 8))
			{
				Set_day_setpoint(((setpoint_buf >> 8)&0xff), (setpoint_buf & 0xff));
				refresh_setpoint(DAY_MODE);
			  update_flag = 0;
			}			

			
			else if(update_flag == 9)//switch rs485 and zigbee mode 
			{
				//disp_str(FORM15X30, 0,  0,  "flag=90",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
				update_flag = 0;
				if(RS485_Mode == 1)//zigbee mode
				{
					#ifdef TSTAT_ZIGBEE 
					USART_DeInit(USART1); 
					uart4_init(115200);
					#endif
				}
				else //if(RS485_Mode == 0) //rs485 mode or auto mode
				{
					#ifdef TSTAT_ZIGBEE
					if(RS485_Mode == 0)
						USART_DeInit(UART4);
					else
						uart4_init(115200);
					#endif 
					switch(EEP_Baudrate)
					{
						case BAUDRATE_9600:
							uart1_init(9600);
						break;
						
						case BAUDRATE_19200:
							uart1_init(19200);
						break;

						case BAUDRATE_38400:
							uart1_init(38400);
						break;

						case BAUDRATE_57600:
							uart1_init(57600);
						break;

						case BAUDRATE_115200:
							uart1_init(115200);
						break;		

						case BAUDRATE_76800:
							uart1_init(76800);
						break;
								
						case BAUDRATE_1200:
							uart1_init(1200);
						break;

						case BAUDRATE_4800:
							uart1_init(4800);
						break;

						case BAUDRATE_14400:
							uart1_init(14400);
						break;
					
						default:
							uart1_init(19200);
						break;	
					}
					
				}
				#ifdef TSTAT_ZIGBEE
				if(RS485_Mode == 2)	
					serial_restartB();
				#endif
				serial_restart();
			}
			
			

			
			
			
			else
			{
					blink_parameter = !blink_parameter;
					if(menu_mode == 1)
					{				
						if((blink_parameter == 0)&&((item_to_adjust<EEP_CLOCK_YEAR) || (item_to_adjust>EEP_PROTOCOL_SEL/*EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE*/)))
								clear_line(2);
						else 
								show_parameter(); 
					}
					else
					{	
						if(testlcd)
						{
							;
						}
						else
						{
							if(exist_menu)
							{
								clear_lines();
								icon.setpoint = 1;
								icon.fan = 1;
								icon.sysmode = 1;
//								icon.heatcool = 1;
								icon.occ_unocc = 1;

								draw_tangle(102,105);
								draw_tangle(102,148);
								draw_tangle(102,191);
								exist_menu = 0;
							}
							blink_flag = !blink_flag;
							display_flag = 1;
							if(top_refresh == 5)
								//Top_area_display(TOP_AREA_DISP_ITEM_TEMPERATURE, -250, TOP_AREA_DISP_UNIT_C);
								Top_area_display(TOP_AREA_DISP_ITEM_TEMPERATURE, temperature, TOP_AREA_DISP_UNIT_C);	
							if(Show_ID_Enable)
							{
								if(showidbit == 0)
								{
								clear_lines();
								showidbit = 1;
								}						
								disp_str(FORM15X30, 0,  MENU_ITEM1,  "MODBUS",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
								disp_str(FORM15X30, 0,  MENU_ITEM2,  "ID",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
								display_value(0,laddress, ' ');
							}	
							else							
							{
								display_SP(loop_setpoint[0]);
								display_fanspeed(fanspeedbuf);
								display_mode();	
							}	
								
									
								for(i=0;i<9;i++)
									icon_control_output(i);
							
								display_icon();
								//refresh_icon_output();
								display_fan();
							
							display_scroll();
							if(icon.cmnct_send > 0)
								disp_icon(13, 26, cmnct_send, 	0,	0, TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
							else
								disp_null_icon(13, 26, 0, 0,0,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);//(26, 26, cmnct_icon, 	0,	0, TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);

							if(icon.cmnct_rcv > 0)
								disp_icon(13, 26, cmnct_rcv, 	13,	0, TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
							else
								disp_null_icon(13, 26, 0, 13,0,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);//(26, 26, cmnct_icon, 	0,	0, TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
	
							#ifndef TSTAT7_ARM
							if(icon.cmnct_send > 0)
								icon.cmnct_send--;
							if(icon.cmnct_rcv > 0)
								icon.cmnct_rcv--;
							#endif	

							
							#ifdef TSTAT8_HUNTER
							if((icon.cmnct_send > 0) || (icon.cmnct_rcv > 0))//uart is active 
								RS485_LED_ON
							else
								RS485_LED_OFF
							#endif //TSTAT8_HUNTER
							
						}
					}					
					DealWithKey(menu_id);										
			}
			
//				if((EEP_PidOutput7 == 1)&&(ctest[0] == 1))
//				ctest[0] = 4;//EEP_PidOutput7 changed before run this task

//				if((EEP_PidOutput7 == 1)&&(ctest[0] == 0))
//				ctest[0] = 5;//EEP_PidOutput7 changed during run this task
			
			delay_ms(300);
		}
}



#endif

//extern uint8 rs485_zigbee;
#ifdef TSTAT_ZIGBEE	
uint16 zigbee_reset_timer =0;
#endif
static void vSoftwareTimer( void *pvParameters)//one second base software timer
{	
	//uint8 zgb_str =0;
//	uint8 i;
	uint8 onemin_cnt = 0;
  uint8 onehour_cnt = 0;
	for(;;)
		{	
			watchdog();
					
		if(idlockflag == NEED_TO_UNLOCK)  
			idlockcnt++;
		if(idlockcnt > 5)
			{
			idlockflag = NEED_TO_LOCK;	
			idlockcnt = 0;
			}
			if(spacetem.source == FRC_REMOTE_SENSOR)
			{
				if(spacetem.update > 0)
				{
					spacetem.status = FRC_STATUS_OK;
					spacetem.update--;
				}
				else
					spacetem.status = FRC_STATUS_ERR;
			}
			
			if(supplytem.source == FRC_REMOTE_SENSOR)
			{
				if(supplytem.update > 0)
				{
					supplytem.status = FRC_STATUS_OK;
					supplytem.update--;
				}
				else
					supplytem.status = FRC_STATUS_ERR;
			}
			
			if(outdoortem.source == FRC_REMOTE_SENSOR)
			{
				if(outdoortem.update > 0)
				{
					outdoortem.status = FRC_STATUS_OK;
					outdoortem.update--;
				}
				else
					outdoortem.status = FRC_STATUS_ERR;
			}
			
			if(indoorhum.source == FRC_REMOTE_SENSOR)
			{
				if(indoorhum.update > 0)
				{
					indoorhum.status = FRC_STATUS_OK;
					indoorhum.update--;
				}
				else
					indoorhum.status = FRC_STATUS_ERR;
			}
			
			if(outdoorhum.source == FRC_REMOTE_SENSOR)
			{
				if(outdoorhum.update > 0)
				{
					outdoorhum.status = FRC_STATUS_OK;
					outdoorhum.update--;
				}
				else
					outdoorhum.status = FRC_STATUS_ERR;
			}
			if(EEP_DeadMaster != 0)
			{
			if(deadmaster_timer > 0)
				deadmaster_timer--;
			if((deadmaster_timer == 0)&&(deadmaster_triggered == 0))
				deadmaster_triggered = 1;
//			else
//				deadmaster_triggered = 0;
			}
			onemin_cnt++;
			if(onemin_cnt >= 60)
			{ 
				if(Show_ID_Enable > 0)
					showid_cnt--;
				onemin_cnt = 0;				
			}

			
			if((showid_cnt == 0)&&(Show_ID_Enable>0))
			{
				Show_ID_Enable = 0;
				exist_menu = 1;
			}
		#ifdef TSTAT7_ARM	
			if(update_flag == 1)
			{
				serial_restart();
				SoftReset();
			}
			else if(update_flag == 2)
			{
				update_flag = 0;
				initialize_eeprom( ) ; // set default parameters
			//	ConverteCF( );
			//	DisRestart( );
			}		
			else if(update_flag == 4)//switch baudrate
			{
			//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				update_flag = 0;
			if(EEP_Baudrate == BAUDRATE_19200)
				{
				uart1_init(19200);
				modbus.baudrate = 19200;
				}
			else if(EEP_Baudrate == BAUDRATE_9600)
				{
				uart1_init(9600);
				modbus.baudrate = 9600;
				}

			else if(EEP_Baudrate == BAUDRATE_38400)
				{
				uart1_init(38400);
				modbus.baudrate = 38400;
				}
			else if(EEP_Baudrate == BAUDRATE_57600)
				{
				uart1_init(57600);
				modbus.baudrate = 57600;
				}
			else if(EEP_Baudrate == BAUDRATE_115200)
				{
				uart1_init(115200);
				modbus.baudrate = 115200;
				}
			else if(EEP_Baudrate == BAUDRATE_76800)
				{
				uart1_init(76800);
				modbus.baudrate = 76800;
				}	
			else if(EEP_Baudrate == BAUDRATE_1200)
				{
				uart1_init(1200);
				modbus.baudrate = 1200;
				}
			else if(EEP_Baudrate == BAUDRATE_4800)
				{
				uart1_init(4800);
				modbus.baudrate = 4800;
				}
			else if(EEP_Baudrate == BAUDRATE_14400)
				{
				uart1_init(14400);
				modbus.baudrate = 14400;
				}
				
      serial_restart();				
			}

			
		#endif	
				
			#ifdef TSTAT_ZIGBEE	
     zigbee_reset_timer++;
			
				if(testlcd) 
					{
						f[0] = utest/100 + 0x30;
						f[1] = (utest%100)/10 + 0x30;
						f[2] = utest%10+ 0x30;
						f[3] = '\0';
						disp_str(FORM15X30, 0,  50,  f,TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
						
						f[0] = utest2/100 + 0x30;
						f[1] = (utest2%100)/10 + 0x30;
						f[2] = utest2%10+ 0x30;
						f[3] = '\0';
						disp_str(FORM15X30, 0,  0,  f,TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);	
						
						f[0] = zigbee_reset_timer/100 + 0x30;
						f[1] = (zigbee_reset_timer%100)/10 + 0x30;
						f[2] = zigbee_reset_timer%10+ 0x30;
						f[3] = '\0';
						disp_str(FORM15X30, 0,  100,  f,TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);	
					}	

		  if(RS485_Mode == 1)//zigbee mode
			{
				if(zigbee_reset_timer >= 600)//reset zigbee module
				{
					uart4_init(115200);
					modbus_initB();
					zigbee_reset_timer = 0;	
					//write value 1 to register 34 of zigbee module to reset the zigbeee module
					//00 06 00 22 00 01 e9 d1
					uart_sendB[0] = 0x00;
					uart_sendB[1] = 0x06;
					uart_sendB[2] = 0x00;
					uart_sendB[3] = 0x22;
					uart_sendB[4] = 0x00;
					uart_sendB[5] = 0x01;
					uart_sendB[6] = 0xe9;
					uart_sendB[7] = 0xd1;

					USART_SendDataStringB(8);
					
				}
			}
//				zgb_str++;
//				if(zgb_str>=10)
//				{
//					init_crc16();
//					zgb_str = 0;
//					uart_sendB[0] = 0x00;
//					uart_sendB[1] = 0x03;
//					uart_sendB[2] = 0x00;
//					uart_sendB[3] = 0x33;
//					uart_sendB[4] = 0x00;
//					uart_sendB[5] = 0x01;
//					uart_sendB[6] = 0x75;
//					uart_sendB[7] = 0xd4;
//					USART_SendDataStringB(8);
//					read_zgb_index = 1; 
//				}
//				if(zigbee_index > 0)//there are other zigbee units connect, need to read rssi
//				{
//					//init_crc16();
//					uart_sendB[0] = 0x00;
//					uart_sendB[1] = 0x03;
//					uart_sendB[2] = 0x00;
//					uart_sendB[3] = 0x34;
//					uart_sendB[4] = 0x00;
//					uart_sendB[5] = zigbee_index*2;

//					for(i=0;i<6;i++)
//						crc16_byte(uart_sendB[i]);
//					uart_sendB[6] = CRChi;
//					uart_sendB[7] = CRClo;
//					USART_SendDataStringB(8);				
//				}
				
//			 }
			#endif
//	if(rs485_zigbee == RS485_ENABLE)
//		GPIO_SetBits(GPIOA, GPIO_Pin_0);//backlight ON
//	else
//		GPIO_ResetBits(GPIOA, GPIO_Pin_0);//backlight OFF	
		
		today_dayofyear = get_day_of_year();
    if((override_timer_time==0)&&(schedule_on_off))	
		{			
			RunSchedule();
			#ifndef TSTAT7_ARM
			icon.occ_unocc = 1;
			icon.fan = 1;	
			icon.setpoint = 1;
			#endif
		}
//		if(schedule_on_off == 0)
//		{
//			EEP_OverrideTimer = 0;
//			override_timer_time = 0;
//		}
		if(menu_mode == 1)
		{
		menu_timer++;
		if(menu_timer == 5)
			{
			menu_mode = 0;
			menu_timer = 0;
			#ifndef TSTAT7_ARM	
			accept_parameter();
			#endif	
			exist_menu = 1; //indicate just exist from menu mode 	
			//clear_lines();
			}			
		}
		if((override_timer_time > 0)&&(EEP_OverrideTimer != 0)/*&&(schedule_on_off == SCHEDULE_ON)*/)
		{
			override_timer_time--;	
			if(override_timer_time == 0)//if override timer reduce to 0, go to off mode
			{
				#ifdef OCCTEST
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				#endif
				fan_speed_user = read_eeprom(EEP_FAN_SPEED);
				EEP_FanSpeed = fan_speed_user;
				fanspeedbuf = fan_speed_user;
				#ifndef TSTAT7_ARM
				icon.fan = 1;
				#endif
//				if(schedule_on_off == SCHEDULE_OFF)
//				{
//					occupied = 0;
//					fan_speed_user = FAN_OFF;
//					info_byte &= 0xfe;//
//					write_eeprom(EEP_INFO_BYTE,info_byte);
//				}
				override_state = OVERRIDE_NOTTRIGGER;
			}
//			else
//			{
//				occupied = 1;
//				setpoint_select = DHOME_SP;	
//				info_byte |= 0x01;
//			}
		}
		if(scan_timer>1)
			scan_timer--;
		
		#ifndef OCCTEST
		if(lcd_turn_off_timer > 0)
			lcd_turn_off_timer--;
		if(lcd_turn_off_timer == 0)
		{
			if(LCDTurnOff != 255)
				#ifdef TSTAT7_ARM
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
				#else
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				#endif
				//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		}
		if(LCDTurnOff == 255)
		#ifdef TSTAT7_ARM
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		#else
			GPIO_SetBits(GPIOA, GPIO_Pin_0);//backlight_ctrl(BK_ON);
		#endif
    #endif //OCCTEST
#ifndef TSTAT7_ARM		
		if(sp_blink_timer>0)
		{
			icon.setpoint = 1;
			sp_blink_timer--;
		}
		if(fan_blink_timer>0)
		{
			icon.fan = 1;
			fan_blink_timer--;
		}
		if(fan_blink_timer == 0)
		{
			//faninflag = 0;
			scheduleoff = 1;
			if(faninflag == 1)
				accept_fan_setting();
			faninflag = 0;
		}
		if(hc_blink_timer>0)
		{
			hc_blink_timer--;		
			icon.sysmode = 1;
		}
#endif//TSTAT7_ARM		
		outsidetem_timer--;
		if(outsidetem_timer == 0)//outside tem sensor detect
			outsidetem_health = 1; //5 minutes no write by master, outside temp unhealth
		
		if(dtime[0]>0)
			dtime[0]--;
		if(dtime[0] == 0)
			set_bit(&OUT_PINS_1TO5,1,0);
		
		if(dtime[1]>0)
			dtime[1]--;
		if(dtime[1] == 0)
			set_bit(&OUT_PINS_1TO5,3,0);

		if(dtime[2]>0)
			dtime[2]--;
		if(dtime[2] == 0)
			set_bit(&OUT_PINS_1TO5,5,0);

		if(dtime[3]>0)
			dtime[3]--;
		if(dtime[3] == 0)
			set_bit(&OUT_PINS_1TO5,7,0);

		if(dtime[4]>0)
			dtime[4]--;
		if(dtime[4] == 0)
			set_bit(&OUT_PINS_1TO5,9,0);

		if(dtime[5]>0)
			dtime[5]--;
		if(dtime[5] == 0)
			set_bit(&OUT_PINS_1TO5,11,0);

		if(dtime[6]>0)
			dtime[6]--;
		if(dtime[6] == 0)
			set_bit(&OUT_PINS_1TO5,13,0);

			
		delay_ms(1000);
	  }		
}








extern Wr_one_day wr_times[MAX_WR][MAX_SCHEDULES_PER_WEEK];

static void vMSTP_TASK(void *pvParameters )
{
//	static u16 count_start_task ;
	uint16_t pdu_len = 0; 
	BACNET_ADDRESS  src;
	Inital_Bacnet_Server();
	//modbus.com_config[0] = BAC_MSTP;
	dlmstp_init(NULL);
	//if(modbus.com_config[0] == BAC_MSTP)
	Recievebuf_Initialize(0);
	Set_Object_Name((void *)&(UserInfo_Name(0)));
	AVS = MAX_AVS;
  AIS = MAX_AIS;
  AOS = MAX_AOS;
  BIS = 0;
  BOS = MAX_BOS;
	
	//ar_dates[0][22] = 0x55;
	
	for (;;)
    {			
		if(modbus.com_config[0] == BAC_MSTP)
		{
				modbus.protocal = BAC_MSTP;
				pdu_len = datalink_receive(&src, &PDUBuffer[0], sizeof(PDUBuffer), 0,	modbus.protocal);
						
				if(pdu_len) 
					{
						npdu_handler(&src, &PDUBuffer[0], pdu_len, BAC_MSTP);		
					} 						
		}
			delay_ms(1);
	}
	
}



