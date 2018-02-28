
#include "crc.h"
#include "usart.h"
#include "delay.h"
#include "define.h"
#include "modbus.h"
#include "registerlist.h"
#include "inputs.h"
#include "../filter/filter.h"
#include "led.h"
#include "fifo.h"
//#include "bacnettimer.h"
#include "types.h"
#include "eepDefine.h"
#include "eep_mapping.h"
#include "TstatFunctions.h"
#include "24cxx.h"
#include "setpoint.h"
#include "pid.h"
#include "output.h"
#include "key.h"
#include "revision.h"
//#include "testcode.h"
#include "rs485.h"
#include "stdlib.h"
#include "stdbool.h"
#include "rtc.h"
#include "control_logic.h"
#include "lcd.h"
#include "bacnet.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ProductModel.h"

extern int16 ao1_fdbk;
extern int16 ao2_fdbk;
uint8 bdtest1 = 0,bdtest2=0;
extern int16 ctest1;
extern uint16  xdata output_temp ;
uint8 update_flag = 0;
void Timer_Silence_Reset( void);
uint8 dec_blink = 0;
uint8 scan_enable =1;
uint16 scan_timer = 0;
uint8 dealwith_package = 0;
uint8 randval;
uint8 rand_read_ten_count;
extern uint8 first_output;
extern uint16 current_temp; 
extern uint16 crnt_crnt;
uint8 startbit = 0;
extern uint8 uartbuf2[9];
uint8 send_count2 = 0;
uint16 co2_dataB = 0;
//PID function variables
extern int16  loop_setpoint[3]; 
int32 xdata iterm_sum_1sec[3];
uint8 xdata pid3_input_select;
uint8 xdata pid_setpoint[3] = {50,50,50}  ;
extern uint8 xdata pid[3];
//extern uint8 xdata universal_pid[3][6]; 
//extern uint8 xdata heat_pid3_table;
//extern uint8 xdata cool_pid3_table;
uint16 xdata cooling_db;
uint16 xdata heating_db;
extern uint8 init_PID_flag; 	// Initialize the PID flag, stops reset windup problems
int16 xdata analog_input[4];
//int16 xdata mul_analog_input[10];
//int16 xdata mul_analog_in_buffer[10];
//int16 xdata pre_mul_analog_input[10]; //used to filter  readings

/*****************use these variables for testing****************/
uint8 SN_WR_Flag = 0; 
extern et_mode_of_operation  current_mode_of_operation[3];
extern PTBDSTR OUTPUT_1TO5;
extern PTBDSTR RELAY_1TO5;
extern int16  valve[3];
extern int16  valve_target_percent ; 

int8 test1;
extern uint8  gucHeatCool;
extern uint8 xdata gucManualValve; 
int8 HUM_CAL_UNLOCK_FLAG = 0;
uint16  sensor_hum_fre = 0;
uint16  sensor_hum_value = 0;
uint16  sensor_hum_temp = 0;
uint8 hum_size_copy;
extern uint16 COOLING_MODE;
extern uint16 HEATING_MODE;
/****************************************************************/
uint8 xdata temp_info_byte;
uint8 idlockflag = NEED_TO_LOCK;
uint8 idlockcnt = 0;
//u8 laddress = 254;
u8 serialnumber[4];
u8 ledtf= 0;
u8 USART_RX_BUFA[USART_REC_LEN];     //?車那??o3?,℅?∩車USART_REC_LEN??℅??迆.
u8 USART_RX_BUFB[USART_REC_LEN];
u8 USART_RX_BUFC[10];
u8 uart_send[USART_SEND_LEN] ;
vu8 transmit_finished = 0 ; 
vu8 rece_count = 0 ;
vu8 rece_countB = 0 ;
vu8 rece_size = 0 ;
vu8 serial_receive_timeout_count ;
u8 SERIAL_RECEIVE_TIMEOUT ;
u8 dealwithTag ;
STR_MODBUS modbus ;
u8 DealwithTag ;
u16 sendbyte_num = 0 ;
u16 uart_num = 0 ;
u8 Station_NUM= 12;

u8 serial_response_delay;
u8 gucResponseDelay;
//uint8 gbSerialFlag = 0;
uint8 serial_no_activity = 0;
//extern uint8_t Receive_Buffer_Data0[512];
/***************************************************/

/***************************************************/
extern FIFO_BUFFER Receive_Buffer0;

void InitCRC16(void)
{
	CRClo = 0xFF;
	CRChi = 0xFF;
}


 
void USART1_IRQHandler(void)                	//∩??迆1?D??﹞t??3足D辰
{	
static uint8 testbit;	
	u8 receive_buf ;
	uint8 i;
	static u16 send_count1 = 0 ;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)	//?車那??D??
	{
			if(modbus.com_config[0] == MODBUS )	
			{
						if(rece_count < 50)
							USART_RX_BUFA[rece_count++] = USART_ReceiveData(USART1);//(USART1->DR);		//?芍豕??車那?米?米?那y?Y
						else
							serial_restart();
						
						if(rece_count == 1)
						{
							// This starts a timer that will reset communication.  If you do not
							// receive the full packet, it insures that the next receive will be fresh.
							// The timeout is roughly 7.5ms.  (3 ticks of the hearbeat)
							rece_size = 50;
							serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
							
							if((USART_RX_BUFA[0] != 255) && (USART_RX_BUFA[0] != laddress) && (USART_RX_BUFA[0] != 0))
							{
								serial_restart();
								return;
							}
						}
						else if(rece_count == 4)
						{
							//check if it is a scan command
							if((((vu16)(USART_RX_BUFA[2] << 8) + USART_RX_BUFA[3]) == 0x0a) && (USART_RX_BUFA[1] == WRITE_VARIABLES))
							{
								rece_size = DATABUFLEN_SCAN;
								serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;	
							}
						}
						else if(rece_count == 7)
						{
							if((USART_RX_BUFA[1] == READ_VARIABLES) || (USART_RX_BUFA[1] == WRITE_VARIABLES))
							{
								rece_size = 8;
								serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
								//dealwithTag = 1;
							}
							else if(USART_RX_BUFA[1] == MULTIPLE_WRITE_VARIABLES)
							{
							//	ctest1 = 90;
								rece_size = USART_RX_BUFA[6] + 9;
								serial_receive_timeout_count = USART_RX_BUFA[6] + 8;
							}
							else if(USART_RX_BUFA[0] == 0x55 && USART_RX_BUFA[1] == 0xff && USART_RX_BUFA[2] == 0x01 && USART_RX_BUFA[5] == 0x00 && USART_RX_BUFA[6] == 0x00)
							{//bacnet protocal detected
							modbus.com_config[0] = BAC_MSTP;
							write_eeprom(EEP_MODBUS_BACNET_SWITCH, BAC_MSTP);
							Recievebuf_Initialize(0);							
							}				
							//else
							//{
								//rece_size = 50;
								//serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
							//}
						}
						
						else if(rece_count == 6 && USART_RX_BUFA[1] == CHECKONLINE)
							{
								if((scan_timer == 0)&& scan_enable)
								{
									rece_size = rece_count;
									serial_receive_timeout_count = 0;
									dealwithTag = SERIALDEALTIME_DELAY;
									for(i=0;i<rece_size;i++)
										{
										USART_RX_BUFB[i] = USART_RX_BUFA[i];									
										}	
								}
							}
						else if((rece_count == rece_size) && (USART_RX_BUFA[1] == READ_VARIABLES || USART_RX_BUFA[1] == WRITE_VARIABLES || USART_RX_BUFA[1] == MULTIPLE_WRITE_VARIABLES))
						{
							testbit = ~testbit;
							if(testbit)
								GPIO_SetBits(GPIOA, GPIO_Pin_0);
							else
								GPIO_ResetBits(GPIOA, GPIO_Pin_0);
								
							// full packet received - turn off serial timeout
							if(dealwith_package == 0)//if dealwith_package !=0, previous package is not done	
							{								
								if(checkCrc())
								{								
								serial_receive_timeout_count = 0;
								for(i=0;i<rece_size;i++)
									{
									USART_RX_BUFB[i] = USART_RX_BUFA[i];
									dealwith_package = 1;										
									}
									dealwithTag = SERIALDEALTIME_DELAY;		// making this number big to increase delay
									dec_blink = 1;
								//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);				//disable serial interrupt
								}
								else							
									serial_restart();
							}

						}
						else
							{
							serial_restart();
							}
		
			}
			else if(modbus.com_config[0] == BAC_MSTP )
			{
//					if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//					{
							receive_buf =  USART_ReceiveData(USART1); 
							FIFO_Put(&Receive_Buffer0, receive_buf);
					}
//			}
	}

	else  if( USART_GetITStatus(USART1, USART_IT_TXE) == SET ) //Tansmit Data Register empty interrupt = transmit has finished
			{
			if((modbus.com_config[0] == MODBUS )||(modbus.com_config[0] == BAC_MSTP))
				{
				if( send_count1 > sendbyte_num)
					{
					serial_restart();
					USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
					send_count1 = 0;
					Timer_Silence_Reset();			
					}
				else
					{
					USART_SendData(USART1, uart_send[send_count1++]);						
					Timer_Silence_Reset();
					}
				}					
			}
}