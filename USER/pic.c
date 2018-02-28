#include "types.h"

//void i2c_pic_start()
//{
//	// PIC chip requires a special double start condition in order
//	// to initiate communication.  This is to insure the PIC does not 
//	// read any false starts.
//	// MDF 12/01/04
//	
//	// 1st start condition


//	  SDA = 0;
//	  delay_us(15);

//	  SCL = 0;

//	  delay_us(10);

//	
//	  // reset bus
//	  SDA = 1;
//	  SCL = 1;
//	  	delay_us(10);

//	
//	  // 2nd start condition
//	  SDA = 0;
//	  delay_us(10);

//	  SCL = 0;

//}

//void GIVE_PIC_ACK( void )
//{
//   int16 j=0;
//  // Wait until the data signal goes high
//	
//	  while (!SDA){
//	    j++;
//	    // If no clock, exit i2c_read routine
//	    if (j == 100)
//	      return; 
//		   }
//	
//	  // Bring the data line low
//	  SDA=0;

//	  SCL=1;
//	  delay_us(10);

//	  SCL=0;
//	  SDA=1;
//  	

//}

///*;************************************/
///*;send a 8-bool data to 12F675 */
//void i2c_pic_write( uint8 ch )
//{
//  uint8 i = 8;
//	  do
//	  {    
//	    SDA = ( ch & 0x80 );
//	    ch<<=1;
//	    delay_us(10); 

//	    SCL=1;
//	    delay_us(10); 
//	    SCL=0;

//	  } while( --i != 0 );
//	
//	  SDA=1;

//}


///*;**************************************/
///*;receive a 8-bool data from 12F675 */
//int16 i2c_pic_read( void )
//{
// uint8  i;
// int16  data1 = 0;
//  

//	  for( i=0; i<16; i++ )
//	  	{
//	    delay_us(10);
//	    SCL = 1;
//	    delay_us(10);
//	    data1 = ( data1 << 1 ) | SDA;
//	    SCL = 0;
//	  	}

//  return data1;
//}

///*;**************************************/
///*;detect the ACK signal to 24Cxx*/	
//// returns a 1 for no ACK.  returns a 0 for successful ACK		
//bool GET_ACK( void )
//{ 
// 	uint8 c=0;
//    uint8 i=0;

//	    for (i=0; i<10; i++)
//	    	{
//			c=SDA;
//			if (c == 0)
//				{
//			// if data line is low, pulse the clock.
//			  	SCL=1;
//	    		delay_us(10);
//			  	SCL=0;
//				return 0;
//				}		
//	    	}
//	    SCL=0;
//	    return 1;

//}
///*********FOR CALIBRATE HUM SENSOR******************/
///******************add by ye************************/
//#ifdef  HUM_CALIBRATION_ENABLE
//uint8 clear_sensor_humtab18(void)
//{
//	uint8 pic_check;	
//	i2c_stop();
//	delay_us(10);
// 	i2c_pic_start();
//	
//	i2c_pic_write(0xe4);  //
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
//		return 0;	
//	}
//	delay_us(1);

//	pic_check = i2c_pic_read();

//	i2c_stop();
//	
//	if(pic_check == 0xe4)
//		return 1;
//	else 
//		return 0;	
//}



//uint8  write_sensor_humtable18(uint8 humH,uint8 humL,uint8 freqH,uint8 freqL)
//{
// 	uint16 check_sum;

//  serial_int_flag=0;
//	 	 
//  i2c_stop();
//	delay_us(10); 
//	
//	i2c_pic_start();

//	
//	i2c_pic_write(0xd3);  //falg
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
// 	  
//		return 0;	
//	}
//	delay_us(1);

//	i2c_pic_write(humH);  //humH
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
// 		  
//		return 0;	
//	}
//	delay_us(1);

//	i2c_pic_write(humL);  //humL
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
// 		 
//		return 0;	
//	}
//	delay_us(1);

//	i2c_pic_write(freqH);  //freqH
//	delay_us(10);

//	if (GET_ACK())
//	{
//		i2c_stop();
// 		  
//		return 0;	
//	}
//	delay_us(1);

//	i2c_pic_write(freqL);  //freqL
//	delay_us(10);

//	if (GET_ACK())
//	{
//		i2c_stop();
// 		  
//		return 0;	
//	}
//	delay_us(1);

//	check_sum = i2c_pic_read();

//	i2c_stop();
// 	 
//	if((check_sum == (0xd3 + humH+humL+freqH+freqL) & 0xffff)&&(serial_int_flag == 0)	)
//	{	
// 		return 1;
//	}
//	else
//	{
// 		return 0;
//	}
//}

//uint8  calibrate_temperature( uint8 tempH,uint8 tempL)  //calibrate temprature
//{
// 
//	uint16 xdata check_sum;	  
//  serial_int_flag=0;
//	 	 
//			  
//	i2c_pic_start();	    
//	i2c_pic_write(0xe5);  //flag
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
// 					 
//		return 0;	
//	}
//	delay_us(1);
// 

//	i2c_pic_write(tempH);  //tempH
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
//					    
//		return 0;	
//	}
//	delay_us(1);

//	i2c_pic_write(tempL);  //tempL
//	delay_us(1);

//	if (GET_ACK())
//	{
//		i2c_stop();
//					  
//		return 0;	
//	}
//	delay_us(1);
// 
//	check_sum = i2c_pic_read();  
//										    
//	i2c_stop();

//	if((check_sum == (0xe5 + tempL + tempH))&&(serial_int_flag==0))
//	{																		  
//		return 1;
//	}
//	else
//	{							  
//		return 0;
//	}
//}

//#endif
/*********************end***************************/