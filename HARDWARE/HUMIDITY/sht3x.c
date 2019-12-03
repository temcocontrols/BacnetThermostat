//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 44, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  SHT3x Sample Code (V1.0)
// File      :  sht3x.c (V1.0)
// Author    :  RFU
// Date      :  16-Jun-2014
// Controller:  STM32F100RB
// IDE       :  µVision V4.71.2.0
// Compiler  :  Armcc
// Brief     :  Sensor Layer: Implementation of functions for sensor access.
//==============================================================================

//-- Includes ------------------------------------------------------------------
#include "sht3x.h"
#include "i2c_hal.h"
#include "delay.h"
#include "define.h"


//-- Defines -------------------------------------------------------------------
// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

//extern float ctest[10];
//------------------------------------------------------------------------------
// IO-Pins                             /* -- adapt the defines for your uC -- */
//------------------------------------------------------------------------------
// Reset on port B, bit 12
#define RESET_LOW()  (GPIOB->BSRR = 0x10000000) // set Reset to low
#define RESET_HIGH() (GPIOB->BSRR = 0x00001000) // set Reset to high

// Alert on port B, bit 10
#define ALERT_READ   (GPIOB->IDR  & 0x0400)     // read Alert
//------------------------------------------------------------------------------

//-- Global variables ----------------------------------------------------------
uint8 _i2cWriteHeader ;
uint8 _i2cReadHeader ;

//==============================================================================
void SHT3X_Init(uint8 i2cAdr){              /* -- adapt the init for your uC -- */
//==============================================================================
	// init I/O-pins
//	RCC->APB2ENR |= 0x00000008;  // I/O port B clock enabled
//	
//  // Alert on port B, bit 10
//	GPIOB->CRH   &= 0xFFFFF0FF;  // set floating input for Alert-Pin
//  GPIOB->CRH   |= 0x00000400;  //
//	
//  // Reset on port B, bit 12
//	GPIOB->CRH   &= 0xFFF0FFFF;  // set push-pull output for Reset pin
//  GPIOB->CRH   |= 0x00010000;  //
//	RESET_LOW();
//	
//  I2c_Init(); // init I2C
  SHT3X_SetI2cAdr(i2cAdr);
//	
//	// release reset
//	RESET_HIGH();
}

//==============================================================================
void SHT3X_SetI2cAdr(uint8 i2cAdr){
//==============================================================================
  _i2cWriteHeader = i2cAdr << 1;
  _i2cReadHeader = _i2cWriteHeader | 0x01;
}

//==============================================================================
etError SHT3x_ReadSerialNumber(uint32 *serialNbr){
//==============================================================================
  etError error; // error code
  uint16 serialNumWords[2];

  error = SHT3X_StartWriteAccess();

  // write "read serial number" command
  error |= SHT3X_WriteCommand(CMD_READ_SERIALNBR);
  // if no error, start read access
  if(error == NO_ERROR) error = SHT3X_StartReadAccess();
  // if no error, read first serial number word
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&serialNumWords[0], ACK, 100);
  // if no error, read second serial number word
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&serialNumWords[1], NACK, 0);

  SHT3X_StopAccess();
  
  // if no error, calc serial number as 32-bit integer
  if(error == NO_ERROR)
  {
    *serialNbr = (serialNumWords[0] << 16) | serialNumWords[1];
  }

  return error;
}

//==============================================================================
etError SHT3X_ReadStatus(uint16 *status){
//==============================================================================
  etError error; // error code

  error = SHT3X_StartWriteAccess();

  // if no error, write "read status" command
  if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_READ_STATUS);
  // if no error, start read access
  if(error == NO_ERROR) error = SHT3X_StartReadAccess(); 
  // if no error, read status
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(status, NACK, 0);

  SHT3X_StopAccess();

  return error;
}

//==============================================================================
etError SHT3X_ClearAllAlertFlags(void){
//==============================================================================
  etError error; // error code

  error = SHT3X_StartWriteAccess();

  // if no error, write clear status register command
  if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_CLEAR_STATUS);

  SHT3X_StopAccess();

  return error;
}

//==============================================================================
etError SHT3X_GetTempAndHumi(float         *temp,
                             float         *humi,
                             etRepeatab repeatab,
                             etMode     mode,
                             uint8        timeout){
//==============================================================================
  etError error;
                               
  switch(mode)
  {    
    case MODE_CLKSTRETCH: // get temperature with clock stretching mode
      error = SHT3X_GetTempAndHumiClkStretch(temp, humi, repeatab, timeout); break;
    case MODE_POLLING:    // get temperature with polling mode
      error = SHT3X_GetTempAndHumiPolling(temp, humi, repeatab, timeout); break;
    default:              
      error = PARM_ERROR; break;
  }
  
  return error;
}


//==============================================================================
etError SHT3X_GetTempAndHumiClkStretch(float         *temp,
                                       float         *humi,
                                       etRepeatab repeatab,
                                       uint8        timeout){
//==============================================================================
  etError error;        // error code
  uint16    rawValueTemp; // temperature raw value from sensor
  uint16    rawValueHumi; // humidity raw value from sensor

  error = SHT3X_StartWriteAccess();

  // if no error ...
  if(error == NO_ERROR)
  {
    // start measurement in clock stretching mode
    // use depending on the required repeatability, the corresponding command
    switch(repeatab)
    {
      case REPEATAB_LOW:    error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_L); break;
      case REPEATAB_MEDIUM: error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_M); break;
      case REPEATAB_HIGH:   error = SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_H); break;
      default:              error = PARM_ERROR; break;
    }
  }

  // if no error, start read access
  if(error == NO_ERROR) error = SHT3X_StartReadAccess();
  // if no error, read temperature raw values
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, timeout);
  // if no error, read humidity raw values
  if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);

  SHT3X_StopAccess();

  // if no error, calculate temperature in °C and humidity in %RH
  if(error == NO_ERROR)
  {
    *temp = SHT3X_CalcTemperature(rawValueTemp);
    *humi = SHT3X_CalcHumidity(rawValueHumi);
  }

  return error;
}

//==============================================================================
etError SHT3X_GetTempAndHumiPolling(float         *temp,
                                    float         *humi,
                                    etRepeatab repeatab,
                                    uint8        timeout){
//==============================================================================
  etError error;           // error code
  uint16    rawValueTemp;    // temperature raw value from sensor
  uint16    rawValueHumi;    // humidity raw value from sensor

  error  = SHT3X_StartWriteAccess();
 
  // if no error ...
  if(error == NO_ERROR)
  {
    // start measurement in polling mode
    // use depending on the required repeatability, the corresponding command
    switch(repeatab)
    {
      case REPEATAB_LOW:    error = SHT3X_WriteCommand(CMD_MEAS_POLLING_L); break;
      case REPEATAB_MEDIUM: error = SHT3X_WriteCommand(CMD_MEAS_POLLING_M); break;
      case REPEATAB_HIGH:		error = SHT3X_WriteCommand(CMD_MEAS_POLLING_H); break;
      default:         			error = PARM_ERROR; break;
    }
		
  }

  // if no error, wait until measurement ready
  if(error == NO_ERROR)
  {
    // poll every 1ms for measurement ready until timeout
    while(timeout--)
    {
      // check if the measurement has finished
      error = SHT3X_StartReadAccess();
      
      // if measurement has finished -> exit loop
      if(error == NO_ERROR) break;

      // delay 1ms
      delay_us(1000);
    }
		
		// check for timeout error
		if(timeout == 0) error = TIMEOUT_ERROR;
  }
	
	// if no error, read temperature and humidity raw values
  if(error == NO_ERROR)
  {
    error |= SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
    error |= SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
  } 

  SHT3X_StopAccess();

  // if no error, calculate temperature in °C and humidity in %RH
  if(error == NO_ERROR)
  {
    *temp = SHT3X_CalcTemperature(rawValueTemp);
    *humi = SHT3X_CalcHumidity(rawValueHumi);
  }

  return error;
}

//==============================================================================
etError SHT3X_StartPeriodicMeasurment(etRepeatab  repeatab,
                                      etFrequency freq){
//==============================================================================
  etError error;        // error code

  error = SHT3X_StartWriteAccess();

	// if no error, start periodic measurement 
	if(error == NO_ERROR)
	{
    // use depending on the required repeatability and frequency,
	  // the corresponding command
		switch(repeatab)
		{
			case REPEATAB_LOW: // low repeatability
				switch(freq)
				{
					case FREQUENCY_HZ5:  // low repeatability,  0.5 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_L); break;					
					case FREQUENCY_1HZ:  // low repeatability,  1.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_L); break;					
					case FREQUENCY_2HZ:  // low repeatability,  2.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_L); break;					
					case FREQUENCY_4HZ:  // low repeatability,  4.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_L); break;					
					case FREQUENCY_10HZ: // low repeatability, 10.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_L); break;					
					default:
						error |= PARM_ERROR; break;
				}
			  break;
				
			case REPEATAB_MEDIUM: // medium repeatability
				switch(freq)
				{
					case FREQUENCY_HZ5:  // medium repeatability,  0.5 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_M); break;
					case FREQUENCY_1HZ:  // medium repeatability,  1.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_M); break;				
					case FREQUENCY_2HZ:  // medium repeatability,  2.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_M); break;				
					case FREQUENCY_4HZ:  // medium repeatability,  4.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_M); break;			
					case FREQUENCY_10HZ: // medium repeatability, 10.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_M); break;
					default:
						error |= PARM_ERROR; break;
				}
			  break;
				
			case REPEATAB_HIGH: // high repeatability
				switch(freq)
				{
					case FREQUENCY_HZ5:  // high repeatability,  0.5 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_05_H); break;
					case FREQUENCY_1HZ:  // high repeatability,  1.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_1_H); break;
					case FREQUENCY_2HZ:  // high repeatability,  2.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_2_H); break;
					case FREQUENCY_4HZ:  // high repeatability,  4.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_4_H); break;
					case FREQUENCY_10HZ: // high repeatability, 10.0 Hz
						error |= SHT3X_WriteCommand(CMD_MEAS_PERI_10_H); break;
					default:
						error |= PARM_ERROR; break;
				}
			  break;
			default:
				error |= PARM_ERROR; break;
		}
	}

  SHT3X_StopAccess();

  return error;
}

//==============================================================================
etError SHT3X_ReadMeasurementBuffer(float *temp, float *humi){
//==============================================================================
  etError  error;        // error code
	uint16     rawValueTemp; // temperature raw value from sensor
  uint16     rawValueHumi; // humidity raw value from sensor

	error = SHT3X_StartWriteAccess();

	// if no error, read measurements
	if(error == NO_ERROR)	error = SHT3X_WriteCommand(CMD_FETCH_DATA);
	if(error == NO_ERROR)	error = SHT3X_StartReadAccess();	
	if(error == NO_ERROR)	error = SHT3X_Read2BytesAndCrc(&rawValueTemp, ACK, 0);
	if(error == NO_ERROR) error = SHT3X_Read2BytesAndCrc(&rawValueHumi, NACK, 0);
	
	// if no error, calculate temperature in °C and humidity in %RH
	if(error == NO_ERROR)
	{
		*temp = SHT3X_CalcTemperature(rawValueTemp);
		*humi = SHT3X_CalcHumidity(rawValueHumi);
	}
	
	SHT3X_StopAccess();
	
	return error;
}

//==============================================================================
etError SHT3X_EnableHeater(void){
//==============================================================================
  etError error; // error code

  error = SHT3X_StartWriteAccess();

  // if no error, write heater enable command
  if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_HEATER_ENABLE);

  SHT3X_StopAccess();

  return error;
}

//==============================================================================
etError SHT3X_DisableHeater(void){
//==============================================================================
  etError error; // error code

  error = SHT3X_StartWriteAccess();

  // if no error, write heater disable command
  if(error == NO_ERROR) error = SHT3X_WriteCommand(CMD_HEATER_DISABLE);

  SHT3X_StopAccess();

  return error;
}

//==============================================================================
etError SHT3X_SofloatReset(void){
//==============================================================================
  etError error; // error code

  error = SHT3X_StartWriteAccess();

  // write reset command
  error |= SHT3X_WriteCommand(CMD_SHT3X_SOFT_RESET);

  SHT3X_StopAccess();
	
	// if no error, wait 50 ms afloater reset
	if(error == NO_ERROR) delay_us(50000); 

  return error;
}

//==============================================================================
void SHT3X_HardReset(void){
//==============================================================================
	// set reset low
	RESET_LOW();

	// wait 100 ms
	delay_us(100000);
	
	// release reset
	RESET_HIGH();
	
	// wait 50 ms afloater reset
	delay_us(50000);
}

//==============================================================================
etError SHT3X_StartWriteAccess(void){
//==============================================================================
  etError error; // error code

  // write a start condition
  I2c_StartCondition();

  // write the sensor I2C address with the write flag
  error = I2c_WriteByte(_i2cWriteHeader);
  

  return error;
}

//==============================================================================
etError SHT3X_StartReadAccess(void){
//==============================================================================
  etError error; // error code

  // write a start condition
  I2c_StartCondition();

  // write the sensor I2C address with the read flag
  error = I2c_WriteByte(_i2cReadHeader);

  return error;
}

//==============================================================================
void SHT3X_StopAccess(void){
//==============================================================================
  // write a stop condition
  I2c_StopCondition();
}

//==============================================================================
etError SHT3X_WriteCommand(etCommands cmd){
//==============================================================================
  etError error; // error code

  // write the upper 8 bits of the command to the sensor
  error  = I2c_WriteByte(cmd >> 8);

  // write the lower 8 bits of the command to the sensor
  error |= I2c_WriteByte(cmd & 0xFF);

  return error;
}

//==============================================================================
etError SHT3X_Read2BytesAndCrc(uint16 *data, etI2cAck finaleAckNack, uint8 timeout){
//==============================================================================
  etError error;    // error code
  uint8     bytes[2]; // read data array
  uint8     checksum; // checksum byte
 
  // read two data bytes and one checksum byte
	                      error = I2c_ReadByte(&bytes[0], ACK, timeout);
	 
	if(error == NO_ERROR) error = I2c_ReadByte(&bytes[1], ACK, 0);
	 
  if(error == NO_ERROR) error = I2c_ReadByte(&checksum, finaleAckNack, 0);
	 
  
  // verify checksum
  if(error == NO_ERROR) error = SHT3X_CheckCrc(bytes, 2, checksum);
	 
  
  // combine the two bytes to a 16-bit value
  *data = (bytes[0] << 8) | bytes[1];
  
  return error;
}

//==============================================================================
etError SHT3X_Write2BytesAndCrc(uint16 data){
//==============================================================================
  etError error;    // error code
  uint8     bytes[2]; // read data array
  uint8     checksum; // checksum byte
	
	bytes[0] = data >> 8;
	bytes[1] = data & 0xFF;
	checksum = SHT3X_CalcCrc(bytes, 2);
 
	// write two data bytes and one checksum byte
	                      error = I2c_WriteByte(bytes[0]); // write data MSB
	if(error == NO_ERROR) error = I2c_WriteByte(bytes[1]); // write data LSB
	if(error == NO_ERROR) error = I2c_WriteByte(checksum); // write checksum
  
  return error;
}

//==============================================================================
uint8 SHT3X_CalcCrc(uint8 data[], uint8 nbrOfBytes){
//==============================================================================
	uint8 bitmask;        // bit mask
  uint8 crc = 0xFF; // calculated checksum
  uint8 byteCtr;    // byte counter
  
  // calculates 8-Bit checksum with given polynomial
  for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
  {
    crc ^= (data[byteCtr]);
    for(bitmask = 8; bitmask > 0; --bitmask)
    {
      if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
      else           crc = (crc << 1);
    }
  }
	
	return crc;
}

//==============================================================================
etError SHT3X_CheckCrc(uint8 data[], uint8 nbrOfBytes, uint8 checksum){
//==============================================================================
  uint8 crc;     // calculated checksum
  
	// calculates 8-Bit checksum
	crc = SHT3X_CalcCrc(data, nbrOfBytes);
  
  // verify checksum
  if(crc != checksum) return CHECKSUM_ERROR;
  else                return NO_ERROR;
}

//==============================================================================
float SHT3X_CalcTemperature(uint16 rawValue){
//==============================================================================
  float temp;
	// calculate temperature [°C]
  // T = -45 + 175 * rawValue / (2^16-1)
  //return 175 * (uint32)rawValue*10 / 65535 - 45;
	 temp = 175 * (float)rawValue*10/ 65535 - 450;
	 return (float)temp;
}

//==============================================================================
float SHT3X_CalcHumidity(uint16 rawValue){
//==============================================================================
  // calculate relative humidity [%RH]
  // RH = rawValue / (2^16-1) * 100
  return 100 * (float)rawValue*10 / 65535;
}

//==============================================================================
uint16 SHT3X_CalcRawTemperature(float temperature){
//==============================================================================
  // calc raw value from a temperature [°C]
  // rawValue = (T + 45) / 175 * (2^16-1)
	return (uint16)((temperature + 45) / 175 * 65535);
}

//==============================================================================
uint16 SHT3X_CalcRawHumidity(float humidity){
//==============================================================================
  // calc raw value from a relative humidity [%RH]
  // rawValue = RH / 100 * (2^16-1)
	return (uint16)(humidity / 100 * 65535);
}

//==============================================================================
bt SHT3X_ReadAlert(void){
//==============================================================================
  // read alert pin
  return (ALERT_READ != 0) ? TRUE : FALSE;
}
