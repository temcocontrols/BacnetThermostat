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
#include "store.h"
#include "stmflash.h"


uint8 zigbee_alive = 0;
uint8 ScheduleModeNum = 0;
extern u32 Instance;

#ifdef TSTAT_ZIGBEE
uint8 read_zgb_index = 0;
uint8 zigbee_rssi = 0;
uint8 zigbee_index = 0;
uint8 zigbee_point_info[48];
#endif

extern int16 gsiPara;
uint8 showidbit = 0;
uint8 showid_cnt = 0;
int16 ctest[10];
extern uint8 pid_flag;
extern et_mode_of_operation   intended_mode_of_operation[3]; 
extern uint16 pir_value;
extern int16 ao1_fdbk;
extern int16 ao2_fdbk;
//uint8 bdtest1 = 0,bdtest2=0;
//extern int16 ctestpid;
extern uint16  xdata output_temp ;
uint8 update_flag = 0;
void Timer_Silence_Reset( void);
uint8 dec_blink = 0;
uint8 scan_enable =1;
uint16 scan_timer = 0;
//uint8 dealwith_package = 0;
uint8 randval;
uint8 rand_read_ten_count;
extern uint8 first_output;
//extern uint16 current_temp; 
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

uint16 xdata cooling_db;
uint16 xdata heating_db;
extern uint8 init_PID_flag; 	// Initialize the PID flag, stops reset windup problems
int16 xdata analog_input[4];

/*****************use these variables for testing****************/
uint8 SN_WR_Flag = 0; 
extern et_mode_of_operation  current_mode_of_operation[3];
extern PTBDSTR OUTPUT_1TO5;
extern PTBDSTR RELAY_1TO5;
extern int16  valve[3];
extern int16  valve_target_percent ; 

int8 test1;

extern uint8 xdata gucManualValve; 
int8 HUM_CAL_UNLOCK_FLAG = 0;

extern uint16 COOLING_MODE;
extern uint16 HEATING_MODE;
/****************************************************************/
uint8 xdata temp_info_byte;
uint8 idlockflag = NEED_TO_LOCK;
uint8 idlockcnt = 0;
//u8 laddress = 254;
//u8 serialnumber[4];
u8 ledtf= 0;
u8 USART_RX_BUFA[USART_REC_LEN];     
u8 USART_RX_BUFB[USART_REC_LEN];
u8 USART_RX_BUFC[50];     
u8 USART_RX_BUFD[50];
u8 USART_RX_BUFE[10];
u8 uart_send[USART_SEND_LEN];
u8 uart_sendB[USART_SEND_LEN];
u8 uart_sendC[9] = {0xFF, 0x01, 0x86, 0x00,0x00,0x00,0x00,0x00,0X79}/* MHZ19B */ ;//{0xff, 0x03, 0x00, 0x6b, 0x00, 0x01, 0xe0, 0x08} temco co2 module;
//vu8 transmit_finished = 0 ; 
vu8 rece_count = 0;
vu8 rece_countB = 0;
vu8 rece_countC = 0;
vu8 rece_size = 0 ;
vu8 rece_sizeB = 0 ;
vu8 serial_receive_timeout_count ;
vu8 serial_receive_timeout_countB = 0;
u8 SERIAL_RECEIVE_TIMEOUT ;
u8 dealwithTag ;
u8 dealwithTagB = 0 ;
STR_MODBUS modbus ;
//u8 DealwithTag ;
u16 sendbyte_num = 0 ;
u16 sendbyte_numB = 0 ;
u16 sendbyte_numC = 0 ;
//u16 uart_num = 0 ;
u8 Station_NUM = 0;

//u8 serial_response_delay;
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

//uint8 rs485_zigbee = RS485_DISABLE;

uint8 bacnet_flag = 0;
uint8 bacnet_temp[7];
void USART1_IRQHandler(void)                	
{	
	static uint8 testbit;	
	u8 receive_buf ;
	uint8 i;
	static u16 send_count1 = 0 ;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)	//
	{

			 
			if(modbus.com_config[0] == MODBUS )	
			{
						if(rece_count < 50)
							USART_RX_BUFA[rece_count++] = USART_ReceiveData(USART1);//(USART1->DR);		//?áè??óê?μ?μ?êy?Y
						else
							serial_restart();
						
						if(rece_count == 1)
						{
							// This starts a timer that will reset communication.  If you do not
							// receive the full packet, it insures that the next receive will be fresh.
							// The timeout is roughly 7.5ms.  (3 ticks of the hearbeat)
							rece_size = 50;
							serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
							
//							if((USART_RX_BUFA[0] != 255) && (USART_RX_BUFA[0] != laddress) && (USART_RX_BUFA[0] != 0))
//							{
//								serial_restart();
//								return;
//							}
						}
						else if(rece_count == 4)
						{
							//check if it is a scan command
							if((((vu16)(USART_RX_BUFA[2] << 8) + USART_RX_BUFA[3]) == 0x0a) && (USART_RX_BUFA[1] == WRITE_VARIABLES))
							{
								rece_size = DATABUFLEN_SCAN;
								serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT+8;	
							}
						}
						else if(rece_count == 7)
						{
							if((USART_RX_BUFA[1] == READ_VARIABLES) || (USART_RX_BUFA[1] == WRITE_VARIABLES))
							{
								if(((vu16)(USART_RX_BUFA[2] << 8) + USART_RX_BUFA[3]) != 0x0a)
								{
								rece_size = 8;
								serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
								}
							}
							else if(USART_RX_BUFA[1] == MULTIPLE_WRITE_VARIABLES)
							{
								rece_size = USART_RX_BUFA[6] + 9;
								serial_receive_timeout_count = USART_RX_BUFA[6] + 8;
							}
							
							
//							else if(USART_RX_BUFA[0] == 0x55 && USART_RX_BUFA[1] == 0xff && USART_RX_BUFA[2] == 0x01 && USART_RX_BUFA[5] == 0x00 && USART_RX_BUFA[6] == 0x00)
//							{//bacnet protocal detected
//								rs485_zigbee = RS485_ENABLE;
//								USART_DeInit(UART4); 	
//								modbus.com_config[0] = BAC_MSTP;
//								write_eeprom(EEP_MODBUS_BACNET_SWITCH, BAC_MSTP);
//								Recievebuf_Initialize(0);							
//							}				
						}
						
						else if(rece_count == 6 && USART_RX_BUFA[1] == CHECKONLINE)
							{
//								rs485_zigbee = RS485_ENABLE;
//								USART_DeInit(UART4); 	
								
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
//								rs485_zigbee = RS485_ENABLE;
//								USART_DeInit(UART4); 	
							// full packet received - turn off serial timeout
//							if(dealwith_package == 0)//if dealwith_package !=0, previous package is not done	
//							{	

								if(checkCrc())
								{
								#ifndef TSTAT7_ARM							
								icon.cmnct_rcv = 1;	
								#endif
								if(EEP_DeadMaster != 0)								
									deadmaster_timer = EEP_DeadMaster * 60;							
								serial_receive_timeout_count = 0;
								for(i=0;i<rece_size;i++)
									{
									USART_RX_BUFB[i] = USART_RX_BUFA[i];
//									dealwith_package = 1;										
									}
									dealwithTag = SERIALDEALTIME_DELAY;		// making this number big to increase delay
									dec_blink = 1;
								}
							else
							{
							serial_restart();
							}
						}
		
			}
			else if(modbus.com_config[0] == BAC_MSTP )
			{
				receive_buf = USART_ReceiveData(USART1); 
				FIFO_Put(&Receive_Buffer0, receive_buf);
//				#ifndef TSTAT7_ARM							
//				icon.cmnct_rcv = 2;	
//				#endif
			}
	}

	else  if( USART_GetITStatus(USART1, USART_IT_TXE) == SET ) //Tansmit Data Register empty interrupt = transmit has finished
			{
				#ifndef TSTAT7_ARM							
				icon.cmnct_send = 1;	
				#endif	
			if((modbus.com_config[0] == MODBUS )||(modbus.com_config[0] == BAC_MSTP))
				{
				if( send_count1 >= sendbyte_num)
					{
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
            USART_ClearFlag(USART1, USART_FLAG_TC);
            
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
            send_count1 = 0;
            serial_restart();	
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
//--------------------------------------------------------------------
#ifdef TSTAT_ZIGBEE	
//uint8 bkstate = 0;
extern uint16 zigbee_reset_timer;
extern uint8 utest;
extern uint8 utest2;
void UART4_IRQHandler(void)                	//
	{		
//  
//	u8 receive_buf ;
	uint8 i;
	static u16 send_countB = 0 ;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)	//
	{
		zigbee_reset_timer = 0;
		zigbee_alive = 1;
		utest2++;
			if(1/*modbus.com_config[0] == MODBUS */)	
			{
		
						if(rece_countB < 50)
						{
							USART_RX_BUFC[rece_countB++] = USART_ReceiveData(UART4);//(USART1->DR);	
//						  if((USART_RX_BUFC[0] != 0xff)&&(USART_RX_BUFC[0] != laddress))
//							{
//								serial_restartB();
//							}
						}							
						else
							 serial_restartB();
						if(rece_countB == 1)
						{
							// This starts a timer that will reset communication.  If you do not
							// receive the full packet, it insures that the next receive will be fresh.
							// The timeout is roughly 7.5ms.  (3 ticks of the hearbeat)
							rece_sizeB = 50;
							serial_receive_timeout_countB = SERIAL_RECEIVE_TIMEOUT;
						}
						else if(rece_countB == 4)
						{
							//check if it is a scan command
							if((((vu16)(USART_RX_BUFC[2] << 8) + USART_RX_BUFC[3]) == 0x0a) && (USART_RX_BUFC[1] == WRITE_VARIABLES))
							{
								rece_sizeB = DATABUFLEN_SCAN;
								serial_receive_timeout_countB = SERIAL_RECEIVE_TIMEOUT;	
							}
						}
						else if(rece_countB == 7)
						{
							if((USART_RX_BUFC[1] == READ_VARIABLES) || (USART_RX_BUFC[1] == WRITE_VARIABLES))
							{
								if(((vu16)(USART_RX_BUFC[2] << 8) + USART_RX_BUFC[3]) != 0x0a)
								{
								rece_sizeB = 8;
								serial_receive_timeout_countB = SERIAL_RECEIVE_TIMEOUT;
								}
							}
							else if(USART_RX_BUFC[1] == MULTIPLE_WRITE_VARIABLES)
							{
							
								rece_sizeB = USART_RX_BUFC[6] + 9;
								serial_receive_timeout_countB = USART_RX_BUFC[6] + 8;
							}
						}
						
						else if(rece_countB == 6 && USART_RX_BUFC[1] == CHECKONLINE)
							{
								if((scan_timer == 0)&& scan_enable)
								{
									utest = rece_sizeB;
									rece_sizeB = rece_countB;
									serial_receive_timeout_countB = 0;
									dealwithTagB = SERIALDEALTIME_DELAY;
									for(i=0;i<rece_sizeB;i++)
										{
										USART_RX_BUFD[i] = USART_RX_BUFC[i];									
										}	
								}
							}
						else if((rece_countB == rece_sizeB) && (USART_RX_BUFC[1] == READ_VARIABLES || USART_RX_BUFC[1] == WRITE_VARIABLES || USART_RX_BUFC[1] == MULTIPLE_WRITE_VARIABLES))
						{	
						
							if(checkCrcB())//&&((USART_RX_BUFC[0] == 255) || (USART_RX_BUFC[0] == laddress) || (USART_RX_BUFC[0] == 0)))
							{	
								#ifndef TSTAT7_ARM							
								icon.cmnct_rcv = 1;	
								#endif
															
							serial_receive_timeout_countB = 0;
							for(i=0;i<rece_sizeB;i++)
								{
								USART_RX_BUFD[i] = USART_RX_BUFC[i];										
								}
							dealwithTagB = 5;		// making this number big to increase delay
							}
							else
								serial_restartB();
						}
		
			}
//				else if(modbus.com_config[0] == BAC_MSTP )
//				{
//								receive_buf =  USART_ReceiveData(UART4); 
//								FIFO_Put(&Receive_Buffer0, receive_buf);
//				}
			}

	else  if( USART_GetITStatus(UART4, USART_IT_TXE) == SET ) //Tansmit Data Register empty interrupt = transmit has finished
			{		
			if(modbus.com_config[0] == MODBUS )/*||(modbus.com_config[0] == BAC_MSTP)*/
				{
					#ifndef TSTAT7_ARM							
							icon.cmnct_send = 1;	
							#endif
					
					
				if( send_countB >= sendbyte_numB)
					{
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
						USART_ClearFlag(UART4, USART_FLAG_TC);
            
            USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
            send_countB = 0;
            serial_restartB();	

					//Timer_Silence_Reset();			
					}
				else
					{
					USART_SendData(UART4, uart_sendB[send_countB++]);						
					//Timer_Silence_Reset( );
					}
				}					
			}
}


#else
void UART4_IRQHandler(void)                	//串口4中断服务程序
{	
//  static uint16 i;
//	uint8 bkstate;
//	i++;
//	if(i%2 == 1)
//	{
//	bkstate = ~bkstate;
//	if(bkstate)
//		GPIO_SetBits(GPIOA, GPIO_Pin_0);
//	else
//		GPIO_ResetBits(GPIOA, GPIO_Pin_0);	
//	}
	
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	
	return;
}
#endif //TSTAT_ZIGBEE	
//--------------------------------------------------------------------
#ifdef TSTAT_CO2
void USART3_IRQHandler(void)                	//
	{		
  uint8 checksum;
	uint8 i;
	uint8 temp;
	static uint8 rcv_flag = 0;
	static u16 send_countC = 0 ;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)	//
	{
		temp = USART_ReceiveData(USART3);
	 if((temp == 0xff)&&(rcv_flag == 0))
	 {
			rcv_flag = 1;
			USART_RX_BUFE[rece_countC++] = 0xff;
	 }
	 else if((rcv_flag == 1) && (temp == 0x86))
	 {
			rcv_flag = 2;
			USART_RX_BUFE[rece_countC++] = 0x86;
	 }
	 else if(rcv_flag == 2)
	 {
		USART_RX_BUFE[rece_countC++] = temp;
	 }
	 else 
	 {
		 rece_countC = 0;
		 rcv_flag=0;
	 }
	 
//		if(p[0] != 0xff)
//			return;
//		if(p[1] != 0x86)
//			return;
//		
//		for(i=0;i<8;i++)
//		{

//			checksum += p[i];
//		}
//	  checksum = 0xff - checksum;
//		checksum += 1;
//		//if(checksum == p[8])
//		co2_data_temp = p[2] * 256 + p[3];
		
   if(rece_countC >= 9)
	 {
		 checksum = 0;
		 for(i=0;i<8;i++)
		 {
			checksum += USART_RX_BUFE[i];
//			ctest[i] = USART_RX_BUFE[i];
		 }
		 
	  checksum = 0xff - checksum;
		checksum += 1;
//	   if(checksum == USART_RX_BUFE[8])
			co2_data = USART_RX_BUFE[2] * 256 + USART_RX_BUFE[3];
		 rece_countC = 0;
		  rcv_flag=0;
	 }
	
	}

	else  if( USART_GetITStatus(USART3, USART_IT_TXE) == SET ) //Tansmit Data Register empty interrupt = transmit has finished
			{		

				if( send_countC >= sendbyte_numC)
					{
						while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
						USART_ClearFlag(USART3, USART_FLAG_TC);
            
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
            send_countC = 0;	
					}
				else
					{
					USART_SendData(USART3, uart_sendC[send_countC++]);						
					}				
			}
}
	#endif //TSTAT_CO2 
//--------------------------------------------------------------------
void serial_restart(void)
{
	TXEN = RECEIVE;
	rece_count = 0;
	dealwithTag = 0;
	USART_ITConfig(USART1, USART_IT_RXNE/*|USART_IT_TC*/, ENABLE);				//开启中断
	
} 

#ifdef TSTAT_ZIGBEE
void serial_restartB(void)
{
	rece_countB = 0;
	dealwithTagB = 0;
	USART_ITConfig(UART4, USART_IT_RXNE/*|USART_IT_TC*/, ENABLE);	
} 
#endif//TSTAT_ZIGBEE

//it is ready to send data by serial port . 
static void initSend_COM(void)
{
	TXEN = SEND;
}

//void send_byte(u8 ch, u8 crc)
//{	
//	USART_ClearFlag(USART1, USART_FLAG_TC); 
//	USART_SendData(USART1,  ch);
//	if(crc)
//	{
//		crc16_byte(ch);
//	}
//}

//#ifdef TSTAT_ZIGBEE	
//void send_byteB(u8 ch, u8 crc)
//{	
//	USART_ClearFlag(UART4, USART_FLAG_TC); 
//	USART_SendData(UART4,  ch);	

//	if(crc)
//		crc16_byte(ch);
//}
//#endif //TSTAT_ZIGBEE	

 void USART_SendDataString( u16 num )
 {
	 sendbyte_num = num;
	 USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//
 }

 #ifdef TSTAT_ZIGBEE
 void USART_SendDataStringB( u16 num )
 {
	 sendbyte_numB = num;
	 USART_ITConfig(UART4, USART_IT_TXE, ENABLE);//	 	
 } 
 
 void modbus_initB(void)
{
	serial_restartB();
	SERIAL_RECEIVE_TIMEOUT = 10;
	serial_receive_timeout_countB = SERIAL_RECEIVE_TIMEOUT;
}
 #endif //TSTAT_ZIGBEE

#ifdef TSTAT_CO2

 void USART_SendDataStringC( u16 num )
 {
	 sendbyte_numC = num;
	 USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//	 	
 } 
#endif//TSTAT_CO2



void modbus_init(void)
{
	//uart1_init(19200);
	serial_restart();
	SERIAL_RECEIVE_TIMEOUT = 10;
	serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;
}





static void internalDeal(uint8  *bufadd)
{
	uint16 address;
	uint16 temp;
	//int16 temp_error;
	uint16  temp_i;
	int16 temp_j;
	uint8 i,j;
	uint8  hibyte,lobyte;
	
	//uart_p = bufadd;
	
	address = ((uint16)*(bufadd+2) <<8) + *(bufadd+3); //get modbus register number
	
if(*(bufadd+1) == MULTIPLE_WRITE_VARIABLES)
		{
		
	 		temp_i = (uint16)(*(bufadd+2) << 8) + *(bufadd+3);
			if(*(bufadd+6) > 24)
					*(bufadd+6) = 24;
			if(temp_i >= LINE1_CHAR1 && temp_i <= LINE2_CHAR4)
				{				
				for(i=0;i<*(bufadd+6);i++)			//(data_buffer[6)*2)
					{
					write_eeprom(EEP_LINE1_CHAR1 + i,*(bufadd+7+i));
					User_Info(i) = *(bufadd+7+i);	
					}						
				}

			else if(temp_i  >= INTERNAL_CHAR1 && temp_i <= OUTPUT7_CHAR4)
				{
				for(i=0;i<*(bufadd+6);i++)
					{
					write_eeprom((EEP_INTERNAL_CHAR1 + i + (temp_i - INTERNAL_CHAR1)*2),*(bufadd+7+i));
					if(temp_i == AI1_CHAR1)
						Disp_AI1(i) = *(bufadd+7+i);	
					else if(temp_i == AI2_CHAR1)
						Disp_AI2(i) = *(bufadd+7+i);	
					else if(temp_i == AI3_CHAR1)
						Disp_AI3(i) = *(bufadd+7+i);	
					else if(temp_i == AI4_CHAR1)
						Disp_AI4(i) = *(bufadd+7+i);	
					else if(temp_i == AI5_CHAR1)
						Disp_AI5(i) = *(bufadd+7+i);	
					else if(temp_i == AI6_CHAR1)
						Disp_AI6(i) = *(bufadd+7+i);	
					else if(temp_i == AI7_CHAR1)
						Disp_AI7(i) = *(bufadd+7+i);	
					else if(temp_i == AI8_CHAR1)
						Disp_AI8(i) = *(bufadd+7+i);	
					
					else if(temp_i == OUTPUT1_CHAR1)
						Disp_OUT1(i) = *(bufadd+7+i);
					else if(temp_i == OUTPUT2_CHAR1)
						Disp_OUT2(i) = *(bufadd+7+i);
					else if(temp_i == OUTPUT3_CHAR1)
						Disp_OUT3(i) = *(bufadd+7+i);	
					else if(temp_i == OUTPUT4_CHAR1)
						Disp_OUT4(i) = *(bufadd+7+i);	
					else if(temp_i == OUTPUT5_CHAR1)
						Disp_OUT5(i) = *(bufadd+7+i);	
					else if(temp_i == OUTPUT6_CHAR1)
						Disp_OUT6(i) = *(bufadd+7+i);	
					else if(temp_i == OUTPUT7_CHAR1)
						Disp_OUT7(i) = *(bufadd+7+i);											
					}					
				}

		   else if(temp_i  >= MODE1_NAME1 && temp_i <= MODE5_NAME4)
		   	{
				for(i=0;i<*(bufadd+6);i++)			//	(data_buffer[6)*2)
					{
					write_eeprom((EEP_MODE1_NAME1 + i + (temp_i - MODE1_NAME1)*2),*(bufadd+7+i));
					Modename((temp_i - MODE1_NAME1)*2 + i) = *(bufadd+7+i);
					}
				#ifndef TSTAT7_ARM
				icon.fan = 1;
				#endif	//nTSTAT7_ARM				
					
				}
			
		   else if(temp_i  >= TSTAT_NAME1 && temp_i <= TSTAT_NAME10)
		   	{
				for(i=0;i<*(bufadd+6);i++)			//	(data_buffer[6)*2)
					{
					write_eeprom((EEP_TSTAT_NAME1 + i /*+ (temp_i - TSTAT_NAME1)*2*/),*(bufadd+7+i));
					UserInfo_Name(i) = *(bufadd+7+i);
					}				
				}
					
		   else if(temp_i  >= RTC_YEAR && temp_i <= RTC_SECOND)
			  {
					update_flag = 10;
					RTC_Set(*(bufadd+8)+2000, *(bufadd+10), *(bufadd+14), *(bufadd+16), *(bufadd+18), *(bufadd+20));
//					SetRTCTime(EEP_CLOCK_YEAR, *(bufadd+8)+2000);
//					SetRTCTime(EEP_CLOCK_MONTH, *(bufadd+10));
//					SetRTCTime(EEP_CLOCK_DAY, *(bufadd+14));
//					SetRTCTime(EEP_CLOCK_HOUR, *(bufadd+16));
//					SetRTCTime(EEP_CLOCK_MINUTE, *(bufadd+18));
//					SetRTCTime(EEP_CLOCK_SECOND, *(bufadd+20));				
				}
			 
      			
		   else if(temp_i >= SCHEDULE_MONDAY_EVENT1_H && temp_i <= SCHEDULE_MONDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_MONDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleMondayEvent1(i) = *(bufadd+8+i*2);
					}						
				}	

			 else if(temp_i >= SCHEDULE_TUESDAY_EVENT1_H && temp_i <= SCHEDULE_TUESDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_TUESDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleTuesdayEvent1(i) = *(bufadd+8+i*2);
					}				
				}	
			 else if(temp_i >= SCHEDULE_WENSDAY_EVENT1_H && temp_i <= SCHEDULE_WENSDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_WENSDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleWensdayEvent1(i) = *(bufadd+8+i*2);
					}				
				}					
				
			 else if(temp_i >= SCHEDULE_THURSDAY_EVENT1_H && temp_i <= SCHEDULE_THURSDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_THURSDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleThursdayEvent1(i) = *(bufadd+8+i*2);
					}				
				}	
			 else if(temp_i >= SCHEDULE_FRIDAY_EVENT1_H && temp_i <= SCHEDULE_FRIDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_FRIDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleFridayEvent1(i) = *(bufadd+8+i*2);
					}				
				}	
			 else if(temp_i >= SCHEDULE_SATDAY_EVENT1_H && temp_i <= SCHEDULE_SATDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_SATDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleSatdayEvent1(i) = *(bufadd+8+i*2);
					}				
				}	
			 else if(temp_i >= SCHEDULE_SUNDAY_EVENT1_H && temp_i <= SCHEDULE_SUNDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_SUNDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleSundayEvent1(i) = *(bufadd+8+i*2);
					}				
				}
			 else if(temp_i >= SCHEDULE_HOLIDAY_EVENT1_H && temp_i <= SCHEDULE_HOLIDAY_EVENT6_M)
		   	{
				for(i=0;i<*(bufadd+6)/2;i++)			//(data_buffer[6)*2)
					{
					write_eeprom((EEP_SCHEDULE_HOLIDAY_EVENT1_H + i),*(bufadd+8+i*2));
					ScheduleHolidayEvent1(i) = *(bufadd+8+i*2);
					}				
				}				
		}	
	
	
	
	
	if(*(bufadd+1) == WRITE_VARIABLES)
		{
		if(address == SERIALNUMBER_LOWORD)
			{
			if((SN_WR_Flag & 0x01) == 0x01)//first two bytes of SN have been written
				return;
			else
				{
				SerialNumber(0) = *(bufadd+5);
				SerialNumber(1) = *(bufadd+4);
				SN_WR_Flag |= 0x01;	
				write_eeprom(EEP_SERIALNUMBER_LOWORD, *(bufadd+5));
				write_eeprom(EEP_SERIALNUMBER_LOWORD + 1, *(bufadd+4));
					
				flash_buf[0] = *(bufadd+5);
  			flash_buf[1] = *(bufadd+4);		
				STMFLASH_Write(FLASH_SERIAL_NUM_LO, flash_buf, 2);
				write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, SN_WR_Flag);
				}
			}
		else if(address == SERIALNUMBER_HIWORD)
			{
			if((SN_WR_Flag & 0x02) == 0x02)//first two bytes of SN have been written
				return;
			else
				{
				SerialNumber(2) = *(bufadd+5);
				SerialNumber(3) = *(bufadd+4);
				SN_WR_Flag |= 0x02;
				write_eeprom(EEP_SERIALNUMBER_HIWORD, *(bufadd+5));
				write_eeprom(EEP_SERIALNUMBER_HIWORD + 1, *(bufadd+4));					
				write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, SN_WR_Flag);
				flash_buf[0] = *(bufadd+5);
  			flash_buf[1] = *(bufadd+4);		
				STMFLASH_Write(FLASH_SERIAL_NUM_HI, flash_buf, 2);
				}
			}			
//	  else if(address == VERSION_NUMBER_HI)
//		{
//				write_eeprom(EEPROM_VERSION_NUMBER_HI, *(bufadd+5));
//				FirmwareVersion_HI = *(bufadd+5); 
//		}
		else if(address == TSTAT_ADDRESS)
			{
			if(*(bufadd+5) >= EEP_MinAddress && *(bufadd+5) <= EEP_MaxAddress)		 
				{ 
				if((idlockflag == NEED_TO_UNLOCK)||(ID_Lock == ID_WRITE_ENABLE))
					{
					ID_Lock = ID_WRITE_ENABLE;//after ID write operation, lock ID automatically
					write_eeprom(EEP_ID_WRITE_ENABLE, ID_WRITE_ENABLE);
					write_eeprom(EEP_ADDRESS, *(bufadd+5) );
					flash_buf[0] = *(bufadd+5);	
					STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);	
						
					laddress = *(bufadd+5);	
					Station_NUM = *(bufadd+5);//bacnet station number the same as modbus ID
					ID_Lock = ID_WRITE_DISABLE;//after ID write operation, lock ID automatically
					write_eeprom(EEP_ID_WRITE_ENABLE, ID_WRITE_DISABLE);
					//idlockflag = NEED_TO_LOCK;
					}
				else
					idlockflag = NEED_TO_UNLOCK;

				 idlockcnt = 0;
				}
			}

//		else if(address == PRODUCT_MODEL)
//			{
//			if((SN_WR_Flag & 0x04) == 0x04)//first two bytes of SN have been written
//				return;
//			else
//				{
//				ProductModel = *(bufadd+5);
//				write_eeprom(EEP_PRODUCT_MODEL, PM_TSTAT8);					
//				SN_WR_Flag |= 0x04;	
//				write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, SN_WR_Flag);
//				}
//			}	
			
		else if(address == UPDATE_STATUS)
		{
			UpdateState = *(bufadd+5);
			if(UpdateState == 142)
			{
				
				initialize_eeprom( ) ; // set default parameters
				write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, 0);
				SN_WR_Flag = 0;	
		//		DisRestart( );
			}
			else if(UpdateState == 159)//TBD:save the user table to EEPROM
			{
				save_user_def();
				DisRestart();
			}
			
			else if(UpdateState == 127)//go to ISP mode
			{
			//	vTaskSuspendAll();	
				write_eeprom(EEP_UPDATE_STATUS, UpdateState);
        update_flag = 1;				
			}
		}			
			

		else if(address == HARDWARE_REV)
			{
			if((SN_WR_Flag & 0x08) == 0x08)//first two bytes of SN have been written
				return;
			else
				{
				HardwareVersion = *(bufadd+5);
				write_eeprom(EEP_HARDWARE_REV, HardwareVersion);					
				SN_WR_Flag |= 0x08;
				write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, SN_WR_Flag);					
				}
			}	
			
		else if(address == BASE_ADDRESS)
			{
			EEP_BaseAddress = *(bufadd+5);
			write_eeprom(EEP_BASE_ADDRESS, EEP_BaseAddress);				 	
			}				

		else if(address == HARDWARE_INFORMATION)
			{
			Hardware_Info_HI = *(bufadd+4);
			Hardware_Info_LO = *(bufadd+5);
			write_eeprom(EEP_HARDWARE_INFORMATION, Hardware_Info_LO);
			write_eeprom(EEP_HARDWARE_INFORMATION + 1, Hardware_Info_HI);	
			}
			
		else if(address == PLUG_N_PLAY)	
		{
			if(*(bufadd+4) == 0x55)//special plug and play write command
			{
			laddress = *(bufadd+5); 	
			write_eeprom(EEP_ADDRESS, *(bufadd+5));
			flash_buf[0] = laddress;	
			STMFLASH_Write(FLASH_MODBUS_ID, flash_buf, 1);				
			}
			else if(*(bufadd+4) == 0x66)//disable to response scan request for a few minutes and reset the timer
			{
				if(*(bufadd+5) != 0)
					{
					scan_timer = *(bufadd+5) * 60;
					scan_enable = 1;
					}						
				else
					scan_enable = 0;	
			}
			else if(*(bufadd+4) == 0x77)//special plug and play write command
			{
				scan_timer = 0;
				scan_enable = 1;
			}			
			else
			{
				Plugandplay = *(bufadd+5);
				write_eeprom(ADDRESS_PLUG_N_PLAY, *(bufadd+5));
			}
		}			
			
			
		else if(address == COOLHEATMODE)
		{
//			if(EEP_HeatCoolConfig == HC_CTL_USER)
//			{
				heat_cool_user = *(bufadd+5);
				write_eeprom(EEP_HC_USER, heat_cool_user);
			  write_eeprom(EEP_HEAT_COOL_CONFIG, heat_cool_user);
			#ifndef TSTAT7_ARM
			  icon.heatcool = 1;
				icon.sysmode = 1;
			#endif
//			}
		}			
			

//		else if(address == MODBUS_BACNET_SWITCH)
//			{
//				if(*(bufadd+5) == BAC_MSTP)
//					{
//					modbus.com_config[0] = BAC_MSTP;
//					write_eeprom(EEP_PROTOCOL_SEL, 1);
//					//uart1_init(19200);
//					Recievebuf_Initialize(0);
//					}
//				if(*(bufadd+5) == MODBUS)//switch to modbus protocol
//				{
//					modbus.com_config[0] = MODBUS;
//					write_eeprom(EEP_PROTOCOL_SEL, 0);
//					if(EEP_Baudrate == BAUDRATE_19200)
//						uart1_init(19200);
//					else if(EEP_Baudrate == BAUDRATE_9600)
//						uart1_init(9600);
//					else if(EEP_Baudrate == BAUDRATE_38400)
//						uart1_init(38400);
//					else if(EEP_Baudrate == BAUDRATE_57600)
//						uart1_init(57600);
//					else if(EEP_Baudrate == BAUDRATE_115200)
//						uart1_init(115200);
//					else if(EEP_Baudrate == BAUDRATE_76800)
//						uart1_init(76800);
//				}				
//			}					

		else if(address == SEQUENCE)
			{
			Sys_Sequence = *(bufadd+5);
			write_eeprom(EEP_SEQUENCE, Sys_Sequence);
			sequence_of_operation = Sys_Sequence;
			}					

		else if(address == DEGC_OR_F)
			{
				if(EEP_DEGCorF != *(bufadd+5))
				{
					EEP_DEGCorF = *(bufadd+5);
					write_eeprom(EEP_DEGC_OR_F, EEP_DEGCorF);
					#ifndef TSTAT7_ARM
					icon.unit = 1;	
					#endif
					ConverteCF();
				}
			}

		else if(address == FAN_MODE)
			{
			EEP_FanMode = *(bufadd+5);
			write_eeprom(EEP_FAN_MODE, EEP_FanMode);
			}

		else if(address == POWERUP_MODE)
			{
			EEP_PowerupOnOff = *(bufadd+5);
			write_eeprom(EEP_POWERUP_ON_OFF, EEP_PowerupOnOff);
			}			

		else if(address == AUTO_ONLY)
			{
			EEP_AutoOnly = *(bufadd+5);
			write_eeprom(EEP_AUTO_ONLY, EEP_AutoOnly);
			}		

		else if(address == FACTORY_DEFAULTS)
			{
				if(*(bufadd+5) == 1)
				{	
					update_flag = 2;				
				}
			}					
			
		else if(address == INFO_BYTE)
			{
			if (AI_Function1 != 4 && AI_Function2 != 4)
								{
								// Only do something if a change was made

									// if bool 0 = 1, the tstat should be in occupied mode.
									if ((*(bufadd+5) & 0x01))
										{
										occupied = 1;
										info_byte |= 0x01;
									  if(fan_speed_user == FAN_OFF)
											if(EEP_OverrideTimer != 0)
												override_timer_time = (uint32)EEP_OverrideTimer * 60;
									//	Also switch the Fan to AUTO when changing to occupied mode
										fan_speed_user = FAN_AUTO;
										EEP_FanSpeed = fan_speed_user;	
										fanspeedbuf = FAN_AUTO;	
										backup_fan_speed = FAN_AUTO;
										//ResetTimer();
										}
									// if bool 0 = 0, the tstat should be in unoccupied mode.
									else				
										{
//										if(temp_info_byte != 0x05) 
//											{
											occupied = 0;
											info_byte &= 0xFE;
											// Also switch the Fan to OFF when changing to unoccupied mode
											fan_speed_user = FAN_OFF;
											EEP_FanSpeed = FAN_OFF;	
											fanspeedbuf = FAN_OFF;
											override_timer = OFF;
											override_timer_time = 0;
											backup_fan_speed = EEP_FanSpeed;
//										}
									  }
										#ifndef TSTAT7_ARM
										icon.fan = 1;
										icon.occ_unocc = 1;
										icon.setpoint = 1;
										#endif
	//write value 5 to info byte when start load file,write value 0 to info byte  after load file
	// we do not changge occupied mode and fan speed user after load file	
	//								if(temp_info_byte==0x05 && data_buffer[5) == 0x00)
	//								{	
	//									if(b.eeprom[EEP_FAN_SPEED -  MAXEEPCONSTRANGE) != 0)
	//									{
	//										occupied = 1 ;
	//										info_byte = 0x01;	
	//										fan_speed_user = b.eeprom[EEP_FAN_SPEED -  MAXEEPCONSTRANGE);
	//									}
	//								}
								 
									
									write_eeprom(EEP_INFO_BYTE,info_byte);
								    //if(b.eeprom[EEP_FAN_SPEED -  MAXEEPCONSTRANGE) != 0)
									//backup_fan_speed = 	b.eeprom[EEP_FAN_SPEED -  MAXEEPCONSTRANGE);
									EEP_FanSpeed = fan_speed_user;
									write_eeprom(EEP_FAN_SPEED,fan_speed_user);
	
									//Return here because we do not want the tstat to be inadvertently 
									//reset if the occupied mode is being changed.
								//	return;

								}
						 
						temp_info_byte = *(bufadd+5) ;
							// Only do something if a change was made
						if ((*(bufadd+5) & 0x04) != (info_byte & 0x04))
							{
								// THEN CHECK THE RESET PREVENTION BIT
							if ((*(bufadd+5) & 0x04))
								{
								info_byte |= 0x04;
								write_eeprom(EEP_INFO_BYTE,info_byte);
								}
							else  // If clearing bit2, initialize eeprom and restart the tstat
								{
									//TBD 
//								info_byte &= 0xFB;
//								write_eeprom(EEP_INFO_BYTE,info_byte);
//								responseData();
								initialize_eeprom( ) ; 
								DisRestart( );
//								one_second_pause();
//								while(1) ;
								}
							}
						if((*(bufadd+5) & 0x20))
							{
							info_byte = info_byte | 0x20;
							gucResponseDelay = 4;
							}
						else
							{
							info_byte = info_byte & 0xdf;
							gucResponseDelay = 2;
							}
	//					//cc 01-14-2015 since no RS485/WIRELESS selection, not used this bool	
	//					if((data_buffer[5) & 0x80))
	//						{
	//						gbDisableRs485 = 1;  //DISABLE RS485 
	//						RS232STATE = SEND;
	//						info_byte |= 0x80;
	//						//write_eeprom(EEP_INFO_BYTE,info_byte);
	//
	//						}
	//					else 
	//						{
	//						RS232STATE = RECEIVE;
	//						gbDisableRs485 = 0;
	//						info_byte &= 0x7f;
	//						///	write_eeprom(EEP_INFO_BYTE,info_byte);
	//						}
	
						write_eeprom(EEP_INFO_BYTE,info_byte);
			}	
			
		else if(address == BAUDRATE)
			{
			EEP_Baudrate = *(bufadd+5);
			write_eeprom(EEP_BAUDRATE, EEP_Baudrate);
			update_flag = 4;		
				
			}					
			
		else if(address == TSTAT_OVERRIDE_TIMER)
			{
			EEP_OverrideTimer = *(bufadd+5);
			write_eeprom(EEP_OVERRIDE_TIMER,EEP_OverrideTimer);				
			override_timer_time = (uint32)60*(*(bufadd+5));
			}				
			
			
		else if(address == HEAT_COOL_CONFIG)
			{
				write_eeprom(EEP_HEAT_COOL_CONFIG,*(bufadd+5));
				EEP_HeatCoolConfig = *(bufadd+5);
//			if (*(bufadd+5) == HC_CTL_AI1)
//				{
//				EEP_AnalogIn1 = 1;
//				write_eeprom(EEP_ANALOG1_RANGE,EEP_AnalogIn1);
//				}
//				
//			// When setting HC to an analog input mode, make sure the corresponding AI is set to 1								
//			if (*(bufadd+5) == HC_ANALOG_IN2 && EEP_AnalogIn2 != 1)
//				{
//				EEP_AnalogIn2 = 1;
//				write_eeprom(EEP_ANALOG2_RANGE,EEP_AnalogIn2);
//				}
			}				

		else if(address == TIMER_ON)
			{
			EEP_TimerOnHi = *(bufadd+4);
			EEP_TimerOn = *(bufadd+5);
			write_eeprom(EEP_TIMER_ON + 1,*(bufadd+4));
			write_eeprom(EEP_TIMER_ON,*(bufadd+5));
			if(*(bufadd+4) != 0 || *(bufadd+5) != 0)
			{
				
				if(EEP_TimerSelect  == 1 || EEP_TimerSelect == 2)
				{
					write_eeprom(EEP_TIMER_OFF + 1,*(bufadd+4));
					EEP_TimerOffHi = *(bufadd+4);
					write_eeprom(EEP_TIMER_OFF,*(bufadd+5));
					EEP_TimerOff = *(bufadd+5);
				
				}
				time_counter = (uint16)(EEP_TimerOffHi << 8) + EEP_TimerOff;
				timer_switch_status[0] = 0;									 
				timer_changed_status = 1;
				if(time_counter > 0)
					timer_enable = 1;
				if(EEP_TimerSelect == 3)
				{
					timer_enable = 0;
					mode_stage_changed[0] = 1;
					mode_stage_changed[1] = 1;
				}
				CalDesireTime(time_counter,EEP_TimerUnits,1);
			
			}
			else
				timer_enable = 0;
			}	

		else if(address == TIMER_OFF)
			{
				EEP_TimerOffHi = *(bufadd+4);
				EEP_TimerOff = *(bufadd+5);
				write_eeprom(EEP_TIMER_OFF+ 1,*(bufadd+4));
				write_eeprom(EEP_TIMER_OFF,*(bufadd+5));
				if(*(bufadd+4) != 0 || *(bufadd+5) != 0)
	
							{
								if(EEP_TimerSelect == 1 || EEP_TimerSelect == 2)
								{
									write_eeprom(EEP_TIMER_ON + 1,*(bufadd+4));
									EEP_TimerOnHi = *(bufadd+4);
									write_eeprom(EEP_TIMER_ON,*(bufadd+5));
									EEP_TimerOn = *(bufadd+5);
								
								}
								time_counter = (uint16)(EEP_TimerOffHi << 8) + EEP_TimerOff;
								timer_switch_status[0] = 0;
							 
								timer_changed_status = 1;
								if(EEP_TimerOnHi != 0 || EEP_TimerOn != 0)
								timer_enable = 1;
								if(EEP_TimerSelect == 3)
								{
									timer_enable = 0;
									mode_stage_changed[0] = 1;
									mode_stage_changed[1] = 1;
								}
								CalDesireTime(time_counter,EEP_TimerUnits,1);
						
							}
							else
								timer_enable = 0;
			}				
			
		else if(address == TIMER_UNITS)
			{
				EEP_TimerSelect =  *(bufadd+5);
				if((EEP_TimerOnHi == 0 && EEP_TimerOn == 0) || (EEP_TimerOffHi == 0 && EEP_TimerOff == 0))
					timer_enable = 0; 
				else
				{
					if(EEP_TimerSelect != 3)
						timer_enable = 1;
					else
					{
						timer_enable = 0;
						mode_stage_changed[0] = 1;
						mode_stage_changed[1] = 1;
					}
				 
					timer_changed_status = 1;
					timer_switch_status[0] = 0;
					time_counter = (uint16)(EEP_TimerOffHi << 8) + EEP_TimerOff;
					CalDesireTime(time_counter,EEP_TimerUnits,1);				
				}
			}

		else if(address == DEAD_MASTER)
			{
			EEP_DeadMaster = *(bufadd+5);
			write_eeprom(EEP_DEAD_MASTER, EEP_DeadMaster);
			deadmaster_timer = EEP_DeadMaster * 60;
			deadmaster_triggered = 0;
			}	
						
		
//		else if(address == FREE_COOL_CONFIG)
//			{
//			//TBD
//			}				

//		else if(address == RS485_MODE)
//			{
//			if(*(bufadd+5) == 100)
//				{
//				write_eeprom(RS485_WRITEFLAG, 0);
//				rs485_writebit = RS485_WRITE_ENABLE;
//				}
//			else
//				{
//				if(*(bufadd+5) == 1)
//					{
//					gbDisableRs485 = 1;  //DISABLE RS485 
//					RS232STATE = SEND;
//					info_byte |= 0x80;
//					write_eeprom(EEP_RS485_MODE,1);
//					}							
//				else 
//					{
//					RS232STATE = RECEIVE;
//					gbDisableRs485 = 0;
//					info_byte &= 0x7f;
//					write_eeprom(EEP_RS485_MODE,0);
//					}
//				write_eeprom(RS485_WRITEFLAG, 99);
//				rs485_writebit = RS485_WRITE_DISABLE;
//				}
//			}				

			else if(address == RS485_MODE)
			{
				if(*(bufadd+5) == 1)//zigbee mode
				{
					write_eeprom(EEP_RS485_MODE, 1);
					RS485_Mode = 1;
				}
				else if(*(bufadd+5) == 2)//auto mode
				{
					write_eeprom(EEP_RS485_MODE, 2);
					RS485_Mode = 2;
				}
				else//rs485 mode
				{
					write_eeprom(EEP_RS485_MODE, 0);
					RS485_Mode = 0;
				}
				update_flag = 9;
			}
			
			else if(address == MODBUS_BACNET_SWITCH)
			{
				#ifdef ZIGBEE_MODE_DEBUG
				for(j=0;j<8;j++)
				{
					write_eeprom(BAC_TEST1+j, *(bufadd+j) );
				}
				#endif
				 if(*(bufadd+5) == 1)//BACNET PROTOCOL
					{
						if(zigbee_alive == 0)
						{
							modbus.com_config[0] = BAC_MSTP;
							write_eeprom(EEP_PROTOCOL_SEL, 1);
							//uart1_init(19200);
							Recievebuf_Initialize(0);
						}
					}
					if(*(bufadd+5) == 0)//switch to modbus protocol
					{
						modbus.com_config[0] = MODBUS;
						write_eeprom(EEP_PROTOCOL_SEL, 0);
						if(EEP_Baudrate == BAUDRATE_19200)
							uart1_init(19200);
						else if(EEP_Baudrate == BAUDRATE_9600)
							uart1_init(9600);
						else if(EEP_Baudrate == BAUDRATE_38400)
							uart1_init(38400);
						else if(EEP_Baudrate == BAUDRATE_57600)
							uart1_init(57600);
						else if(EEP_Baudrate == BAUDRATE_115200)
							uart1_init(115200);
						else if(EEP_Baudrate == BAUDRATE_76800)
							uart1_init(76800);
					}		
			}
		else if((address == TEMPRATURE_CHIP)||(address == INTERNAL_THERMISTOR))
		{
				if(GetByteBit(&b.eeprom[EEP_INTERNAL_SENSOR_MANUAL],0))
				{
					ManualInternalSensor_HI = *(bufadd+4);
					ManualInternalSensor_LO = *(bufadd+5);
//					write_eeprom(MANUAL_INTERNAL_HI, *(bufadd+4));
//					write_eeprom(MANUAL_INTERNAL_LO, *(bufadd+5));
				}
				else
				{
					temp = (int16)(*(bufadd+4) << 8) + *(bufadd+5);

					if((EEP_TempSelect == THERMISTER_TYPE2)||(EEP_TempSelect == THERMISTER_TYPE3))
					{
						internal_calibration = temp - temperature_org;
						write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR , internal_calibration&0xFF ) ;
						write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR + 1, internal_calibration>>8 ) ;
						Calibration_Internal_LO = internal_calibration&0xFF ;
						Calibration_Internal_HI = internal_calibration>>8; 
					}
				}
		}			

		else if(address == ANALOG1_RANGE)
			{
			write_eeprom(EEP_ANALOG1_RANGE,*(bufadd+5));
			EEP_AnalogIn1 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}	
			
		else if(address == ANALOG2_RANGE)	
			{
			write_eeprom(EEP_ANALOG2_RANGE,*(bufadd+5));
			EEP_AnalogIn2 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}				

		else if(address == ANALOG3_RANGE)	
			{
			write_eeprom(EEP_ANALOG3_RANGE,*(bufadd+5));
			EEP_AnalogIn3 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}	
		else if(address == ANALOG4_RANGE)	
			{
			write_eeprom(EEP_ANALOG4_RANGE,*(bufadd+5));
			EEP_AnalogIn4 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}	

		else if(address == ANALOG5_RANGE)	
			{
			write_eeprom(EEP_ANALOG5_RANGE,*(bufadd+5));
			EEP_AnalogIn5 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}	

		else if(address == ANALOG6_RANGE)	
			{
			write_eeprom(EEP_ANALOG6_RANGE,*(bufadd+5));
			EEP_AnalogIn6 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}	

		else if(address == ANALOG7_RANGE)	
			{
			write_eeprom(EEP_ANALOG7_RANGE,*(bufadd+5));
			EEP_AnalogIn7 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}	

		else if(address == ANALOG8_RANGE)	
			{
			write_eeprom(EEP_ANALOG8_RANGE,*(bufadd+5));
			EEP_AnalogIn8 = *(bufadd+5);
			table1_flag = MAX_INPUT_CHANNEL;
			table2_flag = MAX_INPUT_CHANNEL;
			}				


		else if((address >= ANALOG_INPUT1_VALUE) && (address <= ANALOG_INPUT8_VALUE))	
			{
			temp = (uint16)(*(bufadd+4) << 8) + *(bufadd+5);

			if(GetByteBit(&EEP_InputManuEnable,(address - ANALOG_INPUT1_VALUE)))
				{ 
				//mul_analog_input[address - ANALOG_INPUT1_VALUE) = temp;//
				write_eeprom((MANUAL_ANALOG1_HI + (address-ANALOG_INPUT1_VALUE)*2), *(bufadd+4)); 
				write_eeprom((MANUAL_ANALOG1_LO + (address-ANALOG_INPUT1_VALUE)*2) , *(bufadd+5));
				ManualAI_HI(address - ANALOG_INPUT1_VALUE) = *(bufadd+4);
				ManualAI_LO(address - ANALOG_INPUT1_VALUE) = *(bufadd+5);						
				}
			else
				{
					mul_analog_cal[address - ANALOG_INPUT1_VALUE] =  temp - mul_analog_in_buffer[address - ANALOG_INPUT1_VALUE];
					write_eeprom(EEP_CALIBRATION_ANALOG1 + (address - ANALOG_INPUT1_VALUE)*2, mul_analog_cal[address - ANALOG_INPUT1_VALUE]&0xFF );
					write_eeprom(EEP_CALIBRATION_ANALOG1 + (address - ANALOG_INPUT1_VALUE)*2 + 1, (mul_analog_cal[address - ANALOG_INPUT1_VALUE]>> 8) & 0xff );
//				if(mul_analog_cal[address - ANALOG_INPUT1)+temp_error > 0 && mul_analog_cal[address - ANALOG_INPUT1]+temp_error  < 1000)
//					{
//					mul_analog_cal[address - ANALOG_INPUT1] += temp_error;
//					write_eeprom(EEP_CALIBRATION_ANALOG1 + (address - ANALOG_INPUT1)*2, mul_analog_cal[address - ANALOG_INPUT1]&0xFF );
//					write_eeprom(EEP_CALIBRATION_ANALOG1 + (address - ANALOG_INPUT1)*2 + 1, (mul_analog_cal[address - ANALOG_INPUT1]>> 8) & 0xff );
//					}
				}			
			}

//		else if(address == EXTERNAL_SENSOR1)	
//			{
//			temp = ((int16)*(bufadd+4)<<8) + *(bufadd+5);
//			if(temp > mul_analog_in_buffer[9])
//				co2_calibration_data = temp - mul_analog_in_buffer[9); //positive value
//			else
//				co2_calibration_data = mul_analog_in_buffer[9) - temp + 3000; //negtive value
//			
//			write_eeprom((EEP_CO2_CALIBRATION + 1),(co2_calibration_data>>8)& 0xff);
//			write_eeprom(EEP_CO2_CALIBRATION, (co2_calibration_data&0xff)); 
//			}	

		else if(address == EXTERNAL_SENSOR2)	
			{
			temp = ((uint16)*(bufadd+4)<<8) + *(bufadd+5);
			humidity_calibration = temp - org_humidity;
			write_eeprom(EEP_HUM_CALIBRATION, humidity_calibration & 0Xff);
			write_eeprom(EEP_HUM_CALIBRATION + 1, humidity_calibration>>8);
			}				

		else if(address == INPUT_MANU_ENABLE)	
			{
			//b.eeprom[EEP_INPUT_MANU_ENABLE - MAXEEPCONSTRANGE + 1) = data_buffer[4);
			EEP_InputManuEnable = *(bufadd+5);
			write_eeprom(EEP_INPUT_MANU_ENABLE, EEP_InputManuEnable);				
			}	

		else if(address == FILTER)	
			{
			EEP_Filter = *(bufadd+5);
			write_eeprom(EEP_FILTER,*(bufadd+5));							
			}				

		else if((address >= INPUT1_FILTER)	&& (address <= INPUT8_FILTER))
			{
			write_eeprom(EEP_INPUT1_FILTER + address - INPUT1_FILTER ,*(bufadd+5));							
			InputFilter(address - INPUT1_FILTER) = *(bufadd+5);						
			}			

		else if(address == CALIBRATION_INTERNAL_THERMISTOR)
			{
				#ifdef TSTAT7_ARM
				if((*(bufadd+4) * 256 + *(bufadd+5) >= 500) && (*(bufadd+4) * 256 + *(bufadd+5) <= 1000))
				{
					temp_j = *(bufadd+4) * 256 + *(bufadd+5) - 500;
					hibyte = (temp_j >> 8)& 0xff;
					lobyte = temp_j & 0xff;
				}
				else if((*(bufadd+4) * 256 + *(bufadd+5) < 500) && ((*(bufadd+4) * 256 + *(bufadd+5)) >= 200))
				{
					temp_j = *(bufadd+4) * 256 + *(bufadd+5);
					temp_j -= 500;
					hibyte = (temp_j >> 8)& 0xff;
					lobyte = temp_j & 0xff;
				}
				else
				{
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);	
				}
					
				#else
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);				
				
				#endif
			Calibration_Internal_HI = hibyte;
			Calibration_Internal_LO = lobyte;	
			write_eeprom(EEP_CALIBRATION_INTERNAL_THERMISTOR ,lobyte);							
			write_eeprom(EEP_CALIBRATION_INTERNAL_THERMISTOR + 1 ,hibyte);					
			internal_calibration = eep_get_value(Calibration_Internal_HI, Calibration_Internal_LO);
			}		

		else if(address == CALIBRATION_ANALOG1)
			{
				#ifdef TSTAT7_ARM
				if((AI_Range(0) == AI_RANGE_10K_THERMISTOR_TYPE2) || (AI_Range(0) == AI_RANGE_10K_THERMISTOR_TYPE3) || (AI_Range(0) == AI_RANGE_50K_THERMISTOR))
				{
					if((*(bufadd+4) * 256 + *(bufadd+5) >= 500) && (*(bufadd+4) * 256 + *(bufadd+5) <= 1000))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5) - 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else if((*(bufadd+4) * 256 + *(bufadd+5) < 500) && ((*(bufadd+4) * 256 + *(bufadd+5)) >= 200))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5);
						temp_j -= 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else
					{
						hibyte =  *(bufadd+4);
						lobyte =  *(bufadd+5);
					}

				}
				else
				{
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);
				}
				#else
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);						
				#endif
			Calibration_AI1_HI = hibyte;
			Calibration_AI1_LO = lobyte;	
			write_eeprom(EEP_CALIBRATION_ANALOG1 ,lobyte);							
			write_eeprom(EEP_CALIBRATION_ANALOG1 + 1 ,hibyte);	
			mul_analog_cal[0] = ((int16)Calibration_AI1_HI<<8) + Calibration_AI1_LO;	
			}		

		else if(address == CALIBRATION_ANALOG2)
			{
				#ifdef TSTAT7_ARM
				if((AI_Range(1) == AI_RANGE_10K_THERMISTOR_TYPE2) || (AI_Range(1) == AI_RANGE_10K_THERMISTOR_TYPE3) || (AI_Range(1) == AI_RANGE_50K_THERMISTOR))
				{
					if((*(bufadd+4) * 256 + *(bufadd+5) >= 500) && (*(bufadd+4) * 256 + *(bufadd+5) <= 1000))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5) - 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else if((*(bufadd+4) * 256 + *(bufadd+5) < 500) && ((*(bufadd+4) * 256 + *(bufadd+5)) >= 200))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5);
						temp_j -= 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
          else
					{
						hibyte =  *(bufadd+4);
						lobyte =  *(bufadd+5);					
					}
				}
				else
				{
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);
				}
				#else
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);					
				
				#endif				
			Calibration_AI2_HI = hibyte;
			Calibration_AI2_LO = lobyte;	
			write_eeprom(EEP_CALIBRATION_ANALOG2 ,lobyte);							
			write_eeprom(EEP_CALIBRATION_ANALOG2 + 1 ,hibyte);	
			mul_analog_cal[1] = ((int16)Calibration_AI2_HI<<8) | Calibration_AI2_LO;				
			}		

		else if(address == CALIBRATION_ANALOG3)
			{
				#ifdef TSTAT7_ARM
				if((AI_Range(2) == AI_RANGE_10K_THERMISTOR_TYPE2) || (AI_Range(2) == AI_RANGE_10K_THERMISTOR_TYPE3) || (AI_Range(2) == AI_RANGE_50K_THERMISTOR))
				{
					if((*(bufadd+4) * 256 + *(bufadd+5) >= 500) && (*(bufadd+4) * 256 + *(bufadd+5) <= 1000))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5) - 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else if((*(bufadd+4) * 256 + *(bufadd+5) < 500) && ((*(bufadd+4) * 256 + *(bufadd+5)) >= 200))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5);
						temp_j -= 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else
					{
						hibyte =  *(bufadd+4);
						lobyte =  *(bufadd+5);
					}
				}
				else
				{
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);
				}	
				#else
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);	
				#endif				
			Calibration_AI3_HI = hibyte;
			Calibration_AI3_LO = lobyte;	
			write_eeprom(EEP_CALIBRATION_ANALOG3 ,lobyte);							
			write_eeprom(EEP_CALIBRATION_ANALOG3 + 1 ,hibyte);	
			mul_analog_cal[2] = ((int16)Calibration_AI3_HI<<8) | Calibration_AI3_LO;				
			}	

		else if(address == CALIBRATION_ANALOG4)
			{
				#ifdef TSTAT7_ARM
				if((AI_Range(3) == AI_RANGE_10K_THERMISTOR_TYPE2) || (AI_Range(3) == AI_RANGE_10K_THERMISTOR_TYPE3) || (AI_Range(3) == AI_RANGE_50K_THERMISTOR))
				{
					if((*(bufadd+4) * 256 + *(bufadd+5) >= 500) && (*(bufadd+4) * 256 + *(bufadd+5) <= 1000))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5) - 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else if((*(bufadd+4) * 256 + *(bufadd+5) < 500) && ((*(bufadd+4) * 256 + *(bufadd+5)) >= 200))
					{
						temp_j = *(bufadd+4) * 256 + *(bufadd+5);
						temp_j -= 500;
						hibyte = (temp_j >> 8)& 0xff;
						lobyte = temp_j & 0xff;
					}
					else
					{
						hibyte =  *(bufadd+4);
						lobyte =  *(bufadd+5);
					}
					
				}
				else
				{
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);
				}
				#else
					hibyte =  *(bufadd+4);
					lobyte =  *(bufadd+5);	
				
				#endif				
			Calibration_AI4_HI = hibyte;
			Calibration_AI4_LO = lobyte;	
			write_eeprom(EEP_CALIBRATION_ANALOG4 ,lobyte);							
			write_eeprom(EEP_CALIBRATION_ANALOG4 + 1 ,hibyte);	
			mul_analog_cal[3] = ((int16)Calibration_AI4_HI<<8) | Calibration_AI4_LO;				
			}

		else if(address == CALIBRATION_ANALOG5)
			{
			Calibration_AI5_HI = *(bufadd+4);
			Calibration_AI5_LO = *(bufadd+5);	
			write_eeprom(EEP_CALIBRATION_ANALOG5 ,*(bufadd+5));							
			write_eeprom(EEP_CALIBRATION_ANALOG5 + 1 ,*(bufadd+4));	
			mul_analog_cal[4] = ((int16)Calibration_AI5_HI<<8) | Calibration_AI5_LO;				
			}					

		else if(address == CALIBRATION_ANALOG6)
			{
			Calibration_AI6_HI = *(bufadd+4);
			Calibration_AI6_LO = *(bufadd+5);	
			write_eeprom(EEP_CALIBRATION_ANALOG6 ,*(bufadd+5));							
			write_eeprom(EEP_CALIBRATION_ANALOG6 + 1 ,*(bufadd+4));
			mul_analog_cal[5] = ((int16)Calibration_AI6_HI<<8) | Calibration_AI6_LO;				
			}	

		else if(address == CALIBRATION_ANALOG7)
			{
			Calibration_AI7_HI = *(bufadd+4);
			Calibration_AI7_LO = *(bufadd+5);	
			write_eeprom(EEP_CALIBRATION_ANALOG7 ,*(bufadd+5));							
			write_eeprom(EEP_CALIBRATION_ANALOG7 + 1 ,*(bufadd+4));
			mul_analog_cal[6] = ((int16)Calibration_AI7_HI<<8) | Calibration_AI7_LO;				
			}	

		else if(address == CALIBRATION_ANALOG8)
			{
			Calibration_AI8_HI = *(bufadd+4);
			Calibration_AI8_LO = *(bufadd+5);	
			write_eeprom(EEP_CALIBRATION_ANALOG8 ,*(bufadd+5));							
			write_eeprom(EEP_CALIBRATION_ANALOG8 + 1 ,*(bufadd+4));	
			mul_analog_cal[7] = ((int16)Calibration_AI8_HI<<8) | Calibration_AI8_LO;				
			}	
						
//	HUMCOUNT1_H ,       //		    //  sensor frequency	   //these parameters must be continued,it won't work  if have blank,don't know why
//	HUMRH1_H	,          //				//  calibration point
//	HUMCOUNT2_H ,       //			//
//	HUMRH2_H	,          //

		else if(address == CALIBRATION_ANALOG8)
			{
			Calibration_AI8_HI = *(bufadd+4);
			Calibration_AI8_LO = *(bufadd+5);	
			write_eeprom(EEP_CALIBRATION_ANALOG6 ,*(bufadd+5));							
			write_eeprom(EEP_CALIBRATION_ANALOG6 + 1 ,*(bufadd+4));						
			}				

//		else if((address >= ANALOG1_FUNCTION) && (address <= ANALOG8_FUNCTION))
//			{
//			AI_function(address - ANALOG1_FUNCTION) = *(bufadd+5);							
//			write_eeprom(EEP_ANALOG1_FUNCTION + address - ANALOG1_FUNCTION  ,*(bufadd+5));						
//			}			
		else if(address == ANALOG1_FUNCTION)	
			{
			AI_Function1 = *(bufadd+5);
			write_eeprom(EEP_ANALOG1_FUNCTION,*(bufadd+5));							
			}		
			
		else if(address == ANALOG2_FUNCTION)	
			{
			AI_Function2 = *(bufadd+5);
			write_eeprom(EEP_ANALOG2_FUNCTION,*(bufadd+5));							
			}		

		else if(address == ANALOG3_FUNCTION)	
			{
			AI_Function3 = *(bufadd+5);
			write_eeprom(EEP_ANALOG3_FUNCTION,*(bufadd+5));							
			}		

		else if(address == ANALOG4_FUNCTION)	
			{
			AI_Function4 = *(bufadd+5);
			write_eeprom(EEP_ANALOG4_FUNCTION,*(bufadd+5));							
			}		

		else if(address == ANALOG5_FUNCTION)	
			{
			AI_Function5 = *(bufadd+5);
			write_eeprom(EEP_ANALOG5_FUNCTION,*(bufadd+5));							
			}		

		else if(address == ANALOG6_FUNCTION)	
			{
			AI_Function6 = *(bufadd+5);
			write_eeprom(EEP_ANALOG6_FUNCTION,*(bufadd+5));							
			}		

		else if(address == ANALOG7_FUNCTION)	
			{
			AI_Function7 = *(bufadd+5);
			write_eeprom(EEP_ANALOG7_FUNCTION,*(bufadd+5));							
			}		

		else if(address == ANALOG8_FUNCTION)	
			{
			AI_Function8 = *(bufadd+5);
			write_eeprom(EEP_ANALOG8_FUNCTION,*(bufadd+5));							
			}					
			
			
		else if(address == TABLE1_ZERO)
			{
			Table1_ZERO_LO = *(bufadd+5);
			Table1_ZERO_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_ZERO ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_ZERO + 1 ,*(bufadd+4));	
			if(((uint16)Table1_ZERO_HI << 8) + Table1_ZERO_LO < ((uint16)Table1_Five_LO << 8) + Table1_Five_LO)
					slope_type[0] = 1;
			else
				slope_type[0] = 0;				
			}				

		else if(address == TABLE1_HALFONE)
			{
			Table1_HalfOne_LO = *(bufadd+5);
			Table1_HalfOne_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_HALFONE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_HALFONE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE1_ONE)
			{
			Table1_One_LO = *(bufadd+5);
			Table1_One_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_ONE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_ONE + 1 ,*(bufadd+4));						
			}				

		else if(address == TABLE1_HALFTWO)
			{
			Table1_HalfTwo_LO = *(bufadd+5);
			Table1_HalfTwo_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_HALFTWO ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_HALFTWO + 1 ,*(bufadd+4));						
			}			

		else if(address == TABLE1_TWO)
			{
			Table1_Two_LO = *(bufadd+5);
			Table1_Two_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_TWO ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_TWO + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE1_HALFTHREE)
			{
			Table1_HalfThree_LO = *(bufadd+5);
			Table1_HalfThree_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_HALFTHREE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_HALFTHREE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE1_THREE)
			{
			Table1_Three_LO = *(bufadd+5);
			Table1_Three_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_THREE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_THREE + 1 ,*(bufadd+4));						
			}				


		else if(address == TABLE1_HALFFOUR)
			{
			Table1_HalfFour_LO = *(bufadd+5);
			Table1_HalfFour_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_HALFFOUR ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_HALFFOUR + 1 ,*(bufadd+4));						
			}			

		else if(address == TABLE1_FOUR)
			{
			Table1_Four_LO = *(bufadd+5);
			Table1_Four_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_FOUR ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_FOUR + 1 ,*(bufadd+4));						
			}

		else if(address == TABLE1_HALFFIVE)
			{
			Table1_HalfFive_LO = *(bufadd+5);
			Table1_HalfFive_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_HALFFIVE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_HALFFIVE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE1_FIVE)
			{
			Table1_Five_LO = *(bufadd+5);
			Table1_Five_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE1_FIVE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE1_FIVE + 1 ,*(bufadd+4));
			if(((uint16)Table1_ZERO_HI << 8) + Table1_ZERO_LO < ((uint16)Table1_Five_LO << 8) + Table1_Five_LO)
					slope_type[0] = 1;
			else
				slope_type[0] = 0;				
			}			

		else if(address == TABLE2_ZERO)
			{
			Table2_ZERO_LO = *(bufadd+5);
			Table2_ZERO_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_ZERO ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_ZERO + 1 ,*(bufadd+4));
			if(((uint16)Table2_ZERO_HI << 8) + Table2_ZERO_LO < ((uint16)Table2_Five_LO << 8) + Table2_Five_LO)
					slope_type[1] = 1;
			else
				slope_type[1] = 0;					
			}				

		else if(address == TABLE2_HALFONE)
			{
			Table2_HalfOne_LO = *(bufadd+5);
			Table2_HalfOne_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_HALFONE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_HALFONE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE2_ONE)
			{
			Table2_One_LO = *(bufadd+5);
			Table2_One_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_ONE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_ONE + 1 ,*(bufadd+4));						
			}				

		else if(address == TABLE2_HALFTWO)
			{
			Table2_HalfTwo_LO = *(bufadd+5);
			Table2_HalfTwo_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_HALFTWO ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_HALFTWO + 1 ,*(bufadd+4));						
			}			

		else if(address == TABLE2_TWO)
			{
			Table2_Two_LO = *(bufadd+5);
			Table2_Two_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_TWO ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_TWO + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE2_HALFTHREE)
			{
			Table2_HalfThree_LO = *(bufadd+5);
			Table2_HalfThree_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_HALFTHREE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_HALFTHREE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE2_THREE)
			{
			Table2_HalfThree_LO = *(bufadd+5);
			Table2_HalfThree_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_HALFTHREE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_HALFTHREE + 1 ,*(bufadd+4));						
			}				

		else if(address == TABLE2_THREE)
			{
			Table2_Three_LO = *(bufadd+5);
			Table2_Three_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_THREE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_THREE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE2_HALFFOUR)
			{
			Table2_HalfFour_LO = *(bufadd+5);
			Table2_HalfFour_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_HALFFOUR ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_HALFFOUR + 1 ,*(bufadd+4));						
			}			

		else if(address == TABLE2_FOUR)
			{
			Table2_Four_LO = *(bufadd+5);
			Table2_Four_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_FOUR ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_FOUR + 1 ,*(bufadd+4));						
			}

		else if(address == TABLE2_HALFFIVE)
			{
			Table2_HalfFive_LO = *(bufadd+5);
			Table2_HalfFive_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_HALFFIVE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_HALFFIVE + 1 ,*(bufadd+4));						
			}	

		else if(address == TABLE2_FIVE)
			{
			Table2_Five_LO = *(bufadd+5);
			Table2_Five_HI = *(bufadd+4);				
			write_eeprom(EEP_TABLE2_FIVE ,*(bufadd+5));							
			write_eeprom(EEP_TABLE2_FIVE + 1 ,*(bufadd+4));	
			if(((uint16)Table2_ZERO_HI << 8) + Table2_ZERO_LO < ((uint16)Table2_Five_LO << 8) + Table2_Five_LO)
					slope_type[1] = 1;
			else
				slope_type[1] = 0;					
			}	
			
		else if((address >= MODE_OUTPUT1) && (address <= MODE_OUTPUT5))
			{
				write_eeprom(EEP_RANGE_OUTPUT1 + address - MODE_OUTPUT1,*(bufadd+5));
				OutputRange(address - MODE_OUTPUT1) = *(bufadd+5);
				if((0<*(bufadd+5)) && (*(bufadd+5)<3))
				{
				output_floating_flag = OUTPUT_FLOATING;
//				TR2=1;
//				daytimer_ticks_buf=0;
//				start_timer(REFRESH_VALVE_POSITION_TIMER,DEFAULT_TIMEOUT);
//				command=INITIALIZE;
//				initialize_valve=1;
				}
			 else
				{
				output_floating_flag = OUTPUT_NO_FLOATING;
//				TR2=0;
//				stop_timer(REFRESH_VALVE_POSITION_TIMER);
				}
				//TBD: need to re-write this function
			}	
						
//	MODUBS_HUMIDITY_RH,	       //			// relative humidity in percentage
//	HUMIDITY_FREQUENCY, //		// raw frequency reading
//	HUM_PIC_UPDATE,     //			// write current calibration table to PIC, which table decided by register 427
//	HUM_CAL_NUM,	       //			// calibration data number
//	HUM_CURRENT_DEFAULT,//			// decide which table will run, default tabel or customer table   current=1 default=0	

		else if(address == OUTPUT1_SCALE)
			{
			EEP_Output1Scale = *(bufadd+5);				
			write_eeprom(EEP_OUTPUT1_SCALE,*(bufadd+5));						
			}			
			
		else if(address == OUTPUT2_SCALE)
			{
			EEP_Output2Scale = *(bufadd+5);				
			write_eeprom(EEP_OUTPUT2_SCALE,*(bufadd+5));						
			}				

		else if(address == TSTAT_DIGITAL_OUTPUT_STATUS)
			{
			write_eeprom(MANUAL_RELAY,*(bufadd+5));
			ManualRelayAll = *(bufadd+5);
			for(j=0;j<5;j++)
				{
				ManualRelay(j) = GetByteBit(&ManualRelayAll,j);
				write_eeprom(EEP_MANU_RELAY1+j, ManualRelay(j));	
				}
			}				

		else if(address == TSTAT_COOLING_VALVE)
			{
			if(GetByteBit(&EEP_OutputManuEnable,5))
				{
					ManualAO1_HI = *(bufadd+4);
					ManualAO1_LO = *(bufadd+5);
					
					write_eeprom(MANUAL_COOL_VALVE_HI,*(bufadd+4));
				
					write_eeprom(MANUAL_COOL_VALVE_LO,*(bufadd+5));
					
					set_output(TIM4,2, (uint16)*(bufadd+4)*256 + *(bufadd+5));
					first_output = 1;
//					current_temp = (uint16)*(bufadd+4)*256 + *(bufadd+5);
				}			
			}		
			
		else if(address == TSTAT_HEATING_VALVE)
			{
			if(GetByteBit(&EEP_OutputManuEnable,6))
				{
					ManualAO2_HI = *(bufadd+4);
					ManualAO2_LO = *(bufadd+5);
					
					write_eeprom(MANUAL_HEAT_VALVE_HI,*(bufadd+4));
				
					write_eeprom(MANUAL_HEAT_VALVE_LO,*(bufadd+5));
					set_output(TIM4,2, (uint16)*(bufadd+4)*256 + *(bufadd+5));
				}			
			}		
			
//	DAC_OFFSET,	           //					 // 0	255	DAC voltage offset
			
		else if((address >= OUTPUT1_DELAY_OFF_TO_ON) && (address <= OUTPUT7_DELAY_OFF_TO_ON))
			{
			Output_Delay_OfftoOn(address - OUTPUT1_DELAY_OFF_TO_ON) = *(bufadd+5);
			write_eeprom(EEP_OUTPUT1_DELAY_OFF_TO_ON + address - OUTPUT1_DELAY_OFF_TO_ON,*(bufadd+5));	
			}				

		else if((address >= OUTPUT1_DELAY_ON_TO_OFF) && (address <= OUTPUT7_DELAY_ON_TO_OFF))
			{
			Output_Delay_OntoOff(address - OUTPUT1_DELAY_ON_TO_OFF) = *(bufadd+5);
			write_eeprom(EEP_OUTPUT1_DELAY_ON_TO_OFF + address - OUTPUT1_DELAY_ON_TO_OFF,*(bufadd+5));	
			}		

		else if(address == CYCLING_DELAY)
			{
			CyclingDelay = *(bufadd+5);
			write_eeprom(EEP_CYCLING_DELAY,*(bufadd+5));	
			}			
		
		else if(address == CHANGOVER_DELAY)
			{
			ChangeOverDelay = *(bufadd+5);
			write_eeprom(EEP_CHANGOVER_DELAY,*(bufadd+5));	
			}		

		else if(address == VALVE_TRAVEL_TIME)
			{
			EEP_ValveTravelTime = *(bufadd+5);
			write_eeprom(EEP_VALVE_TRAVEL_TIME,*(bufadd+5));	
			}		
//TBD			
//		else if(address == VALVE_PERCENT)
//			{
//			if(*(bufadd+5) < 101)
//				{
//					gucManualValve = *(bufadd+5);
//					write_eeprom(MANUAL_VALVE,gucManualValve);			 
//				}
//			}		
			
		else if(address == INTERLOCK_OUTPUT1)
			{
			EEP_Interlock1 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT1,*(bufadd+5));	
			}		
		else if(address == INTERLOCK_OUTPUT2)
			{
			EEP_Interlock2 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT2,*(bufadd+5));	
			}	
		else if(address == INTERLOCK_OUTPUT3)
			{
			EEP_Interlock3 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT3,*(bufadd+5));	
			}	
		else if(address == INTERLOCK_OUTPUT4)
			{
			EEP_Interlock4 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT4,*(bufadd+5));	
			}	
		else if(address == INTERLOCK_OUTPUT5)
			{
			EEP_Interlock5 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT5,*(bufadd+5));	
			}	
		else if(address == INTERLOCK_OUTPUT6)
			{
			EEP_Interlock6 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT6,*(bufadd+5));	
			}	
		else if(address == INTERLOCK_OUTPUT7)
			{
			EEP_Interlock7 = *(bufadd+5);
			write_eeprom(EEP_INTERLOCK_OUTPUT7,*(bufadd+5));	
			}				
			
		else if(address == FREEZE_DELAY_ON)
			{
			EEP_FreezeDelayOn = *(bufadd+5);
			write_eeprom(EEP_FREEZE_DELAY_ON,*(bufadd+5));	
			}

		else if(address == FREEZE_DELAY_OFF)
			{
			EEP_FreezeDelayOff = *(bufadd+5);
			write_eeprom(EEP_FREEZE_DELAY_OFF,*(bufadd+5));	
			}				

		else if(address == OUTPUT_MANU_ENABLE)
			{
			EEP_OutputManuEnable = *(bufadd+5);
			write_eeprom(EEP_OUTPUT_MANU_ENABLE,*(bufadd+5));	
			}					

		else if((address >= MANU_RELAY1) && (address <= MANU_RELAY5))
			{
			ManualRelay(address - MANU_RELAY1) = *(bufadd+5);
			write_eeprom(EEP_MANU_RELAY1 + address - MANU_RELAY1, *(bufadd+5));	
			SetByteBit(&ManualRelayAll,address - MANU_RELAY1, *(bufadd+5));
			write_eeprom(MANUAL_RELAY, ManualRelayAll);
			}				
			
		else if(address == MANUAL_AO1)
			{
			ManualAO1_HI = *(bufadd+4);		
			ManualAO1_LO = *(bufadd+5);				
			write_eeprom(EEP_MANUAL_AO1 + 1,*(bufadd+4));
			write_eeprom(EEP_MANUAL_AO1,*(bufadd+5));					
			}	

		else if(address == MANUAL_AO2)
			{
			ManualAO2_HI = *(bufadd+4);		
			ManualAO2_LO = *(bufadd+5);				
			write_eeprom(EEP_MANUAL_AO2 + 1,*(bufadd+4));
			write_eeprom(EEP_MANUAL_AO2,*(bufadd+5));					
			}				

		else if(address == DEADMASTER_AUTO_MANUAL)// DEADMASTER_AUTO_MANUAL = 1, the output will be trigger to "AUTO" mode  //DEADMASTER_AUTO_MANUAL = 0, the default, outputs will not change, stay in whatever mode they were last commanded
			{																				//DEADMASTER_AUTO_MANUAL = 2, the outputs will go to manual on or off as defined in register 313.
			DeadMsater_AutoManual = *(bufadd+5);
			write_eeprom(EEP_DEADMASTER_AUTO_MANUAL,*(bufadd+5));	
			}				

		else if(address == DEADMASTER_OUTPUT_STATE)//(relay1-5) deadmaster config value during deadmaster be triggered
			{
			DeadMsater_OutputState = *(bufadd+5);
			write_eeprom(EEP_DEADMASTER_OUTPUT_STATE,*(bufadd+5));	
			}	
			
		else if(address == DEADMASTER_COOL_OUTPUT)//(analog output - cooling)deadmaster config value during deadmaster be triggered
			{
			DeadMsater_CoolOutput_LO = *(bufadd+5);
			DeadMsater_CoolOutput_HI = *(bufadd+4);	
			write_eeprom(EEP_DEADMASTER_COOL_OUTPUT,*(bufadd+5));
			write_eeprom(EEP_DEADMASTER_COOL_OUTPUT + 1,*(bufadd+4));				
			}				

		else if(address == DEADMASTER_HEAT_OUTPUT)//(analog output - heating)deadmaster config value during deadmaster be triggered
			{
			DeadMsater_HeatOutput_LO = *(bufadd+5);
			DeadMsater_HeatOutput_HI = *(bufadd+4);	
			write_eeprom(EEP_DEADMASTER_HEAT_OUTPUT,*(bufadd+5));
			write_eeprom(EEP_DEADMASTER_HEAT_OUTPUT + 1,*(bufadd+4));				
			}					

		else if(address == OUTPUT1_FUNCTION)
			{
			EEP_Output1Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT1_FUNCTION,*(bufadd+5));	
			}	

		else if(address == OUTPUT2_FUNCTION)
			{
			EEP_Output2Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT2_FUNCTION,*(bufadd+5));	
			}

		else if(address == OUTPUT3_FUNCTION)
			{
			EEP_Output3Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT3_FUNCTION,*(bufadd+5));	
			}	

		else if(address == OUTPUT4_FUNCTION)
			{
			EEP_Output4Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT4_FUNCTION,*(bufadd+5));	
			}	

		else if(address == OUTPUT5_FUNCTION)
			{
			EEP_Output5Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT5_FUNCTION,*(bufadd+5));	
			}	
//TBD		
		else if(address == OUTPUT6_FUNCTION)
			{
			Out6_Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT6_FUNCTION,*(bufadd+5));	
			}	

		else if(address == OUTPUT7_FUNCTION)
			{
			Out7_Function = *(bufadd+5);
			write_eeprom(EEP_OUTPUT7_FUNCTION,*(bufadd+5));	
			}				

		else if(address == FAN_SPEED)
			{
			if(*(bufadd+5)> EEP_FanMode)
				*(bufadd+5) = FAN_AUTO;	
			EEP_FanSpeed = *(bufadd+5);
			fan_speed_user = *(bufadd+5);
			fanspeedbuf = *(bufadd+5);
			write_eeprom(EEP_FAN_SPEED,*(bufadd+5));
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

		else if(address == PID_OUTPUT1)
			{
			EEP_PidOutput1 = *(bufadd+5);
			pid_ctrl_bit[0] = *(bufadd+5);	
			write_eeprom(EEP_PID_OUTPUT1,*(bufadd+5));	
			}
		else if(address == PID_OUTPUT2)
			{
			EEP_PidOutput2 = *(bufadd+5);
			pid_ctrl_bit[1] = *(bufadd+5);	
			write_eeprom(EEP_PID_OUTPUT2,*(bufadd+5));	
			}
		else if(address == PID_OUTPUT3)
			{
			EEP_PidOutput3 = *(bufadd+5);
			pid_ctrl_bit[2] = *(bufadd+5);	
			write_eeprom(EEP_PID_OUTPUT3,*(bufadd+5));	
			}
		else if(address == PID_OUTPUT4)
			{
			EEP_PidOutput4 = *(bufadd+5);
			pid_ctrl_bit[3] = *(bufadd+5);	
			write_eeprom(EEP_PID_OUTPUT4,*(bufadd+5));	
			}
		else if(address == PID_OUTPUT5)
			{
			EEP_PidOutput5 = *(bufadd+5);
			pid_ctrl_bit[4] = *(bufadd+5);	
			write_eeprom(EEP_PID_OUTPUT5,*(bufadd+5));	
			}
		else if(address == PID_OUTPUT6)
			{
			EEP_PidOutput6 = *(bufadd+5);
			pid_ctrl_bit[5] = *(bufadd+5);	
			pid_flag = 1;
			write_eeprom(EEP_PID_OUTPUT6,*(bufadd+5));	
			pid_flag = 0;
			}
		else if(address == PID_OUTPUT7)
			{
			EEP_PidOutput7 = *(bufadd+5);
			pid_ctrl_bit[6] = *(bufadd+5);	
			pid_flag = 1;
			write_eeprom(EEP_PID_OUTPUT7,*(bufadd+5));
			pid_flag = 0;				
			}			

		else if((address >= UNIVERSAL_OUTPUT_BEGIN) && (address <= UNIVERSAL_OUTPUT_HEAT3))
			{
			PID2_Output_Table(address - UNIVERSAL_OUTPUT_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_OUTPUT_BEGIN + address - UNIVERSAL_OUTPUT_BEGIN ,*(bufadd+5));	
			}				

		else if((address >= FAN0_OPER_TABLE_COAST) && (address <= FAN0_OPER_TABLE_HEAT3))
			{
			FanTable0(address - FAN0_OPER_TABLE_COAST) = *(bufadd+5);
			write_eeprom(EEP_FAN0_OPER_TABLE_COAST + address - FAN0_OPER_TABLE_COAST ,*(bufadd+5));	
			}	
			
		else if((address >= FAN1_OPER_TABLE_COAST) && (address <= FAN1_OPER_TABLE_HEAT3))
			{
			FanTable1(address - FAN1_OPER_TABLE_COAST) = *(bufadd+5);
			write_eeprom(EEP_FAN1_OPER_TABLE_COAST + address - FAN1_OPER_TABLE_COAST ,*(bufadd+5));	
			}				

		else if((address >= FAN2_OPER_TABLE_COAST) && (address <= FAN2_OPER_TABLE_HEAT3))
			{
			FanTable2(address - FAN2_OPER_TABLE_COAST) = *(bufadd+5);
			write_eeprom(EEP_FAN2_OPER_TABLE_COAST + address - FAN2_OPER_TABLE_COAST ,*(bufadd+5));	
			}						
			
		else if((address >= FAN3_OPER_TABLE_COAST) && (address <= FAN3_OPER_TABLE_HEAT3))
			{
			FanTable3(address - FAN3_OPER_TABLE_COAST) = *(bufadd+5);
			write_eeprom(EEP_FAN3_OPER_TABLE_COAST + address - FAN3_OPER_TABLE_COAST ,*(bufadd+5));	
			}					
	
		else if((address >= FANAUT_OPER_TABLE_COAST) && (address <= FANAUT_OPER_TABLE_HEAT3))
			{
			FanTableAuto(address - FANAUT_OPER_TABLE_COAST) = *(bufadd+5);
			write_eeprom(EEP_FANAUT_OPER_TABLE_COAST + address - FANAUT_OPER_TABLE_COAST ,*(bufadd+5));	
			}				

		else if((address >= VALVE_OPER_TABLE_BEGIN) && (address <= VALVE_OPER_TABLE_HEAT3))
			{
			PID1_Valve_Table(address - VALVE_OPER_TABLE_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_VALVE_OPER_TABLE_BEGIN + address - VALVE_OPER_TABLE_BEGIN ,*(bufadd+5));	
			}	
			
		else if(address == HEAT_UNIVERSAL_TABLE)
			{
			EEP_HEAT_TABLE2 = *(bufadd+5);
			write_eeprom(EEP_HEAT_UNIVERSAL_TABLE,*(bufadd+5));	
			}		
			
		else if(address == COOL_UNIVERSAL_TABLE)
			{
			EEP_COOL_TABLE2 = *(bufadd+5);
			write_eeprom(EEP_COOL_UNIVERSAL_TABLE,*(bufadd+5));	
			}					

		else if(address == HEAT_ORIGINAL_TABLE)
			{
			EEP_HEAT_TABLE1 = *(bufadd+5);
			write_eeprom(EEP_HEAT_ORIGINAL_TABLE,*(bufadd+5));	
			}					

		else if(address == COOL_ORIGINAL_TABLE)
			{
			EEP_COOL_TABLE1 = *(bufadd+5);
			write_eeprom(EEP_COOL_ORIGINAL_TABLE,*(bufadd+5));	
			}	

		else if((address >= VALVE_OFF_TABLE_COAST) && (address <= VALVE_OFF_TABLE_HEAT3))
			{
			PID1_Valve_Off_Table(address - VALVE_OFF_TABLE_COAST) = *(bufadd+5);
			write_eeprom(EEP_VALVE_OFF_TABLE_COAST + address - VALVE_OFF_TABLE_COAST ,*(bufadd+5));	
			}				
			
		else if(address == DEFAULT_SETPOINT)
			{
			EEP_DefaultSetpoint = *(bufadd+5);
			write_eeprom(EEP_DEFAULT_SETPOINT,*(bufadd+5));	
			}	

		else if(address == SETPOINT_CONTROL)
			{
			EEP_SetpointControl = *(bufadd+5);
			write_eeprom(EEP_SETPOINT_CONTROL,*(bufadd+5));	
			}

		else if(address == DAY_SETPOINT)
			{
			EEP_DaySpLo = *(bufadd+5);
			EEP_DaySpHi = *(bufadd+4);
			Set_day_setpoint(*(bufadd+4), *(bufadd+5));
			init_PID_flag |= 0x01;
		  refresh_setpoint(DAY_MODE);
			}			
//	DAYSETPOINT_OPTION,		//	
//	MIDDLE_SETPOINT,			//
			
		else if(address == DAY_COOLING_DEADBAND)
			{
			change_setpoint_deadband(DAY_COOLING_DB, *(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(DAY_MODE);
			}		
			
		else if(address == DAY_HEATING_DEADBAND)
			{
			change_setpoint_deadband(DAY_HEATING_DB, *(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(DAY_MODE);
			}

		else if(address == DAY_COOLING_SETPOINT )
			{
			Set_day_cooling_setpoint(*(bufadd+4),*(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(DAY_MODE);
			}			

		else if(address == DAY_HEATING_SETPOINT )
			{
			Set_day_heating_setpoint(*(bufadd+4),*(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(DAY_MODE);
			}

		else if(address == NIGHT_SETPOINT )
			{
			Set_night_setpoint(*(bufadd+4),*(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(NIGHT_MODE);
			}			

		else if(address == APPLICATION)// 0	1	0=Office, 1=Hotel or Residential
			{
			EEP_Application = *(bufadd+5);
			write_eeprom(EEP_APPLICATION,*(bufadd+5));	
			}				

		else if(address == NIGHT_HEATING_DEADBAND) 
			{
			change_setpoint_deadband(NIGHT_HEATING_DB, *(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(NIGHT_MODE);
			}			
			
		else if(address == NIGHT_COOLING_DEADBAND) 
			{
			change_setpoint_deadband(NIGHT_COOLING_DB, *(bufadd+5));
			init_PID_flag |= 0x01;
			refresh_setpoint(NIGHT_MODE);
			}

		else if(address == NIGHT_HEATING_SETPOINT )
			{
			Set_night_heating_setpoint(*(bufadd+4),*(bufadd+5));
			init_PID_flag |= 0x01;	
			refresh_setpoint(NIGHT_MODE);
			}

		else if(address == NIGHT_COOLING_SETPOINT )
			{
			Set_night_cooling_setpoint(*(bufadd+4),*(bufadd+5));
			init_PID_flag |= 0x01;	
			refresh_setpoint(NIGHT_MODE);
			}			

		else if(address == WINDOW_INTERLOCK_COOLING_SETPOINT)
			{
			Day_WindowInterlock_CSP_LO = *(bufadd+5);
			Day_WindowInterlock_CSP_HI = *(bufadd+4);
			write_eeprom(WINDOW_INTERLOCK_COOLING_SETPOINT,*(bufadd+5));
			write_eeprom(WINDOW_INTERLOCK_COOLING_SETPOINT + 1,*(bufadd+4));
			}

		else if(address == UNIVERSAL_NIGHTSET )
			{
			EEP_UniversalNightSetpointHi = *(bufadd+4);
			EEP_UniversalNightSetpointLo = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_NIGHTSET,*(bufadd+5));
			write_eeprom(EEP_UNIVERSAL_NIGHTSET + 1,*(bufadd+4));			
			}				

		else if(address == UNIVERSAL_SET )
			{
			EEP_UniversalSetpointHi = *(bufadd+4);
			EEP_UniversalSetpointLo = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_SET,*(bufadd+5));
			write_eeprom(EEP_UNIVERSAL_SET + 1,*(bufadd+4));			
			}	

		else if(address == UNIVERSAL_HEAT_DB )
			{
			UniversalHeatDB_HI = *(bufadd+4);
			UniversalHeatDB_LO = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_HEAT_DB,*(bufadd+5));
			write_eeprom(EEP_UNIVERSAL_HEAT_DB + 1,*(bufadd+4));			 
			}			

		else if(address == UNIVERSAL_COOL_DB )
			{
			UniversalCoolDB_HI = *(bufadd+4);
			UniversalCoolDB_LO = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_COOL_DB,*(bufadd+5));
			write_eeprom(EEP_UNIVERSAL_COOL_DB + 1,*(bufadd+4));			 
			}	
			
//	ECOMONY_COOLING_SETPOINT,
//	ECOMONY_HEATING_SETPOINT,
		else if(address == POWERUP_SETPOINT )
			{
			EEP_PowerupSetpoint = *(bufadd+5);
			write_eeprom(EEP_POWERUP_SETPOINT,*(bufadd+5));		 
			}				

		else if(address == MAX_SETPOINT )
			{
			EEP_MaxSetpoint = *(bufadd+5);
			write_eeprom(EEP_MAX_SETPOINT,*(bufadd+5));		 
			}					

		else if(address == MIN_SETPOINT )
			{
			EEP_MinSetpoint = *(bufadd+5);
			write_eeprom(EEP_MIN_SETPOINT,*(bufadd+5));		 
			}		
					
		else if(address == SETPOINT_INCREASE )
			{
			EEP_SetpointIncrease = *(bufadd+5);
			write_eeprom(EEP_SETPOINT_INCREASE,*(bufadd+5));		 
			}				

		else if(address == FREEZE_TEMP_SETPOINT )
			{
			EEP_FreezeSetpoint = *(bufadd+5);
			write_eeprom(EEP_FREEZE_TEMP_SETPOINT,*(bufadd+5));		 
			}				

//	WALL_SETPOINT,//    wall setpoint ,normal setpoint
//	CEILING_SETPOINT,//    celling setpoint
//	AVERAGE_SETPOINT,//
//	UNOCCUPIED_HEATING,//
//	UNOCCUPIED_COOLING,//
//	RH_SETPOINT,//
// 	CURRENT1_SETPOINT,//  tbd  get rid of this
			
		else if(address == TEMP_SELECT )
			{
			EEP_TempSelect = *(bufadd+5);
			write_eeprom(EEP_TEMP_SELECT,*(bufadd+5));		 
			}	
			
		else if(address == INPUT1_SELECT )
			{
			EEP_Input1Select = *(bufadd+5);
			write_eeprom(EEP_INPUT1_SELECT,*(bufadd+5));		 
			}	

		else if(address == COOLING_PTERM )
			{
			EEP_PTERM1 = *(bufadd+5);
			write_eeprom(EEP_COOLING_PTERM,*(bufadd+5));
			init_PID_flag |= 0x01;
			}			

		else if(address == COOLING_ITERM )
			{
			EEP_ITERM1 = *(bufadd+5);
			write_eeprom(EEP_COOLING_ITERM,*(bufadd+5));	
			init_PID_flag |= 0x01;	
			}				

		else if(address == UNIVERSAL_PTERM )
			{
			EEP_PTERM2_LO = *(bufadd+5);
			EEP_PTERM2_HI = *(bufadd+4);
			write_eeprom(EEP_UNIVERSAL_PTERM,*(bufadd+5));
			write_eeprom(EEP_UNIVERSAL_PTERM + 1,*(bufadd+4));				
			}		
			
		else if(address == UNIVERSAL_ITERM )
			{
			EEP_ITERM2_LO = *(bufadd+5);
			EEP_ITERM2_HI = *(bufadd+4);
			write_eeprom(EEP_UNIVERSAL_ITERM,*(bufadd+5));
			write_eeprom(EEP_UNIVERSAL_ITERM + 1,*(bufadd+4));				
			}		
			
		else if(address == UNITS1_HIGH)// 
		{
			UnitS1_HI_LO = *(bufadd+5);
			UnitS1_HI_HI = *(bufadd+4);
			write_eeprom(EEP_UNITS1_HIGH,*(bufadd+5));
			write_eeprom(EEP_UNITS1_HIGH + 1,*(bufadd+4));
		}
			
		else if(address == UNITS1_LOW)
		{
			UnitS1_LO_LO = *(bufadd+5);
			UnitS1_LO_HI = *(bufadd+4);
			write_eeprom(EEP_UNITS1_LOW,*(bufadd+5));
			write_eeprom(EEP_UNITS1_LOW + 1,*(bufadd+4));		
		}			

		else if(address == UNITS2_HIGH)// 
		{
			UnitS2_HI_LO = *(bufadd+5);
			UnitS2_HI_HI = *(bufadd+4);
			write_eeprom(EEP_UNITS2_HIGH,*(bufadd+5));
			write_eeprom(EEP_UNITS2_HIGH + 1,*(bufadd+4));
		}
			
		else if(address == UNITS2_LOW)
		{
			UnitS2_LO_LO = *(bufadd+5);
			UnitS2_LO_HI = *(bufadd+4);
			write_eeprom(EEP_UNITS2_LOW,*(bufadd+5));
			write_eeprom(EEP_UNITS2_LOW + 1,*(bufadd+4));		
		}	
//	PID_UNIVERSAL ,// 		
//	UNITS1_HIGH ,  //                     
//	UNITS1_LOW  ,  //                      
//	UNITS2_HIGH ,  //                    
//	UNITS2_LOW ,   //			
//	PID2_MODE_OPERATION,
			
		else if(address == KEYPAD_SELECT )// 0	3	Number of buttons on the keypad - 0=2, 1=4, 2=6A, 3=6B 
			{
			EEP_KeypadSelect = *(bufadd+5);
			write_eeprom(EEP_KEYPAD_SELECT,*(bufadd+5));		 
			}			

		else if(address == SPECIAL_MENU_LOCK )// 0	3	Number of buttons on the keypad - 0=2, 1=4, 2=6A, 3=6B 
			{
			EEP_SpecialMenuLock = *(bufadd+5);
			write_eeprom(EEP_SPECIAL_MENU_LOCK,*(bufadd+5));		 
			}
	
//	DISPLAY,	//						// 0	1	Display setting.  0 = temperature, 1 = setpoint						
//	ICON,
//	LAST_KEY_TIMER,//
//	KEYPAD_VALUE,//
//	DISPLAY_HUNDRED,//
//	DISPLAY_TEN,//
//	DISPLAY_DIGITAL,//
//	DISPLAY_STATUS,//
//	ROUND_DISPLAY,//
		else if(address == MIN_ADDRESS )
			{
			EEP_MinAddress = *(bufadd+5);
			write_eeprom(EEP_MIN_ADDRESS,*(bufadd+5));		 
			}			

		else if(address == MAX_ADDRESS )
			{
			EEP_MaxAddress = *(bufadd+5);
			write_eeprom(EEP_MAX_ADDRESS,*(bufadd+5));		 
			}	
	
		else if(address == TIMER_SELECT )
			{
			EEP_TimerSelect = *(bufadd+5);
			write_eeprom(EEP_TIMER_SELECT,*(bufadd+5));
			if(*(bufadd+5) == 3)
				{
				timer_enable = 0;
				mode_stage_changed[0] = 1;
				mode_stage_changed[1] = 1;
				}				
			}				

		else if(address == RTC_YEAR)
			{
				SetRTCTime(EEP_CLOCK_YEAR, (*(bufadd+4)<<8) + *(bufadd+5)+2000);
			}			 
		else if(address == RTC_MONTH)
			{
				SetRTCTime(EEP_CLOCK_MONTH, (*(bufadd+4)<<8) + *(bufadd+5));//tbd: change RTC time here
			}	
//		else if(address == RTC_WEEK)//STM32 AUTOMATICLLY CALCULATE THE WEEK VALUE
//			{
//				SetRTCTime(EEP_CLOCK_WEEK, (*(bufadd+4)<<8) + *(bufadd+5));//tbd: change RTC time here
//			}	
		else if(address == RTC_DAY)
			{
				SetRTCTime(EEP_CLOCK_DAY, (*(bufadd+4)<<8) + *(bufadd+5));//tbd: change RTC time here
			}	
		else if(address == RTC_HOUR)
			{
				SetRTCTime(EEP_CLOCK_HOUR, (*(bufadd+4)<<8) + *(bufadd+5));//tbd: change RTC time here
			}	
		else if(address == RTC_MINUTE)
			{
				SetRTCTime(EEP_CLOCK_MINUTE, (*(bufadd+4)<<8) + *(bufadd+5));//tbd: change RTC time here
			}	
		else if(address == RTC_SECOND)
			{
				SetRTCTime(EEP_CLOCK_SECOND, (*(bufadd+4)<<8) + *(bufadd+5));//tbd: change RTC time here
			}

		else if(address == SCHEDULE_DHOME_HOUR )
			{
			ScheduleDHomeHour = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_DHOME_HOUR,*(bufadd+5));		 
			}	
		else if(address == SCHEDULE_DHOME_MIN )
			{
			ScheduleDHomeMin = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_DHOME_MIN,*(bufadd+5));		 
			}	
			
		else if(address == SCHEDULE_WORK_HOUR )
			{
			ScheduleWorkHour = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_WORK_HOUR,*(bufadd+5));		 
			}	
		else if(address == SCHEDULE_WORK_MIN)
			{
			ScheduleWorkMin = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_WORK_MIN,*(bufadd+5));		 
			}	

		else if(address == SCHEDULE_NHOME_HOUR )
			{
			ScheduleNHomeHour = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_NHOME_HOUR,*(bufadd+5));		 
			}	
		else if(address == SCHEDULE_NHOME_MIN )
			{
			ScheduleNHomeMin = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_NHOME_MIN,*(bufadd+5));		 
			}	
			
		else if(address == SCHEDULE_SLEEP_HOUR )
			{
			ScheduleSleepHour = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_SLEEP_HOUR,*(bufadd+5));		 
			}	
		else if(address == SCHEDULE_SLEEP_MIN )
			{
			ScheduleSleepMin = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_SLEEP_MIN,*(bufadd+5));		 
			}
		else if(address == SCHEDULE_HOLIDAY_BYEAR )
			{
			ScheduleHolidayByear = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_HOLIDAY_BYEAR,*(bufadd+5));		 
			}	
		else if(address == SCHEDULE_HOLIDAY_BMONTH )
			{
			ScheduleHolidayBmon = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_HOLIDAY_BMONTH,*(bufadd+5));		 
			}	
		else if(address == SCHEDULE_HOLIDAY_BDAY )
			{
			ScheduleHolidayBday = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_HOLIDAY_BDAY,*(bufadd+5));		 
			}				

		else if(address == SCHEDULE_HOLIDAY_EYEAR )
			{
			ScheduleHolidayEyear = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_HOLIDAY_EYEAR,*(bufadd+5));		 
			}	
			
		else if(address == SCHEDULE_HOLIDAY_EMONTH )
			{
			ScheduleHolidayEmon = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_HOLIDAY_EMONTH,*(bufadd+5));		 
			}	
			
		else if(address == SCHEDULE_HOLIDAY_EDAY )
			{
			ScheduleHolidayEday = *(bufadd+5);
			write_eeprom(EEP_SCHEDULE_HOLIDAY_EDAY,*(bufadd+5));		 
			}	
		

		else if(address == DAY2_EVENT4_HI )
			{
			Day2_Event4_HI = *(bufadd+5);
			write_eeprom(EEP_DAY2_EVENT4_HI,*(bufadd+5));		 
			}	
		else if(address == DAY2_EVENT4_LO )
			{
			Day2_Event4_LO = *(bufadd+5);
			write_eeprom(EEP_DAY2_EVENT4_LO,*(bufadd+5));		 
			}					

		else if(address == LCD_TURN_OFF )
			{
				LCDTurnOff = *(bufadd+5);
				lcd_turn_off_timer = LCDTurnOff;
				lcd_turn_off_timer *=60;	
				write_eeprom(EEP_LCD_TURN_OFF,*(bufadd+5));
				if(lcd_turn_off_timer > 0)
					#ifdef TSTAT7_ARM
					GPIO_ResetBits(GPIOA, GPIO_Pin_0);
					#else
					GPIO_SetBits(GPIOA, GPIO_Pin_0);
					#endif
			}				



		else if(address == DEMAND_RESPONSE )
			{
			DemandResponse = *(bufadd+5);
			write_eeprom(EEP_DEMAND_RESPONSE,*(bufadd+5));		 
			}

		else if(address == LOCK_REGISTER )
			{
			LockRegister = *(bufadd+5);
			write_eeprom(EEP_LOCK_REGISTER,*(bufadd+5));		 
			}			

		else if(address == PIR_STAGE )
			{
			EEP_PirStage = *(bufadd+5);
			write_eeprom(EEP_PIR_STAGE,*(bufadd+5));		 
			}					

		else if(address == LCD_ROTATE_ENABLE )
			{
			lcd_rotate_max = *(bufadd+5);
			write_eeprom(EEP_LCD_ROTATE_ENABLE,*(bufadd+5));		 
			}	
	  
//    else if(address == BAC_INSTANCE_LO)
//		{
//			
//		
//		}			
//			
			

		else if(address == SCHEDULE_ON_OFF )//1:ON  0:OFF
			{
				schedule_on_off = *(bufadd+5);
				write_eeprom(EEP_SCHEDULE_ON_OFF,*(bufadd+5));
				if(schedule_on_off)
				{				
					EEP_OverrideTimer = 60;	
					//override_timer_time = 3600;
					write_eeprom(EEP_OVERRIDE_TIMER,60);
				}

			}	
			
		else if((address >= DISP_ITEM_TEMPERATURE ) && (address <= DISP_ITEM_OUT7))
			{
			disp_item_queue(address - DISP_ITEM_TEMPERATURE) = *(bufadd+5);
			write_eeprom(EEP_DISP_ITEM_TEMPERATURE + address - DISP_ITEM_TEMPERATURE, *(bufadd+5));		 
			}	

		else if((address >= OUTPUT_PWM_AUTO_COAST ) && (address <= OUTPUT_PWM_AUTO_HEAT3))
			{
			Output_PWM_Table(address - OUTPUT_PWM_AUTO_COAST) = *(bufadd+5);
			write_eeprom(EEP_OUTPUT_PWM_AUTO_COAST + address - OUTPUT_PWM_AUTO_COAST, *(bufadd+5));		 
			}				

		else if(address == EXT_SENSOR_TEMP_CAL )
			{
			Calibration_External_Tem_HI = *(bufadd+4);
			Calibration_External_Tem_LO = *(bufadd+5);
			write_eeprom(EEP_EXT_SENSOR_TEMP_CAL + 1,*(bufadd+4));
			write_eeprom(EEP_EXT_SENSOR_TEMP_CAL,*(bufadd+5));					
			}	

		else if(address == BUTTON_LEFT1 )
			{
			LeftButton1 = *(bufadd+5);
			write_eeprom(EEP_BUTTON_LEFT1, *(bufadd+5));		 
			}	

		else if(address == BUTTON_LEFT2 )
			{
			LeftButton2 = *(bufadd+5);
			write_eeprom(EEP_BUTTON_LEFT2, *(bufadd+5));		 
			}	
		else if(address == BUTTON_LEFT3 )
			{
			LeftButton3 = *(bufadd+5);
			write_eeprom(EEP_BUTTON_LEFT3, *(bufadd+5));		 
			}	
		else if(address == BUTTON_LEFT4 )
			{
			LeftButton4 = *(bufadd+5);
			write_eeprom(EEP_BUTTON_LEFT4, *(bufadd+5));		 
			}	

		else if(address == HUM_HEATING_CONTROL )
			{
			HumHeatControl = *(bufadd+5);
			write_eeprom(EEP_HUM_HEATING_CONTROL, *(bufadd+5));		 
			}				

		else if(address == HUM_INPUT_MANUAL_ENABLE )
			{
			hum_manual_enable = *(bufadd+5);
			write_eeprom(EEP_HUM_INPUT_MANUAL_ENABLE, *(bufadd+5));		 
			}					

		else if(address == HUM_INPUT_MANUAL_VALUE )
			{
			HumInputManualValue_HI = *(bufadd+4);
			HumInputManualValue_LO = *(bufadd+5);
			write_eeprom(EEP_HUM_INPUT_MANUAL_VALUE + 1, *(bufadd+4));
			write_eeprom(EEP_HUM_INPUT_MANUAL_VALUE, *(bufadd+5));				
			}				

		else if(address == CO2_INPUT_MANUAL_ENABLE )
			{
			co2_manual_enable = *(bufadd+5);
			write_eeprom(EEP_CO2_INPUT_MANUAL_ENABLE, *(bufadd+5));		 
			}			
			
		else if(address == CO2_INPUT_MANUAL_VALUE )
			{
			CO2InputManualValue_HI = *(bufadd+4);
			CO2InputManualValue_LO = *(bufadd+5);
			write_eeprom(EEP_CO2_INPUT_MANUAL_VALUE + 1, *(bufadd+4));
			write_eeprom(EEP_CO2_INPUT_MANUAL_VALUE, *(bufadd+5));				
			}		
			
		else if(address == CO2_CALIBRATION )
			{
			Calibration_CO2_HI = *(bufadd+4);
			Calibration_CO2_LO = *(bufadd+5);
			write_eeprom(EEP_CO2_CALIBRATION + 1, *(bufadd+4));
			write_eeprom(EEP_CO2_CALIBRATION, *(bufadd+5));				
			}
			
		else if((address >= UNIVERSAL_OFF_TABLE_BEGIN ) && (address <= UNIVERSAL_OFF_TABLE_HEAT3))
			{
			PID2_Output_OFF_Table(address - UNIVERSAL_OFF_TABLE_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_OFF_TABLE_BEGIN + address - UNIVERSAL_OFF_TABLE_BEGIN, *(bufadd+5));		 
			}		
			
		else if((address >= UNIVERSAL_OFF_VALVE_BEGIN ) && (address <= UNIVERSAL_OFF_VALVE_HEAT3))
			{
			PID2_Valve_OFF_Table(address - UNIVERSAL_OFF_VALVE_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_OFF_VALVE_BEGIN + address - UNIVERSAL_OFF_VALVE_BEGIN, *(bufadd+5));		 
			}		
				
		else if(address == KEYPAD_REVERSE )
			{
			KeypadReverse = *(bufadd+5);
			write_eeprom(EEP_KEYPAD_REVERSE, *(bufadd+5));		 
			}	
			
		else if(address == PIR_SENSOR_SELECT )
			{
			PirSensorSelect = *(bufadd+5);
			write_eeprom(EEP_PIR_SENSOR_SELECT, *(bufadd+5));		 
			}				
			
		else if(address == PIR_SENSOR_ZERO )
			{
			PirSensorZero = *(bufadd+5);
			write_eeprom(EEP_PIR_SENSOR_ZERO, *(bufadd+5));		 
			}	

		else if((address >= UNIVERSAL_VALVE_BEGIN ) && (address <= UNIVERSAL_VALVE_HEAT3))
			{
			PID2_Valve_Table(address - UNIVERSAL_VALVE_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_UNIVERSAL_VALVE_BEGIN + address - UNIVERSAL_VALVE_BEGIN, *(bufadd+5));		 
			}				

		else if(address == TSTAT_ID_WRITE_ENABLE )
			{
			ID_Lock = *(bufadd+5);
			write_eeprom(EEP_ID_WRITE_ENABLE, *(bufadd+5));
			if(ID_Lock > 1)
				ID_Lock = 0;				
			}				

		else if(address == PIR_TIMER )
			{
			PirTimer = *(bufadd+5);
			write_eeprom(EEP_PIR_TIMER, *(bufadd+5));
				//tbd: CALCULATE PIR TIMER 				
			}	

		else if(address == SUPPLY_SETPOINT )
			{
			Supply_SP_HI = *(bufadd+4);
			Supply_SP_LO = *(bufadd+5);
			write_eeprom(EEP_SUPPLY_SETPOINT + 1, *(bufadd+4));
			write_eeprom(EEP_SUPPLY_SETPOINT, *(bufadd+5));				
			}		
			
		else if(address == MAX_SUPPLY_SETPOINT )
			{
			Supply_Max_SP_HI = *(bufadd+4);
			Supply_Max_SP_LO = *(bufadd+5);
			write_eeprom(EEP_MAX_SUPPLY_SETPOINT + 1, *(bufadd+4));
			write_eeprom(EEP_MAX_SUPPLY_SETPOINT, *(bufadd+5));				
			}		
	
		else if(address == MIN_SUPPLY_SETPOINT )
			{
			Supply_Min_SP_HI = *(bufadd+4);
			Supply_Min_SP_LO = *(bufadd+5);
			write_eeprom(EEP_MIN_SUPPLY_SETPOINT + 1, *(bufadd+4));
			write_eeprom(EEP_MIN_SUPPLY_SETPOINT, *(bufadd+5));				
			}		

		else if(address == MAX_AIRFLOW_COOLING )
			{
			Max_AirflowSP_Cool_HI = *(bufadd+4);
			Max_AirflowSP_Cool_LO = *(bufadd+5);
			write_eeprom(EEP_MAX_AIRFLOW_COOLING + 1, *(bufadd+4));
			write_eeprom(EEP_MAX_AIRFLOW_COOLING, *(bufadd+5));				
			}				
			
		else if(address == MAX_AIRFLOW_HEATING )
			{
			Max_AirflowSP_Heat_HI = *(bufadd+4);
			Max_AirflowSP_Heat_LO = *(bufadd+5);
			write_eeprom(EEP_MAX_AIRFLOW_HEATING + 1, *(bufadd+4));
			write_eeprom(EEP_MAX_AIRFLOW_HEATING, *(bufadd+5));				
			}		

		else if(address == OCC_MIN_AIRFLOW )
			{
			Occ_Min_Airflow_HI = *(bufadd+4);
			Occ_Min_Airflow_LO = *(bufadd+5);
			write_eeprom(EEP_OCC_MIN_AIRFLOW + 1, *(bufadd+4));
			write_eeprom(EEP_OCC_MIN_AIRFLOW, *(bufadd+5));				
			}
			
		else if(address == UNOCC_AIRFLOW )
			{
			Unocc_Min_Airflow_HI = *(bufadd+4);
			Unocc_Min_Airflow_LO = *(bufadd+5);
			write_eeprom(EEP_UNOCC_MIN_AIRFLOW + 1, *(bufadd+4));
			write_eeprom(EEP_UNOCC_MIN_AIRFLOW, *(bufadd+5));				
			}

    else if(address == PWM_ENABLE)
		{
			if(*(bufadd+5) > 100)
				*(bufadd+5) = 100;
			write_eeprom(EEP_PWM_DUTY, *(bufadd+5));
						
			pwm_duty = *(bufadd+5) * 10;
		}	

    else if(address == SCHEDULE_MODE_NUM)
		{
			write_eeprom(EEP_SCHEDULE_MODE_NUM, *(bufadd+5));
			ScheduleModeNum = *(bufadd+5);
		}			
			
		else if(address == AIRFLOW_SETPOINT )
			{
			Airflow_SP_LO = *(bufadd+4);
			Airflow_SP_LO = *(bufadd+5);
			write_eeprom(EEP_AIRFLOW_SETPOINT + 1, *(bufadd+4));
			write_eeprom(EEP_AIRFLOW_SETPOINT, *(bufadd+5));				
			}			

		else if(address == VAV_CONTROL )
			{
			VAV_Control = *(bufadd+5);
			write_eeprom(EEP_VAV_CONTROL, *(bufadd+5));		 
			}
			
		else if(address == PID3_INPUT_SELECT )
			{
			PID3_InputSelect = *(bufadd+5);
			write_eeprom(EEP_PID3_INPUT_SELECT, *(bufadd+5));		 
			}			

		else if((address >= PID3_VALVE_OPER_TABLE_BEGIN ) && (address <= PID3_VALVE_OPER_TABLE_HEAT3))
			{
			PID3_Valve_Table(address - PID3_VALVE_OPER_TABLE_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_PID3_VALVE_OPER_TABLE_BEGIN + address - PID3_VALVE_OPER_TABLE_BEGIN, *(bufadd+5));		 
			}		

		else if(address == PID3_COOLING_DB )
			{
			pid3_cool_db = *(bufadd+5);
			write_eeprom(EEP_PID3_COOLING_DB, *(bufadd+5));		 
			}	

		else if(address == PID3_HEATING_DB )
			{
			pid3_heat_db = *(bufadd+5);
			write_eeprom(EEP_PID3_HEATING_DB, *(bufadd+5));		 
			}	
			
		else if(address == PID3_PTERM )
			{
			pid3_pterm = *(bufadd+5);
			write_eeprom(EEP_PID3_PTERM, *(bufadd+5));		 
			}	
			
		else if(address == PID3_ITERM )
			{
			pid3_iterm = *(bufadd+5);
			write_eeprom(EEP_PID3_ITERM, *(bufadd+5));		 
			}				

		else if(address == PID3_HEAT_STAGE )
			{
			EEP_HEAT_TABLE3 = *(bufadd+5);
			write_eeprom(EEP_HEAT_PID3_TABLE, *(bufadd+5));		 
			}					

		else if(address == PID3_COOL_STAGE )
			{
			EEP_COOL_TABLE3 = *(bufadd+5);
			write_eeprom(EEP_COOL_PID3_TABLE, *(bufadd+5));		 
			}	

		else if((address >= PID3_OUTPUT_BEGIN ) && (address <= PID3_OUTPUT_HEAT3))
			{
			PID3_Output_Table(address - PID3_OUTPUT_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_PID3_OUTPUT_BEGIN + address - PID3_OUTPUT_BEGIN, *(bufadd+5));		 
			}		
			
		else if((address >= PID3_VALVE_OFF_TABLE_BEGIN ) && (address <= PID3_VALVE_OFF_TABLE_HEAT3))
			{
			PID3_Valve_Off_Table(address - PID3_VALVE_OFF_TABLE_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_PID3_VALVE_OFF_TABLE_BEGIN + address - PID3_VALVE_OFF_TABLE_BEGIN, *(bufadd+5));		 
			}					

		else if((address >= PID3_OFF_OUTPUT_BEGIN ) && (address <= PID3_OFF_OUTPUT_HEAT3))
			{
			PID3_Valve_Off_Table(address - PID3_OFF_OUTPUT_BEGIN) = *(bufadd+5);
			write_eeprom(EEP_PID3_OFF_OUTPUT_BEGIN + address - PID3_OFF_OUTPUT_BEGIN, *(bufadd+5));		 
			}	

		else if(address == TSTAT_HEAT_COOL )
			{
// When setting HC to an analog input mode, make sure the corresponding AI is set to 1								
			if (*(bufadd+5) == HC_ANALOG_IN1 && EEP_AnalogIn1 != 1)
				{
				EEP_AnalogIn1 = 1;
				write_eeprom(EEP_ANALOG1_RANGE,EEP_AnalogIn1);
				}
				
			// When setting HC to an analog input mode, make sure the corresponding AI is set to 1								
			if (*(bufadd+5) == HC_ANALOG_IN2 && EEP_AnalogIn2 != 1)
				{
				EEP_AnalogIn2 = 1;
				write_eeprom(EEP_ANALOG2_RANGE,EEP_AnalogIn2);
				}	 
			}				

		else if(address == INTERNAL_SENSOR_MANUAL )//0 = AUTO, 1 = MANUAL
			{
			EEP_InterThermistorManual = *(bufadd+5);
			write_eeprom(EEP_INTERNAL_SENSOR_MANUAL, *(bufadd+5));		 
			}									

		else if(address == PRESSURE_MANUAL_VALUE )
			{
			PressureManualValue_HI = *(bufadd+4);
			PressureManualValue_LO = *(bufadd+5);
			write_eeprom(EEP_PRESSURE_MANUAL_VALUE + 1, *(bufadd+4));
			write_eeprom(EEP_PRESSURE_MANUAL_VALUE, *(bufadd+5));				
			}	

		else if(address == PRESSURE_MANUAL_ENABLE )//0 = AUTO, 1 = MANUAL
			{
			PressureManualEnable = *(bufadd+5);
			write_eeprom(EEP_PRESSURE_MANUAL_ENABLE, *(bufadd+5));		 
			}	

		else if(address == AQ_MANUAL_VALUE )
			{
			AQManualValue_HI = *(bufadd+4);
			AQManualValue_LO = *(bufadd+5);
			write_eeprom(EEP_AQ_MANUAL_VALUE + 1, *(bufadd+4));
			write_eeprom(EEP_AQ_MANUAL_VALUE, *(bufadd+5));				
			}				
			
		else if(address == AQ_MANUAL_ENABLE )//0 = AUTO, 1 = MANUAL
			{
			AQManualEnable = *(bufadd+5);
			write_eeprom(EEP_AQ_MANUAL_ENABLE, *(bufadd+5));		 
			}	

		else if(address == SP_DISPLAY_SELECT )
			{
			SP_DisplaySelect = *(bufadd+5);
			write_eeprom(EEP_SP_DISPLAY_SELECT, *(bufadd+5));		 
			}	

		else if(address == PID3_DAY_SP )
			{
			PID3_DaySP_HI = *(bufadd+4);
			PID3_DaySP_LO = *(bufadd+5);
			write_eeprom(EEP_PID3_DAY_SP + 1, *(bufadd+4));
			write_eeprom(EEP_PID3_DAY_SP, *(bufadd+5));				
			}
			
		else if(address == PID3_NIGHT_SP )
			{
			PID3_NightSP_HI = *(bufadd+4);
			PID3_NightSP_LO = *(bufadd+5);
			write_eeprom(EEP_PID3_NIGHT_SP + 1, *(bufadd+4));
			write_eeprom(EEP_PID3_NIGHT_SP, *(bufadd+5));				
			}			
 
//	BK_SN,
//	BK_SNLOLO,
//	BK_SNLOHI,	
//	BK_SNHILO,
//	BK_SNHIHI,
//	BK_PN,
//	BK_HWN,
			
//		else if(address == TSTAT_NAME_ENABLE )//default value = 0x55 indicate tstat name function is present
//			{
//			UserInfoEnable = *(bufadd+5);
//			write_eeprom(EEP_TSTAT_NAME_ENABLE, *(bufadd+5));		 
//			}	

//		else if((address >= TSTAT_NAME1 ) && (address <= TSTAT_NAME10))
//			{
//			UserInfo_Name(address - TSTAT_NAME1) = *(bufadd+5);
//			write_eeprom(EEP_TSTAT_NAME1 + address - TSTAT_NAME1, *(bufadd+5));		 
//			}				

		else if(address == SHOW_ID )//this regisger will let user display current ID, the time will depending on the written value 
			{
				if(*(bufadd+5) > 0)
				{
					showid_cnt = *(bufadd+5);//show_id_state = ID_ENABLE;
					showidbit = 0;
				}
				Show_ID_Enable = 1;
				//write_eeprom(EEP_SHOW_ID, *(bufadd+5));		 
			}	

		else if(address == TRANSDUCER_RANGE_MIN )
			{
			MinTransducerRange = *(bufadd+5);
			write_eeprom(EEP_TRANSDUCER_RANGE_MIN, *(bufadd+5));		 
			}	
			
		else if(address == TRANSDUCER_RANGE_MAX )//When set analog output to transducer mode, use these two reg to set range: range_min = 10 means 0V accodrding to 10C  
			{
			MaxTransducerRange = *(bufadd+5);
			write_eeprom(EEP_TRANSDUCER_RANGE_MAX, *(bufadd+5));		 
			}				

		else if(address == ICON_MANUAL_MODE )//manual icons enable/disable   0:auto icon   1: manual icon 
			{
			ICON_ManualMode = *(bufadd+5);
			write_eeprom(EEP_ICON_MANUAL_MODE, *(bufadd+5));		 
			}				

		else if(address == ICON_MANUAL_VALUE )//when set icon to munual mode, set value here 
			{
			ICON_ManualValue = *(bufadd+5);
			write_eeprom(EEP_ICON_MANUAL_VALUE, *(bufadd+5));		 
			}	
			
		else if(address == SOFTWARE_RESET)
		{
			if(*(bufadd+5) == 1)
				SoftReset();
		}
		else if(address == HUM_CALIBRATION)	
			{
			write_eeprom(EEP_HUM_CALIBRATION, *(bufadd+5));
			write_eeprom(EEP_HUM_CALIBRATION+1, *(bufadd+4));
			hum_cal_lo = *(bufadd+5);
			hum_cal_hi = *(bufadd+4);
			}
		else if(address == SPARE1)
			{
			write_eeprom(EEP_SERINALNUMBER_WRITE_FLAG, *(bufadd+5));
			SN_WR_Flag = *(bufadd+5);	
			}	

//		else if(address == USER_DEFAULT_ENABLE)
//		{
//			write_eeprom(USER_SETTING, *(bufadd+5));
//			user_setting_enable = *(bufadd+5);
//		}
		
		else if(address == OUTSIDETEM)
		{
			outside_tem = *(bufadd+4);
			outside_tem = outside_tem << 8;
			outside_tem += *(bufadd+5);
			outside_tem = (int16)outside_tem;
			outsidetem_timer = 300;
      outsidetem_health = 0;			
		}
		
		else if(address == OUTSIDETEM_SLAVE)
		{
		outside_tem_flag = *(bufadd+5);		
		}	

		else if(address == SLEEP_SP)	
			{
			write_eeprom(EEP_SLEEP_SP, *(bufadd+5));
			write_eeprom(EEP_SLEEP_SP+1, *(bufadd+4));
			Sleep_sp_lo = *(bufadd+5);
			Sleep_sp_hi = *(bufadd+4);
			}		

		else if(address == SLEEP_COOLING_SP)	
			{
			write_eeprom(EEP_SLEEP_COOLING_SP, *(bufadd+5));
			write_eeprom(EEP_SLEEP_COOLING_SP+1, *(bufadd+4));
			SleepCoolSp_l = *(bufadd+5);
			SleepCoolSp_h = *(bufadd+4);
			}	

		else if(address == SLEEP_HEATING_SP)	
			{
			write_eeprom(EEP_SLEEP_HEATING_SP, *(bufadd+5));
			write_eeprom(EEP_SLEEP_HEATING_SP+1, *(bufadd+4));
			SleepHeatSp_l = *(bufadd+5);
			SleepHeatSp_h = *(bufadd+4);
			}				
			
		else if(address == SLEEP_COOLING_DB)	
			{
			write_eeprom(EEP_SLEEP_COOLING_DB, *(bufadd+5));		
			SleepCooldb = *(bufadd+5);
			}	

		else if(address == SLEEP_HEATING_DB)	
			{
			write_eeprom(EEP_SLEEP_HEATING_DB, *(bufadd+5));
			SleepHeatdb = *(bufadd+5);
			}
			

		else if(address == MAX_WORK_SP)	
			{
			write_eeprom(EEP_MAX_WORK_SP, *(bufadd+5));
			Max_work_sp = *(bufadd+5);
			}	

		else if(address == MIN_WORK_SP)	
			{
			write_eeprom(EEP_MIN_WORK_SP, *(bufadd+5));
			Min_work_sp = *(bufadd+5);
			}	
			
		else if(address == MAX_SLEEP_SP)	
			{
			write_eeprom(EEP_MAX_SLEEP_SP, *(bufadd+5));
			Max_sleep_sp = *(bufadd+5);
			}				

		else if(address == MIN_SLEEP_SP)	
			{
			write_eeprom(EEP_MIN_SLEEP_SP, *(bufadd+5));
			Min_sleep_sp = *(bufadd+5);
			}
			
		else if(address == MAX_HOLIDAY_SP)	
			{
			write_eeprom(EEP_MAX_HOLIDAY_SP, *(bufadd+5));
			Max_holiday_sp = *(bufadd+5);
			}				

		else if(address == MIN_HOLIDAY_SP)	
			{
			write_eeprom(EEP_MIN_HOLIDAY_SP, *(bufadd+5));
			Min_holiday_sp = *(bufadd+5);
			}

		else if(address == INPUT_VOLTAGE_TERM)	
			{
			write_eeprom(EEP_INPUT_VOLTAGE_TERM, *(bufadd+5));
			InputVoltageTerm = *(bufadd+5);
			}			

		else if((address >= ICON_DAY_OUTPUT_CONTROL) && (address <= ICON_FAN3_OUTPUT_CONTROL))
		{
			write_eeprom(EEP_ICON_DAY_OUTPUT_CONTROL+ address - ICON_DAY_OUTPUT_CONTROL, *(bufadd+5));
			IconOutputControl(address - ICON_DAY_OUTPUT_CONTROL) = *(bufadd+5); 
		}

		else if((address >= OUTPUT_PWM_OFF_COAST) && (address <= OUTPUT_PWM_OFF_HEAT3))
		{
			write_eeprom(EEP_OUTPUT_PWM_OFF_BEGIN+ address - OUTPUT_PWM_OFF_COAST, *(bufadd+5));
			Output_PWM_Off_Table(address - ICON_DAY_OUTPUT_CONTROL) = *(bufadd+5); 
		}		
		
	  else if(address == SET_PIR_SENSETIVITY)
		{
			write_eeprom(EEP_PIR_SENSETIVITY, *(bufadd+5));
			Pir_Sensetivity = *(bufadd+5); 
		}	

		else if((address >= SCHEDULE_MONDAY_FLAG_1)&&(address <= SCHEDULE_HOLIDAY_FLAG_2))		
		{
			if((address - SCHEDULE_MONDAY_FLAG_1)%2 == 0)//first flag reg
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_FLAG + (address - SCHEDULE_MONDAY_FLAG_1)/2*3, *(bufadd+5));
				write_eeprom(EEP_SCHEDULE_MONDAY_FLAG + (address - SCHEDULE_MONDAY_FLAG_1)/2*3 + 1, *(bufadd+4));
				ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG_1)/2*3) = *(bufadd+5);
				ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG_1)/2*3 + 1) = *(bufadd+4);			
			}
			else//second flag reg
			{
				write_eeprom(EEP_SCHEDULE_MONDAY_FLAG + (address - SCHEDULE_MONDAY_FLAG_1 - 1)/2*3+2, *(bufadd+5));
				ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG_1 - 1)/2*3 + 2) = *(bufadd+5);
			}				
//			write_eeprom(EEP_SCHEDULE_MONDAY_FLAG + (address - SCHEDULE_MONDAY_FLAG)*2, *(bufadd+5));
//			write_eeprom(EEP_SCHEDULE_MONDAY_FLAG + (address - SCHEDULE_MONDAY_FLAG)*2 + 1, *(bufadd+4));
//			ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG)*2) = *(bufadd+5);
//			ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG)*2 + 1) = *(bufadd+4);
		}
		
		else if((address >= SCHEDULE_DAY_BEGAIN)&&(address <= SCHEDULE_DAY_END))
		{
			write_eeprom(EEP_SCHEDULE_DAY_BEGAIN + address - SCHEDULE_DAY_BEGAIN, *(bufadd+5));
			ScheduleDay(address - SCHEDULE_DAY_BEGAIN) = *(bufadd+5);
			ar_dates[0][address - SCHEDULE_DAY_BEGAIN] = *(bufadd+5);
		}	

		else if(address == AWAY_SP)
		{
			 Away_sp_hi = *(bufadd+4);
			 Away_sp_lo = *(bufadd+5);
			 write_eeprom(EEP_AWAY_SP, *(bufadd+5));
			 write_eeprom(EEP_AWAY_SP + 1, *(bufadd+4));
		}
		
		else if(address == FRC_ENABLE)//free cooling  function enable/disable 0:DIABLE  1:ENABLE
		{
			write_eeprom(EEP_FRC_ENABLE, *(bufadd+5));
			FrcEnable = *(bufadd+5);
		}
		else if(address == FRC_BASE_SELECT)//1001FREE COOLING BASE SELECT 0: TEMPERATUERE BASE 1: ENTHALPY BASE
		{
			write_eeprom(EEP_FRC_BASE_SELECT, *(bufadd+5));
			FrcBaseSelect = *(bufadd+5);
		}
				
		else if(address == FRC_SPACETEM_SOURCE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_SPACETEM_SOURCE, *(bufadd+5));
			spacetem.source = *(bufadd+5);
			spacetem.value = 0;
		}
		
		else if(address == FRC_SPACETEM_VALUE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			spacetem.value = *(bufadd+4) * 256;
			spacetem.value += *(bufadd+5);
			spacetem.update = FRC_UPDATE_TIME;
		}		
		
		else if(address == FRC_SPACETEM_UNIT)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_SPACETEM_UNIT, *(bufadd+5));
			FrcSpaceTemUnit = *(bufadd+5);
		}
		
		else if(address == FRC_SPACETEM_ID)//1003 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
		{
			write_eeprom(EEP_FRC_SPACETEM_ID, *(bufadd+5));
			write_eeprom(EEP_FRC_SPACETEM_ID + 1, *(bufadd+4));
			spacetem.id = *(bufadd+4) * 256;
			spacetem.id += *(bufadd+5);
		}
//		else if(address == FRC_SPACETEM_LASTUPDATE)//1004 FREE COOLING SPACE TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
//		{
//			write_eeprom(EEP_FRC_SPACETEM_LASTUPDATE, *(bufadd+5));
//			spacetem.update = *(bufadd+5);
//		}
//		else if(address == FRC_SPACETEM_STATUE)//1005 CHECK IF SPACE TEMPERATURE SOURCE IS WORK WELL
//		{
//			write_eeprom(EEP_FRC_SPACETEM_STATUE, *(bufadd+5));
//			spacetem.status = *(bufadd+5);
//		}
//		else if(address == FRC_SPACETEM_CONFIG)//1006 FREE COOLING SPACE TEMPERATURE STATUS, CHECK RANGE,UNIT 	
//		{
//			write_eeprom(EEP_FRC_SPACETEM_CONFIG, *(bufadd+5));
//			spacetem.config = *(bufadd+5);
//		}
		//--------------------------------------
		else if(address == FRC_SUPPLYTEM_SOURCE)//1007 FREE COOLING SUPPLY TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_SUPPLYTEM_SOURCE, *(bufadd+5));
			supplytem.source = *(bufadd+5);
			supplytem.value = 0;
		}
		
		else if(address == FRC_SUPPLYTEM_VALUE)//1002FREE COOLING SUPPLY TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			supplytem.value = *(bufadd+4) * 256;
			supplytem.value += *(bufadd+5);
			supplytem.update = FRC_UPDATE_TIME;
		}
		
		else if(address == FRC_SUPPLYTEM_UNIT)//1007 FREE COOLING SUPPLY TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_SUPPLYTEM_UNIT, *(bufadd+5));
			FrcSupplyTemUnit = *(bufadd+5);
		}		
		else if(address == FRC_SUPPLYTEM_ID)//1008 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
		{
			write_eeprom(EEP_FRC_SUPPLYTEM_ID, *(bufadd+5));
			write_eeprom(EEP_FRC_SUPPLYTEM_ID + 1, *(bufadd+4));
			supplytem.id = *(bufadd+4) * 256;
			supplytem.id += *(bufadd+5);
		}
//		else if(address == FRC_SUPPLYTEM_LASTUPDATE)//1009 FREE COOLING SUPPLY TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
//		{
//			write_eeprom(EEP_FRC_SUPPLYTEM_LASTUPDATE, *(bufadd+5));
//			supplytem.update = *(bufadd+5);
//		}
//		else if(address == FRC_SUPPLYTEM_STATUE)//1010 CHECK IF SUPPLY TEMPERATURE SOURCE IS WORK WELL
//		{
//			write_eeprom(EEP_FRC_SUPPLYTEM_STATUE, *(bufadd+5));
//			supplytem.status = *(bufadd+5);
//		}
//		else if(address == FRC_SUPPLYTEM_CONFIG)//1011 FREE COOLING SUPPLY TEMPERATURE STATUS, CHECK RANGE,UNIT 
//		{
//			write_eeprom(EEP_FRC_SUPPLYTEM_CONFIG, *(bufadd+5));
//			supplytem.config = *(bufadd+5);
//		}
		//--------------------------
		else if(address == FRC_OUTDOORTEM_SOURCE)//1012 FREE COOLING OUTDOOR TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_OUTDOORTEM_SOURCE, *(bufadd+5));
			outdoortem.source = *(bufadd+5);
			outdoortem.value = 0;
		}

		else if(address == FRC_OUTDOORTEM_VALUE)//1002FREE COOLING OUTDOOR TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			outdoortem.value = *(bufadd+4) * 256;
			outdoortem.value += *(bufadd+5);
			outdoortem.update = FRC_UPDATE_TIME;
		}
		
		else if(address == FRC_OUTDOORTEM_UNIT)//1012 FREE COOLING OUTDOOR TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_OUTDOORTEM_UNIT, *(bufadd+5));
			FrcOutdoorTemUnit = *(bufadd+5);
		}
		
		else if(address == FRC_OUTDOORTEM_ID)//1013 COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
		{
			write_eeprom(EEP_FRC_OUTDOORTEM_ID, *(bufadd+5));
			write_eeprom(EEP_FRC_OUTDOORTEM_ID + 1, *(bufadd+4));
			outdoortem.id = *(bufadd+4) * 256;
			outdoortem.id += *(bufadd+5);
		}
//		else if(address == FRC_OUTDOORTEM_LASTUPDATE)//1014 FREE COOLING OUTDOOR TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
//		{
//			write_eeprom(EEP_FRC_OUTDOORTEM_LASTUPDATE, *(bufadd+5));
//			outdoortem.update = *(bufadd+5);
//		}
//		else if(address == FRC_OUTDOORTEM_STATUE)//1015 CHECK IF OUTDOOR TEMPERATURE SOURCE IS WORK WELL
//		{
//			write_eeprom(EEP_FRC_OUTDOORTEM_STATUE, *(bufadd+5));
//			outdoortem.status = *(bufadd+5);
//		}
//		else if(address == FRC_OUTDOORTEM_CONFIG)//1016 FREE COOLING OUTDOOR TEMPERATURE STATUS, CHECK RANGE,UNIT 	
//		{
//			write_eeprom(EEP_FRC_OUTDOORTEM_CONFIG, *(bufadd+5));
//			outdoortem.config = *(bufadd+5);
//		}
    //------------------------------------                                                         	
		else if(address == FRC_INDOORHUM_SOURCE)//1017 FREE COOLING INDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_INDOORHUM_SOURCE, *(bufadd+5));
			indoorhum.source = *(bufadd+5);
			indoorhum.value = 0;
		}
		
		else if(address == FRC_INDOORHUM_VALUE)//free cooling indoor humidity
		{
			indoorhum.value = *(bufadd+4) * 256;
			indoorhum.value += *(bufadd+5);
			indoorhum.update = FRC_UPDATE_TIME;
		}
		
		else if(address == FRC_INDOORHUM_ID)//1018 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
		{
			write_eeprom(EEP_FRC_INDOORHUM_ID, *(bufadd+5));
			write_eeprom(EEP_FRC_INDOORHUM_ID + 1, *(bufadd+4));
			indoorhum.id = *(bufadd+4) * 256;
			indoorhum.id += *(bufadd+5);
		}
		
		else if(address == FRC_INDOORHUM_LASTUPDATE)//1019 FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
		{
			write_eeprom(EEP_FRC_INDOORHUM_LASTUPDATE, *(bufadd+5));
			indoorhum.update = *(bufadd+5);
		}
		
		else if(address == FRC_INDOORHUM_STATUE)//1020 CHECK IF INDOOR HUMIDITY SOURCE IS WORK WELL
		{
			write_eeprom(EEP_FRC_INDOORHUM_STATUE, *(bufadd+5));
			indoorhum.status = *(bufadd+5);
		}
		
		else if(address == FRC_INDOORHUM_CONFIG)//1021 FREE COOLING INDOOR HUMIDITY STATUS, CHECK RANGE,UNIT
		{
			write_eeprom(EEP_FRC_INDOORHUM_CONFIG, *(bufadd+5));
			indoorhum.config = *(bufadd+5);
		}
    //------------------------------------
		else if(address == FRC_OUTDOORHUM_SOURCE)//1022 FREE COOLING OUTDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
		{
			write_eeprom(EEP_FRC_OUTDOORHUM_SOURCE, *(bufadd+5));
			outdoorhum.source = *(bufadd+5);
			outdoorhum.value = 0;
		}

		else if(address == FRC_OUTDOORHUM_VALUE)//free cooling indoor humidity
		{
			outdoorhum.value = *(bufadd+4) * 256;
			outdoorhum.value += *(bufadd+5);
			outdoorhum.update = FRC_UPDATE_TIME;
		}
		
		else if(address == FRC_OUTDOORHUM_ID)//1023 COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
		{
			write_eeprom(EEP_FRC_OUTDOORHUM_ID, *(bufadd+5));
			write_eeprom(EEP_FRC_OUTDOORHUM_ID + 1, *(bufadd+4));
			outdoorhum.id = *(bufadd+5) * 256;
			outdoorhum.id += *(bufadd+4);
		}
//		else if(address == FRC_OUTDOORHUM_LASTUPDATE)//1024 FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
//		{
//			write_eeprom(EEP_FRC_OUTDOORHUM_LASTUPDATE, *(bufadd+5));
//			outdoorhum.update = *(bufadd+5);
//		}
//		else if(address == FRC_OUTDOORHUM_STATUE)//1025 CHECK IF OUTDOOR HUMIDITY SOURCE IS WORK WELL
//		{
//			write_eeprom(EEP_FRC_OUTDOORHUM_STATUE, *(bufadd+5));
//			outdoorhum.status = *(bufadd+5);
//		}
//		else if(address == FRC_OUTDOORHUM_CONFIG)//1026 FREE COOLING OUTDOOR HUMIDITY STATUS, CHECK RANGE,UNIT
//		{
//			write_eeprom(EEP_FRC_OUTDOORHUM_CONFIG, *(bufadd+5));
//			outdoorhum.config = *(bufadd+5);
//		}
		else if(address == FRC_MIN_FRESH_AIR)//1027 MINIMAL FRESH AIR, UNIT PERCENTAGE
		{
			write_eeprom(EEP_FRC_MIN_FRESH_AIR, *(bufadd+5));
			FrcMinFreshAir = *(bufadd+5);
		}
		else if(address == FRC_MIN_FRESH_AIRTEM)//1028 MINIMAL FRESH AIR TEMPERATURE, UNIT DEGREE C/F
		{
			write_eeprom(EEP_FRC_MIN_FRESH_AIRTEM_L, *(bufadd+5));
			write_eeprom(EEP_FRC_MIN_FRESH_AIRTEM_H, *(bufadd+4));
			FrcMinFreshAirTemLow = *(bufadd+5);
			FrcMinFreshAirTemHi = *(bufadd+4);
			
		}
		else if(address == FRC_OUTPUT_SELECT) //1029 FREE COOLING OUTPUT SELECT
		{
			write_eeprom(EEP_FRC_OUTPUT_SELECT, *(bufadd+5));
			frc_output_select = *(bufadd+5);
			if(frc_output_select == FRC_OUTPUT_AO1)//USE AO1 TO CONTROL
			{
				EEP_PidOutput6 = 1;//set AO1 to be controlled by PID2
				write_eeprom(EEP_PID_OUTPUT6,1);
			}
			if(frc_output_select == FRC_OUTPUT_AO2)//USE AO2 TO CONTROL
			{
				EEP_PidOutput7 = 1;//set AO2 to be controlled by PID2
				write_eeprom(EEP_PID_OUTPUT7,1);
			}
			
		}		

		else if(address == FRC_OUTPUT_CONFIG) //1029 FREE COOLING OUTPUT CHECK, OUTPUT SOURCE, OUTPUT FUNCTION
		{
			write_eeprom(EEP_FRC_OUTPUT_CONFIG, *(bufadd+5));
			FrcOutputConfig = *(bufadd+5);
		}
		else if(address == FRC_PID2_CONFIG)  //1030 FREE COOLING PID2 CONFIG CHECK, CHECK IF PID2 HAS INPUT FROM SUPPLY SENSOR, ALSO CHECK IF IT IS SET TO NEGATIVE COOLING
		{
			write_eeprom(EEP_FRC_PID2_CONFIG, *(bufadd+5));
			FrcPid2Config = *(bufadd+5);
		}
//		else if(address == FRC_COOLING_DEADBAND)//1031 FREE COOLING DEADBAND UNIT: DEGREE C/F
//		{
//			write_eeprom(EEP_FRC_COOLING_DEADBAND, *(bufadd+5));
//			FrcCoolDeadband = *(bufadd+5);
//		}
		else if(address == FRC_OUTPUT_MODE)  //1032 SET FAN TO OFF
		{
			write_eeprom(FRC_OUTPUT_MODE, *(bufadd+5));
			FrcOutputMode = *(bufadd+5);
		}
//		else if(address == FRC_FULL_FRESH)//1033 SET FULL FRESH AIR INPUT
//		{
//			write_eeprom(EEP_FRC_FULL_FRESH, *(bufadd+5));
//			FrcFullFresh = *(bufadd+5);
//		}
		else if(address == FRC_TOTAL_CONFIG)//1034 MINIMAL SUPPLY AIR TEMPERATURE UNIT: DEGREE C/F
		{
			//write_eeprom(EEP_FRC_MAITAIN_MIN_SUPPLYTEM, *(bufadd+5));
			write_eeprom(EEP_FRC_TOTAL_CONFIG, *(bufadd+5));
			FrcTotalConfig = *(bufadd+5);
		}
		else if(address == FRC_PRESENT_MODE)//1035
		{
			write_eeprom(EEP_FRC_PRESENT_MODE, *(bufadd+5));
			FrcPresentMode = *(bufadd+5);
		}	

		else if(address == FRC_OUTDOOR_ENTHALPY)
		{
			frc_outdoorhum_enthalpy = (*(bufadd+4))*256 + (*(bufadd+5));		
		}
		
		else if(address == FRC_INDOOR_ENTHALPY)
		{
			frc_indoorhum_enthalpy = (*(bufadd+4))*256 + (*(bufadd+5)); 		
		}

		else if(address == MODBUS_4TO20MA_BOTTOM)
		{
			bottom_of_4to20ma = (*(bufadd+4))*256 + (*(bufadd+5));
			write_eeprom(EEP_4TO20MA_BOTTOM, *(bufadd+5));
			write_eeprom(EEP_4TO20MA_BOTTOM + 1, *(bufadd+4));
		}
		else if(address == MODBUS_4TO20MA_TOP)
		{
			top_of_4to20ma = (*(bufadd+4))*256 + (*(bufadd+5));
			write_eeprom(EEP_4TO20MA_TOP, *(bufadd+5));
			write_eeprom(EEP_4TO20MA_TOP + 1, *(bufadd+4));
		}

		else if(address == MODBUS_4TO20MA_UNIT_HI)
		{
			//top_of_4to20ma = (*(bufadd+4))*256 + (*(bufadd+5));
			write_eeprom(EEP_4TO20MA_UNIT_HI, *(bufadd+5));
			write_eeprom(EEP_4TO20MA_UNIT_HI + 1, *(bufadd+4));
		}				


		else if(address == MODBUS_4TO20MA_UNIT_LO)
		{
			//top_of_4to20ma = (*(bufadd+4))*256 + (*(bufadd+5));
			write_eeprom(EEP_4TO20MA_UNIT_LO, *(bufadd+5));
			write_eeprom(EEP_4TO20MA_UNIT_LO + 1, *(bufadd+4));
		}			
		
		else if(address == BAC_T1)
		{
		 if(*(bufadd+5) == 0)
		 {
			 for(j=0;j<8;j++)
				write_eeprom(BAC_TEST1+j,0);
		 }
		}
		else if(address == DTERM)		
		{
			EEP_DTERM1 = *(bufadd+5);
			write_eeprom(EEP_DTERM ,*(bufadd+5));
		}	
		
		else if(address == PID_SAMPLE_TIME)		
		{
			PidSampleTime = *(bufadd+5);
			write_eeprom(EEP_PID_SAMPLE_TIME ,*(bufadd+5));
		}			
		
		
		else if(address == TSTAT_TEST1) 
		{
			ctest[0] = *(bufadd+4) * 256 + *(bufadd+5);
		}

		else if(address == TSTAT_TEST2) 
		{
			
			ctest[1] = *(bufadd+5);
			#ifdef COLOR_TEST
			TSTAT8_BACK_COLOR = (*(bufadd+4))*256 + (*(bufadd+5)); 
			write_eeprom(TEST3,*(bufadd+4));
			write_eeprom(TEST3+1,*(bufadd+5));
			SoftReset();
			#endif
		}	
		else if(address == TSTAT_TEST3) 
		{
			ctest[2] = *(bufadd+5);
			#ifdef COLOR_TEST
			TANGLE_COLOR = (*(bufadd+4))*256 + (*(bufadd+5)); 
			write_eeprom(TEST4,*(bufadd+4));
			write_eeprom(TEST5,*(bufadd+5));
			#endif
		}	

//		else if(address == TSTAT_TEST4) 
//		{
//			newpid.Kp = *(bufadd+4) * 256 + *(bufadd+5);
//		}
//		
//		else if(address == TSTAT_TEST5) 
//		{
//			newpid.Ki = *(bufadd+4) * 256 + *(bufadd+5);
//		}
//		
//		else if(address == TSTAT_TEST6) 
//		{
//			newpid.Kd = *(bufadd+4) * 256 + *(bufadd+5);
//		}
		
//	TEST2,//
//	TEST3,
//	TEST4,//						
//	TEST5,//
//	TEST6,
//	TEST7,
//	TEST8,
//	TEST9,
//	TEST10,			
	#ifdef HUM_CALIBRATION_ENABLE
					else if(address == HUM_C_TEMP)
						{
							 if(HUM_CAL_UNLOCK_FLAG)
							 {			    
								for(i=0;i<20;i++)
								{
									if(calibrate_temperature(*(bufadd+4),*(bufadd+5)))
										 break;
									else
										delay_us(2000);
								}

							 }
						 }
						
					else if(address == HUM_C_VALUE)
						{ 
						 if(HUM_CAL_UNLOCK_FLAG)
						 {			    
							for(i=0;i<20;i++)
							{
								if(write_sensor_humtable18(*(bufadd+4),*(bufadd+5),sensor_hum_fre>>8,sensor_hum_fre))
									 break;
								else
									delay_us(200);
							}
						 }
						 }
						
					else if(address == HUM_C_ERASE)
						{
						if(!HUM_CAL_UNLOCK_FLAG) 
						if(*(bufadd+5) == 123)
							{													
							for(i=0;i<20;i++)
								{
								if(clear_sensor_humtab18())
									break;									
								else
									delay_us(2000);
								}
							
							write_eeprom(EEP_HUM_C_ERASE,0);
							}
						else
							write_eeprom(EEP_HUM_C_ERASE,*(bufadd+5));
						}
						
					else if(address == HUM_C_REFRESH_TABLE)
						{
						if(*(bufadd+5) == 1)
							{	   
							
							for(i=0;i<10;i++)
								{
								if(!read_sensor_humtab18(i))
									{
									delay_us(2000);
									read_sensor_humtab18(i);
									} 
								delay_us(2000);
								}
							
							write_eeprom(EEP_HUM_C_REFRESH_TABLE,0);
							}
							else
								write_eeprom(EEP_HUM_C_REFRESH_TABLE,*(bufadd+5));
						}
						
					else if(address == HUM_C_LOCK)
						{
						if(*(bufadd+5) == 123)
							HUM_CAL_UNLOCK_FLAG = 1;
						else
							HUM_CAL_UNLOCK_FLAG = 0;
						write_eeprom(EEP_HUM_C_LOCK,*(bufadd+5));
						}
		#endif				
					
					else if(address == PID3_DAY_SP)
						{
						write_eeprom(EEP_PID3_DAY_SP+1,*(bufadd+4));
						write_eeprom(EEP_PID3_DAY_SP,*(bufadd+5));
						}
					
					else if(address == PID3_NIGHT_SP)
						{
						write_eeprom(EEP_PID3_NIGHT_SP+1,*(bufadd+4));
						write_eeprom(EEP_PID3_NIGHT_SP,*(bufadd+5));
						}						
						
		if(LCDTurnOff != 255)
		{
			lcd_turn_off_timer = LCDTurnOff;
			lcd_turn_off_timer *= 60;		
		}
			
		}
}	

 




uint8 bkstate = 0;


static void responseData(uint8  *bufadd, uint8 uartsel)
{
	u8 num, i, temp1, temp2;
	u16 send_cout = 0 ;
	u16 address;
//	u16 address_temp ;
	//if(USART_RX_BUFB[1] == WRITE_VARIABLES)
	if(*(bufadd+1) == WRITE_VARIABLES)
		{	
			if(uartsel == 0)
			{
				for(i = 0; i < rece_size; i++)
					uart_send[send_cout++] = *(bufadd+i);
			}
			else
			{
				for(i = 0; i < rece_sizeB; i++)
					uart_sendB[send_cout++] = *(bufadd+i);

			}				
				#ifdef TSTAT_ZIGBEE
				if(uartsel == 0)
					USART_SendDataString(send_cout);
				else
					USART_SendDataStringB(send_cout);
				#else
				  USART_SendDataString(send_cout);
				#endif
				return;

		}
		
	else if(*(bufadd+1) == MULTIPLE_WRITE_VARIABLES)
		{
		for(i = 0; i < 6; i++)
			{
			 if(uartsel == 0)
				uart_send[send_cout++] = *(bufadd+i) ;
			 else
				uart_sendB[send_cout++] = *(bufadd+i) ; 
			crc16_byte(*(bufadd+i));
			}
		#ifdef TSTAT_ZIGBEE	
		if(uartsel == 0)	
			USART_SendDataString(send_cout);	
		else
			USART_SendDataStringB(send_cout);
		#else
			USART_SendDataString(send_cout);	
		#endif
		}

	else if(*(bufadd+1) == READ_VARIABLES)
		{
//			if(*(bufadd) == 0)//zigbee info
//			{
//				if(read_zgb_index == 1)//read register 51 to get zigbee point index
//				{
//					zigbee_index = *(bufadd+4);
//					read_zgb_index = 0;
//				}
//				else//read each of point rssi
//				{
//					for(i=0;i<(*(bufadd+2));i++)
//						zigbee_point_info[i] = *(bufadd + 4 + i*2); 
//				}
//			
//			}
//			else
//			{
			num = *(bufadd+5);
			if(uartsel == 0)	
			{			
			uart_send[send_cout++] = *(bufadd) ;
			uart_send[send_cout++] = *(bufadd+1) ;
			uart_send[send_cout++] = (*(bufadd+5)<<1);
			}
			else
			{
			uart_sendB[send_cout++] = *(bufadd) ;
			uart_sendB[send_cout++] = *(bufadd+1) ;
			uart_sendB[send_cout++] = (*(bufadd+5)<<1);		
			}
			crc16_byte(*(bufadd));
			crc16_byte(*(bufadd+1));
			crc16_byte((*(bufadd + 5)<<1));
			

			for(i = 0; i < num; i++)
				{
					address = (u16)((*(bufadd+2))<<8) + (*(bufadd+3)) + i;

					
				if(address <= SERIALNUMBER_HIWORD + 1)
					{
					temp1 = 0 ;
					temp2 = SerialNumber(address) ;
					}	
					
					
				else if(address == VERSION_NUMBER_LO)
					{
					temp1 = (EEPROM_VERSION >> 8) & 0xff;
					temp2 = EEPROM_VERSION & 0xff;		
					}				

				else if(address == VERSION_NUMBER_HI)
					{
					temp1 = 0;//(EEPROM_VERSION >> 8) & 0xff;
					temp2 = 0;//FirmwareVersion_HI;			
					}	
					
				else if(address == TSTAT_ADDRESS)
					{
					temp1 = 0; ;
					temp2 = laddress;
					}
				else if(address == PRODUCT_MODEL)
					{
					temp1 = 0 ;
					#ifndef TSTAT7_ARM
					temp2 = PM_TSTAT8;
					#else
					temp2 = PM_TSTAT7_ARM;
					#endif
					}
				else if(address == HARDWARE_REV)
					{
					temp1 = 0 ;
					temp2 = HardwareVersion ;

					}
	//			else if(address == PIC882VERSION)
	//				{
	//				temp1 = (co2_dataB >> 8) & 0xff ;
	//				temp2 = co2_dataB & 0xff;

	//				}	
				else if(address == PLUG_N_PLAY)
					{

						temp1 = 0 ;
						temp2 =  Plugandplay ;
		
					}
				else if(address == BASE_ADDRESS)
					{
					temp1 = 0 ;
					temp2 = EEP_BaseAddress; 

					}	
				else if(address == UPDATE_STATUS)
					{
					temp1 = 0 ;
					temp2 = UpdateState;

					}	
				else if(address == HARDWARE_INFORMATION)
					{
					temp1 = Hardware_Info_HI;
					temp2 = Hardware_Info_LO;

					}	
				else if(address == ISP_MODE_INDICATION)
					{
					temp1 = 0;
					temp2 = 0;
					}			

#ifdef TSTAT_ZIGBEE					
				else if(address == MODBUS_ZIGBEE_INDEX)
					{
					temp1 = 0;
					temp2 = zigbee_index;
					}					

				else if((address >= MODBUS_ZIGBEE_INFO1) && (address <= MODBUS_ZIGBEE_INFO48))
					{
					temp1 = 0;
					temp2 = zigbee_point_info[address - MODBUS_ZIGBEE_INFO1];
					}		
#endif					
				else if(address == COOLHEATMODE)
					{
					temp1 = 0 ;

					temp2 = get_bit(COOLING_MODE,0);
					if(EEP_COOL_TABLE1 == 0)
						temp2 = 0;	
					if(get_bit(HEATING_MODE,0))
						{	
						temp2 += 2;
						if(EEP_HEAT_TABLE1 == 0)
							temp2 = 0;
						}	

					}					
				else if(address == PID1_MODE_OPERATION)
					{
					temp1 = 0 ;
					temp2 = 0;
					if(current_mode_of_operation[0] >=4 && current_mode_of_operation[0] <=6)
						temp2 = current_mode_of_operation[0] + 10;
				// if cooling stages and heating satges exceed 3,we will use these values 14:  cooling4  17:heating4
					else if(current_mode_of_operation[0] >=7 && current_mode_of_operation[0] <=9)
						temp2 = current_mode_of_operation[0] - 3;
					
					else if(current_mode_of_operation[0] >=10 && current_mode_of_operation[0] <=12)
						temp2 = current_mode_of_operation[0] + 7;
				
					else  
						temp2 = current_mode_of_operation[0];					
																

					}	
					
				else if(address == SEQUENCE)
					{
					temp1 = 0 ;
					temp2 = Sys_Sequence;

					}					

				else if(address == DEGC_OR_F)
					{
					temp1 = 0 ;
					temp2 = EEP_DEGCorF;

					}

				else if(address == FAN_MODE)
					{
					temp1 = 0 ;
					temp2 = EEP_FanMode;

					}

				else if(address == POWERUP_MODE)
					{
					temp1 = 0 ;
					temp2 = EEP_PowerupOnOff;

					}
				else if(address == AUTO_ONLY)	
					{
					temp1 = 0 ;
					temp2 = EEP_AutoOnly;
					}
					
	//			else if(address == FACTORY_DEFAULTS)
	//				{
	//				temp1 = 0 ;
	//				temp2 = 0;

	//				}					
					
				else if(address == INFO_BYTE)
					{
					temp1 = 0 ;
					temp2 = info_byte;

					}

				else if(address == BAUDRATE)
					{
					temp1 = 0 ;
					temp2 = EEP_Baudrate;

					}				
				
				else if(address == TSTAT_OVERRIDE_TIMER)
					{
					temp1 = 0 ;
					temp2 = EEP_OverrideTimer;

					}				
					
				else if(address == OVERRIDE_TIMER_LEFT)
					{
					temp1 = 0 ;
						
					if(override_timer_time > 0)
						{
						temp2 = override_timer_time/61;
						temp2 += 1;
						}
					else
						temp2 = 0;									
					//temp2 = EEP_OverrideTimer;

					}

				else if(address == HEAT_COOL_CONFIG)
					{
					temp1 = 0 ;
					temp2 = EEP_HeatCoolConfig;

					}					


				else if(address == TIMER_ON)
					{
					temp1 = EEP_TimerOnHi;
					temp2 = EEP_TimerOn;

					}					
				else if(address == TIMER_OFF)
					{
					temp1 = EEP_TimerOffHi;
					temp2 = EEP_TimerOff;

					}	
					
				else if(address == TIMER_UNITS)
					{
					temp1 = 0;
					temp2 = EEP_TimerUnits;

					}	

				else if(address == DEAD_MASTER)
					{
					temp1 = 0;
					temp2 = EEP_DeadMaster;

					}

	//			else if(address == SYSTEM_TIME_FORMAT)
	//				{
	//				temp1 = 0;
	//				temp2 = TimeFormat;

	//				}

				else if(address == FREE_COOL_CONFIG)
					{
					temp1 = 0;
					temp2 = FreeCoolConfig;

					}	

				else if(address == RS485_MODE)
					{
					temp1 = 0;
	//				if(rs485_zigbee == RS485_ENABLE)
	//					temp2 = 0;
	//				else
					temp2 = RS485_Mode;

					}	

				else if(address == TEMPRATURE_CHIP)
					{
					temp1 = (temperature >> 8) & 0xff;
					temp2 = temperature;

					}

				else if((address >= ANALOG1_RANGE) && (address <= ANALOG8_RANGE))
					{
					temp1 = 0;//
					temp2 = AI_Range(address - ANALOG1_RANGE);

					}				

				else if(address == INTERNAL_THERMISTOR)
					{
					temp1 = (temperature_internal >> 8) & 0xff;
					temp2 = temperature_internal;

					}

				else if((address >= ANALOG_INPUT1_VALUE) && (address <= ANALOG_INPUT8_VALUE))
					{		
	//				if(GetByteBit(&EEP_InputManuEnable,(address - ANALOG_INPUT1_VALUE)))
	//					{
	//					temp1 = ManualAI_HI(address - ANALOG_INPUT1_VALUE); 
	//				 
	//					temp2 = ManualAI_LO(address - ANALOG_INPUT1_VALUE);							
	//					}
	//				else
	//					{
						temp1 = (mul_analog_input[address - ANALOG_INPUT1_VALUE] >> 8) & 0xFF;  
					 
						temp2 = mul_analog_input[address - ANALOG_INPUT1_VALUE] & 0xFF;
	//					}					
									

					}
		
				else if(address == EXTERNAL_SENSOR1)//co2 sensor 
					{
					temp1 = (co2_data >> 8) & 0xff;//
					temp2 = co2_data & 0xff;

					}				

				else if((address == EXTERNAL_SENSOR2)||(address == TSTAT_HUMIDITY_RH))//hum sensor 
					{
					temp1 = (humidity >> 8) & 0xff;//
					temp2 = humidity & 0xff;				
						

					}				
					
				else if(address == INPUT_MANU_ENABLE)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_InputManuEnable;

					}				
					
				else if(address == FILTER)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Filter;

					}	

				else if((address >= INPUT1_FILTER) && (address <= INPUT8_FILTER))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = InputFilter(address - INPUT1_FILTER);

					}	
					
				else if(address == CO2_FILTER)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = CO2_Filter;

					}					

				else if(address == HUM_FILTER)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = HUM_Filter;

					}

	//			else if(address == CALIBRATION)// 
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = HUM_Filter;

	//				}				

				else if(address == CALIBRATION_INTERNAL_THERMISTOR)// 
					{
					temp1 = Calibration_Internal_HI;//(>> 8) & 0xff;//
					temp2 = Calibration_Internal_LO;

					}

				else if(address == CALIBRATION_ANALOG1)// 
					{
					temp1 = (mul_analog_cal[0]>>8)&0xff;//
					temp2 = mul_analog_cal[0]&0xff;

					}	

				else if(address == CALIBRATION_ANALOG2)// 
					{
					temp1 = (mul_analog_cal[1]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[1]&0xff;//Calibration_AI1_LO;

					}	

				else if(address == CALIBRATION_ANALOG3)// 
					{
					temp1 = (mul_analog_cal[2]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[2]&0xff;//Calibration_AI1_LO;

					}	


				else if(address == CALIBRATION_ANALOG4)// 
					{
					temp1 = (mul_analog_cal[3]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[3]&0xff;//Calibration_AI1_LO;

					}
				else if(address == CALIBRATION_ANALOG5)// 
					{
					temp1 = (mul_analog_cal[4]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[4]&0xff;//Calibration_AI1_LO;

					}
				else if(address == CALIBRATION_ANALOG6)// 
					{
					temp1 = (mul_analog_cal[5]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[5]&0xff;//Calibration_AI1_LO;

					}
				else if(address == CALIBRATION_ANALOG7)// 
					{
					temp1 = (mul_analog_cal[6]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[6]&0xff;//Calibration_AI1_LO;

					}				
				else if(address == CALIBRATION_ANALOG8)// 
					{
					temp1 = (mul_analog_cal[7]>>8)&0xff;//Calibration_AI1_HI;//(>> 8) & 0xff;//
					temp2 = mul_analog_cal[7]&0xff;//Calibration_AI1_LO;

					}						

				else if(address == ANALOG1_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function1;

					}					
				else if(address == ANALOG2_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function2;

					}	
				else if(address == ANALOG3_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function3;

					}					
				else if(address == ANALOG4_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function4;

					}					
				else if(address == ANALOG5_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function5;

					}					
				else if(address == ANALOG6_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function6;

					}					
				else if(address == ANALOG7_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function7;

					}					
				else if(address == ANALOG8_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = AI_Function8;

					}					

				else if(address == TABLE1_ZERO)// 
					{
					temp1 = Table1_ZERO_HI;//(>> 8) & 0xff;//
					temp2 = Table1_ZERO_LO;

					}				

				else if(address == TABLE1_HALFONE)// 
					{
					temp1 = Table1_HalfOne_HI;//(>> 8) & 0xff;//
					temp2 = Table1_HalfOne_LO;

					}					

					
				else if(address == TABLE1_ONE)// 
					{
					temp1 = Table1_One_HI;//(>> 8) & 0xff;//
					temp2 = Table1_One_LO;

					}	
					
				else if(address == TABLE1_HALFTWO)// 
					{
					temp1 = Table1_HalfTwo_HI;//(>> 8) & 0xff;//
					temp2 = Table1_HalfTwo_LO;

					}					

				else if(address == TABLE1_TWO)// 
					{
					temp1 = Table1_Two_HI;//(>> 8) & 0xff;//
					temp2 = Table1_Two_LO;

					}	

				else if(address == TABLE1_HALFTHREE)// 
					{
					temp1 = Table1_HalfThree_HI;//(>> 8) & 0xff;//
					temp2 = Table1_HalfThree_LO;

					}	
					
				else if(address == TABLE1_THREE)// 
					{
					temp1 = Table1_Three_HI;//(>> 8) & 0xff;//
					temp2 = Table1_Three_LO;

					}					
					
				else if(address == TABLE1_HALFFOUR)// 
					{
					temp1 = Table1_HalfFour_HI;//(>> 8) & 0xff;//
					temp2 = Table1_HalfFour_LO;

					}	
					
				else if(address == TABLE1_FOUR)// 
					{
					temp1 = Table1_Four_HI;//(>> 8) & 0xff;//
					temp2 = Table1_Four_LO;

					}

				else if(address == TABLE1_HALFFIVE)// 
					{
					temp1 = Table1_HalfFive_HI;//(>> 8) & 0xff;//
					temp2 = Table1_HalfFive_LO;

					}		

				else if(address == TABLE1_FIVE)// 
					{
					temp1 = Table1_Five_HI;//(>> 8) & 0xff;//
					temp2 = Table1_Five_LO;

					}						

				else if(address == TABLE2_ZERO)// 
					{
					temp1 = Table2_ZERO_HI;//(>> 8) & 0xff;//
					temp2 = Table2_ZERO_LO;

					}				

				else if(address == TABLE2_HALFONE)// 
					{
					temp1 = Table2_HalfOne_HI;//(>> 8) & 0xff;//
					temp2 = Table2_HalfOne_LO;

					}					

					
				else if(address == TABLE2_ONE)// 
					{
					temp1 = Table2_One_HI;//(>> 8) & 0xff;//
					temp2 = Table2_One_LO;

					}	
					
				else if(address == TABLE2_HALFTWO)// 
					{
					temp1 = Table2_HalfTwo_HI;//(>> 8) & 0xff;//
					temp2 = Table2_HalfTwo_LO;

					}					

				else if(address == TABLE2_TWO)// 
					{
					temp1 = Table2_Two_HI;//(>> 8) & 0xff;//
					temp2 = Table2_Two_LO;

					}	

				else if(address == TABLE2_HALFTHREE)// 
					{
					temp1 = Table2_HalfThree_HI;//(>> 8) & 0xff;//
					temp2 = Table2_HalfThree_LO;

					}	
					
				else if(address == TABLE2_THREE)// 
					{
					temp1 = Table2_Three_HI;//(>> 8) & 0xff;//
					temp2 = Table2_Three_LO;

					}					
					
				else if(address == TABLE2_HALFFOUR)// 
					{
					temp1 = Table2_HalfFour_HI;//(>> 8) & 0xff;//
					temp2 = Table2_HalfFour_LO;

					}	
					
				else if(address == TABLE2_FOUR)// 
					{
					temp1 = Table2_Four_HI;//(>> 8) & 0xff;//
					temp2 = Table2_Four_LO;

					}

				else if(address == TABLE2_HALFFIVE)// 
					{
					temp1 = Table2_HalfFive_HI;//(>> 8) & 0xff;//
					temp2 = Table2_HalfFive_LO;

					}		

				else if(address == TABLE2_FIVE)// 
					{
					temp1 = Table2_Five_HI;//(>> 8) & 0xff;//
					temp2 = Table2_Five_LO;

					}						
									

				else if(address == TSTAT_HUM_PIC_UPDATE)//TBD: modify this function as before // write current calibration table to PIC, which table decided by register 427
					{
					temp1 = 0;
					temp2 = 0;//Table2_Five_LO;

					}					

				else if(address == TSTAT_HUM_CAL_NUM)//TBD: modify this function as before 	// calibration data number
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = 0;//Table2_Five_LO;

					}	

				else if(address == TSTAT_HUM_CURRENT_DEFAULT)//TBD: modify this function as before // decide which table will run, default tabel or customer table   current=1 default=0		
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = 0;//Table2_Five_LO;

					}					
					
				else if((address >= MODE_OUTPUT1) && (address <= MODE_OUTPUT5))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = OutputRange(address - MODE_OUTPUT1);

					}		
		
				else if(address == OUTPUT1_SCALE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output1Scale;

					}		
					
				else if(address == OUTPUT2_SCALE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output2Scale;

					}					

				else if(address == TSTAT_DIGITAL_OUTPUT_STATUS)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = ~RELAY_1TO5.BYTE;

					}						

				else if(address == TSTAT_COOLING_VALVE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = 0;
						
					if(GetByteBit(&EEP_OutputManuEnable,5))
						{
						temp1 = ManualAO1_HI;//read_eeprom(MANUAL_COOL_VALVE_HI);
						temp2 = ManualAO1_LO;//read_eeprom(MANUAL_COOL_VALVE_LO);
						}							
					else
						{
						temp1 = (valve[0]>> 8) & 0xFF;
						temp2 = valve[0] & 0xFF;					
						}
						

					}					
					
				else if(address == TSTAT_HEATING_VALVE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = 0;
						
					if(GetByteBit(&EEP_OutputManuEnable,6))
						{
						temp1 = ManualAO2_HI;//read_eeprom(MANUAL_HEAT_VALVE_HI);
						temp2 = ManualAO2_LO;//read_eeprom(MANUAL_HEAT_VALVE_LO);
						}							
					else
						{
						temp1 = (valve[1]>> 8) & 0xFF;
						temp2 = valve[1] & 0xFF;					
						}
						

					}
					

	//	DAC_OFFSET,	           //					 // 0	255	DAC voltage offset
					
				else if((address >= OUTPUT1_DELAY_OFF_TO_ON) && (address <= OUTPUT7_DELAY_OFF_TO_ON))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = Output_Delay_OfftoOn(address - OUTPUT1_DELAY_OFF_TO_ON);

					}				

	//	DELAY1_OFF_TO_ON_TIME_CURRENT,//				
	//	DELAY2_OFF_TO_ON_TIME_CURRENT,//				
	//	DELAY3_OFF_TO_ON_TIME_CURRENT,//			
	//	DELAY4_OFF_TO_ON_TIME_CURRENT,//			
	//	DELAY5_OFF_TO_ON_TIME_CURRENT,//
	//	DELAY6_OFF_TO_ON_TIME_CURRENT,//future
	//	DELAY7_OFF_TO_ON_TIME_CURRENT,//future
				else if((address >= OUTPUT1_DELAY_ON_TO_OFF) && (address <= OUTPUT7_DELAY_ON_TO_OFF))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = Output_Delay_OntoOff(address - OUTPUT1_DELAY_ON_TO_OFF);

					}							

	//	DELAY1_ON_TO_OFF_TIME_CURRENT,//	            
	//	DELAY2_ON_TO_OFF_TIME_CURRENT,//				
	//	DELAY3_ON_TO_OFF_TIME_CURRENT,//				
	//	DELAY4_ON_TO_OFF_TIME_CURRENT,//				
	//	DELAY5_ON_TO_OFF_TIME_CURRENT,//	
	//	DELAY6_ON_TO_OFF_TIME_CURRENT,//future		
	//	DELAY7_ON_TO_OFF_TIME_CURRENT,//future
					
				else if(address == CYCLING_DELAY)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = CyclingDelay;

					}	

				else if(address == CHANGOVER_DELAY)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = ChangeOverDelay;

					}					
					

				else if(address == VALVE_TRAVEL_TIME)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_ValveTravelTime;

					}		

					
				else if(address == VALVE_PERCENT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					if(GetByteBit(&EEP_OutputManuEnable,7))	
							temp2 = ManualValvePercent;
					else
							temp2 = valve_target_percent;

					}

				else if(address == INTERLOCK_OUTPUT1)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock1;

					}	
					
				else if(address == INTERLOCK_OUTPUT2)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock2;

					}	
				else if(address == INTERLOCK_OUTPUT3)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock3;

					}	
				else if(address == INTERLOCK_OUTPUT4)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock4;

					}	
				else if(address == INTERLOCK_OUTPUT5)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock5;

					}	
				else if(address == INTERLOCK_OUTPUT6)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock6;

					}	
				else if(address == INTERLOCK_OUTPUT7)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Interlock7;

					}	

				else if(address == FREEZE_DELAY_ON)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_FreezeDelayOn;

					}					
				else if(address == FREEZE_DELAY_OFF)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_FreezeDelayOff;

					}	

				else if(address == OUTPUT_MANU_ENABLE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_OutputManuEnable;  

					}	
					
				else if((address >= MANU_RELAY1) && (address <= MANU_RELAY5))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = ManualRelay(address - MANU_RELAY1);//GetByteBit(&ManualRelay, (address - MANU_RELAY1));

					}
					
				else if(address == MANUAL_AO1)// 
					{
					temp1 = ManualAO1_HI;//(>> 8) & 0xff;//
					temp2 = ManualAO1_LO;

					}					
					
				else if(address == MANUAL_AO2)// 
					{
					temp1 = ManualAO2_HI;//(>> 8) & 0xff;//
					temp2 = ManualAO2_LO;

					}	

				else if(address == DEADMASTER_AUTO_MANUAL)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = DeadMsater_AutoManual;

					}				

				else if(address == DEADMASTER_OUTPUT_STATE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = DeadMsater_OutputState;

					}	
					
				else if(address == DEADMASTER_COOL_OUTPUT)// 
					{
					temp1 = DeadMsater_CoolOutput_HI;//(>> 8) & 0xff;//
					temp2 = DeadMsater_CoolOutput_LO;

					}	

				else if(address == DEADMASTER_HEAT_OUTPUT)// 
					{
					temp1 = DeadMsater_HeatOutput_HI;//(>> 8) & 0xff;//
					temp2 = DeadMsater_HeatOutput_LO;

					}					
					
				else if(address == OUTPUT1_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output1Function;

					}					

				else if(address == OUTPUT2_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output2Function;

					}

				else if(address == OUTPUT3_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output3Function;

					}

				else if(address == OUTPUT4_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output4Function;

					}

				else if(address == OUTPUT5_FUNCTION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Output5Function;

					}		
					
				else if(address == OUTPUT6_FUNCTION)
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = Out6_Function;
					}	

				else if(address == OUTPUT7_FUNCTION)
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = Out7_Function;
					}	
					

				else if(address == FAN_SPEED)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = fan_speed_user;//EEP_FanSpeed;

					}	
					
				else if(address == PID_OUTPUT1)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[0];//EEP_PidOutput1;

					}
				else if(address == PID_OUTPUT2)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[1];//EEP_PidOutput2;

					}
				else if(address == PID_OUTPUT3)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[2];
					//temp2 = EEP_PidOutput3;

					}
				else if(address == PID_OUTPUT4)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[3];//EEP_PidOutput4;

					}
				else if(address == PID_OUTPUT5)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[4];//EEP_PidOutput5;

					}
				else if(address == PID_OUTPUT6)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[5];//EEP_PidOutput6;

					}
				else if(address == PID_OUTPUT7)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid_ctrl_bit[6];//EEP_PidOutput7;

					}				

				else if((address >= UNIVERSAL_OUTPUT_BEGIN) && (address <= UNIVERSAL_OUTPUT_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = PID2_Output_Table(address - UNIVERSAL_OUTPUT_BEGIN);

					}		
					
				else if((address >= FAN0_OPER_TABLE_COAST) && (address <= FAN0_OPER_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = FanTable0(address - FAN0_OPER_TABLE_COAST);

					}

				else if((address >= FAN1_OPER_TABLE_COAST) && (address <= FAN1_OPER_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = FanTable1(address - FAN1_OPER_TABLE_COAST);

					}				

				else if((address >= FAN2_OPER_TABLE_COAST) && (address <= FAN2_OPER_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = FanTable2(address - FAN2_OPER_TABLE_COAST);

					}	

				else if((address >= FAN3_OPER_TABLE_COAST) && (address <= FAN3_OPER_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = FanTable3(address - FAN3_OPER_TABLE_COAST);

					}					
				
				else if((address >= FANAUT_OPER_TABLE_COAST) && (address <= FANAUT_OPER_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = FanTableAuto(address - FANAUT_OPER_TABLE_COAST);

					}	

				else if((address >= VALVE_OPER_TABLE_BEGIN) && (address <= VALVE_OPER_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = PID1_Valve_Table(address - VALVE_OPER_TABLE_BEGIN);

					}	
					
				else if(address == HEAT_UNIVERSAL_TABLE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_HEAT_TABLE2;

					}

				else if(address == COOL_UNIVERSAL_TABLE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_COOL_TABLE2;

					}
					
				else if(address == HEAT_ORIGINAL_TABLE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_HEAT_TABLE1;

					}

				else if(address == COOL_ORIGINAL_TABLE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_COOL_TABLE1;

					}

				else if((address >= VALVE_OFF_TABLE_COAST) && (address <= VALVE_OFF_TABLE_HEAT3))// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = PID1_Valve_Off_Table(address - VALVE_OFF_TABLE_COAST);

					}					
					
				else if(address == DEFAULT_SETPOINT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_DefaultSetpoint;

					}

				else if(address == SETPOINT_CONTROL)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_SetpointControl;

					}
	//DAYSETPOINT_OPTION
	//MIDDLE_SETPOINT
				else if(address == DAY_SETPOINT)// 
					{
				temp1 = EEP_DaySpHi;
				temp2 = EEP_DaySpLo;
						

					}

				else if(address == DAY_COOLING_DEADBAND)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_DayCoolingDeadband;

					}				

				else if(address == DAY_HEATING_DEADBAND)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_DayHeatingDeadband;

					}	

				else if(address == DAY_COOLING_SETPOINT)// 
					{
					temp1 = EEP_DayCoolingSpHi;//(>> 8) & 0xff;//
					temp2 = EEP_DayCoolingSpLo;

					}	

				else if(address == DAY_HEATING_SETPOINT)// 
					{
					temp1 = EEP_DayHeatingSpHi;//(>> 8) & 0xff;//
					temp2 = EEP_DayHeatingSpLo;

					}	

				else if(address == NIGHT_SETPOINT)// 
					{
					temp1 = EEP_NightSpHi;//(>> 8) & 0xff;//
					temp2 = EEP_NightSpLo;

					}				

				else if(address == APPLICATION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Application;

					}	

				else if(address == NIGHT_HEATING_DEADBAND)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_NightHeatingDeadband;

					}	
					
				else if(address == NIGHT_COOLING_DEADBAND)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_NightCoolingDeadband;

					}	
					
				else if(address == NIGHT_HEATING_SETPOINT)// 
					{
					temp1 = EEP_NightHeatingSpHi;//(>> 8) & 0xff;//
					temp2 = EEP_NightHeatingSpLo;

					}					

				else if(address == NIGHT_COOLING_SETPOINT)// 
					{
					temp1 = EEP_NightCoolingSpHi;//(>> 8) & 0xff;//
					temp2 = EEP_NightCoolingSpLo;

					}					
					

		//WINDOW_INTERLOCK_COOLING_SETPOINT,   //TBD
		//WINDOW_INTERLOCK_HEATING_SETPOINT,	//TBD
					
				else if(address == UNIVERSAL_NIGHTSET)// 
					{
					temp1 = EEP_UniversalNightSetpointHi;//(>> 8) & 0xff;//
					temp2 = EEP_UniversalNightSetpointLo;

					}				
					
				else if(address == UNIVERSAL_SET)// 
					{
					temp1 = EEP_UniversalSetpointHi;//(>> 8) & 0xff;//
					temp2 = EEP_UniversalSetpointLo;

					}	

				else if(address == UNIVERSAL_HEAT_DB)// 
					{
					temp1 = UniversalHeatDB_HI;//(>> 8) & 0xff;//
					temp2 = UniversalHeatDB_LO;

					}

				else if(address == UNIVERSAL_COOL_DB)// 
					{
					temp1 = UniversalCoolDB_HI;//(>> 8) & 0xff;//
					temp2 = UniversalCoolDB_LO;

					}				

								
		//ECOMONY_COOLING_SETPOINT,
		//ECOMONY_HEATING_SETPOINT,
					
				else if(address == POWERUP_SETPOINT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_PowerupSetpoint;

					}	

				else if(address == MAX_SETPOINT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_MaxSetpoint;

					}				
		
				else if(address == MIN_SETPOINT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_MinSetpoint;

					}	
					
	//	MAX_SETPOINT2,//			// max and min setpoint for celling setpoint
	//	MIN_SETPOINT2,//
	//	MAX_SETPOINT3,//			// max and min setpoint for average setpoint 
	//	MIN_SETPOINT3,//	
	//	MAX_SETPOINT4,//
	//	MIN_SETPOINT4,//
					
				else if(address == SETPOINT_INCREASE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_SetpointIncrease;

					}				

					
				else if(address == FREEZE_TEMP_SETPOINT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_FreezeSetpoint;

					}	

				
				else if(address == TEMP_SELECT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_TempSelect;

					}

				else if(address == INPUT1_SELECT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_Input1Select;

					}
					
				else if(address == COOLING_PID)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid[0] & 0xFF;

					}				

				else if(address == COOLING_PTERM)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_PTERM1;

					}						
					
				else if(address == COOLING_ITERM)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_ITERM1;

					}					

				else if(address == UNIVERSAL_PTERM)// 
					{
					temp1 = EEP_PTERM2_HI;//(>> 8) & 0xff;//
					temp2 = EEP_PTERM2_LO;

					}					

				else if(address == UNIVERSAL_ITERM)// 
					{
					temp1 = EEP_ITERM2_HI;//(>> 8) & 0xff;//
					temp2 = EEP_ITERM2_LO;

					}	

				else if(address == TSTAT_PID_UNIVERSAL)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = pid[1] & 0xFF;

					}					
		
				else if(address == UNITS1_HIGH)// 
					{
					temp1 = UnitS1_HI_HI;//(>> 8) & 0xff;//
					temp2 = UnitS1_HI_LO;

					}	 

				else if(address == UNITS1_LOW)// 
					{
					temp1 = UnitS1_LO_HI;//(>> 8) & 0xff;//
					temp2 = UnitS1_LO_LO;

					}	 				

				else if(address == UNITS2_HIGH)// 
					{
					temp1 = UnitS2_HI_HI;//(>> 8) & 0xff;//
					temp2 = UnitS2_HI_LO;

					}	 

				else if(address == UNITS2_LOW)// 
					{
					temp1 = UnitS2_LO_HI;//(>> 8) & 0xff;//
					temp2 = UnitS2_LO_LO;

					}	

				else if(address == TSTAT_PID2_MODE_OPERATION)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					if(current_mode_of_operation[1] >= 4 && current_mode_of_operation[1] <= 6)
						temp2 = current_mode_of_operation[1] + 10;
				
					else if(current_mode_of_operation[1] >= 7 && current_mode_of_operation[1] <= 9)
						temp2 = current_mode_of_operation[1] - 3;
					
					else if(current_mode_of_operation[1] >= 10 && current_mode_of_operation[1] <= 12)
						temp2 = current_mode_of_operation[1] + 7;
				
					else  
						temp2 = current_mode_of_operation[1];					
						

					}	

				else if(address == KEYPAD_SELECT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_KeypadSelect;

					}					

				else if(address == SPECIAL_MENU_LOCK)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_SpecialMenuLock;

					}	

	//			else if(address == DISPLAY)// 
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = EEP_Display;
	//				uart_send[send_cout++] = temp1 ;
	//				uart_send[send_cout++] = temp2 ;
	//				crc16_byte(temp1);
	//				crc16_byte(temp2);		
	//				}	
					
	//	ICON,
				else if(address == LAST_KEY_TIMER)// 
					{
					temp1 = (lastkey_counter>> 8) & 0xff;//
					temp2 = lastkey_counter & 0xff;

					}					
					
				else if(address == TSTAT_KEYPAD_VALUE)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = keypad_value;
							 
					keypad_value = 0;//reset once the register has been read

					}	


	//	DISPLAY_HUNDRED,//
	//	DISPLAY_TEN,//
	//	DISPLAY_DIGITAL,//
	//	DISPLAY_STATUS,//
				else if(address == ROUND_DISPLAY)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_RoundDisplay;

					}					

				else if(address == MIN_ADDRESS)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_MinAddress;

					}	

				else if(address == MAX_ADDRESS)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_MaxAddress;

					}						
				
	//	EEPROM_SIZE,//	
				else if(address == TIMER_SELECT)// 
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = EEP_TimerSelect;

					}

				else if(address == RTC_YEAR)//
					{
					temp1 = (calendar.w_year >> 8) & 0xff;//
					temp2 = calendar.w_year;

					}

				else if(address == RTC_MONTH)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = calendar.w_month;

					}

				else if(address == RTC_DAY)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = calendar.w_date;

					}

				else if(address == RTC_HOUR)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = calendar.hour;

					}

				else if(address == RTC_MINUTE)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = calendar.min;

					}

				else if(address == RTC_SECOND)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = calendar.sec;

					}

				else if(address == RTC_WEEK)//
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = calendar.week;

					}				
					
					
					
					
					
	// 	MONTH,//
	// 	WEEK,//
	// 	DAY,//
	//	HOUR,//
	// 	MINUTE,//
	// 	SECOND,//
	//	DIAGNOSTIC_ALARM,//

	//			else if(address == SCHEDULE_DHOME_HOUR)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleDHomeHour;

	//				}	
	//			else if(address == SCHEDULE_DHOME_MIN)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleDHomeMin;

	//				}	
	//			else if(address == SCHEDULE_WORK_HOUR)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleWorkHour;

	//				}	
	//			else if(address == SCHEDULE_WORK_MIN)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleWorkMin;

	//				}	
	//			else if(address == SCHEDULE_NHOME_HOUR)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleNHomeHour;

	//				}	
	//			else if(address == SCHEDULE_NHOME_MIN)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleNHomeMin;

	//				}					
	//			else if(address == SCHEDULE_SLEEP_HOUR)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleSleepHour;

	//				}	
	//			else if(address == SCHEDULE_SLEEP_MIN)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleSleepMin;

	//				}
	//			else if(address == SCHEDULE_HOLIDAY_BYEAR)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleHolidayByear;

	//				}					
	//			else if(address == SCHEDULE_HOLIDAY_BMONTH)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleHolidayBmon;

	//				}	
	//			else if(address == SCHEDULE_HOLIDAY_BDAY)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleHolidayBday;

	//				}	
	//			else if(address == SCHEDULE_HOLIDAY_EYEAR)//
	//				{
	//				temp1 = 0;//
	//				temp2 = ScheduleHolidayEyear;

	//				}					
	//			else if(address == SCHEDULE_HOLIDAY_EMONTH)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleHolidayEmon;

	//				}	
	//			else if(address == SCHEDULE_HOLIDAY_EDAY)//
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = ScheduleHolidayEday;

	//				}	

	//				
	//			else if(address == DAY2_EVENT4_HI)// 
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = Day2_Event4_HI;

	//				}	
	//			else if(address == DAY2_EVENT4_LO)// 
	//				{
	//				temp1 = 0;//(>> 8) & 0xff;//
	//				temp2 = Day2_Event4_LO;

	//				}     

				else if(address == LCD_TURN_OFF)
					{
					temp1 = 0;//(>> 8) & 0xff;//
					temp2 = LCDTurnOff;

					}  

				else if((address >= LINE1_CHAR1) && (address <= LINE2_CHAR4))
					{
					temp1 = User_Info((address - LINE1_CHAR1) * 2);
					temp2 = User_Info((address - LINE1_CHAR1)*2 + 1);

					}  				

				else if((address >= INTERNAL_CHAR1) && (address <= INTERNAL_CHAR4))
					{
					temp1 = Disp_internal_sensor((address - INTERNAL_CHAR1) * 2);
					temp2 = Disp_internal_sensor((address - INTERNAL_CHAR1)*2 + 1);

					}
		
				else if((address >= AI1_CHAR1) && (address <= AI1_CHAR4))
					{
					temp1 = Disp_AI1((address - AI1_CHAR1) * 2);
					temp2 = Disp_AI1((address - AI1_CHAR1)*2 + 1);
			
					}
				else if((address >= AI2_CHAR1) && (address <= AI2_CHAR4))
					{
					temp1 = Disp_AI2((address - AI2_CHAR1) * 2);
					temp2 = Disp_AI2((address - AI2_CHAR1)*2 + 1);
			
					}
				else if((address >= AI3_CHAR1) && (address <= AI3_CHAR4))
					{
					temp1 = Disp_AI3((address - AI3_CHAR1) * 2);
					temp2 = Disp_AI3((address - AI3_CHAR1)*2 + 1);
			
					}
				else if((address >= AI4_CHAR1) && (address <= AI4_CHAR4))
					{
					temp1 = Disp_AI4((address - AI4_CHAR1) * 2);
					temp2 = Disp_AI4((address - AI4_CHAR1)*2 + 1);
			
					}
				else if((address >= AI5_CHAR1) && (address <= AI5_CHAR4))
					{
					temp1 = Disp_AI5((address - AI5_CHAR1) * 2);
					temp2 = Disp_AI5((address - AI5_CHAR1)*2 + 1);
			
					}
				else if((address >= AI6_CHAR1) && (address <= AI6_CHAR4))
					{
					temp1 = Disp_AI6((address - AI6_CHAR1) * 2);
					temp2 = Disp_AI6((address - AI6_CHAR1)*2 + 1);
			
					}
				else if((address >= AI7_CHAR1) && (address <= AI7_CHAR4))
					{
					temp1 = Disp_AI7((address - AI7_CHAR1) * 2);
					temp2 = Disp_AI7((address - AI7_CHAR1)*2 + 1);
			
					}
				else if((address >= AI8_CHAR1) && (address <= AI8_CHAR4))
					{
					temp1 = Disp_AI8((address - AI8_CHAR1) * 2);
					temp2 = Disp_AI8((address - AI8_CHAR1)*2 + 1);
			
					}				

				else if((address >= OUTPUT1_CHAR1) && (address <= OUTPUT1_CHAR4))
					{
					temp1 = Disp_OUT1((address - OUTPUT1_CHAR1) * 2);
					temp2 = Disp_OUT1((address - OUTPUT1_CHAR1)*2 + 1);
			
					}	
				else if((address >= OUTPUT2_CHAR1) && (address <= OUTPUT2_CHAR4))
					{
					temp1 = Disp_OUT2((address - OUTPUT2_CHAR1) * 2);
					temp2 = Disp_OUT2((address - OUTPUT2_CHAR1)*2 + 1);
			
					}
				else if((address >= OUTPUT3_CHAR1) && (address <= OUTPUT3_CHAR4))
					{
					temp1 = Disp_OUT3((address - OUTPUT3_CHAR1) * 2);
					temp2 = Disp_OUT3((address - OUTPUT3_CHAR1)*2 + 1);
			
					}
				else if((address >= OUTPUT4_CHAR1) && (address <= OUTPUT4_CHAR4))
					{
					temp1 = Disp_OUT4((address - OUTPUT4_CHAR1) * 2);
					temp2 = Disp_OUT4((address - OUTPUT4_CHAR1)*2 + 1);
			
					}
				else if((address >= OUTPUT5_CHAR1) && (address <= OUTPUT5_CHAR4))
					{
					temp1 = Disp_OUT5((address - OUTPUT5_CHAR1) * 2);
					temp2 = Disp_OUT5((address - OUTPUT5_CHAR1)*2 + 1);
			
					}
				else if((address >= OUTPUT6_CHAR1) && (address <= OUTPUT6_CHAR4))
					{
					temp1 = Disp_OUT6((address - OUTPUT6_CHAR1) * 2);
					temp2 = Disp_OUT6((address - OUTPUT6_CHAR1)*2 + 1);
			
					}
				else if((address >= OUTPUT7_CHAR1) && (address <= OUTPUT7_CHAR4))
					{
					temp1 = Disp_OUT7((address - OUTPUT7_CHAR1) * 2);
					temp2 = Disp_OUT7((address - OUTPUT7_CHAR1)*2 + 1);
			
					}

					
	//	SCHEDULEA_CHAR1,	//						// 		
	//	SCHEDULEA_CHAR2,
	//	SCHEDULEA_CHAR3,
	//	SCHEDULEA_CHAR4,
	//	SCHEDULEB_CHAR1,							//		
	//	SCHEDULEB_CHAR2,
	//	SCHEDULEB_CHAR3,
	//	SCHEDULEB_CHAR4,
	//	SCHEDULEC_CHAR1,							//		
	//	SCHEDULEC_CHAR2,
	//	SCHEDULEC_CHAR3,//
	//	SCHEDULEC_CHAR4,
	//	SCHEDULED_CHAR1,							//		
	//	SCHEDULED_CHAR2,
	//	SCHEDULED_CHAR3,
	//	SCHEDULED_CHAR4, 
	//	WALL_CHAR1,							//		
	//	WALL_CHAR2,
	//	WALL_CHAR3,
	//	WALL_CHAR4,
	//	CEILING_CHAR1,//						//		
	//	CEILING_CHAR2,
	//	CEILING_CHAR3,
	//	CEILING_CHAR4,
	//	OUTDOOR_CHAR1,//
	//	OUTDOOR_CHAR2,
	//	OUTDOOR_CHAR3,
	//	OUTDOOR_CHAR4,
	//	AVERAGE_CHAR1,//
	//	AVERAGE_CHAR2,
	//	AVERAGE_CHAR3,//
	//	AVERAGE_CHAR4,
	//	LCD_SCREEN1,//						
	//	LCD_SCREEN2,//		

				else if(address == DEMAND_RESPONSE)
					{
					temp1 = 0;
					temp2 = DemandResponse;
			
					}

				else if(address == LOCK_REGISTER)
					{
					temp1 = 0;
					temp2 = LockRegister;
			
					}
		 
				else if(address == PIR_STAGE)
					{
					temp1 = 0;
					temp2 = EEP_PirStage;
			
					}

				else if(address == PIR_STAGE)
					{
					temp1 = 0;
					temp2 = EEP_PirStage;
			
					}				

	//	FIRST_CAL_FLAG, //	        // picdataok				
	//	HUM_CAL_EREASE,//			// erease current PIC calibration data table  
	//	HUMCOUNT3_H ,//		
	//	HUMRH3_H	,	//		
	//	HUMCOUNT4_H ,//		
	//	HUMRH4_H	,	//			
	//	HUMCOUNT5_H ,//	
	//	HUMRH5_H	,	//			
	//	HUMCOUNT6_H ,//			
	//	HUMRH6_H	,	//			
	//	HUMCOUNT7_H ,//			
	//	HUMRH7_H	,	//			
	//	HUMCOUNT8_H ,//			
	//	HUMRH8_H	,	//			
	//	HUMCOUNT9_H ,//			
	//	HUMRH9_H	,	//				
	//	HUMCOUNT10_H,//			
	//	HUMRH10_H,   //  
	//	HUM_LOCK_A,
	//	HUM_LOCK_B,

				else if(address == LCD_ROTATE_ENABLE)
					{
					temp1 = 0;
					temp2 = lcd_rotate_max;
			
					}
					
				else if(address == SCHEDULE_ON_OFF)
					{
					temp1 = 0;
					temp2 = schedule_on_off;
			
					}

				else if((address >= DISP_ITEM_TEMPERATURE) && (address <= DISP_ITEM_OUT7)) 
					{
					temp1 = 0;
					temp2 = disp_item_queue(address - DISP_ITEM_TEMPERATURE);
			
					}				
					
				else if((address >= OUTPUT_PWM_AUTO_COAST) && (address <= OUTPUT_PWM_AUTO_HEAT3)) 
					{
					temp1 = 0;
					temp2 = Output_PWM_Table(address - OUTPUT_PWM_AUTO_COAST);
			
					}	
					
				else if(address == PWM_OUT1)
				{
					temp1 = 0;
					temp2 = pwm1_percent[0];
				}					

				
				else if(address == PWM_OUT1)
				{
					temp1 = 0;
					temp2 = pwm1_percent[1];
				}		
				else if(address == EXT_SENSOR_TEMP_CAL) //temperature sensor on hum board calibration
					{
					temp1 = Calibration_External_Tem_HI;
					temp2 = Calibration_External_Tem_LO;
			
					}
		
	//	PWM_OUT4,
	//	PWM_OUT5,
	//	SUN_ICON_CONTROL, //					
	//	MOON_ICON_CONTROL,//

	//			else if(address == BUTTON_LEFT1) //
	//				{
	//				temp1 = 0;
	//				temp2 = LeftButton1;
	//		
	//				}				
	//				
	//			else if(address == BUTTON_LEFT2) //
	//				{
	//				temp1 = 0;
	//				temp2 = LeftButton2;
	//		
	//				}				

	//			else if(address == BUTTON_LEFT3) //
	//				{
	//				temp1 = 0;
	//				temp2 = LeftButton3;
	//		
	//				}	

	//			else if(address == BUTTON_LEFT4) //
	//				{
	//				temp1 = 0;
	//				temp2 = LeftButton4;
	//		
	//				}

	//			else if(address == HUM_HEATING_CONTROL) //
	//				{
	//				temp1 = 0;
	//				temp2 = HumHeatControl;
	//		
	//				}					

				else if(address == HUM_INPUT_MANUAL_ENABLE) //
					{
					temp1 = 0;
					temp2 = hum_manual_enable;
			
					}
					
				else if(address == HUM_INPUT_MANUAL_VALUE) //
					{
					temp1 = HumInputManualValue_HI;
					temp2 = HumInputManualValue_LO;
			
					}				
					
				else if(address == CO2_INPUT_MANUAL_ENABLE) //
					{
					temp1 = 0;
					temp2 = co2_manual_enable;
			
					}
					
				else if(address == CO2_INPUT_MANUAL_VALUE) //
					{
					temp1 = CO2InputManualValue_HI;
					temp2 = CO2InputManualValue_LO;
			
					}
					
				else if(address == CO2_CALIBRATION) //
					{
					temp1 = Calibration_CO2_HI;
					temp2 = Calibration_CO2_LO;
			
					}
					
				else if((address >= UNIVERSAL_OFF_TABLE_BEGIN) && (address <= UNIVERSAL_OFF_TABLE_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID2_Output_OFF_Table(address - UNIVERSAL_OFF_TABLE_BEGIN);
			
					}					

				else if((address >= UNIVERSAL_OFF_VALVE_BEGIN) && (address <= UNIVERSAL_OFF_VALVE_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID2_Valve_OFF_Table(address - UNIVERSAL_OFF_VALVE_BEGIN);
			
					}
		
				else if(address == KEYPAD_REVERSE) //
					{
					temp1 = 0;
					temp2 = KeypadReverse;
			
					}

				else if(address == LIGHT_SENSOR) //
					{
					temp1 = (light_sensor >> 8) & 0xff;
					temp2 = light_sensor & 0xff;
			
					}				
					
				else if(address == PIR_SENSOR_SELECT) //
					{
					temp1 = 0;
					temp2 = PirSensorSelect;
			
					}	
					
				else if(address == PIR_SENSOR_VALUE) //
					{
					temp1 = (pir_value >> 8) & 0xff;
					temp2 = pir_value & 0xff;
			
					}				

				else if(address == PIR_SENSOR_ZERO) //
					{
					temp1 = 0;
					temp2 = PirSensorZero;
			
					}					

				else if((address >= UNIVERSAL_VALVE_BEGIN) && (address <= UNIVERSAL_VALVE_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID2_Valve_Table(address - UNIVERSAL_VALVE_BEGIN);
			
					}				

				else if(address == TSTAT_ID_WRITE_ENABLE) //
					{
					temp1 = 0;
					temp2 = ID_Lock;			
					}					
				else if(address == PWM_ENABLE)
				{
					temp1 = 0;
					temp2 = pwm_duty/10;
				}					
        else if(address == SCHEDULE_MODE_NUM)
				{
					temp1 = 0;
					temp2 = ScheduleModeNum;
				}
				
				
				else if(address == PIR_TIMER) //
					{
					temp1 = (override_timer_time >> 8) & 0xff;
					temp2 = override_timer_time & 0xff;
			
					}					

				else if(address == SUPPLY_SETPOINT) //
					{
					temp1 = Supply_SP_HI;
					temp2 = Supply_SP_LO;
			
					}					

				else if(address == MAX_SUPPLY_SETPOINT) //
					{
					temp1 = Supply_Max_SP_HI;
					temp2 = Supply_Max_SP_LO;
			
					}						
		
				else if(address == MIN_SUPPLY_SETPOINT) //
					{
					temp1 = Supply_Min_SP_HI;
					temp2 = Supply_Min_SP_LO;
			
					}
					
				else if(address == MAX_AIRFLOW_COOLING) //
					{
					temp1 = Max_AirflowSP_Cool_HI;
					temp2 = Max_AirflowSP_Cool_LO;
			
					}

				else if(address == MAX_AIRFLOW_HEATING) //
					{
					temp1 = Max_AirflowSP_Heat_HI;
					temp2 = Max_AirflowSP_Heat_LO;
			
					}				

					
				else if(address == OCC_MIN_AIRFLOW) //
					{
					temp1 = Occ_Min_Airflow_HI;
					temp2 = Occ_Min_Airflow_LO;
			
					}	

				else if(address == AIRFLOW_SETPOINT) //
					{
					temp1 = Airflow_SP_HI;
					temp2 = Airflow_SP_LO;
			
					}		

				else if(address == VAV_CONTROL) //
					{
					temp1 = 0;
					temp2 = VAV_Control;
			
					}	

				else if(address == PID3_INPUT_SELECT) //
					{
					temp1 = 0;
					temp2 = PID3_InputSelect;
			
					}	
					
				else if((address >= PID3_VALVE_OPER_TABLE_BEGIN) && (address <= PID3_VALVE_OPER_TABLE_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID3_Valve_Table(address - PID3_VALVE_OPER_TABLE_BEGIN);
			
					}				

				else if(address == PID3_COOLING_DB) //
					{
					temp1 = 0;
					temp2 = pid3_cool_db;
			
					}					

					
				else if(address == PID3_HEATING_DB) //
					{
					temp1 = 0;
					temp2 = pid3_heat_db;
			
					}		
					
				else if(address == PID3_PTERM) //
					{
					temp1 = 0;
					temp2 = pid3_pterm;
			
					}						

				else if(address == PID3_ITERM) //
					{
					temp1 = 0;
					temp2 = pid3_iterm;
			
					}					

				else if(address == PID3_HEAT_STAGE) //
					{
					temp1 = 0;
					temp2 = EEP_HEAT_TABLE3;
			
					}	
					
				else if(address == PID3_COOL_STAGE) //
					{
					temp1 = 0;
					temp2 = EEP_COOL_TABLE3;
			
					}	

				else if(address == PID3_OUTPUT) //
					{
					temp1 = 0;
					temp2 = pid[2];
			
					}				

				else if((address >= PID3_OUTPUT_BEGIN) && (address <= PID3_OUTPUT_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID3_Output_Table(address - PID3_OUTPUT_BEGIN);
			
					}					
					
				else if((address >= PID3_VALVE_OFF_TABLE_BEGIN) && (address <= PID3_VALVE_OFF_TABLE_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID3_Valve_Off_Table(address - PID3_VALVE_OFF_TABLE_BEGIN);
			
					}		 

				else if((address >= PID3_OFF_OUTPUT_BEGIN) && (address <= PID3_OFF_OUTPUT_HEAT3)) 
					{
					temp1 = 0;
					temp2 = PID3_Output_OFF_Table(address - PID3_OFF_OUTPUT_BEGIN);
			
					}				
									
	//	WIRELESS_PIR_RESPONSE1,
	//	WIRELESS_PIR_RESPONSE2,
	//	WIRELESS_PIR_RESPONSE3,
	//	WIRELESS_PIR_RESPONSE4,
	//	WIRELESS_PIR_RESPONSE5,
				else if(address == TSTAT_HEAT_COOL) //
					{
					temp1 = 0;
					temp2 = heat_cool_user;
			
					}					
					
	//	SPARE1,
	//	SPARE2,
				else if(address == TSTAT_HUM_PIC_VERSION) //
					{
					temp1 = 0;
					temp2 = Hum_PIC_Version;
			
					}					
									
				else if(address == INTERNAL_SENSOR_MANUAL) //
					{
					temp1 = 0;
					temp2 = EEP_InterThermistorManual;
			
					}					

				else if(address == PRESSURE_VALUE) //
					{
					temp1 = 0;
					temp2 = 0;
			
					}						

				else if(address == PRESSURE_MANUAL_VALUE) //
					{
					temp1 = PressureManualValue_HI;
					temp2 = PressureManualValue_LO;
			
					}		

				else if(address == PRESSURE_MANUAL_ENABLE) //
					{
					temp1 = 0;
					temp2 = PressureManualEnable;
			
					}					
					
				else if(address == AQ_VALUE) //
					{
					temp1 = (aq_value>>8) & 0xff;
					temp2 = aq_value & 0xff;		
					}	
					
				else if(address == AQ_MANUAL_VALUE) //
					{
					temp1 = AQManualValue_HI;
					temp2 = AQManualValue_LO;
			
					}				

				else if(address == AQ_MANUAL_ENABLE) //
					{
					temp1 = 0;
					temp2 = AQManualEnable;
			
					}					
	//	TEMPERATURE_OF_PRESSURE_SENSOR,
	//	TEMPERATURE_OF_HUM_SENSOR,
					
				else if(address == SP_DISPLAY_SELECT) //
					{
					temp1 = 0;
					temp2 = SP_DisplaySelect;
			
					}		

				else if(address == PID3_DAY_SP) //
					{
					temp1 = PID3_DaySP_HI;
					temp2 = PID3_DaySP_LO;
			
					}					

				else if(address == PID3_NIGHT_SP) //
					{
					temp1 = PID3_NightSP_HI;
					temp2 = PID3_NightSP_LO;
			
					}	
	 
	//	BK_SN,
	//	BK_SNLOLO,
	//	BK_SNLOHI,	
	//	BK_SNHILO,
	//	BK_SNHIHI,
	//	BK_PN,
	//	BK_HWN,
					
				else if(address == TSTAT_NAME_ENABLE) //
					{
					temp1 = 0;
					temp2 = 0x56;
			
					}					
					
				else if((address >= TSTAT_NAME1) && (address <= TSTAT_NAME10)) //
					{
					temp1 = UserInfo_Name((address - TSTAT_NAME1)*2);
					temp2 = UserInfo_Name((address - TSTAT_NAME1)*2 + 1);
			
					}	

				else if(address == SHOW_ID) //
					{
					temp1 = 0;
					temp2 = Show_ID_Enable;
			
					}				

				else if(address == TRANSDUCER_RANGE_MIN) //
					{
					temp1 = 0;
					temp2 = MinTransducerRange;
			
					}					

				else if(address == TRANSDUCER_RANGE_MAX) //
					{
					temp1 = 0;
					temp2 = MaxTransducerRange;
			
					}		

				else if(address == ICON_MANUAL_MODE) //
					{
					temp1 = 0;
					temp2 = ICON_ManualMode;
			
					}

				else if(address == ICON_MANUAL_VALUE) //
					{
					temp1 = 0;
					temp2 = ICON_ManualValue;
			
					}	

//				else if(address == HUM_CALIBRATION) //
//					{
//					temp1 = hum_cal_hi;
//					temp2 = hum_cal_lo;
//			
//					}	
//				
				else if(address == DTERM) //
					{
					temp1 = 0;//dew_data >> 8;
					temp2 = EEP_DTERM1 & 0xff;		
					}	

				else if(address == PID_SAMPLE_TIME)		
					{
						temp1 = 0;
						temp2 = PidSampleTime;
					}

					
//					
//				else if(address == HUM_C_TEMP) //
//					{
//					temp1 = 0;//(sensor_hum_temp >> 8) & 0xff;
//					temp2 = 0;//sensor_hum_temp & 0xff;
//			
//					}	
//				else if(address == HUM_C_VALUE) //
//					{
//					temp1 = 0;//(sensor_hum_value >> 8) & 0xff;
//					temp2 = 0;//sensor_hum_value & 0xff;
//			
//					}	
//				else if(address == HUM_C_FRE) //
//					{
//					temp1 = 0;//(sensor_hum_fre >> 8) & 0xff;
//					temp2 = 0;//sensor_hum_fre & 0xff;
//			
//					}				

//				else if(address == HUM_C_POINTS) //
//					{
//					temp1 = 0;
//					temp2 = 0;//hum_size_copy;
//			
//					}	

//				else if((address >= HUM_C_RH1) && (address <= HUM_C_RH10) && ((address-HUM_C_RH1)%2 == 0))//
//					{
//					temp1 = Hum_C_RH_HI((address - HUM_C_RH1)/2);
//					temp2 = Hum_C_RH_LO((address - HUM_C_RH1)/2);
//			
//					}
//				else if((address >= HUM_C_RH1) && (address <= HUM_C_RH10) && ((address-HUM_C_RH1)%2 == 1))//
//					{
//					temp1 = Hum_C_FRE_HI((address - HUM_C_FRE1)/2);
//					temp2 = Hum_C_FRE_LO((address - HUM_C_FRE1)/2);
//			
//					}

//				else if(address == HUM_C_REFRESH_TABLE) //
//					{
//					temp1 = 0;
//					temp2 = HumRefreshTable;
//			
//					}	
//					
//				else if(address == HUM_C_ERASE) //
//					{
//					temp1 = 0;
//					temp2 = HumErase;
//			
//					}	
//				else if(address == HUM_C_LOCK) //
//					{
//					temp1 = 0;
//					temp2 = HumLock;		
//					}	
					
				else if(address == TSTAT_TEST1) //
					{
					temp1 = (ctest[0] >> 8) & 0xff;
					temp2 = ctest[0] & 0xff;			
					}

				else if(address == TSTAT_TEST2) //
					{
					temp1 = (ctest[1] >> 8) & 0xff;
					temp2 = ctest[1] & 0xff;
			
					}
					
				else if(address == TSTAT_TEST3) //
					{
					temp1 = (ctest[2] >> 8) & 0xff;
					temp2 = ctest[2] & 0xff;			
					}
					
				else if(address == TSTAT_TEST4) //
					{
					temp1 = (ctest[3] >> 8) & 0xff;
					temp2 = ctest[3] & 0xff;
					}

				else if(address == TSTAT_TEST5) //
					{
					temp1 = (ctest[4] >> 8) & 0xff;
					temp2 = ctest[4] & 0xff;		
					}
					
				else if(address == TSTAT_TEST6) //
					{
					temp1 = (ctest[5] >> 8) & 0xff;
					temp2 = ctest[5] & 0xff;			
					}	

				else if(address == TSTAT_TEST7) //
					{
					temp1 = (ctest[6] >> 8) & 0xff;
					temp2 = ctest[6] & 0xff;
					}	

				else if(address == TSTAT_TEST8) //
					{
					temp1 = (ctest[7] >> 8) & 0xff;
					temp2 = ctest[7]  & 0xff;
			
					}					

				else if(address == TSTAT_TEST9) //
					{
					temp1 = (ctest[8] >> 8) & 0xff;
					temp2 = ctest[8]  & 0xff;
			
					}			

				else if(address == TSTAT_TEST10) //
					{
					temp1 = (ctest[9] >> 8) & 0xff;
					temp2 = ctest[9]  & 0xff;				
					}							
					
					
				else if(address == SPARE1)
					{
					temp1 = 0;//(ctest1 >> 8) & 0xff;
					temp2 = SN_WR_Flag & 0xff;

					}

				
				
				else if((address >= MODE1_NAME1) && (address <= MODE5_NAME4))
					{
					temp1 = Modename((address - MODE1_NAME1)*2);
					temp2 = Modename((address - MODE1_NAME1)*2 + 1);

					} 
				
				else if(address == OUTSIDETEM)
				{
					temp1 = (outside_tem >> 8) & 0xff;
					temp2 = outside_tem & 0xff;

				}
				
				else if(address == OUTSIDETEM_SLAVE)
				{
					temp1 = 0;//
					temp2 = outside_tem & 0xff;

				}
				
				else if(address == SLEEP_SP)
				{
					temp1 = Sleep_sp_hi;//
					temp2 = Sleep_sp_lo;
		
				}
				
				else if(address == SLEEP_COOLING_SP)
				{
					temp1 = SleepCoolSp_h;//
					temp2 = SleepCoolSp_l;
		
				}			

				else if(address == SLEEP_HEATING_SP)
				{
					temp1 = SleepHeatSp_h;//
					temp2 = SleepHeatSp_l;
		
				}					

				else if(address == SLEEP_COOLING_DB)
				{
					temp1 = 0;//
					temp2 = SleepCooldb;
			
				}

				else if(address == SLEEP_HEATING_DB)
				{
					temp1 = 0;//
					temp2 = SleepHeatdb;
			
				}			

				else if(address == MAX_WORK_SP)
				{
					temp1 = 0;//
					temp2 = Max_work_sp;
		
				}				

				else if(address == MIN_WORK_SP)
				{
					temp1 = 0;//
					temp2 = Min_work_sp;
		
				}

				else if(address == MAX_SLEEP_SP)
				{
					temp1 = 0;//
					temp2 = Max_sleep_sp;
		
				}				

				else if(address == MIN_SLEEP_SP)
				{
					temp1 = 0;//
					temp2 = Min_sleep_sp;
		
				}
				else if(address == MAX_HOLIDAY_SP)
				{
					temp1 = 0;//
					temp2 = Max_holiday_sp;
				}
				
				else if(address == INPUT_VOLTAGE_TERM)
				{
					temp1 = 0;
					temp2 = InputVoltageTerm;
				}

				else if((address >= ICON_DAY_OUTPUT_CONTROL) && (address <= ICON_FAN3_OUTPUT_CONTROL))
				{	
					temp1 = 0;
					temp2 = IconOutputControl(address - ICON_DAY_OUTPUT_CONTROL);
				}					

			  else if((address >= OUTPUT_PWM_OFF_COAST) && (address <= OUTPUT_PWM_OFF_HEAT3))
				{	
					temp1 = 0;
					temp2 = Output_PWM_Off_Table(address - OUTPUT_PWM_OFF_COAST);
				}	
				
				else if(address == MIN_HOLIDAY_SP)
				{
					temp1 = 0;//
					temp2 = Min_holiday_sp;		
				}

//				else if(address == BOARD_SELECT)
//				{
//					temp1 = 0;//
//					temp2 = BoardSelect;
//				}
				
				else if(address == SET_PIR_SENSETIVITY)
				{
					temp1 = 0;//(Pir_Sensetivity >> 8) & 0xff;//
					temp2 = Pir_Sensetivity & 0xff;
					
				}
				
				else if((address >= SCHEDULE_MONDAY_EVENT1_H)&&(address <= SCHEDULE_MONDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleMondayEvent1(address - SCHEDULE_MONDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_TUESDAY_EVENT1_H)&&(address <= SCHEDULE_TUESDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleTuesdayEvent1(address - SCHEDULE_TUESDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_WENSDAY_EVENT1_H)&&(address <= SCHEDULE_WENSDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleWensdayEvent1(address - SCHEDULE_WENSDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_THURSDAY_EVENT1_H)&&(address <= SCHEDULE_THURSDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleThursdayEvent1(address - SCHEDULE_THURSDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_FRIDAY_EVENT1_H)&&(address <= SCHEDULE_FRIDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleFridayEvent1(address - SCHEDULE_FRIDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_SATDAY_EVENT1_H)&&(address <= SCHEDULE_SATDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleSatdayEvent1(address - SCHEDULE_SATDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_SUNDAY_EVENT1_H)&&(address <= SCHEDULE_SUNDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleSundayEvent1(address - SCHEDULE_SUNDAY_EVENT1_H);
			
				}

				else if((address >= SCHEDULE_HOLIDAY_EVENT1_H)&&(address <= SCHEDULE_HOLIDAY_EVENT6_M))
				{
					temp1 = 0;
					temp2 =  ScheduleHolidayEvent1(address - SCHEDULE_HOLIDAY_EVENT1_H);
			
				}
//				else if((address >= SCHEDULE_MONDAY_FLAG)&&(address <= SCHEDULE_HOLIDAY_FLAG))
//				{
//					temp1 = ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG)*2 + 1);
//					temp2 = ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG)*2);
//			
//				}			

				else if((address >= SCHEDULE_MONDAY_FLAG_1)&&(address <= SCHEDULE_HOLIDAY_FLAG_2))
				{
					if((address - SCHEDULE_MONDAY_FLAG_1)%2 == 0)//first flag reg
					{
						temp1 = ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG_1)/2*3 + 1);
						temp2 = ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG_1)/2*3);					
					}						
					else//second flag reg
					{
						temp1 = 0;
						temp2 = ScheduleMondayFlag((address - SCHEDULE_MONDAY_FLAG_1 -1)/2 *3 + 2);	
					}									
				}	
				
				
				else if((address >= SCHEDULE_DAY_BEGAIN)&&(address <= SCHEDULE_DAY_END))
				{
					temp1 = 0;
					temp2 = ScheduleDay(address - SCHEDULE_DAY_BEGAIN);					
				}	
				
				else if(address == AWAY_SP)
				{
					temp1 = Away_sp_hi;
					temp2 = Away_sp_lo;	
				}

				else if(address == FRC_ENABLE)//free cooling  function enable/disable 0:DIABLE  1:ENABLE
				{
					temp1 = 0;
					temp2 =  FrcEnable;
			
				}
				else if(address == FRC_BASE_SELECT)//1001FREE COOLING BASE SELECT 0: TEMPERATUERE BASE 1: ENTHALPY BASE
				{
					temp1 = 0;
					temp2 = FrcBaseSelect;			
				}
				//----------------------------------------
				else if(address == FRC_SPACETEM_SOURCE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = 0;
					temp2 = spacetem.source;			
				}
				else if(address == FRC_SPACETEM_VALUE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = (spacetem.value >> 8)& 0xff;
					temp2 = spacetem.value & 0xff;			
				}

				else if(address == FRC_SPACETEM_UNIT)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = 0;
					if(spacetem.source == FRC_LOCAL_SENSOR)
						temp2 = EEP_DEGCorF;
					else
						temp2 = FrcSpaceTemUnit;
				}
				
				else if(address == FRC_SPACETEM_ID)//1003 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
				{
					temp1 = 0;//FrcSpaceTemIDHi;
					temp2 =  spacetem.id;			
				}
	
				else if(address == FRC_SPACETEM_LASTUPDATE)//1004 FREE COOLING SPACE TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
					{
					temp1 = 0;
					temp2 = 60 - spacetem.update ;
			
				}
				else if(address == FRC_SPACETEM_STATUE)//1005 CHECK IF SPACE TEMPERATURE SOURCE IS WORK WELL
					{
					temp1 = 0;
					temp2 =  spacetem.status;
			
				}
				else if(address == FRC_SPACETEM_CONFIG)//1006 FREE COOLING SPACE TEMPERATURE STATUS, CHECK RANGE,UNIT
				{
					temp1 = 0;
					temp2 = spacetem.config;
			
				}					
  			//---------------------------------------------
				else if(address == FRC_SUPPLYTEM_SOURCE)//1007 FREE COOLING SUPPLY TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = 0;
					temp2 = supplytem.source;
			
				}
				else if(address == FRC_SUPPLYTEM_VALUE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = (supplytem.value >> 8)& 0xff;
					temp2 = supplytem.value & 0xff;			
				}
				
				else if(address == FRC_SUPPLYTEM_UNIT)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = 0;
					if(supplytem.source == FRC_LOCAL_SENSOR)
						temp2 = EEP_DEGCorF;
					else
						temp2 = FrcSupplyTemUnit;
				}
				
				else if(address == FRC_SUPPLYTEM_ID)//1008 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
				{
					temp1 = 0;//FrcSupplyTemIDHi;
					temp2 =  supplytem.id;
			
				}
				else if(address == FRC_SUPPLYTEM_LASTUPDATE)//1009 FREE COOLING SUPPLY TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
					{
					temp1 = 0;
					temp2 = 60 - supplytem.update ;
			
				}
				else if(address == FRC_SUPPLYTEM_STATUE)//1010 CHECK IF SUPPLY TEMPERATURE SOURCE IS WORK WELL
					{
					temp1 = 0;
					temp2 =  supplytem.status;
			
				}
				else if(address == FRC_SUPPLYTEM_CONFIG)//1011 FREE COOLING SUPPLY TEMPERATURE STATUS, CHECK RANGE,UNIT 
					{
					temp1 = 0;
					temp2 =  supplytem.config;
			
				}
	     //----------------------------------------
				else if(address == FRC_OUTDOORTEM_SOURCE)//1012 FREE COOLING OUTDOOR TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
					{
					temp1 = 0;
					temp2 =  outdoortem.source;
			
				}
				else if(address == FRC_OUTDOORTEM_VALUE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = (outdoortem.value >> 8)& 0xff;
					temp2 = outdoortem.value & 0xff;			
				}					
				else if(address == FRC_OUTDOORTEM_UNIT)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = 0;
					if(outdoortem.source == FRC_LOCAL_SENSOR)
						temp2 = EEP_DEGCorF;
					else
						temp2 = FrcOutdoorTemUnit;
				}				
				
				else if(address == FRC_OUTDOORTEM_ID)//1013 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
					{
					temp1 = 0;//FrcOutdoorTemIDHi;
					temp2 =  outdoortem.id;
			
				}
				else if(address == FRC_OUTDOORTEM_LASTUPDATE)//1014 FREE COOLING OUTDOOR TEMPERATURE LAST UPDATE TIME, UNIT: MINUTS
					{
					temp1 = 0;
					temp2 = 60 - outdoortem.update;
			
				}
				else if(address == FRC_OUTDOORTEM_STATUE)//1015 CHECK IF OUTDOOR TEMPERATURE SOURCE IS WORK WELL
					{
					temp1 = 0;
					temp2 =  outdoortem.status;
			
				}
				else if(address == FRC_OUTDOORTEM_CONFIG)//1016 FREE COOLING OUTDOOR TEMPERATURE STATUS, CHECK RANGE,UNIT
				{
					temp1 = 0;
					temp2 = outdoortem.config ;
			
				}					
				//---------------------------------------
				else if(address == FRC_INDOORHUM_SOURCE)//1017 FREE COOLING INDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
					{
					temp1 = 0;
					temp2 = indoorhum.source;
			
				}
				else if(address == FRC_INDOORHUM_VALUE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = (indoorhum.value >> 8)& 0xff;
					temp2 = indoorhum.value & 0xff;			
				}
				
				else if(address == FRC_INDOORHUM_ID)//1018 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
					{
					temp1 = 0;//FrcIndoorHumIDHi;
					temp2 = indoorhum.id;
			
				}
				else if(address == FRC_INDOORHUM_LASTUPDATE)//1019 FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
					{
					temp1 = 0;
					temp2 = 60 - indoorhum.update ;
			
				}
				else if(address == FRC_INDOORHUM_STATUE)//1020 CHECK IF INDOOR HUMIDITY SOURCE IS WORK WELL
					{
					temp1 = 0;
					temp2 =  indoorhum.status;
			
				}
				else if(address == FRC_INDOORHUM_CONFIG)//1021 FREE COOLING INDOOR HUMIDITY STATUS, CHECK RANGE,UNIT
				{
					temp1 = 0;
					temp2 = indoorhum.config ;
			
				}
   		 //------------------------------------------		
				else if(address == FRC_OUTDOORHUM_SOURCE)//1022 FREE COOLING OUTDOOR HUMIDITY SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = 0;
					temp2 = outdoorhum.source;
			
				}
				else if(address == FRC_OUTDOORHUM_VALUE)//1002FREE COOLING SPACE TEMPERATURE SOURCE SELECT, INTERNAL OR NETWORK
				{
					temp1 = (outdoorhum.value >> 8)& 0xff;
					temp2 = outdoorhum.value & 0xff;			
				}
				
				else if(address == FRC_OUTDOORHUM_ID)//1023 ??? COULD BE INTERNAL SENSOR, AI1 TO AI8 OR GET FROM NETWORK
				{
					temp1 = 0;//FrcOutdoorHumIDHi;
					temp2 = outdoorhum.id;
			
				}						
				else if(address == FRC_OUTDOORHUM_LASTUPDATE)//1024 FREE COOLING OUTDOOR HUMIDITY LAST UPDATE TIME, UNIT: MINUTS
				{
					temp1 = 0;
					temp2 = 60 - outdoorhum.update ;
			
				}						
				else if(address == FRC_OUTDOORHUM_STATUE)//1025 CHECK IF OUTDOOR HUMIDITY SOURCE IS WORK WELL
				{
					temp1 = 0;
					temp2 = outdoorhum.status ;
			
				}						
				else if(address == FRC_OUTDOORHUM_CONFIG)//1026 FREE COOLING OUTDOOR HUMIDITY STATUS, CHECK RANGE,UNIT
				{
					temp1 = 0;
					temp2 = outdoorhum.config ;
			
				}		
			//-----------------------------------------
				else if(address == FRC_MIN_FRESH_AIR)//1027 MINIMAL FRESH AIR, UNIT PERCENTAGE
				{
					temp1 = 0;
					temp2 =  FrcMinFreshAir;
			
				}						
				else if(address == FRC_MIN_FRESH_AIRTEM)//1028 MINIMAL FRESH AIR TEMPERATURE, UNIT DEGREE C/F
				{
					temp1 = FrcMinFreshAirTemHi;
					temp2 = FrcMinFreshAirTemLow ;
			
				}	
				else if(address == FRC_OUTPUT_SELECT) 
				{
					temp1 = 0;
					temp2 = frc_output_select;
				}					
				else if(address == FRC_OUTPUT_CONFIG)  //1029 FREE COOLING OUTPUT CHECK, OUTPUT SOURCE, OUTPUT FUNCTION
				{
					temp1 = 0;
					temp2 =  FrcOutputConfig;
			
				}						
				else if(address == FRC_PID2_CONFIG)   //1030 FREE COOLING PID2 CONFIG CHECK, CHECK IF PID2 HAS INPUT FROM SUPPLY SENSOR, ALSO CHECK IF IT IS SET TO NEGATIVE COOLING
				{
					temp1 = 0;
					temp2 = FrcPid2Config ;
			
				}								
//				else if(address == FRC_COOLING_DEADBAND)//1031 FREE COOLING DEADBAND UNIT: DEGREE C/F
//				{
//					temp1 = 0;
//					temp2 = FrcCoolDeadband ;
//			
//				}						
				else if(address == FRC_OUTPUT_MODE)  //1032 SET FAN TO OFF
				{
					temp1 = 0;
					temp2 = FrcOutputMode;
			
				}						
//				else if(address == FRC_FULL_FRESH)//1033 SET FULL FRESH AIR INPUT
//				{
//					temp1 = 0;
//					temp2 = FrcFullFresh;
//			
//				}						
				else if(address == FRC_TOTAL_CONFIG)//1034 MINIMAL SUPPLY AIR TEMPERATURE UNIT: DEGREE C/F
				{
					temp1 = 0;//FrcTotalConfig;
					temp2 = FrcTotalConfig;
			
				}						
				else if(address == FRC_PRESENT_MODE)//1035				
				{
					temp1 = 0;
					temp2 = FrcPresentMode;		
				}	
				else if(address == FRC_OUTDOOR_ENTHALPY)
				{
					temp1 = (frc_outdoorhum_enthalpy >> 8) & 0xff;
					temp2 = frc_outdoorhum_enthalpy & 0xff;
				}					
				else if(address == FRC_INDOOR_ENTHALPY)
				{
					temp1 = (frc_indoorhum_enthalpy >> 8) & 0xff;
					temp2 = frc_indoorhum_enthalpy & 0xff;
				}	
				else if(address == FRC_PID2_VALUE)
				{
					temp1 = 0;
					temp2 = pid[1] & 0xff;
				}
				else if(address == BACNET_STATION_NUM)
				{
					temp1 = 0;
					temp2 = Station_NUM;
				}	

				else if(address == BACNET_INSTANCE)
				{
					temp1 = Instance >> 8;
					temp2 = Instance & 0xff;
				}
				
				else if(address == MODBUS_4TO20MA_BOTTOM)
				{
					temp1 = bottom_of_4to20ma >> 8;
					temp2 = bottom_of_4to20ma & 0xff;
				}

				else if(address == MODBUS_4TO20MA_TOP)
				{
					temp1 = top_of_4to20ma >> 8;
					temp2 = top_of_4to20ma & 0xff;
				}

				else if(address == MODBUS_4TO20MA_UNIT_HI)
				{
					temp1 = read_eeprom(EEP_4TO20MA_UNIT_HI + 1);
					temp2 = read_eeprom(EEP_4TO20MA_UNIT_HI);
				}				
				
				else if(address == MODBUS_4TO20MA_UNIT_LO)
				{
					temp1 = read_eeprom(EEP_4TO20MA_UNIT_LO + 1);
					temp2 = read_eeprom(EEP_4TO20MA_UNIT_LO);
				}								
				
				else if((address >= BAC_T1) && (address <= BAC_T8))
				{
					temp1 = 0;
					temp2 = read_eeprom(BAC_TEST1 + address - BAC_T1);
				}
					
				else
					{
					temp1 = 0 ;
					temp2 = 0;
					}	
					if(uartsel == 0)//
					{				
						uart_send[send_cout++] = temp1 ;
						uart_send[send_cout++] = temp2 ;
						crc16_byte(temp1);
						crc16_byte(temp2);
					}
					else
					{
						uart_sendB[send_cout++] = temp1 ;
						uart_sendB[send_cout++] = temp2 ;
						crc16_byte(temp1);
						crc16_byte(temp2);
					}
					
				}
//			}
		}
		
			
	else if(*(bufadd+1) == CHECKONLINE)
		{
		if(uartsel == 0)	
		{
			uart_send[send_cout++] = *(bufadd) ; //0xff
			uart_send[send_cout++] = *(bufadd+1) ;	//0x19
		}
		else
		{
			uart_sendB[send_cout++] = *(bufadd) ; //0xff
			uart_sendB[send_cout++] = *(bufadd+1) ;	//0x19
		}
		crc16_byte(*(bufadd));
		crc16_byte(*(bufadd+1));
		
		if(uartsel == 0)	
			uart_send[send_cout++] = laddress;					//modbus ID
		else
			uart_sendB[send_cout++] = laddress;					//modbus ID
		
		crc16_byte(laddress);	
		if(uartsel == 0)	
		{
			uart_send[send_cout++] = SerialNumber(0);//serialnumber[0];		//SN first byte
			uart_send[send_cout++] = SerialNumber(1);//serialnumber[1];		//SN second byte
			uart_send[send_cout++] = SerialNumber(2);//serialnumber[2];		//SN third byte
			uart_send[send_cout++] = SerialNumber(3);//serialnumber[3];		//SN fourth byte
		}
		else
		{
			uart_sendB[send_cout++] = SerialNumber(0);//serialnumber[0];		//SN first byte
			uart_sendB[send_cout++] = SerialNumber(1);//serialnumber[1];		//SN second byte
			uart_sendB[send_cout++] = SerialNumber(2);//serialnumber[2];		//SN third byte
			uart_sendB[send_cout++] = SerialNumber(3);//serialnumber[3];		//SN fourth byte
		}
		crc16_byte(SerialNumber(0));
		crc16_byte(SerialNumber(1));
		crc16_byte(SerialNumber(2));
		crc16_byte(SerialNumber(3));			
		}
		
	else
			return;
	
	temp1 = CRChi ;
	temp2 = CRClo; 
  
 #ifdef TSTAT_ZIGBEE	
	if(uartsel == 0)
	{
		uart_send[send_cout++] = temp1 ;
		uart_send[send_cout++] = temp2 ;
		USART_SendDataString(send_cout);
		
	}
	else
	{
		uart_sendB[send_cout++] = temp1 ;
		uart_sendB[send_cout++] = temp2 ;
		USART_SendDataStringB(send_cout);
	}
	#else
		uart_send[send_cout++] = temp1 ;
		uart_send[send_cout++] = temp2 ;
		USART_SendDataString(send_cout);
	#endif
}


uint8 checkdata(void)
{
uint16 xdata crc_val ;
uint8 i;
uint8 minaddr,maxaddr, variable_delay;
	
if((USART_RX_BUFB[0] != 255) && (USART_RX_BUFB[0] != laddress) && (USART_RX_BUFB[0] != 0))
	return false;

if(USART_RX_BUFB[1] != READ_VARIABLES && USART_RX_BUFB[1] != WRITE_VARIABLES && USART_RX_BUFB[1] != MULTIPLE_WRITE_VARIABLES && USART_RX_BUFB[1] != CHECKONLINE)
	return false;

if(USART_RX_BUFB[1] == CHECKONLINE)
	{
	crc_val = crc16(USART_RX_BUFB,4);
	if(crc_val != USART_RX_BUFB[4]*256 + USART_RX_BUFB[5])
		{
		return false;
		}
	minaddr = (USART_RX_BUFB[2] >= USART_RX_BUFB[3] ) ? USART_RX_BUFB[3] : USART_RX_BUFB[2] ;	
	maxaddr = (USART_RX_BUFB[2] >= USART_RX_BUFB[3] ) ? USART_RX_BUFB[2] : USART_RX_BUFB[3] ;	
	if(laddress < minaddr || laddress > maxaddr)
		return false;
	else			
		{	// in the TRUE case, we add a random delay such that the Interface can pick up the packets
		//srand();
		variable_delay = rand() % 10;
//		ctest1 = variable_delay;	
//		ctest2++; 
		for (i=0; i<variable_delay; i++)
			{	
			if(EEP_Baudrate == 0)	
				delay_ms(5);
			else if(EEP_Baudrate == 1)	
				delay_ms(4);
			else if(EEP_Baudrate == 2)	
				delay_ms(3);
			else if(EEP_Baudrate == 3)	
				delay_ms(2);
			else if(EEP_Baudrate == 4)	
				delay_ms(1);
			else
				delay_ms(10);
			}
		return true;
		}
		 
	}
	
if(USART_RX_BUFB[2]*256 + USART_RX_BUFB[3] ==  PLUG_N_PLAY)                        

{                                                                                             
	if(USART_RX_BUFB[4] == 0x55)  //if this is a plug and play write command                                                     
		{                                                                                         
		if(USART_RX_BUFB[6] != SerialNumber(0))                            
		return FALSE;                                                                             
		if(USART_RX_BUFB[7] != SerialNumber(1))                          
		return FALSE;                                                                                
		if(USART_RX_BUFB[8] != SerialNumber(2))                            
		return FALSE;                                                                             
		if(USART_RX_BUFB[9] != SerialNumber(3))                          
		return FALSE;                                                                             
		}                                                                                          
} 

return true;
}

#ifdef TSTAT_ZIGBEE
uint8 checkdataB(void)
{
uint16 xdata crc_val ;
uint8 i;
uint8 minaddr,maxaddr, variable_delay;
	
if((USART_RX_BUFD[0] != 255) && (USART_RX_BUFD[0] != laddress) && (USART_RX_BUFD[0] != 0))
	return false;

if(USART_RX_BUFD[1] != READ_VARIABLES && USART_RX_BUFD[1] != WRITE_VARIABLES && USART_RX_BUFD[1] != MULTIPLE_WRITE_VARIABLES && USART_RX_BUFD[1] != CHECKONLINE)
	return false;

if(USART_RX_BUFD[1] == CHECKONLINE)
	{
	crc_val = crc16(USART_RX_BUFD,4);
	if(crc_val != USART_RX_BUFD[4]*256 + USART_RX_BUFD[5])
		{
		return false;
		}
	minaddr = (USART_RX_BUFD[2] >= USART_RX_BUFD[3] ) ? USART_RX_BUFD[3] : USART_RX_BUFD[2] ;	
	maxaddr = (USART_RX_BUFD[2] >= USART_RX_BUFD[3] ) ? USART_RX_BUFD[2] : USART_RX_BUFD[3] ;	
	if(laddress < minaddr || laddress > maxaddr)
		return false;
	else			
		{	// in the TRUE case, we add a random delay such that the Interface can pick up the packets
		//srand();
		variable_delay = rand() % 10;
//		ctest1 = variable_delay;	
//		ctest2++; 
		for (i=0; i<variable_delay; i++)
			{	
			if(EEP_Baudrate == 0)	
				delay_ms(5);
			else if(EEP_Baudrate == 1)	
				delay_ms(4);
			else if(EEP_Baudrate == 2)	
				delay_ms(3);
			else if(EEP_Baudrate == 3)	
				delay_ms(2);
			else if(EEP_Baudrate == 4)	
				delay_ms(1);
			else
				delay_ms(10);
			}
		return true;
		}
		 
	}
	
if(USART_RX_BUFD[2]*256 + USART_RX_BUFD[3] ==  PLUG_N_PLAY)                        

{                                                                                             
	if(USART_RX_BUFD[4] == 0x55)  //if this is a plug and play write command                                                     
		{                                                                                         
		if(USART_RX_BUFD[6] != SerialNumber(0))                            
		return FALSE;                                                                             
		if(USART_RX_BUFD[7] != SerialNumber(1))                          
		return FALSE;                                                                                
		if(USART_RX_BUFD[8] != SerialNumber(2))                            
		return FALSE;                                                                             
		if(USART_RX_BUFD[9] != SerialNumber(3))                          
		return FALSE;                                                                             
		}                                                                                          
} 

return true;
}
#endif //TSTAT_ZIGBEE

 void dealwithData(void)
{
	if(checkdata())
	{
		//USART_ITConfig(USART1, USART_IT_RXNE/*|USART_IT_TC*/, ENABLE);				//开启中断
		initSend_COM();

	// Initialize CRC
		InitCRC16();
	
	// Store any data being written
		
		responseData(USART_RX_BUFB, SENDUART1);
		internalDeal(USART_RX_BUFB);

		
		
	}	

	// Restart the serial receive.
	else
		serial_restart();
	//dealwith_package = 0;
}

#ifdef TSTAT_ZIGBEE
 void dealwithDataB(void)
{

	if(checkdataB())
	{
		//USART_ITConfig(USART1, USART_IT_RXNE/*|USART_IT_TC*/, ENABLE);				//开启中断
//		initSend_COM();

	// Initialize CRC
		InitCRC16();
	
	// Store any data being written
		internalDeal(USART_RX_BUFD);

		responseData(USART_RX_BUFD, SENDUART4);
		
	}	

	// Restart the serial receive.
	else
		serial_restartB();
	//dealwith_package = 0;
}
#endif 

void modbus_data_cope(u8 XDATA* pData, u16 length, u8 conn_id)
{
	
}

uint8 checkCrc(void)
{
	u16 crc_val ;
	crc_val = crc16(USART_RX_BUFA,rece_size -2) ;

	if(crc_val == USART_RX_BUFA[rece_size-2]*256 + USART_RX_BUFA[rece_size-1] )
	{	
		return true;
	}
	else
	{  
		return false;
	}
}


#ifdef TSTAT_ZIGBEE
uint8 checkCrcB(void)
{
	u16 crc_val ;
	crc_val = crc16(USART_RX_BUFC,rece_sizeB - 2) ;

	if(crc_val == USART_RX_BUFC[rece_sizeB - 2 ]*256 + USART_RX_BUFC[rece_sizeB - 1] )
	{	
		return true;
	}
	else
	{  
		return false;
	}
}
#endif //TSTAT_ZIGBEE