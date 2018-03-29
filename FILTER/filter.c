#include "filter.h"
#include "modbus.h"
#include "bacnet.h"


//static u8 thermistor_type ;
static u16 analog_in_buffer[MAX_AI_CHANNEL] ; 



//u8 const  def_tab_pic_Type2_10K[21] =
//			{					  
//			// 10k termistor GREYSTONE -50 to 150 Deg.C 
//			15,25,41,61,83,102,113,112,101,85,67,51,38,28,21,15,11,8,6,5,19	   //changed by ye

// 			};
//u8 const  def_tab_pic_Type3_10K[21] =
//			{					
//				// 10k termistor GREYSTONE -50 to 150 Deg.C 
//				18,29, 45, 63, 81, 96, 104, 103, 94, 80, 65, 51, 40, 30, 23, 17,13,10,8,6,24 //MHF 20010_07 REVISE TEMP LOOKUP TABLE PER NATHANEAL
// 			};

//s16 look_up_table1(u16 count)
//{
//	s16    val;
//    u8     index=20;			//chagned by  ye 
//	s16    work_var;
//	if(thermistor_type == TYPE2_10K_THERM)
//		work_var= def_tab_pic_Type2_10K[index];
//	else if(thermistor_type == TYPE3_10K_THERM)
//		work_var= def_tab_pic_Type3_10K[index];
//		  
//	if (work_var > count )
//		{
//			val =  (index-5)  * 100 ;	 //the highest temperature is 150c, changed by ye
//			return ( val );
//		  
//		}
//	do 
//		{
//			index--;
//			if(thermistor_type == TYPE2_10K_THERM)
//			work_var += def_tab_pic_Type2_10K[index];
//			else if(thermistor_type == TYPE3_10K_THERM)
//			work_var += def_tab_pic_Type3_10K[index];

//			if( work_var > count)
//				{
//				val = ( work_var - count )*100;
//				if(thermistor_type == TYPE2_10K_THERM)
//				val /= def_tab_pic_Type2_10K[index];
//				else if(thermistor_type == TYPE3_10K_THERM)
//				val /= def_tab_pic_Type3_10K[index];
//				if(index >= 5)				      //changed by ye ,the 0c is fifth
//				{
//					val +=  (index - 5) * 100;
//					val = val & 0x7fff;
//				}
//				else
//				{
//					val += index*100;
//					val = 500 - val;			  //the lowest temperature is -50c
//					val = val | 0x8000;
//				}			 
//				return (val);
//				}
//		} while (index) ;

//			//val =  33768;

//			return ( val );
//}



/******************************************RangeConverter******************************************/
/*
Description: Convert the  raw data from adc to correspond engineer units.
parameter:	finction,	The engineer units want to get,
			para,		Raw data from ADC
			i, 			Be used for function = 4,customer sensor,because there are only two 
						customer tables,so should check this parameter not bigger than 2 on fun4.
			cal,		calibration data for the correspond input channel
Return:		Changed input to the expected engineer units.	
			
*/
/*********************************RangeConverter funtion start**************************************/
//signed int T3_RangeConverter(unsigned char function, signed int para,signed int cal,unsigned char i)
//{
//	s16  siAdcResult;
//	u8  ucFunction;
//	s16   siInput;
//	s16   uiCal;
//	s16 siResult;
//	//u16 uiResult;
//	u8 bAnalogInputStatus;
//	ucFunction = function;
//	siInput = para;
//	uiCal = cal;
//	switch(ucFunction)
//	{
//	 case RAW_DATA :
//		  siResult = siInput + (s16)(uiCal - modbus.offset[i]);
//		  break ;		 
//	//-----------10K Thermistor---------------
//	case  C_TYPE2 :
//	case  F_TYPE2 :
//	case  C_TYPE3 : 
//	case  F_TYPE3 :
//		if((ucFunction == C_TYPE2)||(ucFunction == F_TYPE2))
//		thermistor_type = TYPE2_10K_THERM ;
//		else if((ucFunction == C_TYPE3)||(ucFunction == F_TYPE3))
//		thermistor_type = TYPE3_10K_THERM ;
//       	siAdcResult = look_up_table1(siInput>>2);
//		if(siAdcResult & 0x8000)
//		siInput = -(signed int)(siAdcResult & 0x7fff);
//		else
//		siInput = siAdcResult;
//		if((ucFunction == F_TYPE2)||(ucFunction == F_TYPE3))  //F
//		{		 
//			siInput = (siInput * 9)/5 +320; 		 
//		}
//		// Add the calibration term to the input.
//		siResult = siInput + (signed int)(uiCal - modbus.offset[i]); 
//	break;	 	
//	//-----------0-100%---------------
//	case PERCENT: 					//MHF: Feb 24th 2005 new range setting for analog inputs
//		siResult = (float)(siInput)/4095*100;
//		break;
//	//-----------ON/OFF---------------
//	case ON_OFF : 
//	case OFF_ON	:
//		siAdcResult = (float)(siInput)/4095*50;
//		if(siAdcResult <= 24)
//		{
//			if(ucFunction == OFF_ON)
//				bAnalogInputStatus = 1; 
//			else if(ucFunction == ON_OFF)
//				bAnalogInputStatus = 0; 
//		}
//		else if(siAdcResult >= 26)
//		{
//			if(ucFunction == OFF_ON)
//			bAnalogInputStatus = 0; 
//			else if(ucFunction == ON_OFF)
//			bAnalogInputStatus = 1; 
//		}
//		siResult = (unsigned int)(bAnalogInputStatus);
//	 	break ;
//		case NO_USE: 
//		siResult = 0;
//		break;
//		case V0_5:
//		siResult =  (5000L * siInput ) >> 12;
//		break;
//		case V0_10:
//		siResult = ( 10000L * siInput ) >> 12;
//		break;
//		case I0_20ma:
//		siResult = ( 2000L * siInput ) >> 12;
//		break;
//		}
// 	return siResult;
//}

//u16 filter_inputs(u8 channel,u16 input)
//{
//	// -------------FILTERING------------------
//	s16  siDelta = 0;
//	u16  uiResult = 0;
//	u8  i = 0;
//  	u16  uiTemp = 0;
//	i = channel;
//	uiTemp = input;  
//	siDelta = uiTemp - analog_in_buffer[i];    //compare new reading and old reading

//	// If the difference in new reading and old reading is greater than 5 degrees, implement rough filtering.
//  if (( siDelta >= 100 ) || ( siDelta <= -100)) // deg f
//	  	analog_in_buffer[i] = analog_in_buffer[i] + (siDelta >> 1);//1 
// 			
//	// Otherwise, implement fine filtering.
//	else
//	{			
//	  analog_in_buffer[i] = (u32)analog_in_buffer[i]*modbus.filter_value[i];
//	  analog_in_buffer[i] += (u32)uiTemp;
//	  analog_in_buffer[i] = (u16)(analog_in_buffer[i]/(modbus.filter_value[i]+1));			 	 
//	}
//	uiResult = analog_in_buffer[i]; 
// 

//	return uiResult;	
//}
