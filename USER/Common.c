#include "define.h"
#include "eepDefine.h"
#include "LibTstatIO.h"
#include "constcode.h"


//#ifdef BROADCASTTEST
//uing8 xdata bctest = 0;
//#endif

extern uint8 xdata laddress;
extern bool int_flag;
extern bool gbDisableRs485;
extern bool  OVERRIDE_NEWDAY;
extern bool  ROTATION_NEWDAY;
extern bool time_caculate; 
extern uint8 xdata hour,minute,second;
extern uint16 xdata  millisecond;
extern uint16 xdata override_timer_time;
extern int16 idata valve_pulse ;
extern int16 xdata valve_current_position;
extern int16 idata valve_pulse ;
extern int8  idata command;
extern uint32 xdata daytimer_ticks;
extern uint32 xdata daytimer_ticks_buf;
extern bool initialize_valve;
extern uint8 xdata lcd_rotate_max; 
extern bool gbSerialFlag;
extern bool serial_int_flag;
extern bool new_heartbeat;
extern uint8 xdata serial_receive_timeout_count;
extern uint8 xdata SERIAL_RECEIVE_TIMEOUT;
extern uint8 xdata rece_count;
extern uint8 xdata rece_size ;
extern uint8 xdata serial_response_delay;
extern uint8 xdata gucResponseDelay;
extern bool transmit_finished;
extern uint8 xdata  data_buffer[DATABUFLEN_MUL];
extern uint8 xdata display_wrote[4];
extern bool serial_no_activity;
extern uint8 xdata dealwithTag;
extern uint8 xdata net_activity_counter;
extern bool net_activity_flag;
extern uint8 xdata gucLedCounter;
extern uint8 heart_beat; 
//extern bool gbLED;
extern uint8 xdata DispBuf[9]; 
extern bool menu_mode;
extern bool dis6;
extern struct link xdata  b;

#ifdef CONFIGURE_TOOL
extern uint8 xdata  receive_data;
extern bool receiving_schedual_byte;
extern bool success_write_schedual;
extern void configure_info( void );
#endif
void clock( void );
extern void watchdog();
extern void setwatchdog ( void );
extern void initSend_COM(void);
extern void serial_restart(void);
extern bool checkData(void);
extern bool checkCrc(void);
extern void delay_us(uint16);
extern void OperateMotor( int8 motor, int8 action );	


void timer0(void) interrupt 1 					 
{
	
  	heart_beat++ ; //decrement the global heartbeat	counter	controlling software timers
		//Originally: 	FFFF - DF80 = 8319 counts 
		//Try: 			FFFF - EC77 = 5000 counts <- flashing display
		//Try: 			FFFF - EE6B = 4500 counts <- ??

//	  	TL0  = 0x6b ;  //MDOCT1 used to be 80   //Reset the interrupt counter
//	    TH0  = 0xee ;  //MDOCT2, used to be df  //we experimented with different values, this is OK
		TL0 = 0x4;     //interal : 2.5ms
		TH0 = 0xF7;
   #ifdef RUNAR
	gucLedCounter++;
	if(gucLedCounter > 3)
	{
	//	gbLED = !gbLED;
		gucLedCounter = 0;
	}
	#endif
	millisecond++; 
	new_heartbeat = TRUE; 
//	if(EEP_Display == 5)
//		{
//		DispBuf[0] = display_wrote[0];	
//		DispBuf[1] = display_wrote[1];
//		DispBuf[2] = display_wrote[2];
//		DispBuf[3] = display_wrote[3];
//		}
	if(EEP_Display == 6 && menu_mode == 0 && dis6 == 0)
		{
		DispBuf[0] = 0xff;	
		DispBuf[1] = 0xff;
		DispBuf[2] = 0xff;
		DispBuf[3] = 0xff;
		}
	#ifdef CONFIGURE_TOOL
  		configure_info( );
	#endif	

    clock( );
//	#ifdef ZIGBEE
//	if(ZIGBEE_CONTROL == ZIGBEE_ENABLE) //ZIGBEE module need to communicate with main chip, so disable RS485
//		{
//		RS232STATE = SEND;
//		gbDisableRs485 = 1;
//		}
//	else
//		{
//		RS232STATE = RECEIVE;
//		gbDisableRs485 = 0;
//		}
//	#endif
} //-------------timer0() interrupt 1------------------------

/////////////////////////////////////////////////////////////////////////
//timer2 interrupt service routine
void Time2(void) interrupt 5
{
	TF2=0;
	watchdog();
	 
	daytimer_ticks_buf++; 
    if(initialize_valve)
	{
		if(daytimer_ticks_buf>(uint16)EEP_ValveTravelTime*100)
		{
			OperateMotor( CLOSE_MOTOR, STOP );
			OperateMotor( OPEN_MOTOR, STOP );
			daytimer_ticks_buf=0;
			initialize_valve=0;
			command=REST;
		}
	}
	if( command == DRIVE_VALVE )
	{
		if( valve_pulse > 0 )
		{
			valve_pulse--;    //valve pulse has the extra time at 0% and 100% added already. 
			//if(!opening_flag)
			valve_current_position++;
			if(valve_current_position > (int16)EEP_ValveTravelTime*100)
				valve_current_position = (int16)EEP_ValveTravelTime*100 ;
		//	else 
			//valve_current_position=(int16)EEP_ValveTravelTime*100;
		}
		else if( valve_pulse < 0 )
		{

			valve_pulse++;
			//if (!closing_flag)
			if(valve_current_position > 0)
			valve_current_position-- ;

			 
			//else  
			//valve_current_position=0 ;

		}
	}
}
/////////////////////////////////////////////////////////////////////////
//serial interrupt service routine
//	serial port interrupt , deal with all action about serial port. include receive data and 
//		send data and deal with interal.
void SerialPort() interrupt 4
{
	if(RI == 1  )
	{	
	
		int_flag = 1;	
	    serial_int_flag = 1;
		RI = 0;
//		#ifdef CONFIGURE_TOOL 
//		 if(receive_data == RECEIVE_PC_DATA)
//		{
//		#endif
			// Make sure that you do not exceed
			// the length of the data bufer
//			if(rece_count < DATABUFLEN_SCAN)
		if(rece_count < DATABUFLEN_MUL)
			data_buffer[rece_count++] = SBUF;
		else
			serial_restart();
	
		if(rece_count == 1)
			{
				// This starts a timer that will reset communication.  If you do not
				// receive the full packet, it insures that the next receive will be fresh.
				// The timeout is roughly 7.5ms.  (3 ticks of the hearbeat)
				rece_size = DATABUFLEN;
				serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;	
			}
		else if(rece_count == 4)
			{
			//check if it is a scan command
			if((uint16)(data_buffer[2]<<8) + data_buffer[3] == 0x0a && data_buffer[1] == WRITE_VARIABLES)
				{
				rece_size = DATABUFLEN_SCAN;
				serial_receive_timeout_count = SERIAL_RECEIVE_TIMEOUT;

				}
			}
		else if(rece_count == 6 && data_buffer[1] == CHECKONLINE)
			{
			serial_response_delay = gucResponseDelay;
				rece_size = rece_count;
				gbSerialFlag = 1;
			//	ES = DISABLE_UART;
			}

		else if(rece_count == 7)
			{
			if(data_buffer[1] == MULTIPLE_WRITE_VARIABLES)
				{
				rece_size = data_buffer[6] + 9;
			//	serial_receive_timeout_count = data_buffer[6] + 8;//from 4 to 8 to see if can resolve multiple problem
				}
			}



			// As soon as you receive the final byte, switch to SEND mode
		else if( ((rece_count == rece_size )&& (data_buffer[1] == READ_VARIABLES || data_buffer[1] == WRITE_VARIABLES || data_buffer[1] == MULTIPLE_WRITE_VARIABLES))/* || (rece_count == 6 && data_buffer[1] == CHECKONLINE)*/)
			{	 								   
				// full packet received - turn off serial timeout
				serial_receive_timeout_count = 0;
				// MDF 6/25/04 changed the timing of responses to valid and invalid packets.
				if(checkCrc())//((data_buffer[0] == laddress)||(data_buffer[0] == 0xff))
				{
					// this give us a minimum 10ms delay for responding to a valid packet
					// do not delay if the packet is invalid
					serial_response_delay = gucResponseDelay;	
					serial_no_activity = 1;
					gbSerialFlag = 1;					 
				}
				else
				{
					gbSerialFlag = 0;						   
					dealwithTag = INVALID_PACKET;
					serial_restart( );
				}
			 
			}
	
	}
	else if(TI == 1)
	{
		TI = 0;
		transmit_finished = 1;
	}
	return;
}
/////////////////////////////////////////////////////////////////////////
void clock( void )
{
	if(millisecond > 399)
	{
		millisecond = 0;
		second++;
		if(second > 20 && time_caculate == 1)
		{
			time_caculate = 0;
			second -= 2;
		}
 
		if(override_timer_time > 0)
		override_timer_time-- ;
	}
	if(second > 59)
	{
		second = 0;
		minute++;
		if((minute & 0x07) == 0)
		time_caculate = 1;
 
	}
	if(minute > 59 )
	{
		minute = 0;
		hour++;
 
	}
	if(hour > 23)
	{
		hour = 0;
		ROTATION_NEWDAY = 1;
		OVERRIDE_NEWDAY = 1;
	}	
}
