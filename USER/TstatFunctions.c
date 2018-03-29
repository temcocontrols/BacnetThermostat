#include "types.h"
#include "24cxx.h"
#include "define.h"
#include "eep_mapping.h"
#include "TstatFunctions.h"
#include "eepdefine.h"
#include "setpoint.h"
#include "types.h"
#include "constcode.h"
#include "key.h"
#include "control_logic.h"
#include "output.h"
#include "lcd.h"
#include "inputs.h"
#include "rtc.h"
#include "delay.h"
#include "myiic.h"
#include "math.h"
//#include "testcode.h"
#include "usart.h"
#include "modbus.h"
#include "bacnet.h"
#include "rs485.h"
#include "stmflash.h"

uint16_t flash_buf[2];
extern Wr_one_day wr_times[MAX_WR][MAX_SCHEDULES_PER_WEEK];

extern uint8 zigbee_alive;
uint16 today_dayofyear;
extern u32 Instance;
//uint8 pir_threshold = 150;
extern uint8 ScheduleModeNum;
uint8 pre_event = 0;
uint8 deadmaster_triggered = 0;
uint16 deadmaster_timer = 0;
uint8 pre_mode = 5;
uint8 pre_fanspeed = 0;
uint8 fanspeedbuf = 0;
uint8 cooltest;
uint8 out6buf;
extern int16 ctest[10]; 
uint8 scheduleoff;
uint8 faninflag = 0;
uint8  buffer_occupied;
uint8  back_buffer_occupied;
uint8 setpoint_select;
uint8 pre_setpoint_select = 100;
uint16 outsidetem_timer = 300;
uint8 outsidetem_health = 0; //0:health  1:unhealth  
uint8 outside_tem_flag = 0;
uint8 fan_name_check = 0;
uint8 sp_blink_timer = 0;
uint8 fan_blink_timer = 0;
uint8 hc_blink_timer = 0;
uint8 blink_flag = BLINK_OFF;
uint16 lcd_turn_off_timer = 0;
void save_user_def(void);
int16 eep_get_value(uint8 hibyte, uint8 lobyte);
//uint8 manual_icon_control = MANUAL_ICON_DISABLE;
uint8  disp_item_queue[15];
//uint16 time_counter;
//uint8 timer_switch_status[2] = {0,0};
//uint8 timer_enable = 0;
//uint8 serial_no_activity = 0;
//uint8 lighting_stage;
int16 pir_timer = 0;
extern uint8 SN_WR_Flag;
int16 humidity_calibration = 0;
int16 org_humidity;
//extern void SoftReset(void);
uint8 menu_timer = 0;
union list
{
  int16    new_setpoint ;
  int16    new_parameter ;
};
uint8 clock_menu_item;//indicate which item will blink 
uint8 xdata analog_output_type = 0;
//uint8 xdata id_write_protect = ID_WRITE_ENABLE;
uint8 time_data[9];
uint8 xdata demand_response;  //0:function disabled 1: NO  2:YES 
union list menubuffer;
uint8 init_PID_flag;
int16 loop_setpoint[3];
uint8 part_lock = 0;
uint8 occupied; 
uint8 fan_speed_user;
uint8 backup_fan_speed; 
//uint8 override_timer;
uint8 serial_int_flag = 0;
//uint8 show_id_state = ID_DISABLE;
uint16  show_id_cnt = 0;
//uint8 blink_flag = 0; // 1-bool  make sure its 0 on startup or display will be off
uint8 dis6_display;
uint8 dis6 = 0;
uint8 display_blink_flag;  //0: disable blink  1:enable blink
uint8 gbDisplayChanged = 0;
uint8 override_timer_flag = OFF;
uint8 occupancy = 0;
uint32 override_timer_time = 0;
uint8 one_minute_counter;
uint8 menu_mode = 0;
uint8 menu_addi;
#ifndef TSTAT7_ARM
uint16  item_to_adjust;
#endif
//////////////////////////////////////////////////

int32 mul( signed	long x1	, int32 x2 )
{
	return(	x1 * x2	);
}
uint8 get_bit(uint16 c,uint8 pos)
{
 	uint16  newval;
	newval = c >> pos;
	newval &= 0X01;
	return newval;
}

void SetByteBit(uint8* c,uint8 pos,uint8 vb)   
{
	uint8  temp;
	//
	temp = 0x01;
	temp <<= pos;
	if(vb)//set bool 
	{	
		*c |= temp;
	}
	else
	{//clear bool
		temp = ~temp ;	 	
		*c &= temp ;
	}
	//ET1= 1;
}

void set_bit(uint16* c,uint8 pos, uint8 on_or_off)	 //TURN ON THE ACTUAL BITS IN HARDWARE
                     //ON_OR_OFF IS THE STATE, 1 = ON, 0 = OFF
					 //POS IS TEH BIT POSITION IN THE OUT_PINS_1TO5 INT 
{
	uint16  temp;
	temp = 0x01;
	temp <<= pos;
	if(on_or_off)
	{	
		*c |= temp;
	}
	else
	{
		temp = ~temp ;	 	
		*c &= temp ;
	}
}

uint8 GetByteBit(uint8* c,uint8 pos)   
{
	uint8  temp;
	//
	temp = 0x01;
	temp <<= pos;
	if((*c & temp)==0)
		return 0;//(uint8)(*c & temp);
	else
		return 1;
	
}


//void frc_read_srsid(STR_FRC_INPUT str_frc, uint16 eep_adr)
//{
//	str_frc.source = read_eeprom(eep_adr);
//	str_frc.id = read_eeprom(eep_adr + 2);
//	str_frc.id *= 256;
//	str_frc.id += read_eeprom(eep_adr + 1);
//}

void frc_ini(void)
{
//	frc_read_srsid(spacetem, EEP_FRC_SPACETEM_SOURCE);
//	frc_read_srsid(supplytem, EEP_FRC_SUPPLYTEM_SOURCE);
//	frc_read_srsid(outdoortem, EEP_FRC_OUTDOORTEM_SOURCE);
//	frc_read_srsid(outdoorhum, EEP_FRC_OUTDOORHUM_SOURCE);
//	frc_read_srsid(indoorhum, EEP_FRC_INDOORHUM_SOURCE);
	spacetem.source = read_eeprom(EEP_FRC_SPACETEM_SOURCE);
	spacetem.id = read_eeprom(EEP_FRC_SPACETEM_SOURCE + 2);
	spacetem.id *= 256;
	spacetem.id += read_eeprom(EEP_FRC_SPACETEM_SOURCE + 1);
	
	supplytem.source = read_eeprom(EEP_FRC_SUPPLYTEM_SOURCE);
	supplytem.id = read_eeprom(EEP_FRC_SUPPLYTEM_SOURCE + 2);
	supplytem.id *= 256;
	supplytem.id += read_eeprom(EEP_FRC_SUPPLYTEM_SOURCE + 1);

	outdoortem.source = read_eeprom(EEP_FRC_OUTDOORTEM_SOURCE);
	outdoortem.id = read_eeprom(EEP_FRC_OUTDOORTEM_SOURCE + 2);
	outdoortem.id *= 256;
	outdoortem.id += read_eeprom(EEP_FRC_OUTDOORTEM_SOURCE + 1);

	outdoorhum.source = read_eeprom(EEP_FRC_OUTDOORHUM_SOURCE);
	outdoorhum.id = read_eeprom(EEP_FRC_OUTDOORHUM_SOURCE + 2);
	outdoorhum.id *= 256;
	outdoorhum.id += read_eeprom(EEP_FRC_OUTDOORHUM_SOURCE + 1);

	indoorhum.source = read_eeprom(EEP_FRC_INDOORHUM_SOURCE);
	indoorhum.id = read_eeprom(EEP_FRC_INDOORHUM_SOURCE + 2);
	indoorhum.id *= 256;
	indoorhum.id += read_eeprom(EEP_FRC_INDOORHUM_SOURCE + 1);
}

void InitialRAM(void)
{
	uint16 i,j;
	uint8 temp;
	uint8 serialnum[4];
	EEP_DEGCorF = read_eeprom(EEP_DEGC_OR_F);
	if(EEP_DEGCorF>1)
	{
		EEP_DEGCorF = 0;
		write_eeprom(EEP_DEGC_OR_F,0);
	}
	for(i=0;i<TOTAL_EEP_VARIABLES;i++)//i = MAXEEPCONSTRANGE ; i < TOTAL_EE_PARAMETERS
		{
		temp = read_eeprom(i);
		if((i>=MAXEEPCONSTRANGE)&&(i<TOTAL_EE_PARAMETERS))
		{			
			if((temp >= parameter_array[EEP_DEGCorF][i-MAXEEPCONSTRANGE][LOWER])	&& (temp <= parameter_array[EEP_DEGCorF][i-MAXEEPCONSTRANGE][UPPER]))		
				b.eeprom[i] = temp;
			else
				{
				b.eeprom[i] = parameter_default[EEP_DEGCorF][i - MAXEEPCONSTRANGE];
				write_eeprom(i, b.eeprom[i]);
				}
		}
		else
				b.eeprom[i] = temp;
		}
	//if ID which store in EEPROM is 0xff or 0, use backup ID which store in flash.
	STMFLASH_Read(FLASH_MODBUS_ID, flash_buf, 2);
	temp = flash_buf[0];		
  if((laddress == 0xff) || (laddress == 0))
  {		
		STMFLASH_Read(FLASH_MODBUS_ID, flash_buf, 2);	
		laddress = flash_buf[0];
		if((laddress == 255) || (laddress == 0))
			laddress = 254;
	}
	else if(temp != laddress)//if eeprom id not equal to flash id, copy eeprom id to flash
	{
		flash_buf[0] = laddress;	
		STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);		
	}

	
	
	for(i=0;i<4;i++)
		serialnum[i] = read_eeprom(i);
	//if eeprom serial number all are 0xff or 0x00, use flash back up infomation 
	if((serialnum[0] == 0xff)&&(serialnum[1] == 0xff)&&( serialnum[2] == 0xff)&&( serialnum[3] == 0xff))
	{
		STMFLASH_Read(FLASH_SERIAL_NUM_LO, flash_buf, 2);	
		SerialNumber(0) = flash_buf[0];
		SerialNumber(1) = flash_buf[1];

		STMFLASH_Read(FLASH_SERIAL_NUM_HI, flash_buf, 2);	
		SerialNumber(2) = flash_buf[0];
		SerialNumber(3) = flash_buf[1];	
	}
	else if((serialnum[0] == 0)&&(serialnum[0] == 0)&&( serialnum[0] == 0)&&( serialnum[0] == 0))
	{
		STMFLASH_Read(FLASH_SERIAL_NUM_LO, flash_buf, 2);	
		SerialNumber(0) = flash_buf[0];
		SerialNumber(1) = flash_buf[1];

		STMFLASH_Read(FLASH_SERIAL_NUM_HI, flash_buf, 2);	
		SerialNumber(2) = flash_buf[0];
		SerialNumber(3) = flash_buf[1];		
	}
	else if((serialnum[0] != SerialNumber(0)) && (serialnum[1] != SerialNumber(1)) && (serialnum[2] != SerialNumber(2)) && (serialnum[3] != SerialNumber(3)))//
	{
		flash_buf[0] = serialnum[0];
		flash_buf[1] = serialnum[1];		
		STMFLASH_Write(FLASH_SERIAL_NUM_LO, flash_buf, 2);

		flash_buf[0] = serialnum[2];
		flash_buf[1] = serialnum[3];		
		STMFLASH_Write(FLASH_SERIAL_NUM_LO, flash_buf, 2);
	}
	
	if(hum_manual_enable == 0xff)
		{
		hum_manual_enable = 0;
		write_eeprom(EEP_HUM_INPUT_MANUAL_ENABLE, 0);
		}	

    if(hum_cal_hi == 0xff)
		{
		hum_cal_hi = 0;
		hum_cal_lo = 0;
		write_eeprom(EEP_HUM_CALIBRATION, 0);	
		write_eeprom(EEP_HUM_CALIBRATION + 1, 0);	
		}
		humidity_calibration = hum_cal_hi << 8;
		humidity_calibration |= hum_cal_lo;
		
	if(((uint16)Table1_ZERO_HI << 8) + Table1_ZERO_LO < ((uint16)Table1_Five_LO << 8) + Table1_Five_LO)
  	slope_type[0] = 1;
	else
		slope_type[0] = 0;
	
	
	if(((uint16)Table2_ZERO_HI << 8) + Table2_ZERO_LO < ((uint16)Table2_Five_LO << 8) + Table2_Five_LO)
  		slope_type[1] = 1;
	else
		slope_type[1] = 0;		
		
	SN_WR_Flag = read_eeprom(EEP_SERINALNUMBER_WRITE_FLAG);	
	if(SN_WR_Flag == 0xff)	
		{
		SN_WR_Flag = 0;
		write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, SN_WR_Flag);			
		}
	if((Calibration_Internal_HI == 0xff) && (Calibration_Internal_LO==0xff))
		{
		internal_calibration = 0;
		write_eeprom(EEP_CALIBRATION_INTERNAL_THERMISTOR,0);
		write_eeprom(EEP_CALIBRATION_INTERNAL_THERMISTOR+1,0);
		}
	else
		{
		internal_calibration = eep_get_value(Calibration_Internal_HI, Calibration_Internal_LO);
	//internal_calibration = (int16)internal_calibration << 8;	
	//internal_calibration |= Calibration_Internal_LO;
		}
//	if(EEP_HeatCoolConfig == 0)
//		heat_cool_user = HC_CFG_AUTO;
//	els
//		heat_cool_user
	
	if(Modename(0) == 0xff)//mode1 is not initialized
		{
			Modename(0) = 'O';
			Modename(1) = 'f';
			Modename(2) = 'f';
			Modename(3) = '\0';
			Modename(4) = ' ';
			Modename(5) = ' ';
			Modename(6) = ' ';
			Modename(7) = ' ';
		}

		if(Modename(8) == 0xff)//mode1 is not initialized
		{
			Modename(8) = 'L';
			Modename(9) = 'o';
			Modename(10) = 'w';
			Modename(11) = '\0';
			Modename(12) = ' ';
			Modename(13) = ' ';
			Modename(14) = ' ';
			Modename(15) = ' ';
		}
		if(Modename(16) == 0xff)//mode1 is not initialized
		{
			Modename(16) = 'M';
			Modename(17) = 'e';
			Modename(18) = 'd';
			Modename(19) = '\0';
			Modename(20) = ' ';
			Modename(21) = ' ';
			Modename(22) = ' ';
			Modename(23) = ' ';
		}
		if(Modename(24) == 0xff)//mode1 is not initialized
		{
			Modename(24) = 'H';
			Modename(25) = 'i';
			Modename(26) = '\0';
			Modename(27) = ' ';
			Modename(28) = ' ';
			Modename(29) = ' ';
			Modename(30) = ' ';
			Modename(31) = ' ';
		}
		if(Modename(32) == 0xff)//mode1 is not initialized
		{
			Modename(32) = 'A';
			Modename(33) = 'u';
			Modename(34) = 't';
			Modename(35) = 'o';
			Modename(36) = '\0';
			Modename(37) = ' ';
			Modename(38) = ' ';
			Modename(39) = ' ';
		}
		if(Modename(8) != 'L')
			fan_name_check = 1; //user has change the low name
		
		if(LCDTurnOff != 255)//if time set to 99, then LCD always on
		{
			lcd_turn_off_timer = LCDTurnOff;
			lcd_turn_off_timer *=60;	  	
		}
		if(heat_cool_user == 0xff)
			heat_cool_user = 0;	
		
		if(Hardware_Info_HI == 0xff && Hardware_Info_LO == 0xff)
		{
			write_eeprom(EEP_HARDWARE_INFORMATION + 1, 0);	
			Hardware_Info_HI = 0;			
			write_eeprom(EEP_HARDWARE_INFORMATION, 1);
			Hardware_Info_LO = 1;			
		}	

		if(ScheduleMondayEvent1(0) == 0xff)//if the first event value = 0xff, schedule table need to be initialized
		{
			
			for(i=0;i<7;i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT1_H + i*12, 6);
				ScheduleMondayEvent1(i*12) = 6;
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT1_H + i*12 + 1, 0);
				ScheduleMondayEvent1(i*12 + 1) = 0;
			}
			
			for(i=0;i<7;i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT2_H + i*12, 9);
				ScheduleMondayEvent2(i*12) = 9;
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT2_H + i*12 + 1, 0);
				ScheduleMondayEvent2(i*12 + 1) = 0;
			}
			
			for(i=0;i<7;i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT3_H + i*12, 18);
				ScheduleMondayEvent3(i*12) = 18;
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT3_H + i*12 + 1, 0);
				ScheduleMondayEvent3(i*12 + 1) = 0;
			}
			
			for(i=0;i<7;i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT4_H + i*12, 21);
				ScheduleMondayEvent4(i*12) = 22;
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT4_H + i*12 + 1, 0);
				ScheduleMondayEvent4(i*12 + 1) = 0;
			}
			for(i=0;i<7;i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT5_H + i*12, 0);
				ScheduleMondayEvent5(i*12) = 0;
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT5_H + i*12 + 1, 0);
				ScheduleMondayEvent5(i*12 + 1) = 0;
			}
			for(i=0;i<7;i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT6_H + i*12, 0);
				ScheduleMondayEvent6(i*12) = 0;
				write_eeprom(EEP_SCHEDULE_MONDAY_EVENT6_H + i*12 + 1, 0);
				ScheduleMondayEvent6(i*12 + 1) = 0;
			}
			
			for(i=0;i<(EEP_SCHEDULE_HOLIDAY_FLAG - EEP_SCHEDULE_MONDAY_FLAG + 2);i++)
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_FLAG+i, 0);
				ScheduleMondayFlag(i) = 0;				
			}
			
			for(i = 0;i<46;i++)
			{
				write_eeprom(EEP_SCHEDULE_DAY_BEGAIN+i, 0);
				ScheduleDay(i) = 0;
			}
			
		}
	

		if(RS485_Mode >2)
			RS485_Mode = 0;
		
		if(protocol_select > 1)
		{
			protocol_select = 0;
			modbus.com_config[0] = MODBUS;
		}
		
		if(Pir_Sensetivity == 0xff)//pir sensetivity not ini
		{
			Pir_Sensetivity = 30;
			write_eeprom(EEP_PIR_SENSETIVITY, Pir_Sensetivity);	
		}	
		
		pwm_duty = read_eeprom(EEP_PWM_DUTY)*10;
		if(pwm_duty > 1000)
			pwm_duty = 1000;
		else if(pwm_duty < 500)
			pwm_duty = 700;
	  //free cooling initialize
		ScheduleModeNum = read_eeprom(EEP_SCHEDULE_MODE_NUM);
		frc_ini();
		
		if((PirSensorZero == 255) || (PirSensorZero == 0))
		 PirSensorZero = 160;
		
//		Station_NUM = read_eeprom(EEP_STATION_NUMBER);
//		if(Station_NUM > 127)
//		{
//			Station_NUM = 1;
//		  write_eeprom(EEP_STATION_NUMBER, 1);
//		}
		Station_NUM = laddress;
		
		
		if((read_eeprom(EEP_INSTANCE1) == 0XFF)&&(read_eeprom(EEP_INSTANCE2) == 0XFF)/* && (read_eeprom(EEP_INSTANCE3) == 0XFF) && (read_eeprom(EEP_INSTANCE4) == 0XFF)*/)
		{	
			Instance = SerialNumber(0);
			Instance |= SerialNumber(1) << 8;
//			Instance |= SerialNumber(2) << 16;
//			Instance |= SerialNumber(3) << 24;
			
		}
		else
		{
			Instance = read_eeprom(EEP_INSTANCE1);
			Instance |= read_eeprom(EEP_INSTANCE2) << 8;
		}
		// wr_times[MAX_WR][MAX_SCHEDULES_PER_WEEK];
		for(i=0;i<7;i++)
		{
			for(j=0;j<8;j++)
			{
				if(ScheduleMondayEvent1(i*12 + j*2) > 24)
					wr_times[0][i].time[j].hours = 0;
				else
					wr_times[0][i].time[j].hours = ScheduleMondayEvent1(i*12 + j*2);
				
				if(ScheduleMondayEvent1(i*12 + j*2+1) > 59)
					wr_times[0][i].time[j].minutes = 0;
				else
					wr_times[0][i].time[j].minutes = ScheduleMondayEvent1(i*12 + j*2+1);
			}
		}
		for(i=0;i<AR_DATES_SIZE;i++)
		{
			ar_dates[0][i] = ScheduleDay(i);
		}

    if((InputVoltageTerm>150)||(InputVoltageTerm<50))
		{
			InputVoltageTerm = 100;
		}
		
		bottom_of_4to20ma = read_eeprom(EEP_4TO20MA_BOTTOM + 1);
		bottom_of_4to20ma *= 256; 
		bottom_of_4to20ma |= read_eeprom(EEP_4TO20MA_BOTTOM);

		top_of_4to20ma = read_eeprom(EEP_4TO20MA_TOP + 1);
		top_of_4to20ma *= 256; 
		top_of_4to20ma |= read_eeprom(EEP_4TO20MA_TOP);		
		
		#ifndef TSTAT7_ARM
		for(i=0;i<9;i++)
		{
			if(IconOutputControl(i) == 0xff)//EEPROM orignal data, need to be intitialized
			{
				IconOutputControl(i) = 0;
				write_eeprom(EEP_ICON_DAY_OUTPUT_CONTROL+i, 0);
			}
		}
		#endif //nTstat7	

    if(EEP_DTERM1 == 0xff)
		{
			EEP_DTERM1 = 0;
			write_eeprom(EEP_DTERM , 0 );
		}
		if(PidSampleTime == 0xff)
		{
			PidSampleTime = 0;
			write_eeprom(EEP_PID_SAMPLE_TIME , 0 );
		}
		
//	wr_times[0][0].time[0].hours = ScheduleMondayEvent1(0);
//	wr_times[0][0].time[0].minutes = ScheduleMondayEvent1(1);	
//	wr_times[0][0].time[1].hours = ScheduleMondayEvent1(2);
//	wr_times[0][0].time[1].minutes = ScheduleMondayEvent1(3);	
//	wr_times[0][0].time[2].hours = ScheduleMondayEvent1(4);
//	wr_times[0][0].time[2].minutes = ScheduleMondayEvent1(5);	
//	wr_times[0][0].time[3].hours = ScheduleMondayEvent1(6);
//	wr_times[0][0].time[3].minutes = ScheduleMondayEvent1(7);	
//	wr_times[0][0].time[4].hours = ScheduleMondayEvent1(8);
//	wr_times[0][0].time[4].minutes = ScheduleMondayEvent1(9);	
//	wr_times[0][0].time[5].hours = ScheduleMondayEvent1(10);
//	wr_times[0][0].time[5].minutes = ScheduleMondayEvent1(11);			
//	wr_times[0][0].time[6].hours = 0;//ScheduleMondayEvent1(6);
//	wr_times[0][0].time[6].minutes = 1;//ScheduleMondayEvent1(7);	
//	wr_times[0][0].time[7].hours = 22;//ScheduleMondayEvent1(8);
//	wr_times[0][0].time[7].minutes = 13;//ScheduleMondayEvent1(9);	
			
		
		
		
		

}


void initialize_eeprom(void)
{
	
uint16 i;
//uint8 table_sel = 0;
//	uint8 read_buf = 0;
	//table_sel = read_eeprom(USER_SETTING);
//	if(0)//table_sel == 99)//using user default table
//	{
//		for( i = MAXEEPCONSTRANGE ; i < TOTAL_EE_PARAMETERS ; i++ )
//		{
//		read_buf = read_eeprom(USER_MAXEEPCONSTRANGE + i - MAXEEPCONSTRANGE);	
////		write_eeprom(i, read_buf) ;
//		b.eeprom[i] = read_buf; 
//		}
////		read_buf = read_eeprom(USER_MAXEEPCONSTRANGE);
////		write_eeprom(MAXEEPCONSTRANGE, read_buf) ;
////		
////		read_buf = read_eeprom(USER_MAXEEPCONSTRANGE+1);
////		write_eeprom(MAXEEPCONSTRANGE+1, read_buf) ;
////		
////		read_buf = read_eeprom(USER_MAXEEPCONSTRANGE+4);
////		write_eeprom(MAXEEPCONSTRANGE+4, read_buf) ;
//		
//		laddress = read_eeprom(USER_ADDRESS);
//		write_eeprom(EEP_ADDRESS, laddress) ;
//	}
//	else//using temco factory default setting
//	{
//		for( i = 4 ; i < TOTAL_EEP_VARIABLES ; i++ )
//			{
//			write_eeprom(i	, 0xff) ; 
//			}
		


		for( i = MAXEEPCONSTRANGE ; i < TOTAL_EE_PARAMETERS ; i++ )
			{
				if(i != EEP_BAUDRATE)
				{
					write_eeprom(i	, parameter_default[EEP_DEGCorF][ i  - MAXEEPCONSTRANGE]) ;
					b.eeprom[i] =  parameter_default[EEP_DEGCorF][ i  - MAXEEPCONSTRANGE]; 
				}
			}
		if(EEP_DEGCorF == 0)//degree C
		{
			write_eeprom(EEP_DAY_SETPOINT, 200);
			write_eeprom(EEP_DAY_SETPOINT+1, 0);	
						
			EEP_DaySpLo = read_eeprom(EEP_DAY_SETPOINT); //day setpoint is not in the default range
			EEP_DaySpHi = read_eeprom(EEP_DAY_SETPOINT+1);		
		}
		else if(EEP_DEGCorF == 1)//degree F
		{
			write_eeprom(EEP_DAY_SETPOINT, 0xa8);
			write_eeprom(EEP_DAY_SETPOINT+1, 0x02);	
						
			EEP_DaySpLo = read_eeprom(EEP_DAY_SETPOINT); //day setpoint is not in the default range
			EEP_DaySpHi = read_eeprom(EEP_DAY_SETPOINT+1);		
		}			
		init_extern_operation_eeprom(sequence_of_operation);
		
		init_valve_operation_eeprom( );
		
		init_output_delay_eeprom( );

		init_table_custom_eeprom( );
		
		init_pwm_operation_eeprom( );               

		init_universal_db_eeprom( );

		write_eeprom(EEP_MODE1_NAME1, 'O');
		write_eeprom(EEP_MODE1_NAME1+1, 'f');
		write_eeprom(EEP_MODE1_NAME2, 'f');
		write_eeprom(EEP_MODE1_NAME2+1, '\0');
		write_eeprom(EEP_MODE1_NAME3, ' ');
		write_eeprom(EEP_MODE1_NAME3+1, ' ');
		write_eeprom(EEP_MODE1_NAME4, ' ');
		write_eeprom(EEP_MODE1_NAME4+1, ' ');
	

		write_eeprom(EEP_MODE2_NAME1, 'L');
		write_eeprom(EEP_MODE2_NAME1+1, 'o');
		write_eeprom(EEP_MODE2_NAME2, 'w');
		write_eeprom(EEP_MODE2_NAME2+1, '\0');
		write_eeprom(EEP_MODE2_NAME3, ' ');	
		write_eeprom(EEP_MODE2_NAME3+1, ' ');
		write_eeprom(EEP_MODE2_NAME4, ' ');
		write_eeprom(EEP_MODE2_NAME4+1, ' ');

		write_eeprom(EEP_MODE3_NAME1, 'M');
		write_eeprom(EEP_MODE3_NAME1+1, 'e');
		write_eeprom(EEP_MODE3_NAME2, 'd');
		write_eeprom(EEP_MODE3_NAME2+1, '\0');
		write_eeprom(EEP_MODE3_NAME3, ' ');
		write_eeprom(EEP_MODE3_NAME3+1, ' ');
		write_eeprom(EEP_MODE3_NAME4, ' ');
		write_eeprom(EEP_MODE3_NAME4+1, ' ');
		
		write_eeprom(EEP_MODE4_NAME1, 'H');
		write_eeprom(EEP_MODE4_NAME1+1, 'i');
		write_eeprom(EEP_MODE4_NAME2, '\0');
		write_eeprom(EEP_MODE4_NAME2+1, ' ');
		write_eeprom(EEP_MODE4_NAME3, ' ');	
		write_eeprom(EEP_MODE4_NAME3+1, ' ');
		write_eeprom(EEP_MODE4_NAME4, ' ');
		write_eeprom(EEP_MODE4_NAME4+1, ' ');
		
		write_eeprom(EEP_MODE5_NAME1, 'A');
		write_eeprom(EEP_MODE5_NAME1+1, 'u');
		write_eeprom(EEP_MODE5_NAME2, 't');
		write_eeprom(EEP_MODE5_NAME2+1, 'o');
		write_eeprom(EEP_MODE5_NAME3, '\0');
		write_eeprom(EEP_MODE5_NAME3+1, ' ');		
		write_eeprom(EEP_MODE5_NAME4, ' ');
		write_eeprom(EEP_MODE5_NAME4+1, ' ');
		
		write_eeprom(EEP_PROTOCOL_SEL, 0);		
		
		for(i=0;i<8;i++)
			{
				if(read_eeprom(EEP_CALIBRATION_ANALOG1 + 1 + i*2) == 0xff)
				{
					write_eeprom(EEP_CALIBRATION_ANALOG1+i*2,0);
					write_eeprom(EEP_CALIBRATION_ANALOG1 + 1+i*2,0);
				}
			}
			
		lcd_rotate_max = 1;
		write_eeprom(EEP_LCD_ROTATE_ENABLE,1);
		write_eeprom(EEP_DISP_ITEM_TEMPERATURE,14);
		disp_item_queue(0) = 14;	
			
	  if(UserInfo_Name(0) == 0xff)
		{
			UserInfo_Name(0) = 'T';
			UserInfo_Name(1) = 'S';
			UserInfo_Name(2) = 'T';
			UserInfo_Name(3) = 'A';
			UserInfo_Name(4) = 'T';
			#ifdef TSTAT7_ARM
			UserInfo_Name(5) = '7';
			#else
			UserInfo_Name(5) = '8';
			#endif
			UserInfo_Name(6) = 0X00;
			UserInfo_Name(7) = 0X00;	
			write_eeprom(EEP_TSTAT_NAME1,'T');
			write_eeprom(EEP_TSTAT_NAME1+1,'S');	
			write_eeprom(EEP_TSTAT_NAME1+2,'T');	
			write_eeprom(EEP_TSTAT_NAME1+3,'A');	
			write_eeprom(EEP_TSTAT_NAME1+4,'T');	
			#ifdef TSTAT7_ARM
			write_eeprom(EEP_TSTAT_NAME1+5,'7');
      #else
			write_eeprom(EEP_TSTAT_NAME1+5,'8');
			#endif
			write_eeprom(EEP_TSTAT_NAME1+6,0X00);	
			write_eeprom(EEP_TSTAT_NAME1+7,0X00);
			write_eeprom(EEP_TSTAT_NAME1+8,0X00);
			write_eeprom(EEP_TSTAT_NAME1+9,0X00);
			write_eeprom(EEP_TSTAT_NAME1+10,0X00);
			write_eeprom(EEP_TSTAT_NAME1+11,0X00);
			write_eeprom(EEP_TSTAT_NAME1+12,0X00);
			write_eeprom(EEP_TSTAT_NAME1+13,0X00);
			write_eeprom(EEP_TSTAT_NAME1+14,0X00);
			write_eeprom(EEP_TSTAT_NAME1+15,0X00);
			write_eeprom(EEP_TSTAT_NAME1+16,0X00);
			write_eeprom(EEP_TSTAT_NAME1+17,0X00);
			write_eeprom(EEP_TSTAT_NAME1+18,0X00);
			write_eeprom(EEP_TSTAT_NAME1+19,0X00);
		}	

   write_eeprom(EEP_SLEEP_SP , 200);		
	 write_eeprom(EEP_SLEEP_SP+1 , 0);
   write_eeprom(EEP_HOLIDAY_SP , 200);		
	 write_eeprom(EEP_HOLIDAY_SP+1 , 0);	
	 write_eeprom(EEP_MAX_WORK_SP, 40);
	 write_eeprom(EEP_MIN_WORK_SP, 16);
	 write_eeprom(EEP_MAX_SLEEP_SP, 30);
	 write_eeprom(EEP_MIN_SLEEP_SP, 15);		
	 write_eeprom(EEP_MAX_HOLIDAY_SP, 50);
	 write_eeprom(EEP_MIN_HOLIDAY_SP, 5);	

	write_eeprom(EEP_SCHEDULE_DHOME_HOUR, 6);				
	write_eeprom(EEP_SCHEDULE_DHOME_MIN, 30);				
	write_eeprom(EEP_SCHEDULE_WORK_HOUR, 8);
	write_eeprom(EEP_SCHEDULE_WORK_MIN, 30);		
	write_eeprom(EEP_SCHEDULE_NHOME_HOUR, 18);		
	write_eeprom(EEP_SCHEDULE_NHOME_MIN, 30);		
	write_eeprom(EEP_SCHEDULE_SLEEP_HOUR, 22);		
	write_eeprom(EEP_SCHEDULE_SLEEP_MIN, 30);	
//	#ifdef TSTAT_OCC	
//  write_eeprom(EEP_SCHEDULE_ON_OFF, 1);	
//	#else
	write_eeprom(EEP_SCHEDULE_ON_OFF, 0);
//	#endif
	if(Hardware_Info_HI == 0xff && Hardware_Info_LO == 0xff)
	{
		write_eeprom(EEP_HARDWARE_INFORMATION + 1, 0);		
		write_eeprom(EEP_HARDWARE_INFORMATION, 1);		
	}		
	#ifndef TSTAT8_HUNTER
	SetRTCTime(EEP_CLOCK_YEAR, 2017);	
	#endif
	
	#ifndef TSTAT7_ARM
	for(i=0;i<9;i++)
	{
		if(IconOutputControl(i) == 0xff)//EEPROM orignal data, need to be intitialized
		{
			IconOutputControl(i) = 0;
			write_eeprom(EEP_ICON_DAY_OUTPUT_CONTROL+i, 0);
		}
	}
	#endif //nTstat7	
	
}


void ConverteCF(void)
{
	uint8  i;
//	uint16 xdata unit_temp;
	uint16 xdata temp_i; 
#ifdef TSTAT7_ON
	uint16 xdata gsiPara;
#endif
							
	if(EEP_DEGCorF == 0)
		{
#ifdef TSTAT7_ON
		for(i=0;i<6;i++)
			{		
			gsiPara = (int16)(b.eeprom[EEP_WALL_SETPOINT + 1 + (i << 1)] << 8) + b.eeprom[EEP_WALL_SETPOINT + (i << 1)];
			gsiPara = gsiPara - 3200;
			gsiPara = gsiPara*5/9;
		
			b.eeprom[EEP_WALL_SETPOINT + 1 + (i << 1)] = (gsiPara >> 8) & 0xff;
			b.eeprom[EEP_WALL_SETPOINT + (i << 1)] = gsiPara & 0xff;
			write_eeprom(EEP_WALL_SETPOINT + 1 + (i << 1) ,b.eeprom[EEP_WALL_SETPOINT + 1 + (i << 1)]);
			write_eeprom(EEP_WALL_SETPOINT + (i << 1),b.eeprom[EEP_WALL_SETPOINT + (i << 1)]);		
			}
#endif
		//MINIMAL SETPOINT
		i = EEP_MinSetpoint - 32;
		EEP_MinSetpoint = (uint16)i*5/9;
		write_eeprom(EEP_MIN_SETPOINT,EEP_MinSetpoint);
	
		//MAXIMUM SETPOINT
		i = EEP_MaxSetpoint - 32;
		EEP_MaxSetpoint = (uint16)i*5/9;
		write_eeprom(EEP_MAX_SETPOINT,EEP_MaxSetpoint);
	
		//POWER-UP SETPOINT
		i = EEP_PowerupSetpoint - 32;
		EEP_PowerupSetpoint = (uint16)i*5/9;
		write_eeprom(EEP_POWERUP_SETPOINT,EEP_PowerupSetpoint);

		//DEFAULT SETPOINT
		i = EEP_DefaultSetpoint - 32;
		EEP_DefaultSetpoint = (uint16)i*5/9;
		write_eeprom(EEP_DEFAULT_SETPOINT,EEP_DefaultSetpoint);

		//DAY SETPOINT
		temp_i = (uint16 )(EEP_DaySpHi << 8) + EEP_DaySpLo;
		temp_i = temp_i - 320;
		temp_i = temp_i *5/9;				
		EEP_DaySpHi = temp_i >> 8;
		EEP_DaySpLo = temp_i & 0xff;
		write_eeprom(EEP_DAY_SETPOINT + 1,EEP_DaySpHi);
		write_eeprom(EEP_DAY_SETPOINT,EEP_DaySpLo);
	
		//DAY COOLING SETPOINT
		temp_i = (uint16 )(EEP_DayCoolingSpHi << 8) + EEP_DayCoolingSpLo;
		temp_i = temp_i - 320;
		temp_i = temp_i *5/9;
		EEP_DayCoolingSpHi = temp_i >> 8;
		EEP_DayCoolingSpLo = temp_i & 0xff;
		write_eeprom(EEP_DAY_COOLING_SETPOINT + 1,EEP_DayCoolingSpHi);
		write_eeprom(EEP_DAY_COOLING_SETPOINT,EEP_DayCoolingSpLo);

		//DAY_HEATING SETPOINT
		temp_i = (uint16 )(EEP_DayHeatingSpHi << 8) + EEP_DayHeatingSpLo;
		temp_i = temp_i - 320;
		temp_i = temp_i *5/9;
		EEP_DayHeatingSpHi = temp_i >> 8;
		EEP_DayHeatingSpLo = temp_i & 0xff;
		write_eeprom(EEP_DAY_HEATING_SETPOINT + 1,EEP_DayHeatingSpHi);
		write_eeprom(EEP_DAY_HEATING_SETPOINT,EEP_DayHeatingSpLo);

		//NIGHT SETPOINT
		temp_i = ((uint16)EEP_NightSpHi << 8 ) + EEP_NightSpLo - 320;	
		temp_i = temp_i *5/9;
		EEP_NightSpHi = (temp_i >> 8) & 0xff;
	    EEP_NightSpLo = temp_i & 0xff;
		write_eeprom(EEP_NIGHT_SETPOINT + 1,EEP_NightSpHi);
		write_eeprom(EEP_NIGHT_SETPOINT,EEP_NightSpLo);
	
		//NIGHT COOLING SETPOINT
		temp_i = ((uint16)EEP_NightCoolingSpHi << 8 ) + EEP_NightCoolingSpLo - 320;	
		temp_i = temp_i *5/9;
		EEP_NightCoolingSpHi = (temp_i >> 8) & 0xff;
	    EEP_NightCoolingSpLo = temp_i & 0xff;
		write_eeprom(EEP_NIGHT_COOLING_SETPOINT + 1,EEP_NightCoolingSpHi);
		write_eeprom(EEP_NIGHT_COOLING_SETPOINT,EEP_NightCoolingSpLo);
	
		//NIGHT HEATING SETPOINT
		temp_i = ((uint16)EEP_NightHeatingSpHi << 8 ) + EEP_NightHeatingSpLo - 320;	
		temp_i = temp_i *5/9;
		EEP_NightHeatingSpHi = (temp_i >> 8) & 0xff;
	  EEP_NightHeatingSpLo = temp_i & 0xff;
		write_eeprom(EEP_NIGHT_HEATING_SETPOINT + 1,EEP_NightHeatingSpHi);
		write_eeprom(EEP_NIGHT_HEATING_SETPOINT,EEP_NightHeatingSpLo);

//PID2 is universal PID, so should not translate this to degree F
//		//UNIVERSAL SETPOINT	
//		temp_i = ((uint16) b.eeprom[EEP_UNIVERSAL_SET + 1] << 8) + EEP_UniversalSetpointLo;
//		temp_i = temp_i - 320;
//		temp_i = temp_i *5/9;
//		EEP_UniversalSetpointHi = temp_i >> 8;
//		EEP_UniversalSetpointLo = temp_i & 0xff;
//		write_eeprom(EEP_UNIVERSAL_SET + 1,b.eeprom[EEP_UNIVERSAL_SET + 1]);
//		write_eeprom(EEP_UNIVERSAL_SET,EEP_UniversalSetpointLo);
//	
//		//UNIVERSAL NIGHT SETPOINT
//		temp_i = ((uint16 )b.eeprom[EEP_UNIVERSAL_NIGHTSET + 1] << 8) + EEP_UniversalNightSetpointLo;
//		temp_i = temp_i - 320;
//		temp_i = temp_i *5/9;
//		b.eeprom[EEP_UNIVERSAL_NIGHTSET + 1] = temp_i >> 8;
//		EEP_UniversalNightSetpointLo = temp_i & 0xff;
//		write_eeprom(EEP_UNIVERSAL_NIGHTSET + 1,b.eeprom[EEP_UNIVERSAL_NIGHTSET + 1]);
//		write_eeprom(EEP_UNIVERSAL_NIGHTSET,EEP_UniversalNightSetpointLo);
	
	
		}
	else
		{
		#ifdef TSTAT7_ON
		for(i=0;i<6;i++)
			{		
			gsiPara = (uint16)(b.eeprom[EEP_WALL_SETPOINT + 1 + (i << 1)] << 8) + b.eeprom[EEP_WALL_SETPOINT + (i << 1)];
			if(i == 0)
			ssmm[0] =  gsiPara;
				gsiPara = gsiPara*9/5;
			if(i == 0)
			ssmm[1] =  gsiPara;
				gsiPara = gsiPara + 3200;
			if(i == 0)
				ssmm[2] =  gsiPara;
			
			b.eeprom[EEP_WALL_SETPOINT + 1 + (i << 1)] = (gsiPara >> 8) & 0xff;
			b.eeprom[EEP_WALL_SETPOINT + (i << 1)] = gsiPara & 0xff;
			write_eeprom(EEP_WALL_SETPOINT + 1 + (i << 1) ,b.eeprom[EEP_WALL_SETPOINT + 1 + (i << 1)]);
			write_eeprom(EEP_WALL_SETPOINT + (i << 1),b.eeprom[EEP_WALL_SETPOINT + (i << 1)]);		
			}
		#endif
		
		i = (uint16)EEP_MinSetpoint *9/5;
		EEP_MinSetpoint = i + 32;
		write_eeprom(EEP_MIN_SETPOINT,EEP_MinSetpoint);
	
		i = (uint16)EEP_MaxSetpoint *9/5;
		EEP_MaxSetpoint = i + 32;
		write_eeprom(EEP_MAX_SETPOINT,EEP_MaxSetpoint);
		
		i = (uint16)EEP_PowerupSetpoint *9/5;
		EEP_PowerupSetpoint =i + 32;
		write_eeprom(EEP_POWERUP_SETPOINT,EEP_PowerupSetpoint);

		i = (uint16)EEP_DefaultSetpoint *9/5;
		EEP_DefaultSetpoint =i + 32;
		write_eeprom(EEP_DEFAULT_SETPOINT,EEP_DefaultSetpoint);

		temp_i = ((uint16)EEP_DaySpHi << 8) + EEP_DaySpLo;
		temp_i = temp_i*9/5;
		temp_i = temp_i + 320;
		EEP_DaySpHi = (temp_i >> 8) & 0xff;
		EEP_DaySpLo = temp_i & 0xff;
		write_eeprom(EEP_DAY_SETPOINT + 1,EEP_DaySpHi);
		write_eeprom(EEP_DAY_SETPOINT,EEP_DaySpLo);

		temp_i = ((uint16)EEP_DayCoolingSpHi << 8) + EEP_DayCoolingSpLo;
		temp_i = temp_i*9/5;
		temp_i = temp_i + 320;
		EEP_DayCoolingSpHi = (temp_i >> 8) & 0xff;
		EEP_DayCoolingSpLo = temp_i & 0xff;
		write_eeprom(EEP_DAY_COOLING_SETPOINT + 1,EEP_DayCoolingSpHi);
		write_eeprom(EEP_DAY_COOLING_SETPOINT,EEP_DayCoolingSpLo);

		temp_i = ((uint16)EEP_DayHeatingSpHi << 8) + EEP_DayHeatingSpLo;
		temp_i = temp_i*9/5;
		temp_i = temp_i + 320;
		EEP_DayHeatingSpHi = (temp_i >> 8) & 0xff;
		EEP_DayHeatingSpLo = temp_i & 0xff;
		write_eeprom(EEP_DAY_HEATING_SETPOINT + 1,EEP_DayHeatingSpHi);
		write_eeprom(EEP_DAY_HEATING_SETPOINT,EEP_DayHeatingSpLo);


		temp_i = ((uint16)EEP_NightSpHi<<8) + EEP_NightSpLo;
		temp_i = temp_i*9/5;
		temp_i = temp_i + 320;
		EEP_NightSpHi = (temp_i>>8) & 0xff;
		EEP_NightSpLo = temp_i & 0xff;
		write_eeprom(EEP_NIGHT_SETPOINT + 1,EEP_NightSpHi);
		write_eeprom(EEP_NIGHT_SETPOINT,EEP_NightSpLo);


        temp_i = ((uint16)EEP_NightCoolingSpHi<<8) + EEP_NightCoolingSpLo;
		temp_i = temp_i*9/5;
		temp_i = temp_i + 320;
		EEP_NightCoolingSpHi = (temp_i>>8) & 0xff;
		EEP_NightCoolingSpLo = temp_i & 0xff;
		write_eeprom(EEP_NIGHT_COOLING_SETPOINT + 1,EEP_NightCoolingSpHi);
		write_eeprom(EEP_NIGHT_COOLING_SETPOINT,EEP_NightCoolingSpLo);


        temp_i = ((uint16)EEP_NightHeatingSpHi<<8) + EEP_NightHeatingSpLo;
		temp_i = temp_i*9/5;
		temp_i = temp_i + 320;
		EEP_NightHeatingSpHi = (temp_i>>8) & 0xff;
		EEP_NightHeatingSpLo = temp_i & 0xff;
		write_eeprom(EEP_NIGHT_HEATING_SETPOINT + 1,EEP_NightHeatingSpHi);
		write_eeprom(EEP_NIGHT_HEATING_SETPOINT,EEP_NightHeatingSpLo);
	
//		temp_i = ((uint16)EEP_UniversalSetpointHi << 8) + EEP_UniversalSetpointLo;
//		temp_i = temp_i *9/5;
//		temp_i = temp_i + 320;
//		EEP_UniversalSetpointHi = (temp_i >> 8)& 0xff;
//		EEP_UniversalSetpointLo = temp_i & 0xff;
//		write_eeprom(EEP_UNIVERSAL_SET + 1,EEP_UniversalSetpointHi);
//		write_eeprom(EEP_UNIVERSAL_SET,EEP_UniversalSetpointLo);
//	
//		temp_i = ((uint16 )EEP_UniversalNightSetpointHi << 8) + EEP_UniversalNightSetpointLo;
//		temp_i = temp_i *9/5;
//		temp_i = temp_i + 320;
//		b.eeprom[EEP_UNIVERSAL_NIGHTSET + 1] = (temp_i >> 8) & 0xff;
//		EEP_UniversalNightSetpointLo = temp_i & 0xff;
//		write_eeprom(EEP_UNIVERSAL_NIGHTSET + 1,b.eeprom[EEP_UNIVERSAL_NIGHTSET + 1]);
//		write_eeprom(EEP_UNIVERSAL_NIGHTSET,EEP_UniversalNightSetpointLo);
		}
}



void refresh_setpoint(uint8 day_night)
{
if(day_night == DAY_MODE)
	{
	EEP_DaySpHi = read_eeprom(EEP_DAY_SETPOINT + 1);
	EEP_DaySpLo = read_eeprom(EEP_DAY_SETPOINT);
	EEP_DayCoolingDeadband = read_eeprom(EEP_DAY_COOLING_DEADBAND);		
	EEP_DayHeatingDeadband = read_eeprom(EEP_DAY_HEATING_DEADBAND);
	EEP_DayCoolingSpHi = read_eeprom(EEP_DAY_COOLING_SETPOINT + 1);		
	EEP_DayCoolingSpLo = read_eeprom(EEP_DAY_COOLING_SETPOINT);						
	EEP_DayHeatingSpHi = read_eeprom(EEP_DAY_HEATING_SETPOINT + 1);				
	EEP_DayHeatingSpLo = read_eeprom(EEP_DAY_HEATING_SETPOINT);		
	}
else
	{
 	EEP_NightSpHi = read_eeprom(EEP_NIGHT_SETPOINT + 1);                  
	EEP_NightSpLo = read_eeprom(EEP_NIGHT_SETPOINT);                 
	EEP_NightHeatingDeadband = read_eeprom(EEP_NIGHT_HEATING_DEADBAND);		
	EEP_NightCoolingDeadband = read_eeprom(EEP_NIGHT_COOLING_DEADBAND);
	EEP_NightCoolingSpHi = read_eeprom(EEP_NIGHT_COOLING_SETPOINT + 1);
	EEP_NightCoolingSpLo = read_eeprom(EEP_NIGHT_COOLING_SETPOINT);			    
	EEP_NightHeatingSpHi = read_eeprom(EEP_NIGHT_HEATING_SETPOINT + 1);		
	EEP_NightHeatingSpLo = read_eeprom(EEP_NIGHT_HEATING_SETPOINT);			    
	}								
}



void init_extern_operation_eeprom(uint8 sop_num)
{
	uint8 i;// temp_char;
	for(i = 0;i<42;i++)
	{
		switch(sop_num)
			{
			case FAN_COIL_COMMON_SEQUENCE: //fan coil common sequence
				write_eeprom(EEP_FAN0_OPER_TABLE_COAST + i, extern_operation_sop1[i] );//fill output table with old default table
				if(i==1)
				{	
				write_eeprom(EEP_HEAT_ORIGINAL_TABLE, 3 );//set pid stages to 3 heat + 3 cool 
				write_eeprom(EEP_COOL_ORIGINAL_TABLE, 3 );//set pid stages to 3 heat + 3 cool 				
				}
				if(i<7)//config outputs to PID
					write_eeprom(EEP_PID_OUTPUT1+i, 0);						
				write_eeprom(EEP_FAN_MODE, 1);
				EEP_FanMode = 1;	
			break;

			case CUSTOM_SEQUENCE: //custom squence.
					//write_eeprom(EEP_FAN0_OPER_TABLE_COAST + i, extern_operation_customer[i] );
				//write_eeprom(EEP_FAN_SPEED, 1);	
			break;

			case STANDARD_A_C_UNIT_SEQUENCE: //2heat 2cool
				write_eeprom(EEP_FAN0_OPER_TABLE_COAST + i, extern_operation_default[i]);
				if(i==1)
				{	
				write_eeprom(EEP_HEAT_ORIGINAL_TABLE, 2 );//set pid stages to 2 heat + 2 cool 
				write_eeprom(EEP_COOL_ORIGINAL_TABLE, 2 );//set pid stages to 2 heat + 2 cool 				
				}
				if(i<7)//config outputs to PID
					write_eeprom(EEP_PID_OUTPUT1+i, 0);						
				write_eeprom(EEP_FAN_SPEED, 4);
				write_eeprom(EEP_FAN_MODE, 3);
				EEP_FanMode = 3;		
			break;

			case HEAT_PUMP_SEQUENCE_FOR_HEATING://heat pump sequence,
				write_eeprom(EEP_FAN0_OPER_TABLE_COAST + i, extern_operation_sop4[i] );
				if(i==1)
				{	
				write_eeprom(EEP_HEAT_ORIGINAL_TABLE, 2 );//set pid stages to 2 heat + 2 cool 
				write_eeprom(EEP_COOL_ORIGINAL_TABLE, 2 );//set pid stages to 2 heat + 2 cool 				
				}
				if(i<7)//config outputs to PID
					write_eeprom(EEP_PID_OUTPUT1+i, 0);						
				write_eeprom(EEP_FAN_MODE, 1);
				EEP_FanMode = 1;	
			break;

			case HEAT_PUMP_SEQUENCE_FOR_COOLING://heat pump sequence with reversing valve on for cooling
				write_eeprom(EEP_FAN0_OPER_TABLE_COAST + i, extern_operation_sop5[i] );
				if(i==1)
				{	
				write_eeprom(EEP_HEAT_ORIGINAL_TABLE, 2 );//set pid stages to 2 heat + 2 cool 
				write_eeprom(EEP_COOL_ORIGINAL_TABLE, 2 );//set pid stages to 2 heat + 2 cool 				
				}
				if(i<7)//config outputs to PID
					write_eeprom(EEP_PID_OUTPUT1+i, 0);						
				write_eeprom(EEP_FAN_MODE, 1);
				EEP_FanMode = 1;	
			break;

			default:				
			break;				
			}
	}
}

void init_valve_operation_eeprom(void )
{
	uint8 col;// temp_char;
		for(col = 0; col < 21; col++)
		{
			write_eeprom(EEP_VALVE_OPER_TABLE_BEGIN + col,default_valve_table[ col ]);
		}            
}

void init_output_delay_eeprom()
{  
	uint8 i,k;// temp_char;
//        if(flash_or_eeprom)
//        {
		for(i=0;i<2;i++)
		{
			for(k=0;k<5;k++)
			{
				write_eeprom(EEP_OUTPUT_DELAY_OFF_TO_ON+i*5+k,OUTPUT_DELAY_TABLE[i][k]);
			}
		}
//	}
//	else
//	{
//		for(i=0; i<10; i++)
//		{
//			FlashRead_Absolute(DATA_FAC_START + EEP_OUTPUT_DELAY_OFF_TO_ON + i, &temp_char);
//			write_eeprom(EEP_OUTPUT_DELAY_OFF_TO_ON + i,temp_char) ;
//		}
//	}
}

//--------- init_table_custom_eeprom ----------------------
// init custom sensor look up table. and write to eeprom .

void init_table_custom_eeprom(void)
{
	uint8 i;// temp_char;
//	if(flash_or_eeprom)
//	{
		for(i = 0;i<21;i++,i++)
		{
			write_eeprom(EEP_TABLE1_ZERO + i,table1_custom_default[i/2] % 256 );	
		}
		for(i = 1;i<22;i++,i++)
		{
			write_eeprom(EEP_TABLE1_ZERO + i,table1_custom_default[i/2] / 256 );	
		}
	
	        for(i = 0;i<21;i++,i++)
		{
			write_eeprom(EEP_TABLE2_ZERO + i,table2_custom_default[i/2] % 256 );	
		}
		for(i = 1;i<22;i++,i++)
		{
			write_eeprom(EEP_TABLE2_ZERO + i,table2_custom_default[i/2] / 256 );	
		}
//	}
//	else
//	{
//		for(i=0; i<44; i++)
//		{
//			FlashRead_Absolute(DATA_FAC_START + EEP_TABLE1_ZERO + i, &temp_char);
//			write_eeprom(EEP_TABLE1_ZERO + i,temp_char) ;
//		}
//	}
}


//--------- init_pid_select_eeprom ----------------------
// init pid select table. and write to eeprom .
  
void init_universal_db_eeprom(void)
{	 
		write_eeprom(EEP_UNIVERSAL_HEAT_DB ,1);//0xa0
		write_eeprom(EEP_UNIVERSAL_HEAT_DB+1 ,0);
		write_eeprom(EEP_UNIVERSAL_COOL_DB ,1);//0xa0
		write_eeprom(EEP_UNIVERSAL_COOL_DB+1 ,0);

}

void init_pwm_operation_eeprom(void)
{
	uint8 xdata col;// temp_char;
		for(col = 0; col < 7; col++)
		{
			write_eeprom(EEP_OUTPUT_PWM_AUTO_BEGIN + col,default_pwm_table[ col ]);
		}        
}




#ifndef TSTAT7_ARM
int16 setpoint_buf = 0;
void increase_cooling_setpoint(void) 
{	
	if((schedule_on_off == SCHEDULE_OFF)&&(fan_speed_user == FAN_OFF))
	{
		setpoint_buf = EEP_NightSpHi * 256 + EEP_NightSpLo;
		setpoint_buf += EEP_SetpointIncrease;
		if(SetpointUNLimit == SPLIMIT)
			if( setpoint_buf	> (int16)EEP_MaxSetpoint*10) 
				setpoint_buf	= (int16)EEP_MaxSetpoint*10;
			loop_setpoint[0] = setpoint_buf;
		//write_eeprom(EEP_NIGHT_SETPOINT + 1, (setpoint_buf >> 8) & 0xff);
		//write_eeprom(EEP_NIGHT_SETPOINT, setpoint_buf & 0xff);
		//Set_night_setpoint(((setpoint_buf >> 8)&0xff), (setpoint_buf & 0xff));
		update_flag = 5;
			
	}
	else
	{
		setpoint_buf = EEP_DaySpHi * 256 + EEP_DaySpLo;//loop_setpoint[0];
		setpoint_buf += EEP_SetpointIncrease;
		if(SetpointUNLimit == SPLIMIT)
			if( setpoint_buf	> (int16)EEP_MaxSetpoint*10) 
				setpoint_buf	= (int16)EEP_MaxSetpoint*10;
	  loop_setpoint[0] = setpoint_buf;
		//write_eeprom(EEP_DAY_SETPOINT + 1, (setpoint_buf >> 8) & 0xff);
		//write_eeprom(EEP_DAY_SETPOINT, setpoint_buf & 0xff);
		//Set_day_setpoint(((setpoint_buf >> 8)&0xff), (setpoint_buf & 0xff));
		update_flag = 6;//refresh_setpoint(DAY_MODE);
	}		
		
	//init_PID_flag |= 0x01;
	
  blink_flag = BLINK_OFF;
  sp_blink_timer = 5;	
	icon.setpoint = 1;	
}


void decrease_cooling_setpoint(void) 
{
//	int16 setpoint_buf = 0;

	out6buf = EEP_PidOutput6;
	if((schedule_on_off == SCHEDULE_OFF)&&(fan_speed_user == FAN_OFF))
	{
		setpoint_buf = EEP_NightSpHi * 256 + EEP_NightSpLo;//loop_setpoint[0];
		setpoint_buf -= EEP_SetpointIncrease;
		if(SetpointUNLimit == SPLIMIT)
			if( setpoint_buf	< (int16)EEP_MinSetpoint*10) 
				setpoint_buf	= (int16)EEP_MinSetpoint*10;
		loop_setpoint[0] = setpoint_buf;
//		write_eeprom(EEP_NIGHT_SETPOINT + 1, (setpoint_buf >> 8) & 0xff);
//		write_eeprom(EEP_NIGHT_SETPOINT, setpoint_buf & 0xff);
//		Set_night_setpoint(((setpoint_buf >> 8)&0xff), (setpoint_buf & 0xff));
//		refresh_setpoint(NIGHT_MODE);	
		update_flag = 7;	
	if(out6buf != EEP_PidOutput6)	
			cooltest = 99;
	}
	else
	{
		setpoint_buf = EEP_DaySpHi * 256 + EEP_DaySpLo;//loop_setpoint[0];
		setpoint_buf -= EEP_SetpointIncrease;
		if(SetpointUNLimit == SPLIMIT)
			if( setpoint_buf	< (int16)EEP_MinSetpoint*10) 
				setpoint_buf	= (int16)EEP_MinSetpoint*10;
		loop_setpoint[0] = setpoint_buf;
//		write_eeprom(EEP_DAY_SETPOINT + 1, (setpoint_buf >> 8) & 0xff);
//		write_eeprom(EEP_DAY_SETPOINT, setpoint_buf & 0xff);
//		Set_day_setpoint(((setpoint_buf >> 8)&0xff), (setpoint_buf & 0xff));
//		refresh_setpoint(DAY_MODE);	
			update_flag = 8;
			if(out6buf != EEP_PidOutput6)	
			cooltest = 99;
	}	
//	init_PID_flag |= 0x01;	
	blink_flag = BLINK_OFF;
  sp_blink_timer = 5;
	icon.setpoint = 1;	
}


void accept_fan_setting(void)
{
	if(fanspeedbuf == 0)
	{
//		if(pre_fanspeed != FAN_OFF)
//		{
		info_byte &= 0xfe;
		occupied = 0;
		write_eeprom(EEP_INFO_BYTE,info_byte);
		override_timer = OFF;
		override_timer_time = 0;
		fan_speed_user = fanspeedbuf;
		EEP_FanSpeed = fanspeedbuf;
		if(schedule_on_off == SCHEDULE_OFF)
			write_eeprom(EEP_FAN_SPEED, fanspeedbuf);		
		pre_setpoint_select = 0;
		setpoint_select = NIGHT_SP;
//		}
	}
	else 
	{
//		if(override_timer == OFF)
//		{
			info_byte |= 0x01;
			occupied = 1;
			write_eeprom(EEP_INFO_BYTE,info_byte);//occupied mode unoccupied mode
			//override_timer = OFF;
		  override_timer_time = (uint32)60 * EEP_OverrideTimer;;
			fan_speed_user = fanspeedbuf;
			EEP_FanSpeed = fanspeedbuf;
			if(schedule_on_off == SCHEDULE_OFF)		
				write_eeprom(EEP_FAN_SPEED, fanspeedbuf);
//		}
		setpoint_select = DHOME_SP;
	}
	pre_fanspeed = fan_speed_user;
	icon.occ_unocc = 1;
	icon.setpoint = 1;
}

void increase_fan_speed(void)
{

//	fanspeedbuf = fan_speed_user;
//	if((fanspeedbuf == FAN_OFF)&&(faninflag == 0))
//	{
		faninflag = 1; 
//		fan_blink_timer = 4;
//		fanspeedbuf++;
//		//return;
//	}
//	else
		fanspeedbuf++;
	
	if(fanspeedbuf > FAN_AUTO)
		fanspeedbuf = FAN_OFF;
	
	if(fanspeedbuf > EEP_FanMode)
			fanspeedbuf = FAN_AUTO;
	
//	fan_speed_user = fanspeedbuf;
//	EEP_FanSpeed = fanspeedbuf;
//	
//  write_eeprom(EEP_FAN_SPEED, fanspeedbuf);
//	accept_fan_setting();
	blink_flag = BLINK_OFF;
  fan_blink_timer = 4;
	fan_flag = 1;
	display_fanspeed(fanspeedbuf);
	fan_flag = 0;
	icon.fan = 1;
	
	
}	

void increase_sysmode(void)
{
	icon.sysmode = 1;
	if(EEP_HeatCoolConfig == HC_CTL_USER)
	{
		heat_cool_user++;
		if((EEP_HEAT_TABLE1 == 0)&&(heat_cool_user == HC_CFG_HEAT))//no heating stage
			heat_cool_user = HC_CFG_AUTO;
		if((EEP_COOL_TABLE1 == 0)&&(heat_cool_user == HC_CFG_COOL))//no heating stage
			heat_cool_user = HC_CFG_HEAT;		
		if(heat_cool_user > HC_CFG_HEAT)
			heat_cool_user = HC_CFG_AUTO;
		write_eeprom(EEP_HC_USER, heat_cool_user);
	}
	blink_flag = BLINK_OFF;
  hc_blink_timer = 4;
}


void decrease_parameter(void ) 
{
clear_line(2);

	if(item_to_adjust == EEP_POWERUP_SETPOINT ||  item_to_adjust == EEP_MAX_SETPOINT) 	// add shi ,slo
	  	{
			if(item_to_adjust == EEP_POWERUP_SETPOINT)
				{
				if(read_eeprom(EEP_SETPOINT_UNLIMIT) == SPUNLIMIT)
				if(menubuffer.new_parameter > EEP_MinSetpoint && menubuffer.new_parameter > parameter_array[EEP_DEGCorF][ EEP_POWERUP_SETPOINT  - MAXEEPCONSTRANGE ][LOWER])
					{
					menubuffer.new_parameter-- ;
//					show_parameter() ;
					}
//				else
//					{
//					menubuffer.new_parameter-- ;
//					show_parameter() ;
//					}	
				}
			else  //item_to_adjust == EEP_MAX_SETPOINT
				{
				if(read_eeprom(EEP_SETPOINT_UNLIMIT) == SPUNLIMIT)
					{
					menubuffer.new_parameter--;
//					show_parameter() ;
					}

				else if(menubuffer.new_parameter > EEP_MinSetpoint + 1 && menubuffer.new_parameter > parameter_array[EEP_DEGCorF][ EEP_MAX_SETPOINT  - MAXEEPCONSTRANGE ][LOWER])
					{
					menubuffer.new_parameter--;
//					show_parameter() ;
					}
		
				}
	 		}
		 else if(item_to_adjust == EEP_NIGHT_HEATING_DEADBAND)
		  	  {
			  menubuffer.new_parameter-=10;
			  EEP_NightHeatingDeadband = menubuffer.new_parameter;
			  write_eeprom(item_to_adjust,EEP_NightHeatingDeadband);
//			  show_parameter() ;
			  }

		 else if(item_to_adjust == EEP_NIGHT_COOLING_DEADBAND)
		  	{
			  menubuffer.new_parameter-=10;
			  EEP_NightCoolingDeadband = menubuffer.new_parameter;
			  write_eeprom(item_to_adjust,EEP_NightCoolingDeadband);
//			  show_parameter();
			  }	
			else if(item_to_adjust == EEP_RS485_MODE)
			{
				if(menubuffer.new_parameter > 0)
					menubuffer.new_parameter--;
			}
	
		 else if(item_to_adjust == EEP_HEAT_COOL_CONFIG)
		 	{
			if (menubuffer.new_parameter >= 3)
				menubuffer.new_parameter--;
		
			else if (menubuffer.new_parameter == 2)
			 	{
				if (EEP_KeypadSelect == 3 || EEP_KeypadSelect <= 1)
					menubuffer.new_parameter = 0;	
				else 
					menubuffer.new_parameter = 1;
				}
		
			else if (menubuffer.new_parameter == 1 && (EEP_KeypadSelect == 3 || EEP_KeypadSelect <= 1))
				menubuffer.new_parameter--;
			//show_parameter() ;	
		 	}
		
			else if(item_to_adjust == MIN_MENU && menu_addi )
				{

				if (menubuffer.new_parameter > EEP_MinAddress)
					menubuffer.new_parameter--;
				else
					menubuffer.new_parameter = EEP_MaxAddress;

//				show_parameter() ;		
		
				}
	
			else if((item_to_adjust == EEP_CALIBRATION && !menu_addi) || item_to_adjust == EEP_CALIBRATION_EXTERNAL)
				  {
				  	menubuffer.new_parameter-- ;
	//				show_parameter() ;		
				  }	
//			else if(item_to_adjust == EEP_DIGITAL_IN1)
//				{
//				if(menubuffer.new_parameter != parameter_array[EEP_DEGCorF][ EEP_DIGITAL1_FUNCTION - MAXEEPCONSTRANGE ][LOWER])
//				menubuffer.new_parameter-- ;
//				show_parameter() ;
//				}
			else if(item_to_adjust == EEP_NIGHT_HEATING_SETPOINT)
				{
			  menubuffer.new_parameter--;// -= 10;
			  EEP_NightSpHi = (menubuffer.new_parameter >> 8) & 0xff;
			  EEP_NightSpLo = menubuffer.new_parameter & 0xff;
			  //show_parameter() ;
				}
			else if(item_to_adjust == EEP_BAUDRATE)
			{
				if(menubuffer.new_parameter == BAUDRATE_9600)
					menubuffer.new_parameter = BAUDRATE_4800;
				else if(menubuffer.new_parameter == BAUDRATE_4800)
					menubuffer.new_parameter = BAUDRATE_1200;
				else if(menubuffer.new_parameter == BAUDRATE_19200)
					menubuffer.new_parameter = BAUDRATE_14400;
				else if(menubuffer.new_parameter == BAUDRATE_14400)
					menubuffer.new_parameter = BAUDRATE_9600;
				else if(menubuffer.new_parameter == BAUDRATE_115200)
					menubuffer.new_parameter = BAUDRATE_76800;
				else if(menubuffer.new_parameter == BAUDRATE_76800)
					menubuffer.new_parameter = BAUDRATE_57600;
				else if(menubuffer.new_parameter == BAUDRATE_57600)
					menubuffer.new_parameter = BAUDRATE_38400;	
				else if(menubuffer.new_parameter == BAUDRATE_38400)
					menubuffer.new_parameter = BAUDRATE_19200;					
				else if(menubuffer.new_parameter == BAUDRATE_1200)
					menubuffer.new_parameter = BAUDRATE_115200;
				else
					menubuffer.new_parameter--;
			}	
				
	 	  else if( menubuffer.new_parameter != parameter_array[EEP_DEGCorF][ item_to_adjust - MAXEEPCONSTRANGE ][LOWER])
				{
				// if((item_to_adjust < EEP_CLOCK_YEAR)||(item_to_adjust > EEP_CLOCK_SECOND)) 
						menubuffer.new_parameter-- ;
				 // If the temp select is set to external, make sure AI1 does not get set to raw data
				 if(item_to_adjust == EEP_ANALOG1_RANGE && EEP_TempSelect == 1)
						menubuffer.new_parameter = 1 ;
				 if(item_to_adjust == EEP_TEMP_SELECT && menubuffer.new_parameter == 0 )
						menubuffer.new_parameter++;
				 //show_parameter() ;
	  		 }
	
		if(item_to_adjust==EEP_DAC_OFFSET)
			{
		  	EEP_DacOffset = menubuffer.new_parameter;
	  	}	
	
//		else
//		{
//		 	start_blink( ) ;		 //start the display blinking
//		}
//		#ifdef RTC8563
//		if((item_to_adjust>=EEP_CLOCK_YEAR)&&(item_to_adjust<=EEP_CLOCK_SECOND))
//			{
//			if( menubuffer.new_parameter != parameter_array[EEP_DEGCorF][ item_to_adjust - MAXEEPCONSTRANGE ][LOWER])
//				menubuffer.new_parameter-- ;
//			else
//				{
//				if((item_to_adjust == EEP_CLOCK_YEAR) && (menubuffer.new_parameter == 0))
//						menubuffer.new_parameter = 99;
//				else if((item_to_adjust == EEP_CLOCK_MONTH) && (menubuffer.new_parameter == 1))
//						menubuffer.new_parameter = 12;
//				else if((item_to_adjust == EEP_CLOCK_WEEK) && (menubuffer.new_parameter == 0))
//						menubuffer.new_parameter = 7;
//			    else if((item_to_adjust == EEP_CLOCK_DAY) && (menubuffer.new_parameter == 0))
//						menubuffer.new_parameter = 31;
//			   	else if(((item_to_adjust == EEP_CLOCK_HOUR) ||(item_to_adjust == EEP_SCHEDULE_WORK_DAYTIME_HOUR) ||(item_to_adjust == EEP_SCHEDULE_WORK_NIGHTTIME_HOUR)
//				||(item_to_adjust == EEP_SCHEDULE_WEEKEND_DAYTIME_HOUR) ||(item_to_adjust == EEP_SCHEDULE_WEEKEND_NIGHTTIME_HOUR)) && (menubuffer.new_parameter == 0))
//						menubuffer.new_parameter = 23;
//			   	else if(((item_to_adjust == EEP_CLOCK_MINUTE) ||(item_to_adjust == EEP_SCHEDULE_WORK_DAYTIME_MINUTE) ||(item_to_adjust == EEP_SCHEDULE_WORK_NIGHTTIME_MINUTE)
//				||(item_to_adjust == EEP_SCHEDULE_WEEKEND_DAYTIME_MINUTE) ||(item_to_adjust == EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE)) && (menubuffer.new_parameter == 0))
//						menubuffer.new_parameter = 59;
//			    else if((item_to_adjust == EEP_CLOCK_SECOND) && (menubuffer.new_parameter == 0))
//						menubuffer.new_parameter = 59;
//				}
			//show_parameter() ;
//			}		

//	if((item_to_adjust >= EEP_CLOCK_YEAR) && (item_to_adjust <= EEP_CLOCK_SECOND))
//		time_data[item_to_adjust - EEP_CLOCK_YEAR] = menubuffer.new_parameter;
//    if((item_to_adjust >= EEP_SCHEDULE_WORK_DAYTIME_HOUR) && (item_to_adjust <= EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE))
//			time_data[item_to_adjust - EEP_SCHEDULE_WORK_DAYTIME_HOUR] = menubuffer.new_parameter;
//	#endif
	}
	
	

void increase_parameter(void)
{
	clear_line(2);				

		  if(item_to_adjust == EEP_POWERUP_SETPOINT ||  item_to_adjust == EEP_MIN_SETPOINT) 	// add shi ,slo
		  	{
					if(read_eeprom(EEP_SETPOINT_UNLIMIT) == SPLIMIT)
						{
						if(item_to_adjust == EEP_POWERUP_SETPOINT)
							{
							if(menubuffer.new_parameter < EEP_MaxSetpoint && menubuffer.new_parameter < parameter_array[EEP_DEGCorF][ EEP_POWERUP_SETPOINT  - MAXEEPCONSTRANGE ][UPPER])
								{
								menubuffer.new_parameter++ ;
					//			show_parameter() ;
								}
							}
						else  //item_to_adjust == EEP_MIN_SETPOINT
							{
							if(menubuffer.new_parameter < EEP_MaxSetpoint-1 && menubuffer.new_parameter < parameter_array[EEP_DEGCorF][ EEP_MIN_SETPOINT  - MAXEEPCONSTRANGE ][UPPER])
								{
								menubuffer.new_parameter++ ;
					//			show_parameter() ;
								}
					
							}
						}
					else
						{
						menubuffer.new_parameter++ ;
						}
				
		  	}	
//		  else if((item_to_adjust == EEP_OUTPUT1_SCALE || item_to_adjust == EEP_OUTPUT2_SCALE ) && !analog_output_type)
//		  	{
//			show_parameter() ;		
//		  	}	
		
		  else if(item_to_adjust == MIN_MENU && menu_addi )
		  	{
			  if (menubuffer.new_parameter <EEP_MaxAddress)
					menubuffer.new_parameter++;

				else
					menubuffer.new_parameter = EEP_MinAddress;
		//	show_parameter() ;		
		  	}
		
		  else if(item_to_adjust == EEP_HEAT_COOL_CONFIG)
		 	{
				if (menubuffer.new_parameter == 0 && (EEP_KeypadSelect == 3 || EEP_KeypadSelect <= 1))
					menubuffer.new_parameter = 2;
		
				else if (menubuffer.new_parameter < parameter_array[EEP_DEGCorF][ item_to_adjust - MAXEEPCONSTRANGE ][UPPER])
					menubuffer.new_parameter++;
		
			//	show_parameter() ;	
		 	}
		  
		  else if(item_to_adjust == EEP_MENU_ITEM_BEGINNING && !menu_addi)
			  {
			  	menubuffer.new_parameter++ ;
			//	show_parameter() ;		
			  }	
				
		 else if(item_to_adjust == EEP_NIGHT_HEATING_DEADBAND)
		  	  {
			  menubuffer.new_parameter+=10;
			  EEP_NightHeatingDeadband = menubuffer.new_parameter;
			  write_eeprom(item_to_adjust,EEP_NightHeatingDeadband);
			//  show_parameter() ;
			  }

		 else if(item_to_adjust == EEP_NIGHT_COOLING_DEADBAND)
		  	  {
			  menubuffer.new_parameter+=10;
			  EEP_NightCoolingDeadband = menubuffer.new_parameter;
			  write_eeprom(item_to_adjust,EEP_NightCoolingDeadband);
			//  show_parameter() ;
			  }


		 else if(item_to_adjust == EEP_NIGHT_HEATING_SETPOINT)
		  	  {
			  menubuffer.new_parameter++;// += 10;
			  EEP_NightSpHi = (menubuffer.new_parameter >> 8) & 0xff;
			  EEP_NightSpLo = menubuffer.new_parameter & 0xff;
			//  show_parameter() ;
			  }
					
		 else if(item_to_adjust == EEP_RS485_MODE)
			{
				if(menubuffer.new_parameter < 2)
					menubuffer.new_parameter++;
			}
			
			else if(item_to_adjust == EEP_BAUDRATE)
			{
				if(menubuffer.new_parameter == BAUDRATE_1200)
					menubuffer.new_parameter = BAUDRATE_4800;
				else if(menubuffer.new_parameter == BAUDRATE_4800)
					menubuffer.new_parameter = BAUDRATE_9600;
				else if(menubuffer.new_parameter == BAUDRATE_9600)
					menubuffer.new_parameter = BAUDRATE_14400;
				else if(menubuffer.new_parameter == BAUDRATE_14400)
					menubuffer.new_parameter = BAUDRATE_19200;
				else if(menubuffer.new_parameter == BAUDRATE_19200)
					menubuffer.new_parameter = BAUDRATE_38400;
				else if(menubuffer.new_parameter == BAUDRATE_38400)
					menubuffer.new_parameter = BAUDRATE_57600;
				else if(menubuffer.new_parameter == BAUDRATE_57600)
					menubuffer.new_parameter = BAUDRATE_76800;
				else if(menubuffer.new_parameter == BAUDRATE_76800)
					menubuffer.new_parameter = BAUDRATE_115200;
				else if(menubuffer.new_parameter == BAUDRATE_115200)
					menubuffer.new_parameter = BAUDRATE_1200;
			}

		 else
		 {
		 if( menubuffer.new_parameter != parameter_array[EEP_DEGCorF][ item_to_adjust - MAXEEPCONSTRANGE ][UPPER]	)
			  {
						menubuffer.new_parameter++ ;
			  }
		
			  if(item_to_adjust==EEP_DAC_OFFSET)
				  {
					  EEP_DacOffset = menubuffer.new_parameter;
				  }	 
			}
//			if((item_to_adjust >= EEP_SCHEDULE_WORK_DAYTIME_HOUR) && (item_to_adjust <= EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE))
//				time_data[item_to_adjust - EEP_SCHEDULE_WORK_DAYTIME_HOUR] = menubuffer.new_parameter;

		//	show_parameter() ;
}	



void accept_parameter(void)
{
// clear_line(1);
// clear_line(2);
	
	if( item_to_adjust == EEP_FACTORY_DEFAULTS )
	  {
		if(menubuffer.new_parameter%2==0)
		  {
				//TBD 
		  update_flag = 2;
			//defaults_pending = YES;
// 			one_second_pause();
//			while(1) ;
    	} 		
	   }  //endif factory defualts
//else if( item_to_adjust == EEP_STORE_DEFAULTS )
//	  {
//		if(menubuffer.new_parameter%2==0)
//			{
//			save_user_def();			
//			DisRestart( );
//    	} 		
//	  }   
		else if( item_to_adjust == EEP_DAC_OFFSET )
		{
			write_eeprom( EEP_DAC_OFFSET , EEP_DacOffset);
			EEP_DacOffset = menubuffer.new_parameter;
		}
     
	 else if(item_to_adjust == EEP_MENU_ITEM_BEGINNING && !menu_addi)
	    {
				if ((EEP_TempSelect == THERMISTER_TYPE2)||(EEP_TempSelect == THERMISTER_TYPE3))
				{
					internal_calibration = menubuffer.new_parameter - temperature_internal;
					write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR , internal_calibration&0xFF ) ;
					write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR + 1, internal_calibration>>8 ) ;
					Calibration_Internal_LO = internal_calibration&0xFF ;
					Calibration_Internal_HI = internal_calibration>>8; 				
				}
	    }
	else if(item_to_adjust == MIN_MENU && menu_addi)
		{//allow user change ID by keypads, do not lock
				write_eeprom(EEP_ADDRESS , menubuffer.new_parameter) ;
			  laddress =  menubuffer.new_parameter ;
				flash_buf[0] = laddress;	
				STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);
				Station_NUM = laddress;
		}

		

		else if(item_to_adjust == EEP_DEMAND_RESPONSE)
			{
			if(menubuffer.new_parameter%2)	
				{
	  		write_eeprom(EEP_DEMAND_RESPONSE, 1);
				demand_response = 1;
				}
			else 
				{
	  		write_eeprom(EEP_DEMAND_RESPONSE, 2);
				demand_response = 2;
				}  
			}
	  //-------------------------------------------------------------

//		else if((item_to_adjust >= EEP_CLOCK_YEAR) && (item_to_adjust <= EEP_CLOCK_SECOND))	
//			 SetRTCTime(item_to_adjust, menubuffer.new_parameter);
//			

//		else if((item_to_adjust >= EEP_SCHEDULE_WORK_DAYTIME_HOUR) && (item_to_adjust <= EEP_SCHEDULE_WORK_NIGHTTIME_MINUTE))
//			{
//			 write_eeprom((EEP_DAY1_EVENT1_HI + (item_to_adjust-EEP_SCHEDULE_WORK_DAYTIME_HOUR)),(menubuffer.new_parameter & 0xff));
//			}
//		else if((item_to_adjust >= EEP_SCHEDULE_WEEKEND_DAYTIME_HOUR) && (item_to_adjust <= EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE))
//			{
//			 write_eeprom((EEP_DAY2_EVENT1_HI + (item_to_adjust-EEP_SCHEDULE_WEEKEND_DAYTIME_HOUR)),(menubuffer.new_parameter & 0xff));
//			}

	  //---------------------------------------------------------------
	  else if(item_to_adjust == EEP_DAY_COOLING_SETPOINT)
	  	{
		  //menubuffer.new_parameter *=10;
		  Set_day_setpoint((((menubuffer.new_parameter *10)>> 8)&0xff),((menubuffer.new_parameter*10)&0xff));
		  refresh_setpoint(DAY_MODE);
			}
	  
		else if(item_to_adjust == EEP_FAN_SPEED)
			{
			fan_speed_user = menubuffer.new_parameter;
			EEP_FanSpeed = fan_speed_user;	
			fanspeedbuf = fan_speed_user;
			write_eeprom(EEP_FAN_SPEED,fan_speed_user);
			icon.fan = 1;
			icon.occ_unocc = 1;
			}			
   	else if(item_to_adjust == EEP_NIGHT_HEATING_SETPOINT)
	    {
		  menubuffer.new_parameter *=10;
		  Set_night_setpoint(((menubuffer.new_parameter >> 8)&0xff),(menubuffer.new_parameter&0xff));
		  refresh_setpoint(NIGHT_MODE);
			}	

	  if( b.eeprom[item_to_adjust] !=menubuffer.new_parameter )
		{
			b.eeprom[item_to_adjust] = menubuffer.new_parameter;

			if(item_to_adjust != EEP_MENU_ITEM_BEGINNING)
				write_eeprom( item_to_adjust , menubuffer.new_parameter	) ;
	
			// handle exception for	EEP_SEQUENCE
			//TBD: Winter: need to check for YES/NO here and only reset if YES was selected
			if( item_to_adjust == EEP_SEQUENCE )     //sequence of operations setting
				{
					write_eeprom( item_to_adjust , menubuffer.new_parameter	) ;
					init_extern_operation_eeprom(menubuffer.new_parameter);
					DisRestart();
//    				one_second_pause();
//					while(1) ;
 				}

			if( item_to_adjust == EEP_DEGC_OR_F )     // 
				{
				EEP_DEGCorF = menubuffer.new_parameter;
				write_eeprom(EEP_DEGC_OR_F, EEP_DEGCorF);
				icon.unit = 1;	
				ConverteCF( );
 				}
			if( item_to_adjust == EEP_KEYPAD_SELECT )     // 
				{
					DisRestart( );
					one_second_pause();
				//	while(1) ;
				}
			if(item_to_adjust == EEP_AUTO_ONLY )
			{
//				fan_speed_user = FAN_OFF ;
////mhf 11-11-05 if fan_speed_user == 0,it means the tstat is working in unoccupied mode
//				info_byte &= 0xfe;
//				occupied = 0;
//				write_eeprom(EEP_INFO_BYTE,info_byte);
//				override_timer = OFF;
//				override_timer_time = 0;
//			//	if(b.eeprom[EEP_FAN_SPEED -  MAXEEPCONSTRANGE] != 0)
//				backup_fan_speed = 	EEP_FanSpeed;
//				EEP_FanSpeed = FAN_OFF ;
//				write_eeprom(EEP_FAN_SPEED,EEP_FanSpeed);
			}

			// If changing the temperature select to external sensor, make sure AI1 is set to 10K thermistor curve
			//cc 01-13-2015 since we have extend the pid1 to universal pid loop, then AI1 could be any range, so do not need do this
			if(item_to_adjust == EEP_TEMP_SELECT)
			{
				EEP_TempSelect = menubuffer.new_parameter;
				write_eeprom(EEP_TEMP_SELECT,EEP_TempSelect);
			}		

			if( item_to_adjust == EEP_HEAT_COOL_CONFIG )     
			{	

				// When setting HC to an analog input mode, make sure the corresponding AI is set to 1								
				if (menubuffer.new_parameter == HC_ANALOG_IN1 && EEP_AnalogIn1 != 1)
				{
					EEP_AnalogIn1 = 1;
					write_eeprom(EEP_ANALOG1_RANGE,b.eeprom[EEP_ANALOG1_RANGE]);
				}
					
				// When setting HC to an analog input mode, make sure the corresponding AI is set to 1								
				if (menubuffer.new_parameter == HC_ANALOG_IN2 && EEP_AnalogIn2 != 1)
				{
					EEP_AnalogIn2 = 1;
					write_eeprom(EEP_ANALOG2_RANGE,b.eeprom[EEP_ANALOG_IN2]);
				}
			}
		
			if(item_to_adjust == EEP_BAUDRATE)
				{
				
				write_eeprom(EEP_BAUDRATE, menubuffer.new_parameter);			
				if(menubuffer.new_parameter == BAUDRATE_9600)
				{
					EEP_Baudrate = BAUDRATE_9600;
					uart1_init(9600);
					modbus.baudrate = 9600;
				}
				else if(menubuffer.new_parameter == BAUDRATE_19200)
				{
					EEP_Baudrate = BAUDRATE_19200;
					uart1_init(19200);
					modbus.baudrate = 19200;					
				}
				else if(menubuffer.new_parameter == BAUDRATE_38400)
				{
					EEP_Baudrate = BAUDRATE_38400;
					uart1_init(38400);
					modbus.baudrate = 38400;					
				}
				else if(menubuffer.new_parameter == BAUDRATE_57600)
				{
					EEP_Baudrate = BAUDRATE_57600;
					uart1_init(57600);
					modbus.baudrate = 57600;					
				}
				else if(menubuffer.new_parameter == BAUDRATE_115200)
				{
					EEP_Baudrate = BAUDRATE_115200;
					uart1_init(115200);
					modbus.baudrate = 115200;					
				}
				
				else if(menubuffer.new_parameter == BAUDRATE_76800)
				{
					EEP_Baudrate = BAUDRATE_76800;
					uart1_init(76800);
					modbus.baudrate = 76800;					
				}		

				else if(menubuffer.new_parameter == BAUDRATE_1200)
				{
					EEP_Baudrate = BAUDRATE_1200;
					uart1_init(1200);
					modbus.baudrate = 1200;					
				}	

				else if(menubuffer.new_parameter == BAUDRATE_4800)
				{
					EEP_Baudrate = BAUDRATE_4800;
					uart1_init(4800);
					modbus.baudrate = 4800;					
				}	

				else if(menubuffer.new_parameter == BAUDRATE_14400)
				{
					EEP_Baudrate = BAUDRATE_14400;
					uart1_init(14400);
					modbus.baudrate = 14400;					
				}					
				serial_restart();
//				else
//				{
//					EEP_Baudrate = BAUDRATE_115200;
//					uart1_init(115200);
//					modbus.baudrate = 115200;			
//				}				
//				write_eeprom(EEP_BAUDRATE, menubuffer.new_parameter);
				//DisRestart( );
				}


       if(item_to_adjust == EEP_DEMAND_RESPONSE)
	      	{
					if(menubuffer.new_parameter%2)	
						{
							write_eeprom(EEP_DEMAND_RESPONSE, 1);
						demand_response = 1;
						}
					else
						{
							write_eeprom(EEP_DEMAND_RESPONSE, 2);
						demand_response = 2;
						}  
					}


		 if(item_to_adjust == EEP_DAY_COOLING_DEADBAND || item_to_adjust == EEP_DAY_HEATING_DEADBAND)
		 {
		  if( item_to_adjust == EEP_DAY_COOLING_DEADBAND)
			{
				if(menubuffer.new_parameter >= EEP_DayCoolingDeadband * 10)
				{
					EEP_DayCoolingDeadband = menubuffer.new_parameter / 10 + 1;
					write_eeprom( EEP_DAY_COOLING_DEADBAND , EEP_DayCoolingDeadband);
					change_setpoint_deadband(DAY_COOLING_DB, EEP_DayCoolingDeadband);					
				}
			}
			else
			{
				if(menubuffer.new_parameter  >= EEP_DayHeatingDeadband * 10)
				{
					EEP_DayHeatingDeadband = menubuffer.new_parameter / 10 + 1;
					write_eeprom( EEP_DAY_HEATING_DEADBAND , EEP_DayHeatingDeadband);
					change_setpoint_deadband(DAY_HEATING_DB, EEP_DayHeatingDeadband);
				}
			}
			refresh_setpoint(DAY_MODE);	
			init_PID_flag |= 0x01 ; //FUTURE: need to reset the	PID controllers	at this point (reset =1)
			init_delay_flag = 0 ;
		 }
		if(item_to_adjust == EEP_FILTER)
		{
			EEP_Filter = menubuffer.new_parameter;
			write_eeprom(EEP_FILTER,EEP_Filter);
		}
		
		if(item_to_adjust == EEP_FAN_MODE)
		{
			if(fan_speed_user > EEP_FanMode)
			{
				fan_speed_user = FAN_AUTO;
				EEP_FanSpeed = FAN_AUTO;
				write_eeprom(EEP_FAN_SPEED, EEP_FanSpeed);
			}
		}
		
			if(item_to_adjust == EEP_PROTOCOL_SEL)
				{
						if((menubuffer.new_parameter%2) == 1)//bacnet protocol
						{
							modbus.com_config[0] = BAC_MSTP;
							Recievebuf_Initialize(0);
							write_eeprom(EEP_PROTOCOL_SEL,1);					
							write_eeprom(EEP_RS485_MODE,0);
						}
						else //modbus protocol
						{
							modbus.com_config[0] = MODBUS;
							write_eeprom(EEP_PROTOCOL_SEL,0);
							update_flag = 4;
						}
						//write_eeprom(EEP_INFO_BYTE,info_byte);

				}
			if(item_to_adjust == EEP_RS485_MODE)
			{
				if(menubuffer.new_parameter == 1)//zigbee mode
				{
					write_eeprom(EEP_RS485_MODE,1);
					RS485_Mode = 1;
				}
				else if(menubuffer.new_parameter == 2)//auto mode
				{
					write_eeprom(EEP_RS485_MODE,2);
					RS485_Mode = 2;
				}
				else
				{
					write_eeprom(EEP_RS485_MODE,0);
					RS485_Mode = 0;
				}
				update_flag = 9;
			}		
		
			
		}  //endif menubuffer.new_parameter  

	

}	
void start_menu_mode(void)
{  //start_timer( KEYPAD_ACTIVE_TIMER ,	MENU_TIMEOUT ) ;
  if(part_lock)
    {
		item_to_adjust = EEP_MENU_ITEM_BEGINNING;
		if(EEP_SpecialMenuLock == 3)
			item_to_adjust = EEP_DAY_COOLING_SETPOINT;
		menu_addi = 0;                               // sets the menu to display the Address parameter
		}
	else
		{
		item_to_adjust = MIN_MENU;
		menu_addi = 1; 
		}
	start_menu();

}	

void start_next_menu(void)
{
	clear_line(1);
	clear_line(2);
   	if(item_to_adjust == MAX_MENU && part_lock == 0)
			{
			item_to_adjust = MIN_MENU;
			menu_addi = 1;
			}
  	else if( part_lock == 1 )
			{		
			if(EEP_SpecialMenuLock == 3)
				{
				if(item_to_adjust == EEP_DAY_COOLING_SETPOINT)
					item_to_adjust = EEP_FAN_SPEED;
				else if(item_to_adjust == EEP_FAN_SPEED)
					item_to_adjust = EEP_DAY_COOLING_SETPOINT;//EEP_MENU_ITEM_BEGINNING;
//				else if(item_to_adjust == EEP_MENU_ITEM_BEGINNING)
//					item_to_adjust = EEP_DAY_COOLING_SETPOINT;
//				else if(item_to_adjust == EEP_OVERRIDE_TIMER)
//					item_to_adjust = EEP_DAY_COOLING_SETPOINT;
				}
			else
				item_to_adjust = EEP_OVERRIDE_TIMER;//MHF 02-17-06 add overide mode to part lock menu
			menu_addi = 0 ;
			}  	
  	else if(item_to_adjust ==MIN_MENU && menu_addi)
  		menu_addi = 0 ;   
  	else
		{
		if(EEP_SpecialMenuLock == 4)
			{
			if(item_to_adjust == EEP_MENU_ITEM_BEGINNING)
				item_to_adjust += 11;
			else if(item_to_adjust >= EEP_COOLING_PTERM && item_to_adjust < EEP_SEQUENCE)
				item_to_adjust++ ;
			else if(item_to_adjust == EEP_SEQUENCE)
				 item_to_adjust +=2 ;
			else if(item_to_adjust >= EEP_DAY_COOLING_DEADBAND && item_to_adjust < EEP_DEGC_OR_F)
				item_to_adjust++ ;

      else if((item_to_adjust == EEP_RS485_MODE)&&(demand_response == 0))
        item_to_adjust = item_to_adjust + 2; 
//			else if((item_to_adjust == EEP_VALVE_TRAVEL_TIME)&&(demand_response == 0))
//				item_to_adjust = item_to_adjust + 3; 

//			else if(item_to_adjust == EEP_PROTOCOL_SE)
//			{
//			
//			}
			
			else if(item_to_adjust == EEP_DEGC_OR_F)
				item_to_adjust = MAX_MENU;	
			}
		else
			{
				
			if(item_to_adjust == EEP_FILTER)
				item_to_adjust = item_to_adjust + 3;
			else if(item_to_adjust == EEP_RS485_MODE)
			{
			if(DemandResponse == 0)
				item_to_adjust = EEP_PROTOCOL_SEL;//EEP_STORE_DEFAULTS;
			else
				item_to_adjust++ ;	
			}

			else if(item_to_adjust == EEP_DEMAND_RESPONSE)
				item_to_adjust = EEP_PROTOCOL_SEL;//EEP_STORE_DEFAULTS; 		
			else if(item_to_adjust == EEP_NIGHT_HEATING_SETPOINT)
				item_to_adjust = EEP_APPLICATION;
			else if(item_to_adjust == EEP_PROTOCOL_SEL)
				item_to_adjust = EEP_STORE_DEFAULTS;
			else
				item_to_adjust++ ;
		}
	}
  	start_menu() ;
}


void start_previous_menu(void)
{
	clear_line(1);
	clear_line(2);
  //	start_menu() ;
  	if(part_lock)
		{			        
			if(EEP_SpecialMenuLock == 3)
			{
				if(item_to_adjust == EEP_DAY_COOLING_SETPOINT)
					item_to_adjust = EEP_FAN_SPEED;
				else if(item_to_adjust == EEP_FAN_SPEED)
					item_to_adjust = EEP_DAY_COOLING_SETPOINT;
//				else if(item_to_adjust == EEP_MENU_ITEM_BEGINNING)
//				item_to_adjust = EEP_FAN_SPEED;
//				else if(item_to_adjust == EEP_FACTORY_DEFAULTS)
//				item_to_adjust = EEP_FAN_SPEED;
			}
			else
		 	item_to_adjust = EEP_MENU_ITEM_BEGINNING ;			           
    }
    else //no partlock
    {
      if( item_to_adjust == MIN_MENU && menu_addi )
			{
				item_to_adjust = MAX_MENU ;
				menu_addi = 0;
			}
	   	else if( item_to_adjust == MIN_MENU && menu_addi == 0)
	   		menu_addi = 1;
	   	else
			{
				if(EEP_SpecialMenuLock == 4)
				{
					if(item_to_adjust == EEP_COOLING_PTERM)
					{
						item_to_adjust = EEP_MENU_ITEM_BEGINNING;
						menu_addi = 0 ;
					}
					else if(item_to_adjust > EEP_COOLING_PTERM && item_to_adjust <= EEP_SEQUENCE)
						item_to_adjust-- ;
					else if(item_to_adjust == EEP_DAY_COOLING_DEADBAND)
						 item_to_adjust -=2 ;
					else if(item_to_adjust > EEP_DAY_COOLING_DEADBAND && item_to_adjust <= EEP_DEGC_OR_F)
						item_to_adjust-- ;
					else if(item_to_adjust == MAX_MENU)
						item_to_adjust = EEP_DEGC_OR_F;
					else if(item_to_adjust == EEP_PROTOCOL_SEL/*EEP_STORE_DEFAULTS*/)
					{
						if(demand_response == 0)
							item_to_adjust = EEP_RS485_MODE;
						else
							item_to_adjust = EEP_DEMAND_RESPONSE;
					}
				}
				else
				{
					if(item_to_adjust == EEP_OVERRIDE_TIMER)
						item_to_adjust = item_to_adjust - 3;
					else if(item_to_adjust == EEP_PROTOCOL_SEL/*EEP_STORE_DEFAULTS*/) 
						{
							if(DemandResponse == 1)
								item_to_adjust = EEP_DEMAND_RESPONSE;
							else
								item_to_adjust = EEP_RS485_MODE;								
						}	

//					else if(item_to_adjust == EEP_CLOCK_YEAR)
//						{
//						if(read_eeprom(EEP_DEMAND_RESPONSE) == 0)
//							//item_to_adjust = EEP_VALVE_TRAVEL_TIME;
//							item_to_adjust = EEP_RS485_MODE;						
//						else
//							item_to_adjust = EEP_DEMAND_RESPONSE;
//						}

					else if(item_to_adjust == EEP_APPLICATION)
						item_to_adjust = EEP_NIGHT_COOLING_SETPOINT;				
					else
						item_to_adjust-- ;			
				}
		}
	}
  	start_menu();
}	
void show_parameter(void)
{
  switch( item_to_adjust )
  {	 
	case EEP_DEMAND_RESPONSE:

//		Display_menu(0x82,"DEMAND");			
//		if (menubuffer.new_parameter%2)
//			DisplayLine2(0xC3,"NO");  
//		else
//			DisplayLine2(0xC3,"YES");			
	break;


	case EEP_MENU_ITEM_BEGINNING :	   
		if(menu_addi)
			{	  					 
			display_value(0,menubuffer.new_parameter, ' ');	 
			}
		else
			{
			if(EEP_DEGCorF)						
				display_value(0, menubuffer.new_parameter, 'F');
			else
				display_value(0, menubuffer.new_parameter, 'C');	
			}
	  break	;

	case EEP_DEGC_OR_F :
		if (menubuffer.new_parameter)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"-F- ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"-C- ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);

		break;
			

// MDF 6/30/04 Added menu items, night heating setpoint, night cooling setpoint.

	case EEP_NIGHT_HEATING_SETPOINT:
	case EEP_NIGHT_COOLING_SETPOINT:
	case EEP_MAX_SETPOINT:
	case EEP_MIN_SETPOINT:
	case EEP_POWERUP_ON_OFF :
	case EEP_POWERUP_SETPOINT:  
	case EEP_TEMP_SELECT : 
//	case EEP_ANALOG_IN1:
//	case EEP_ANALOG_IN2 : 
  case EEP_ANALOG1_RANGE:
	case EEP_ANALOG2_RANGE:
	case EEP_OVERRIDE_TIMER : 
	case EEP_APPLICATION :
	case EEP_FAN_MODE :
	case EEP_AUTO_ONLY :
	case EEP_DISPLAY:
	case EEP_CYCLING_DELAY:
	case EEP_CHANGOVER_DELAY:
	case EEP_OUTPUT1_SCALE:
	case EEP_OUTPUT2_SCALE:
	case EEP_FILTER:
	case EEP_HEAT_COOL_CONFIG:
	case EEP_SEQUENCE :   //sequence of operations setting
	  display_value(0,menubuffer.new_parameter, ' ')	;
	  break	;

	case EEP_NIGHT_HEATING_DEADBAND:
	case EEP_NIGHT_COOLING_DEADBAND:
		display_value(0,menubuffer.new_parameter/10, ' ');
	  break	;

	case EEP_KEYPAD_SELECT:
		disp_str(FORM15X30, 0,MENU_VALUE_POS,"4A",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);	
	   	break ;

	case EEP_FACTORY_DEFAULTS :  
	case EEP_STORE_DEFAULTS : 

//		if(item_to_adjust == EEP_FACTORY_DEFAULTS )
//			//DisplayLine1(0x81,"FAC DEF");
//			disp_str(FORM15X30, 60,290,"FAC DEF",0,0xffff);
//		else
//			disp_str(FORM15X30, 67,290,"SAVE DEF",0,0xffff);
//			//DisplayLine1(0x81,"SAVE DEF");
		if(menubuffer.new_parameter%2)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"NO",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);			 
				//DisplayLine2(0xC4,"NO");  
		else
			 disp_str(FORM15X30, 0,MENU_VALUE_POS,"YES",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);	
				//DisplayLine2(0xC3,"YES");  
	
	  break	;

	case EEP_DAC_OFFSET:
		disp_str(FORM15X30, 0,MENU_VALUE_POS,"100  ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
//		DisplayLine1(0x81,"CAL DAC");
//	 	display(  100 ) ;
	  break ;
	
	case EEP_FAN_SPEED:
		switch(menubuffer.new_parameter)
		{
			case 0:
				disp_str(FORM15X30, 0,MENU_VALUE_POS,"Off",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			break;
			case 1:
				disp_str(FORM15X30, 0,MENU_VALUE_POS,"Mode1",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			break;
			case 2:
				disp_str(FORM15X30, 0,MENU_VALUE_POS,"Mode2",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			break;
			case 3:
				disp_str(FORM15X30, 0,MENU_VALUE_POS,"Mode3",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			break;
			case 4:
				disp_str(FORM15X30, 0,MENU_VALUE_POS,"Auto",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			break;
		}
			
	break;
	
	case EEP_RS485_MODE:
		  if(menubuffer.new_parameter == 1)
				 disp_str(FORM15X30, 0,MENU_VALUE_POS,"ZIGBEE",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			else if(menubuffer.new_parameter == 2)
				 disp_str(FORM15X30, 0,MENU_VALUE_POS," AUTO",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			else
				 disp_str(FORM15X30, 0,MENU_VALUE_POS,"RS485",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		break;

	case EEP_PROTOCOL_SEL:
		  if((menubuffer.new_parameter%2) == 1)
				 disp_str(FORM15X30, 0,MENU_VALUE_POS,"BACNET",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
			else
				 disp_str(FORM15X30, 0,MENU_VALUE_POS,"MODBUS",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		break;
			
	case EEP_SPECIAL_MENU_LOCK:
		display_value(90,menubuffer.new_parameter, ' ')	;
		//display_integer(menubuffer.new_parameter);
	break;
	case EEP_BAUDRATE:
		if(menubuffer.new_parameter == BAUDRATE_9600)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"9600  ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_19200)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"19200 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_38400)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"38400 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_57600)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"57600 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_115200)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"115200 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_76800)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"76800 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_1200)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"1200 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_4800)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"4800 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		else if(menubuffer.new_parameter == BAUDRATE_14400)
			disp_str(FORM15X30, 0,MENU_VALUE_POS,"14400 ",TSTAT8_CH_COLOR,TSTAT8_BACK_COLOR);
		
	break;
 
  	default	:
	  display_value(0,menubuffer.new_parameter, ' ' ) ;
	  break	;

  } // end switch

}	
void start_menu(void)
{
uint8 i;
uint8 menu_buf1[10];
uint8 menu_buf2[10];
clear_lines();
 
	if(item_to_adjust == MIN_MENU && menu_addi == 1 )  //display add
		{
		for(i=0;i<10;i++)
			{
			menu_buf1[i] = menu[TOTAL_MENU_PARAMETERS].first_line[i];//
			menu_buf2[i] = menu[TOTAL_MENU_PARAMETERS].second_line[i];//
			}
		}
	else if(item_to_adjust == EEP_DAY_COOLING_SETPOINT)
	{
		for(i=0;i<10;i++)
			{
			menu_buf1[i] = menu[56].first_line[i];//menu[TOTAL_MENU_PARAMETERS].name[i];
			menu_buf2[i] = menu[56].second_line[i];
			}	
	}	
	else if(item_to_adjust == EEP_FAN_SPEED)
	{
		for(i=0;i<10;i++)
			{
			menu_buf1[i] = menu[57].first_line[i];//menu[TOTAL_MENU_PARAMETERS].name[i];
			menu_buf2[i] = menu[57].second_line[i];
			}	
	}		
	else		
		{
		for(i=0;i<10;i++)
			{
			menu_buf1[i] = menu[item_to_adjust - MIN_MENU].first_line[i];//menu[TOTAL_MENU_PARAMETERS].name[i];
			menu_buf2[i] = menu[item_to_adjust - MIN_MENU].second_line[i];
			}
		}
		clear_line(1);
			
//	if((item_to_adjust == EEP_PROTOCOL_SEL)||(item_to_adjust == EEP_RS485_MODE))
//		{
//		display_menu(menu_buf1,menu_buf2);
//		}	
//	else
//		display_menu(90,menu_buf1);
		display_menu(menu_buf1,menu_buf2);
		
	if(item_to_adjust == MIN_MENU && menu_addi)
		{
			if(laddress > EEP_MaxAddress)
				laddress =EEP_MaxAddress;
			else if(laddress < EEP_MinAddress)
				laddress = EEP_MinAddress;
		  menubuffer.new_parameter = laddress;
		}

	else if(item_to_adjust == EEP_DEMAND_RESPONSE)
		{
		menubuffer.new_parameter = demand_response;
		}

	else if(item_to_adjust == EEP_NIGHT_HEATING_SETPOINT)
		{
//		EEP_NightSpHi = read_eeprom(EEP_NIGHT_SETPOINT+1);
//		EEP_NightSpLo = read_eeprom(EEP_NIGHT_SETPOINT);
		menubuffer.new_parameter = ((int16)EEP_NightSpHi << 8 ) + EEP_NightSpLo;
		menubuffer.new_parameter /=10;				
		}
	else if(item_to_adjust == EEP_DAY_COOLING_SETPOINT)
		{
		menubuffer.new_parameter = ((int16)EEP_DaySpHi << 8 ) + EEP_DaySpLo;
		menubuffer.new_parameter /=10;				
		}		
		
	else if(item_to_adjust == EEP_RS485_MODE)	
	{
		menubuffer.new_parameter = RS485_Mode;
	}

	else if(item_to_adjust == EEP_PROTOCOL_SEL)	
	{
		if(modbus.com_config[0] == BAC_MSTP)
			menubuffer.new_parameter = 1;
		else
			menubuffer.new_parameter = 0;
	}	
	//----------------------------------------------------------
//	#ifdef RTC8563
//	else if((item_to_adjust >= EEP_CLOCK_YEAR) && (item_to_adjust <= EEP_CLOCK_SECOND))
//		{
//			menubuffer.new_parameter = GetRTCTime(item_to_adjust);
//		//menubuffer.new_parameter = time_data[item_to_adjust - EEP_CLOCK_YEAR];
//		}
//	else if((item_to_adjust >= EEP_SCHEDULE_WORK_DAYTIME_HOUR) && (item_to_adjust <= EEP_SCHEDULE_WEEKEND_NIGHTTIME_MINUTE))
//		{
//		menubuffer.new_parameter = time_data[item_to_adjust - EEP_SCHEDULE_WORK_DAYTIME_HOUR];//read_eeprom(EEP_DAY1_EVENT1_HI + (item_to_adjust-EEP_SCHEDULE_WORK_DAYTIME_HOUR));
//		time_data[8] = menubuffer.new_parameter ;				
//		}
//	#endif	
	//------------------------------------------------------------
	else
		menubuffer.new_parameter = b.eeprom[item_to_adjust];
		
		if(item_to_adjust == EEP_MENU_ITEM_BEGINNING && !menu_addi)
			{
			menubuffer.new_parameter = temperature;
			}
//			if(item_to_adjust==EEP_DAC_OFFSET)
//				{
//				set_dac_volts_flag = 1 ;
//				}
				
//		show_parameter();     //put the current parameter on the display 	
}	





#endif


#ifndef TSTAT7_ARM
void DealWithKey(uint8 key_id)
{
//	uint8 i;
//	uint8 temp_bit = 0;


	switch(	menu_id	)
	{
		case INCREASE_COOLING_SETPOINT :
	 		  
			increase_cooling_setpoint() ;
		break ;

		case DECREASE_COOLING_SETPOINT :
		 		  
			decrease_cooling_setpoint() ;
		break ;

		case INCREASE_FAN_SPEED	:
		 		  
			increase_fan_speed() ;
		break;

		case INCREASE_SYS_MODE://DECREASE_FAN_SPEED:
	 		increase_sysmode() ;  
			//decrease_fan_speed();
		break ;

		case START_MENU_MODE :
			clear_lines();
				
			start_menu_mode();
			if(EEP_SpecialMenuLock == 0 || EEP_SpecialMenuLock == 4)   // USER UNLOCK
			{
				part_lock = 0;
			  start_menu_mode();
			}
			else if(EEP_SpecialMenuLock == 2 || EEP_SpecialMenuLock == 3)
			{
				start_menu_mode();
				part_lock = 1 ;
			}
			
		break ;

//		case EXIT_MENU_MODE :
//		  
//			exit_menu_mode() ;
//		break ;

		case START_NEXT_MENU :
		  
			start_next_menu() ;
		break ;

		case START_PREVIOUS_MENU :
		  
			start_previous_menu() ;
		break ;

		case INCREASE_PARAMETER	:
		  
			increase_parameter() ;
		break ;

		case DECREASE_PARAMETER	:
		  
			decrease_parameter() ;
		break ;

		case ACCEPT_PARAMETER :
		  
			accept_parameter() ; 

	    break ;

		case ACCEPT_EXIT_PARAMETER :
		  
			accept_parameter() ;
//			exit_menu_mode() ;
		
		break ;

  	default:
			break;

	}	// end switch
	menu_id = 0;
}	// end run_function()

#endif

void DisRestart(void)
{
SoftReset();
}

void SetRTCTime(uint16 item, uint16 value)
{
	switch(item)
	{
		case EEP_CLOCK_YEAR:
			RTC_Set(value, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
		break;
		
		case EEP_CLOCK_MONTH:
			RTC_Set(calendar.w_year, value, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
		break;
		
	//	case SET_WEEK:
	//		RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min, calendar.sec);
	//	break;

		case EEP_CLOCK_DAY:
			RTC_Set(calendar.w_year, calendar.w_month, value, calendar.hour, calendar.min, calendar.sec);
		break;
		
		case EEP_CLOCK_HOUR:
			RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, value, calendar.min, calendar.sec);
		break;
		
		case EEP_CLOCK_MINUTE:
			RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, value, calendar.sec);
		break;
		
		case EEP_CLOCK_SECOND:
			RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date, calendar.hour, calendar.min,value);
		break;
		
		default:
			break;
	}

}


int16 GetRTCTime(uint16 item)
{
	uint16 value = 0;
switch(item)
	{
	case EEP_CLOCK_YEAR:
			value = calendar.w_year;
	break;

	case EEP_CLOCK_MONTH:
			value = calendar.w_month;
	break;

	case EEP_CLOCK_WEEK:
			value = calendar.week;
	break;

	case EEP_CLOCK_DAY:
			value = calendar.w_date;
	break;

	case EEP_CLOCK_HOUR:
			value = calendar.hour;
	break;

	case EEP_CLOCK_MINUTE:
			value = calendar.min;
	break;

	case EEP_CLOCK_SECOND:
			value = calendar.sec;
	break;

	default:
			break;
	}
return value;
}

void one_second_pause(void)
{
delay_ms(1000);
}



uint8 htu21d_soft_reset(void)
{
//	int8 ack;
	IIC_Start();
	IIC_Send_Byte(DEV_ADDR | I2C_WR);
   if(IIC_Wait_Ack())
		 return 0;
   
	IIC_Send_Byte(CMD_SOFT_RESET);
   if(IIC_Wait_Ack())
		 return 0;
   
	IIC_Stop();
	return 1;
}

int8 htu21d_trigger_measurement_no_hold_master(int8 type)
{
   IIC_Start();
	 IIC_Send_Byte(DEV_ADDR | I2C_WR);
   if(IIC_Wait_Ack())
		 return 0;
   
   if(type == SEL_TEMP)
      IIC_Send_Byte(CMD_TRIGGER_TEMP_NO_HOLD_MASTER);
   else
      IIC_Send_Byte(CMD_TRIGGER_HUM_NO_HOLD_MASTER);

   if(IIC_Wait_Ack())
		 return 0;
     
   IIC_Stop();
   return 1;
}


uint8 htu21d_read_measurement_no_hold_master(uint8 type, uint16 *value)
{
//	uint8 ack;
	uint16 temp = 0;

	IIC_Start();
	IIC_Send_Byte(DEV_ADDR | I2C_RD);
	IIC_Wait_Ack();

	temp = IIC_Read_Byte(1);
	temp <<= 8;
	temp |= IIC_Read_Byte(1);

	IIC_Read_Byte(0); // crc, ignore this byte

	IIC_Stop();
	*value = temp;
	return 1;
}

uint8 htu21d_calculation(uint8 type, uint16 src, int16 *dst)
	{
	int32 ret;
	if(type != (src & 0x0002))
	return 0;

	ret = (int32)(src & 0xfffc);

	if(type == SEL_TEMP)
	{
	ret = ret * 17572;
	//ret >>= 16;
	ret /= 65536;
	*dst = (uint16)(ret - 4685);
	*dst = *dst / 10;

	}
	else
	{
	ret = ret * 1250;
	//ret >>= 16;
	ret /= 65536;
	*dst = (uint16)(ret - 60);
	}

	return 1;
}



uint16 read_hum_sensor(uint8 item)
{
//	int8 ack;
	uint16 temp = 0;
	int32 org_value = 0;

	IIC_Start();
	IIC_Send_Byte(DEV_ADDR | I2C_RD);
   if(IIC_Wait_Ack())
		 return 0;
	temp = IIC_Read_Byte(1);
	temp <<= 8;
	temp += IIC_Read_Byte(1);

	IIC_Read_Byte(0); // crc, ignore this byte

	IIC_Stop();
	 
	org_value = temp;
	if(item == SEL_HUM)
		org_value	= -60 + ((1250 * org_value) >> 16); 
	else
		org_value	= (-4685 + ((17572 * org_value) >> 16))/10;
	temp = org_value;
	return temp;

}

uint8 htu21d_read_user_register(void)
{
//   uint8 ack;
   int8 reg_tmp;
   
   IIC_Start();
   IIC_Send_Byte(DEV_ADDR | I2C_WR);
   if(IIC_Wait_Ack())
		 return 0;
   
   IIC_Send_Byte(CMD_READ_USER_REGISTER);
   if(IIC_Wait_Ack())
		 return 0;
   
   IIC_Start();
   IIC_Send_Byte(DEV_ADDR | I2C_RD);
   if(IIC_Wait_Ack())
		 return 0;
   
   reg_tmp = IIC_Read_Byte(0);
   
   IIC_Stop();
   return reg_tmp;
}

int16 CalculateTemperatureCompensation(int16 hummity_back,int16 temp_back ) 
{                                              
//RHcompensatedT = RHactualT + (25 - Tactual) * CoeffTemp    
 int16 get_rh=0;
 get_rh = hummity_back + (-0.15) * (250 - temp_back);
// ctest1 = (-0.15) * (250 - temp_back);
 //get_rh=(((signed int32)B1*hummity_back+13250)*(temp_back/10)-((signed int32)B3*hummity_back+308760))/10000; 
 return get_rh;   
}

int16 get_dew_pt(int16 para_t,int16 para_h)
{
   int16 temp;
   float ph,pt,ps;
   
   ph = log((float)para_h/1000);
   pt = 17.625*para_t;
   pt = pt/(2430.4+para_t);
   ps =ph+pt;
// temp =2430.4*(log((float)para_h/1000)+((17.625*para_t)/(2430.4+para_t)))/(17.625-log((float)para_h/1000)-((17.625*para_t)/(2430.4+para_t)));
   temp = ps*2430.4/(17.625-ps);
       
   return temp;
}

int16 eep_get_value(uint8 hibyte, uint8 lobyte)
{
int16 value_temp = 0;	
if(hibyte > 127)//negative value
{
	value_temp = hibyte<<8;
	value_temp += lobyte;
	value_temp = value_temp - 0x10000;
}
else
{
	value_temp = hibyte;
	value_temp *=256;
	value_temp += lobyte;
}

return value_temp;
}


void save_user_def(void)
{
//uint16 i;
//for(i=0;i<(MAXEEPCONSTRANGE - USER_MAXEEPCONSTRANGE);i++)
//	write_eeprom(USER_TEMP_SELECT + i, b.eeprom[EEP_TEMP_SELECT +i]);
//write_eeprom(USER_ADDRESS, laddress);	
write_eeprom(USER_SETTING, 99);
}

uint16 get_day_of_year( void )
{
	uint16 temp;
	
	if(calendar.w_year%4 == 0)//leap year
	{
		switch(calendar.w_month)
		{
			case 1:
				temp = calendar.w_date;
			break;
			
			case 2:
				temp = calendar.w_date + 31;
			break;
			
			case 3:
				temp = calendar.w_date + 31 + 28;
			break;			
			
			case 4:
				temp = calendar.w_date + 31 + 28 + 31;
			break;

			case 5:
				temp = calendar.w_date + 31 + 28 + 31 + 30;
			break;

			case 6:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31;
			break;

			case 7:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31 + 30;
			break;

			case 8:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31 + 30 + 31;
			break;

			case 9:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31;
			break;

			case 10:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30;
			break;

			case 11:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
			break;

			case 12:
				temp = calendar.w_date + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
			break;		
			
			default:
				break;
		}
	}
	else
	{
		switch(calendar.w_month)
		{
			case 1:
				temp = calendar.w_date;
			break;
			
			case 2:
				temp = calendar.w_date + 31;
			break;
			
			case 3:
				temp = calendar.w_date + 31 + 29;
			break;			
			
			case 4:
				temp = calendar.w_date + 31 + 29 + 31;
			break;

			case 5:
				temp = calendar.w_date + 31 + 29 + 31 + 30;
			break;

			case 6:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31;
			break;

			case 7:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31 + 30;
			break;

			case 8:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31 + 30 + 31;
			break;

			case 9:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31;
			break;

			case 10:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30;
			break;

			case 11:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31;
			break;

			case 12:
				temp = calendar.w_date + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30;
			break;		
			
			default:
				break;
		}	
	}

	return temp;
}

uint8 cal_holiday(void)
{
	uint8 holiday_bit;
	uint8 seg;

	today_dayofyear = get_day_of_year();
	seg = today_dayofyear / 8;//which byte of the holiday array
	holiday_bit = today_dayofyear % 8;
	if(GetByteBit(&ScheduleDay(seg),holiday_bit))//holiday
		return 1;
  else 
		return 0;
}

#define EVENT_NULL    0
#define EVENT_DHOME   1
#define EVENT_WORK    2
//#define EVENT_NHOME   3
#define EVENT_SLEEP   3
#define EVENT_AWAY    4

void event_dealwith(uint8 event_type)
{

	switch(event_type)
	{
		case EVENT_NULL:
			
		break;
		
		case EVENT_DHOME:
			setpoint_select = DHOME_SP;
			info_byte |= 0x01;
			occupied = 1;

			#ifndef TSTAT7_ARM
			fan_speed_user = FAN_AUTO;
		
			fanspeedbuf = fan_speed_user;
			icon.fanspeed = 1;
			icon.fan = 1;
			#endif
		break;
		
		case EVENT_WORK:	
			setpoint_select = NIGHT_SP;
			info_byte &= 0xfe;
		  occupied = 0;
			if(fan_speed_user != FAN_OFF)
			{
				fan_speed_user = FAN_OFF;
				fanspeedbuf = fan_speed_user;
				#ifndef TSTAT7_ARM
				icon.fanspeed = 1;
				icon.fan = 1;
				#endif
			}
		break;

//		case EVENT_NHOME:
//			setpoint_select = NHOME_SP;
//			info_byte |= 0x01;
//			occupied = 1;
//		break;
		
		case EVENT_SLEEP:
			setpoint_select = SLEEP_SP_LIB;
		break;
		
		case EVENT_AWAY:
			setpoint_select = AWAY_SP_LIB;
			info_byte &= 0xfe;
		  occupied = 0;
			if(fan_speed_user != FAN_OFF)
			{
				fan_speed_user = FAN_OFF;
				fanspeedbuf = fan_speed_user;
				#ifndef TSTAT7_ARM
				icon.fanspeed = 1;
				icon.fan = 1;
				#endif
			}			
			//setpoint_select = AWAY_SP_LIB;
		break;		
		
		default:			
		break;
	}
	
}

#define SCHEDULE_NORMAL_DAY   	0
#define SCHEDULE_HOLIDAY  	1	
#define EVENT1	1
#define EVENT2	2
#define EVENT3	3
#define EVENT4	4
#define EVENT5	5
#define EVENT6	6

int8 get_event_flag(uint8 event,uint8 type)//day = 0: normal day   =1: holiday
{
	uint8 temp;
	uint8 flag;
	if(calendar.week == 0)//sunday
		temp = 7;
	else
		temp = calendar.week;
	
	if(type == SCHEDULE_HOLIDAY)
		temp = 8;
	

	switch(event)
	{
		case 1://			
			flag = ScheduleMondayFlag((temp - 1)*3) & 0x07;	
		break;
		
		case 2://
			flag = (ScheduleMondayFlag((temp - 1)*3)>>3) & 0x07;	
		break;
		
		case 3://
			flag = ScheduleMondayFlag(((temp - 1)*3)+1) & 0x07;	
		break;
		
		case 4://
			flag = (ScheduleMondayFlag(((temp - 1)*3)+1)>>3) & 0x07;	
		break;
		
		case 5://
			flag = ScheduleMondayFlag(((temp - 1)*3)+2) & 0x07;	
		break;
		
		case 6://
			flag = (ScheduleMondayFlag((((temp - 1)*3)+2))>>3) & 0x07;	
		break;
			
	}

	return flag;
	
}



void RunSchedule(void)
{
	uint32 currentnum;
	uint16 event1, event2,event3,event4,event5,event6;
	uint8 event_type;
	uint8 temp;

	if(calendar.week == 0)//sunday
		temp = 6;
	else
		temp = calendar.week - 1;

	if(cal_holiday())//if it is holiday
	{
		event1 = ScheduleHolidayEvent1(0) * 60 + ScheduleHolidayEvent1(1);
		event2 = ScheduleHolidayEvent2(0) * 60 + ScheduleHolidayEvent2(1);
		event3 = ScheduleHolidayEvent3(0) * 60 + ScheduleHolidayEvent3(1);
		event4 = ScheduleHolidayEvent4(0) * 60 + ScheduleHolidayEvent4(1);
		event5 = ScheduleHolidayEvent5(0) * 60 + ScheduleHolidayEvent5(1);
		event6 = ScheduleHolidayEvent6(0) * 60 + ScheduleHolidayEvent6(1);
		
   	currentnum = calendar.hour * 60 + calendar.min;
		
		if((currentnum >= event1) && (currentnum < event2))//event1 triggered 
		{
			event_type = get_event_flag(EVENT1, SCHEDULE_HOLIDAY);
		}
		else if((currentnum >= event2) && (currentnum < event3))//event2
		{
			event_type = get_event_flag(EVENT2, SCHEDULE_HOLIDAY);
			
		}	
		else if((currentnum >= event3) && (currentnum < event4))//event3
		{
			event_type = get_event_flag(EVENT3, SCHEDULE_HOLIDAY);
		}	
		else if((currentnum >= event4) && (currentnum < event5))//event4
		{
			event_type = get_event_flag(EVENT4, SCHEDULE_HOLIDAY);
		}	
		
		else if((currentnum >= event5) && (currentnum < event6))//event5
		{
			event_type = get_event_flag(EVENT5, SCHEDULE_HOLIDAY);
		}	
		else //if(currentnum >= event6) 
		{
			event_type = get_event_flag(EVENT6, SCHEDULE_HOLIDAY);
		}
		event_dealwith(event_type);
	}	
	else
	{
		event1 = ScheduleMondayEvent1(0 + temp*12) * 60 + ScheduleMondayEvent1(1 + temp*12);
		event2 = ScheduleMondayEvent2(0 + temp*12) * 60 + ScheduleMondayEvent2(1+ temp*12);
		event3 = ScheduleMondayEvent3(0+ temp*12) * 60 + ScheduleMondayEvent3(1+ temp*12);
		event4 = ScheduleMondayEvent4(0+ temp*12) * 60 + ScheduleMondayEvent4(1+ temp*12);
		event5 = ScheduleMondayEvent5(0+ temp*12) * 60 + ScheduleMondayEvent5(1+ temp*12);
		event6 = ScheduleMondayEvent6(0+ temp*12) * 60 + ScheduleMondayEvent6(1+ temp*12);
		
   	currentnum = calendar.hour * 60 + calendar.min;
		
		if((currentnum >= event1) && (currentnum < event2))//event1 triggered 
		{
			event_type = get_event_flag(EVENT1, SCHEDULE_NORMAL_DAY);
		}
		else if((currentnum >= event2) && (currentnum < event3))//event2
		{
			event_type = get_event_flag(EVENT2, SCHEDULE_NORMAL_DAY);
			
		}	
		else if((currentnum >= event3) && (currentnum < event4))//event3
		{
			event_type = get_event_flag(EVENT3, SCHEDULE_NORMAL_DAY);
		}	
		else if((currentnum >= event4) && (currentnum < event5))//event4
		{
			event_type = get_event_flag(EVENT4, SCHEDULE_NORMAL_DAY);
		}	
		
		else if((currentnum >= event5) && (currentnum < event6))//event5
		{
			event_type = get_event_flag(EVENT5, SCHEDULE_NORMAL_DAY);
		}	
		else //if(currentnum >= event6) 
		{
			event_type = get_event_flag(EVENT6, SCHEDULE_NORMAL_DAY);
		}
		
		
		event_dealwith(event_type);
		#ifndef TSTAT7_ARM	
		if(event_type != pre_event) //if event change, refresh icons.
		{
			icon.occ_unocc = 1;
			icon.fan = 1;	
			icon.setpoint = 1;			
		}
		#endif //nTSTAT7_ARM
		pre_event = event_type;
	}
//if(scheduleoff == 0)
//{
//	currentnum = (calendar.w_year%100) * 360 + calendar.w_month*30 + calendar.w_date;
//	setbegain = ScheduleHolidayByear *360 + ScheduleHolidayBmon*30 + ScheduleHolidayBday;
//	setend = ScheduleHolidayEyear *360 + ScheduleHolidayEmon*30 + ScheduleHolidayEday;
//	if((currentnum >= setbegain) && (currentnum <= setend))//it is on holiday
//	{
//		loop_setpoint[0] = Holiday_sp_hi * 256 + Holiday_sp_lo; 
//		setpoint_select = HOLIDAY_SP_LIB;
//	}
//	else//not on holiday
//	{
//		tdhome = ScheduleDHomeHour * 60 + ScheduleDHomeMin;
//		twork = ScheduleWorkHour * 60 + ScheduleWorkMin;
//		tnhome = ScheduleNHomeHour * 60 + ScheduleNHomeMin;
//	  tsleep = ScheduleSleepHour * 60 + ScheduleSleepMin;
//		currentnum = calendar.hour * 60 + calendar.min;
//		if((currentnum >= tdhome) && (currentnum < twork))//day at home/wake up/eating
//		{
//			setpoint_select = DHOME_SP;
//			info_byte |= 0x01;
//			occupied = 1;
//		}
//		else if((currentnum >= twork) && (currentnum < tnhome))//work/leave
//		{
//			setpoint_select = NIGHT_SP;
//			info_byte &= 0xfe;
//		  occupied = 0;
//		}	
//		else if((currentnum >= tnhome) && (currentnum < tsleep))//night at home
//		{
//			setpoint_select = NHOME_SP;
//			info_byte |= 0x01;
//			occupied = 1;
//		}	
//		else if((currentnum >= tsleep) || (currentnum < tdhome))//sleep
//		{
//			setpoint_select = SLEEP_SP_LIB;
//		}			
//		
//	}	
//}
}


//void backlight_ctrl(uint8 onoff)
//{
//	#ifdef TSTAT7_ARM
// if(onoff == BK_ON)
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);
// else
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
// #else
// if(onoff == BK_OFF)
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);
// else
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0); 
// #endif
//}

uint8 icon_flag[9];
void icon_control_output(uint8 icon_name)
{

	if(IconOutputControl(icon_name) != 255)
	{
		if(IconOutputControl(icon_name) == 0)
			icon_flag[icon_name] = 0;
		else if((IconOutputControl(icon_name) < 6)&&(IconOutputControl(icon_name)>0))
		{
		if(GetByteBit(&OUTPUT_1TO5.BYTE, IconOutputControl(icon_name)-1) == 1) //DAY ICON RELATIVE WITH OUTPUT1 i
			icon_flag[icon_name] = 1;
		else
			icon_flag[icon_name] = 0;
		}
		else
		{
			if(IconOutputControl(icon_name) == 6)//AO1
			{
				if(valve[0] > 0)
					icon_flag[icon_name] = 1;
				else
					icon_flag[icon_name] = 0;
			}
			if(IconOutputControl(icon_name) == 7)//AO1
			{
				if(valve[1] > 0)
					icon_flag[icon_name] = 1;
				else
					icon_flag[icon_name] = 0;
			}
			
		}
	}		
}


//void refresh_icon_output(void)
//{
////--------------day and night icon----------------
//  if((icon_flag[0] == 1)&&(icon_flag[1] != 1))
//		disp_icon(ICON_XDOTS, ICON_YDOTS, sunicon, 		FIRST_ICON_POS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else if((icon_flag[1] == 1)&&(icon_flag[0] != 1))
//		disp_icon(ICON_XDOTS, ICON_YDOTS, moonicon, 		FIRST_ICON_POS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else
//		disp_null_icon(ICON_XDOTS, ICON_YDOTS, 0, FIRST_ICON_POS ,ICON_POS,TSTAT8_BACK_COLOR, TSTAT8_BACK_COLOR);

////--------------occ and unocc icon----------------	
//	if((icon_flag[2] == 1)&&(icon_flag[3] != 1))
//		disp_icon(ICON_XDOTS, ICON_YDOTS, athome, 		SECOND_ICON_POS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else if((icon_flag[3] == 1)&&(icon_flag[2] != 1))
//		disp_icon(ICON_XDOTS, ICON_YDOTS, offhome, 		SECOND_ICON_POS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else
//		disp_null_icon(ICON_XDOTS, ICON_YDOTS, 0, SECOND_ICON_POS ,ICON_POS,TSTAT8_BACK_COLOR, TSTAT8_BACK_COLOR);
////--------------heat and cool icon----------------
//	if((icon_flag[4] == 1)&&(icon_flag[5] != 1))
//		disp_icon(ICON_XDOTS, ICON_YDOTS, heaticon, 		THIRD_ICON_POS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else if((icon_flag[5] == 1)&&(icon_flag[4] != 1))
//		disp_icon(ICON_XDOTS, ICON_YDOTS, coolicon, 		THIRD_ICON_POS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else
//		disp_null_icon(ICON_XDOTS, ICON_YDOTS, 0, THIRD_ICON_POS ,ICON_POS,TSTAT8_BACK_COLOR, TSTAT8_BACK_COLOR);
////--------------fan1 and fan2,fan3 icon----------------	
//	if((icon_flag[6] == 1)&&(icon_flag[7] != 1)&&(icon_flag[8] != 1))
//		disp_icon(FANSPEED_XDOTS, FANSPEED_YDOTS, fanspeed1a, FIFTH_ICON_POS,ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else if((icon_flag[7] == 1)&&(icon_flag[6] != 1)&&(icon_flag[8] != 1))
//		disp_icon(FANSPEED_XDOTS, FANSPEED_YDOTS, fanspeed2a, FIFTH_ICON_POS,ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else if((icon_flag[8] == 1)&&(icon_flag[6] != 1)&&(icon_flag[7] != 1))
//		disp_icon(FANSPEED_XDOTS, FANSPEED_YDOTS, fanspeed3a, FIFTH_ICON_POS,ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//	else
//		disp_icon(FANSPEED_XDOTS, FANSPEED_YDOTS, fanspeed0a, FIFTH_ICON_POS,ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);	

//}


//STRCTPID newpid;

//void PID_init()
//{
//	newpid.SetPoint = 0.0;
//	newpid.ActualPoint = 0.0;
//	newpid.err = 0.0;
//	newpid.err_last = 0.0;
//	newpid.result = 0.0;
//	newpid.integral = 0.0;
//	newpid.Kp = 60;
//	newpid.Ki = 0;
//	newpid.Kd = 2;
//}

//float PID_realize(float point, float ActPoint)
//{
//	newpid.SetPoint = point;
//	newpid.ActualPoint = ActPoint;
//	newpid.err = newpid.SetPoint - newpid.ActualPoint;
//	newpid.integral += newpid.err;
//	newpid.result = /*newpid.Kp * newpid.err + newpid.Ki * newpid.integral + */newpid.Kd*(newpid.err - newpid.err_last);
//	newpid.err_last = newpid.err;
//	newpid.ActualPoint = newpid.voltage*1.0;
//	return newpid.result/100;
//	float incrementPoint;
//	newpid.SetPoint = point;
//	newpid.ActualPoint = ActPoint;	
//	newpid.err = newpid.SetPoint - newpid.ActualPoint;
//	incrementPoint = newpid.Kp*(newpid.err-newpid.err_next)+newpid.Ki*newpid.err+newpid.Kd*(newpid.err-2*newpid.err_next+newpid.err_last);
//	//newpid.ActualPoint += incrementPoint;
//	newpid.err_last = newpid.err_next;
//	newpid.err_next = newpid.err;
//	return incrementPoint;//*100/ActPoint;
//}























