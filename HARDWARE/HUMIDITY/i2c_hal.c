//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 44, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  SHT3x Sample Code (V1.0)
// File      :  i2c_hal.c (V1.0)
// Author    :  RFU
// Date      :  16-Jun-2014
// Controller:  STM32F100RB
// IDE       :  µVision V4.71.2.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer
//==============================================================================

//-- Includes ------------------------------------------------------------------
#include "define.h"
#include "i2c_hal.h"
#include "delay.h"
#include "myiic.h"


//-- Defines -------------------------------------------------------------------
// I2C IO-Pins                         /* -- adapt the defines for your uC -- */

// SDA on port B, bit 14
//#define SDA_LOW()  IIC_SDA = 0//(GPIOA->BSRR = 0x40000000) // set SDA to low
//#define SDA_OPEN() IIC_SDA = 1(GPIOB->BSRR = 0x00004000) // set SDA to open-drain
//#define SDA_READ   READ_SDA//(GPIOB->IDR  & 0x4000)     // read SDA

//// SCL on port B, bit 13               /* -- adapt the defines for your uC -- */
//#define SCL_LOW()  IIC_SCL = 0//(GPIOB->BSRR = 0x20000000) // set SCL to low
//#define SCL_OPEN() IIC_SCL = 1//(GPIOB->BSRR = 0x00002000) // set SCL to open-drain
//#define SCL_READ   (GPIOB->IDR  & 0x2000)     // read SCL


//==============================================================================
//void I2c_Init(void){                      /* -- adapt the init for your uC -- */
////==============================================================================
//  RCC->APB2ENR |= 0x00000008;  // I/O port B clock enabled
//  
//  IIC_SDA = 1;// IIC_SDA = 1;                  // I2C-bus idle mode SDA released
//  IIC_SCL = 1;                  // I2C-bus idle mode SCL released
//  
//  // SDA on port B, bit 14
//  // SCL on port B, bit 13
//  GPIOB->CRH   &= 0xF00FFFFF;  // set open-drain output for SDA and SCL
//  GPIOB->CRH   |= 0x05500000;  // 
//}

//==============================================================================
void I2c_StartCondition(void){
//==============================================================================
  SDA_OUT();
	IIC_SDA(1);// = 1;
  //DelayMicroSeconds(1);
	delay_us(1);
  IIC_SCL(1);// = 1;
  delay_us(1);//DelayMicroSeconds(1);
  IIC_SDA(0);// = 0;
  delay_us(10);//DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
  IIC_SCL(0);// = 0;
  delay_us(10);//DelayMicroSeconds(10);
}

//==============================================================================
void I2c_StopCondition(void){
//==============================================================================
  SDA_OUT();
	IIC_SCL(0);// = 0;
  delay_us(1);
  IIC_SDA(0);// = 0;
  delay_us(1);
  IIC_SCL(1);// = 1;
  delay_us(10);  // set-up time stop condition (t_SU;STO)
  IIC_SDA(1);// = 1;
  delay_us(10);
}

//==============================================================================
etError I2c_WriteByte(uint8 txByte){
//==============================================================================
  etError error = NO_ERROR;
  uint8     mask;
	SDA_OUT();
  for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
  {
    if((mask & txByte) == 0) 
			IIC_SDA(0);// = 0; // masking txByte, write bit to SDA-Line
    else                     
			IIC_SDA(1);// = 1;
    delay_us(5);               // data set-up time (t_SU;DAT)
    IIC_SCL(1);// = 1;                         // generate clock pulse on SCL
    delay_us(5);               // SCL high time (t_HIGH)
    IIC_SCL(0);// = 0;
    delay_us(1);               // data hold time(t_HD;DAT)
  }
	error = IIC_Wait_Ack();
//  IIC_SDA = 1;                           // release SDA-line
//	SDA_IN();
//	delay_us(1);
//  IIC_SCL = 1;                           // clk #9 for ack
//  delay_us(5);                 // data set-up time (t_SU;DAT)
//	
//  if(READ_SDA) error = ACK_ERROR;       // check ack from i2c slave
//  IIC_SCL = 0;
//  delay_us(20);                // wait to see byte package on scope
  return error;                         // return error code
}

//==============================================================================
etError I2c_ReadByte(uint8 *rxByte, etI2cAck ack, uint8 timeout){
//==============================================================================
  etError error = NO_ERROR;
  uint8 mask;
  *rxByte = 0x00;
	//SDA_OUT();
  IIC_SDA(1);// = 1;
  SDA_IN();	// release SDA-line
  for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
  { 
    IIC_SCL(1);// = 1;                          // start clock on SCL-line
    delay_us(1);                // clock set-up time (t_SU;CLK)
		error = I2c_WaitWhileClockStreching(timeout);// wait while clock streching
    delay_us(3);                // SCL high time (t_HIGH)
		
		SCL_OUT();
    if(READ_SDA()) *rxByte |= mask;        // read bit
    IIC_SCL(0);// = 0;
    delay_us(1);                // data hold time(t_HD;DAT)
  }
	SDA_OUT();
  if(ack == ACK) 
		IIC_SDA(0);// = 0;              // send acknowledge if necessary
  else           
		IIC_SDA(1);// = 1;
  delay_us(1);                  // data set-up time (t_SU;DAT)
  IIC_SCL(1);// = 1;                            // clk #9 for ack
  delay_us(5);                  // SCL high time (t_HIGH)
  IIC_SCL(0);// = 0;
  IIC_SDA(1);// = 1;                            // release SDA-line
  delay_us(20);                 // wait to see byte package on scope
	
	return error;                          // return with no error
}

//==============================================================================
etError I2c_WaitWhileClockStreching(uint8 timeout){
//==============================================================================
	etError error = NO_ERROR;
	SCL_IN();
  while(READ_SCL() == 0)
  {
    if(timeout-- == 0)	return TIMEOUT_ERROR;
    delay_us(1000);
  }
	
	return error;
}

//==============================================================================
etError I2c_GeneralCallReset(void){
//==============================================================================
	etError error;
	
	I2c_StartCondition();
	                      error = I2c_WriteByte(0x00);
	if(error == NO_ERROR) error = I2c_WriteByte(0x06);
	
	return error;
}
