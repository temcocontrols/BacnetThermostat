
#include "define.h"
#include "inputs.h"
#include "24cxx.h"
#include "delay.h"
#include "modbus.h"
#include "../filter/filter.h"
#include "eepdefine.h"
#include  "Constcode.h"
#include "TstatFunctions.h"
#include "gpio_define.h"
#include "sht3x.h"
#include "pid.h"

uint8 co2_autocal_disable;
uint8 co2_present = 0;
int8 Hum_T_calibration = 0;
uint8 hum_sensor_type = 0; // 0 :htu21d  1:sht3x
float tem_org = 0;
float hum_org = 0;
uint8 voltage_overshoot = 0;
uint16 aq_value = 0;
//uint16 pwmtest = 0;
//extern uint16 ctest[10];
int16 top_of_4to20ma;
int16 bottom_of_4to20ma;
uint16 tem_of_co2 = 0;
uint16 hum_of_co2 = 0;
void frc_mode_check(void);
uint8 frc_mode = 0; //0:frc mode not start 1: frc mode start 
uint8 frc_block_config = 0;//0: config ok  1: config wrong
uint8 frc_output_select = 0;
uint8 frc_output_config = 0;
uint8 frc_pid2_config = 0;
extern int16 idata loop_setpoint[3] ;
//int16 frc_spacetemvalue = 0;
//int16 frc_supplytemvalue = 0;
//int16 frc_outdoortemvalue = 0;
//int16 frc_indoorhumvalue = 0;
//int16 frc_outdoorhumvalue = 0;
//uint8 frc_config = 0;//check if the setting is legal, each bit of byte indicate one setting state 
////b0:	space temperatuer block 
////b1:	supply temperature block
////b2:	outdoor temperature block
////b3: outdoor humidity block
////b4:	indoor humidity block 
////b5: output block
////b6: pid2 block
////b7: unused
uint8 frc_availeble = 0;//0: not availeble  1: availeble 
STR_FRC_INPUT spacetem;
STR_FRC_INPUT supplytem;
STR_FRC_INPUT outdoortem;
STR_FRC_INPUT outdoorhum;
STR_FRC_INPUT indoorhum;

Str_in_point FRC_OUTDOORTEM;
Str_in_point FRC_INDOORHUM;
int16 frc_outdoorhum_enthalpy = 0;
int16 frc_indoorhum_enthalpy = 0;
int16 ao1_fdbk;
int16 ao2_fdbk;
uint16 rs485_p_voltage = 0;
uint16 rs485_n_voltage = 0; 
int16 outside_tem = 333;
uint16 light_sensor = 0;
uint16 pir_value = 0;
int16  co2_calibration_data;
//uint16_t far AI_Present_Value[MAX_AIS];
int16 dew_data = 0;
int16 htu_temp;
int16 xdata analog_calibration[5];
uint8 power_up_timer = 0;
int16 old_temperature = 0;
int16 mul_analog_cal[8];
//uint8 mul_input_filter[10];
int16 pre_mul_analog_input[10]; //used to filter  readings
int16 mul_analog_in_buffer[10];
int16 mul_analog_input[10];
uint8 slope_type[2];
uint16 co2_data = 400;
uint16 co2_data_org = 400;
int16 humidity = 0;									
int16 temperature = 0; // global temperature x 10, filtered
int16 temperature_c = 0; 
int16 temperature_f = 0;
int16 internal_calibration = 0;

int16 delta_temperature;

int16 temperature_internal;
int16 temperature_org;
int16 temperature_humsensor;
uint8 xdata table1_flag = MAX_INPUT_CHANNEL;
uint8 xdata table2_flag = MAX_INPUT_CHANNEL;
//void frc_input(void);
//void check_frc_config(void);
// void range_set_func(u8 range) ;
 
#define ADC_DR_ADDRESS  0x4001244C  

#ifdef TSTAT7_ARM
//vu16 AD_Value[MAX_AI_CHANNEL]; 
const uint8 channel[MAX_AI_CHANNEL] = {11,12,13,14};
#endif

static void INPUT_TYPE0_OUT(u8 status)
{
	if(HardwareVersion < HW_VERSION_OVERSHOOT )
	{
		if(status) 
			GPIO_SetBits(gpio_map[INPUT_TYPE0].GPIOX, gpio_map[INPUT_TYPE0].GPIO_Pin_X);
		else
			GPIO_ResetBits(gpio_map[INPUT_TYPE0].GPIOX, gpio_map[INPUT_TYPE0].GPIO_Pin_X); 
	}
	else
	{
		if(status) 
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_1); 	
	}
}

static void INPUT_TYPE1_OUT(u8 status)
{
	if(HardwareVersion < HW_VERSION_OVERSHOOT )
	{
		if(status) 
			GPIO_SetBits(gpio_map[INPUT_TYPE1].GPIOX, gpio_map[INPUT_TYPE1].GPIO_Pin_X);
		else
			GPIO_ResetBits(gpio_map[INPUT_TYPE1].GPIOX, gpio_map[INPUT_TYPE1].GPIO_Pin_X); 
	}
	else
	{
		if(status) 
			GPIO_SetBits(GPIOE, GPIO_Pin_0);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_0); 	
	}
}

static void CHSEL0_OUT(u8 status)
{
	if(HardwareVersion < HW_VERSION_OVERSHOOT )
	{	
		if(status) 
			GPIO_SetBits(gpio_map[CHSEL0].GPIOX, gpio_map[CHSEL0].GPIO_Pin_X);
		else
			GPIO_ResetBits(gpio_map[CHSEL0].GPIOX, gpio_map[CHSEL0].GPIO_Pin_X);
	}
	else
	{
		if(status) 
			GPIO_SetBits(GPIOE, GPIO_Pin_4);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_4); 		
	}		
}

static void CHSEL1_OUT(u8 status)
{
	if(HardwareVersion < HW_VERSION_OVERSHOOT )
	{	
	if(status) 
			GPIO_SetBits(gpio_map[CHSEL1].GPIOX, gpio_map[CHSEL1].GPIO_Pin_X);
		else
			GPIO_ResetBits(gpio_map[CHSEL1].GPIOX, gpio_map[CHSEL1].GPIO_Pin_X);
	}
	else
	{
		if(status) 
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);	
	}
}

static void CHSEL2_OUT(u8 status)
{
	if(HardwareVersion < HW_VERSION_OVERSHOOT )
	{	
	if(status) 
		GPIO_SetBits(gpio_map[CHSEL2].GPIOX, gpio_map[CHSEL2].GPIO_Pin_X);
	else
		GPIO_ResetBits(gpio_map[CHSEL2].GPIOX, gpio_map[CHSEL2].GPIO_Pin_X); 
	}
	else
	{
		if(status) 
			GPIO_SetBits(GPIOE, GPIO_Pin_2);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_2);	
	}
}

static void AI_MODE_OUT(u8 status)
{
	if(HardwareVersion < HW_VERSION_OVERSHOOT )
	{	
		if(status) 
			GPIO_SetBits(gpio_map[AI_MODE_SEL].GPIOX, gpio_map[AI_MODE_SEL].GPIO_Pin_X);
		else
			GPIO_ResetBits(gpio_map[AI_MODE_SEL].GPIOX, gpio_map[AI_MODE_SEL].GPIO_Pin_X);
	}
	else
	{
		if(status) 
			GPIO_SetBits(GPIOE, GPIO_Pin_5);
		else
			GPIO_ResetBits(GPIOE, GPIO_Pin_5);		
	}
}


#ifdef TSTAT7_ARM
void inputs_io_init(void)
{
GPIO_InitTypeDef	GPIO_InitStructure;
//GPIO_InitTypeDef AI_1;
//GPIO_InitTypeDef AI_2;
//GPIO_InitTypeDef AI_3;
//GPIO_InitTypeDef AI_4;	
//GPIO_InitTypeDef INPUT_TYPE0;
//GPIO_InitTypeDef INPUT_TYPE1;
//GPIO_InitTypeDef CHSEL0;
//GPIO_InitTypeDef CHSEL1;
//GPIO_InitTypeDef CHSEL2;	
//GPIO_InitTypeDef AI_MODE_SEL;
GPIO_InitTypeDef LIGHT_SENSOR;
GPIO_InitTypeDef PIR_SENSOR;	
//GPIO_InitTypeDef AO1_FEEDBACK;
//GPIO_InitTypeDef AO2_FEEDBACK;		
	
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOA, &GPIO_InitStructure);

LIGHT_SENSOR.GPIO_Pin = GPIO_Pin_0;  
LIGHT_SENSOR.GPIO_Mode = GPIO_Mode_AIN; 
LIGHT_SENSOR.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOB, &LIGHT_SENSOR);

PIR_SENSOR.GPIO_Pin = GPIO_Pin_1;  
PIR_SENSOR.GPIO_Mode = GPIO_Mode_AIN; 
PIR_SENSOR.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOB, &PIR_SENSOR);
	
//AO1_FEEDBACK.GPIO_Pin = GPIO_Pin_1;  
//AO1_FEEDBACK.GPIO_Mode = GPIO_Mode_AIN; 
//AO1_FEEDBACK.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOA, &AO1_FEEDBACK);

//AO2_FEEDBACK.GPIO_Pin = GPIO_Pin_4;  
//AO2_FEEDBACK.GPIO_Mode = GPIO_Mode_AIN; 
//AO2_FEEDBACK.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOA, &AO2_FEEDBACK);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOC, &GPIO_InitStructure);	

//AI_2.GPIO_Pin = GPIO_Pin_2; 
//AI_2.GPIO_Mode = GPIO_Mode_AIN; 
//AI_2.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOC, &AI_2);

//AI_3.GPIO_Pin = GPIO_Pin_3; 
//AI_3.GPIO_Mode = GPIO_Mode_AIN; 
//AI_3.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOC, &AI_3);

//AI_4.GPIO_Pin = GPIO_Pin_4; 
//AI_4.GPIO_Mode = GPIO_Mode_AIN; 
//AI_4.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOC, &AI_4);

/**************************PortC configure----ADC1*****************************************/
RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);
}

void inputs_adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //enable AHB clock
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1 ADC2 ADC3 are independed
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//scan mode disable
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//
	ADC_InitStructure.ADC_NbrOfChannel = 1;//
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE); 
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)== SET)
	{
		;
	}		
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1) == SET);
}

#else  //TSTAT8

void inputs_io_init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
//GPIO_InitTypeDef INPUT_TYPE0;
//GPIO_InitTypeDef INPUT_TYPE1;
//GPIO_InitTypeDef CHSEL0;
//GPIO_InitTypeDef CHSEL1;
//GPIO_InitTypeDef CHSEL2;	
//GPIO_InitTypeDef AI_MODE_SEL;
//GPIO_InitTypeDef LIGHT_SENSOR;
//GPIO_InitTypeDef PIR_SENSOR;	
//GPIO_InitTypeDef AO1_FEEDBACK;
//GPIO_InitTypeDef AO2_FEEDBACK;	
GPIO_InitTypeDef AQ_ENABLE;
GPIO_InitTypeDef AQ_INPUT;	
GPIO_InitTypeDef CO2_ENABLE;
GPIO_InitTypeDef RS485_P;
GPIO_InitTypeDef RS485_N;
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOB, &GPIO_InitStructure);

//PIR_SENSOR.GPIO_Pin = GPIO_Pin_1;  
//PIR_SENSOR.GPIO_Mode = GPIO_Mode_AIN; 
//PIR_SENSOR.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOB, &PIR_SENSOR);
	
//AO1_FEEDBACK.GPIO_Pin = GPIO_Pin_1;  
//AO1_FEEDBACK.GPIO_Mode = GPIO_Mode_AIN; 
//AO1_FEEDBACK.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOA, &AO1_FEEDBACK);

//AO2_FEEDBACK.GPIO_Pin = GPIO_Pin_4;  
//AO2_FEEDBACK.GPIO_Mode = GPIO_Mode_AIN; 
//AO2_FEEDBACK.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOA, &AO2_FEEDBACK);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(GPIOC, &GPIO_InitStructure);	

if(HardwareVersion<5)//(HardwareVersion < 5)
{
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	INPUT_TYPE1.GPIO_Pin = GPIO_Pin_2;  
//	INPUT_TYPE1.GPIO_Mode = GPIO_Mode_Out_PP; 
//	INPUT_TYPE1.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOC, &INPUT_TYPE1);		

//	CHSEL0.GPIO_Pin = GPIO_Pin_6;  
//	CHSEL0.GPIO_Mode = GPIO_Mode_Out_PP; 
//	CHSEL0.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOA, &CHSEL0);	

//	CHSEL1.GPIO_Pin = GPIO_Pin_7;  
//	CHSEL1.GPIO_Mode = GPIO_Mode_Out_PP; 
//	CHSEL1.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOA, &CHSEL1);	

//	CHSEL2.GPIO_Pin = GPIO_Pin_4;  
//	CHSEL2.GPIO_Mode = GPIO_Mode_Out_PP; 
//	CHSEL2.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOC, &CHSEL2);	

//	AI_MODE_SEL.GPIO_Pin = GPIO_Pin_5;  
//	AI_MODE_SEL.GPIO_Mode = GPIO_Mode_Out_PP; 
//	AI_MODE_SEL.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOA, &AI_MODE_SEL);	
}
else
{
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);		

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//  INPUT_TYPE0.GPIO_Pin = GPIO_Pin_1;  
//	INPUT_TYPE0.GPIO_Mode = GPIO_Mode_Out_PP; 
//	INPUT_TYPE0.GPIO_Speed = GPIO_Speed_2MHz;	
//	GPIO_Init(GPIOE, &INPUT_TYPE0);		

//	INPUT_TYPE1.GPIO_Pin = GPIO_Pin_0;  
//	INPUT_TYPE1.GPIO_Mode = GPIO_Mode_Out_PP; 
//	INPUT_TYPE1.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOE, &INPUT_TYPE1);		

//	CHSEL0.GPIO_Pin = GPIO_Pin_4;  
//	CHSEL0.GPIO_Mode = GPIO_Mode_Out_PP; 
//	CHSEL0.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOE, &CHSEL0);	

//	CHSEL1.GPIO_Pin = GPIO_Pin_3;  
//	CHSEL1.GPIO_Mode = GPIO_Mode_Out_PP; 
//	CHSEL1.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOE, &CHSEL1);	

//	CHSEL2.GPIO_Pin = GPIO_Pin_2;  
//	CHSEL2.GPIO_Mode = GPIO_Mode_Out_PP; 
//	CHSEL2.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOE, &CHSEL2);	

//	AI_MODE_SEL.GPIO_Pin = GPIO_Pin_5;  
//	AI_MODE_SEL.GPIO_Mode = GPIO_Mode_Out_PP; 
//	AI_MODE_SEL.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOE, &AI_MODE_SEL);	
//	
//	RS485_P.GPIO_Pin = GPIO_Pin_2;  
//	RS485_P.GPIO_Mode = GPIO_Mode_AIN; 
//	RS485_P.GPIO_Speed = GPIO_Speed_2MHz;	
//	GPIO_Init(GPIOC, &RS485_P);
//	
//	RS485_N.GPIO_Pin = GPIO_Pin_3;  
//	RS485_N.GPIO_Mode = GPIO_Mode_AIN; 
//	RS485_N.GPIO_Speed = GPIO_Speed_2MHz;	
//	GPIO_Init(GPIOC, &RS485_N);
}

//AQ_INPUT.GPIO_Pin = GPIO_Pin_0; 
//AQ_INPUT.GPIO_Mode = GPIO_Mode_AIN; 
//AQ_INPUT.GPIO_Speed = GPIO_Speed_2MHz;	
//GPIO_Init(GPIOC, &AQ_INPUT);

//AQ_ENABLE.GPIO_Pin = GPIO_Pin_2;  
//AQ_ENABLE.GPIO_Mode = GPIO_Mode_Out_PP; 
//AQ_ENABLE.GPIO_Speed = GPIO_Speed_2MHz;
//GPIO_Init(GPIOE, &AQ_ENABLE);	

CO2_ENABLE.GPIO_Pin = GPIO_Pin_9;  
CO2_ENABLE.GPIO_Mode = GPIO_Mode_Out_PP; 
CO2_ENABLE.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_Init(GPIOB, &CO2_ENABLE);


/**************************PortC configure----ADC1*****************************************/
RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);
}

void inputs_adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //enable AHB clock
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1 ADC2 ADC3 are independed
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//scan mode disable
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//
	ADC_InitStructure.ADC_NbrOfChannel = 1;//
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE); 
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)== SET)
	{
		;
	}		
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1) == SET);
}



#endif

int32    val;

//int16_t look_up_customtable(uint8_t id,int16_t count,uint8_t slope,uint8 range )
//{
//	int16    val;
//  int8  index=10;
//	int16    work_var;
//	 	 
//	work_var = (int16)(CustomerTable1( id + (index<<1) + 1)<<8) + CustomerTable1(id + (index<<1));		  
//	if ( count >= 1023)
//	{
//		val =  work_var;
//		return ( val );		
//	}
//	index = count*10/1023;
//	if(slope)
//	{
//		work_var = (int16)(CustomerTable1(id +((index + 1)<<1) + 1)<<8) + CustomerTable1(id + ((index + 1)<<1));
//	
//		work_var -= (int16)(CustomerTable1(id + (index<<1) + 1)<<8) + CustomerTable1(id + (index<<1)); 
//	
//		val = (float)(count*10-1023*index)/1023*work_var;
//	
//		val += (int16)(CustomerTable1(id + (index<<1) + 1)<<8) + CustomerTable1(id + (index << 1));
//	}
//	else
//	{
//	
//		work_var = (int16)(CustomerTable2(id +(index<<1) + 1)<<8) + CustomerTable2(id + (index<<1));
//	
//		work_var -= (int16)(CustomerTable2(id + ((index + 1)<<1) + 1)<<8) + CustomerTable2(id + ((index + 1)<<1)); 
//	 
//	 	val = (float)(1023*(index+1) - count*10)/1023*work_var;
//	  
//		val += (int16)(CustomerTable2(id + ((index + 1)<<1) + 1)<<8) + CustomerTable2(id + ((index + 1) << 1));
//	}

//	return( val );
//}

int16 look_up_customtable(uint8 id,int16 count,uint8 slope,uint8 range )
{
//	int32    val;
  int16  index=10;
	int32    work_var;
	uint16 range_para;
	
	if((range == AI_RANGE_USER)||(range == AI_RANGE_CUSTOMER_SENSOR))
		range_para = 500;
	else if((range == AI_RANGE_USER_10V)||(range == AI_RANGE_CUSTOMER_SENSOR_10V))
		range_para = 1000;
	work_var = ((int16)CustomerTable1(id + (index<<1) + 1)<<8) + CustomerTable1(id + (index<<1));		  

	if ( count >= 1023)
	{
		val =  work_var;
		return ( val );		
	}
	index = count*10/range_para;
	if(slope)
	{
		work_var = ((int32)CustomerTable1(id +((index + 1)<<1) + 1)<<8) + CustomerTable1(id + ((index + 1)<<1));
		work_var -= ((int32)CustomerTable1(id + (index<<1) + 1)<<8) + CustomerTable1(id + (index<<1)); 	
		val = count*10;
		val = val - range_para*index;
		val = (val*work_var /range_para);
		val += ((int32)CustomerTable1(id + (index<<1) + 1)<<8) + CustomerTable1(id + (index << 1));
	}

	else
	{
	
		work_var = ((int32)CustomerTable2(id +(index<<1) + 1)<<8) + CustomerTable2(id + (index<<1));
	
		work_var -= ((int32)CustomerTable2(id + ((index + 1)<<1) + 1)<<8) + CustomerTable2(id + ((index + 1)<<1)); 
	 
	 	val = ((float)(index+1)*range_para - count*10)*work_var/range_para;
	  
		val += ((int32)CustomerTable2(id + ((index + 1)<<1) + 1)<<8) + CustomerTable2(id + ((index + 1) << 1));
	}

	return( (int16)val );
}




//int16 look_up_customtable(et_menu_parameter id,int16 count,uint8 slope,uint8 range )
//{
////	int32    val;
//  int16  index=10;
//	int32    work_var;
//	uint16 range_para;
//	
//	if((range == AI_RANGE_USER)||(range == AI_RANGE_CUSTOMER_SENSOR))
//		range_para = 500;
//	else if((range == AI_RANGE_USER_10V)||(range == AI_RANGE_CUSTOMER_SENSOR_10V))
//		range_para = 1000;
//	work_var = ((int16)read_eeprom(id + (index<<1) + 1)<<8) + read_eeprom(id + (index<<1));		  

//	if ( count >= 1023)
//	{
//		val =  work_var;
//		return ( val );		
//	}
//	index = count*10/range_para;
//	if(slope)
//	{
//		work_var = ((int32)read_eeprom(id +((index + 1)<<1) + 1)<<8) + read_eeprom(id + ((index + 1)<<1));
//		work_var -= ((int32)read_eeprom(id + (index<<1) + 1)<<8) + read_eeprom(id + (index<<1)); 	
//		val = count*10;
//		val = val - range_para*index;
//		val = (val*work_var /range_para);
//		val += ((int32)read_eeprom(id + (index<<1) + 1)<<8) + read_eeprom(id + (index << 1));
//	}

//	else
//	{
//	
//		work_var = ((int32)read_eeprom(id +(index<<1) + 1)<<8) + read_eeprom(id + (index<<1));
//	
//		work_var -= ((int32)read_eeprom(id + ((index + 1)<<1) + 1)<<8) + read_eeprom(id + ((index + 1)<<1)); 
//	 
//	 	val = ((float)(index+1)*range_para - count*10)*work_var/range_para;
//	  
//		val += ((int32)read_eeprom(id + ((index + 1)<<1) + 1)<<8) + read_eeprom(id + ((index + 1) << 1));
//	}

//	return( (int16)val );
//}


int16 Filter(uint8 channel,signed input)
{
	// -------------FILTERING------------------
//	int16 xdata siDelta;
	int32 xdata siResult;
	uint8 xdata I;
  int32 xdata siTemp;
	I = channel;
	siTemp = input;
  if(I == 10)
	{
	if(power_up_timer < 5)
    	old_temperature = siTemp;
	siResult = (old_temperature * EEP_Filter + siTemp) / (EEP_Filter + 1);
	old_temperature = siResult;
		 
	}
	else
	{
			siResult = (pre_mul_analog_input[I] * InputFilter(I) + siTemp) *10 / (InputFilter(I) + 1);
			if(siResult%10 >= 5)
				siResult += 10;
			pre_mul_analog_input[I] = siResult/10;// + InputFilter(I);
			siResult /= 10;
	}
	return siResult;
	
}

int16 look_up_table(uint16 count, int8 type)
{
	int32    val;
  int8  index = THERMISTER_RANGE;
	int32    work_var;
    if(type == THERMISTER_TYPE3)
		{ 
			work_var= def_tab_type3[index];
			  
		if (work_var > count )
			{
			val =  index  * 100 ;
			return ( val );			
			}	
		do 
			{
			index--;
			work_var += def_tab_type3[index];

			if( work_var > count)
				{
				val = ( work_var - count )*100;

				val /= def_tab_type3[index];

				if(index >= 4)
				{
					val +=  (index - 4) * 100;
					val = val & 0x7fff;
				}
				else
				{
					val += index*100;
					//val = val - 400;
					val = 400 - val;
					val = val | 0x8000;
				}			 
				return (val);
				}
			} while (index) ;
		}
	else if(type == THERMISTER_TYPE50K)
	{
		index = 9;
			work_var = def_tab_type50K[index];
			  
		if (work_var > count )
			{
			val =  350 ;
			return ( val );			
			}	
		do 
			{
			index--;
			work_var += def_tab_type50K[index];

			if( work_var > count)
				{
				val = ( work_var - count )*30;

				val /= def_tab_type50K[index];

//				if(index >= 4)
//				{
					val +=  index  * 30;
					val = val & 0x7fff;
//				}
//				else
//				{
//					val += index*100;
//					val = 400 - val;//val - 400;
//					val = val | 0x8000;
//				}			 
				return (val+70);
				}
			} while (index) ;	
	}		
				
	else
		{
			work_var= def_tab_pic[index];

			  
		if (work_var > count )
			{
			val =  index  * 100 ;
			return ( val );			
			}
	
		do 
			{
			index--;

				work_var += def_tab_pic[index];


			if( work_var > count)
				{
				val = ( work_var - count )*100;
				val /= def_tab_pic[index];

				if(index >= 4)
				{
					val +=  (index - 4) * 100;
					val = val & 0x7fff;
				}
				else
				{
					val += index*100;
					val = 400 - val;//
					//val = val - 400;
					val = val | 0x8000;
				}			 
				return (val);
				}
			} while (index) ;
		}
			val =  33768;

			return ( val );
}
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

int16 RangeConverter(uint8 function, int16 para,uint8 i,int16 cal)
{
	int16 xdata siAdcResult;
	uint8 xdata ucFunction;
//	uint8 xdata ucI;
	int16  xdata siInput;
	int16  xdata uiCal;
	int32  xdata siResult;
	uint8 bAnalogInputStatus;
	ucFunction = function;
	siInput = para;
//	ucI = i;
	uiCal = cal;

	if((ucFunction == AI_RANGE_UNUSED) || (ucFunction == AI_RANGE_VOLTAGE_5V))
	{
		#ifdef TSTAT7_ARM
		siResult = siInput*300/1024;
		#else
	 	siResult = siInput*518/1000;
		siResult *= InputVoltageTerm;
		siResult /= 100;
		#endif
	}	 

	else if(ucFunction == 99)
	{
		siResult = siInput;
	}	
	
	else if((ucFunction == AI_RANGE_UNUSED_10V )||(ucFunction == AI_RANGE_VOLTAGE_10V))
	{
		#ifdef TSTAT8_HUNTER
		siResult = (int32)siInput*1200/1023;//(int32)siInput*879/1000;
		#else
		//if(HardwareVersion >= 5)
		#ifdef ROLY
			siResult = (int32)siInput*1200/1023;
		#else
		if((HardwareVersion < HW_VERSION_OVERSHOOT)&&(co2_present != 1))
			siResult = (int32)siInput*879/1000;
		else
			siResult = (int32)siInput*1200/1023;
		#endif
			
		#endif
	}	
	//-----------10K Thermistor---------------
	else if( (ucFunction == AI_RANGE_10K_THERMISTOR_TYPE2) || (ucFunction == AI_RANGE_10K_THERMISTOR_TYPE3)|| (ucFunction == AI_RANGE_50K_THERMISTOR))
	{ 
		if(ucFunction == AI_RANGE_10K_THERMISTOR_TYPE2)
			siAdcResult = look_up_table(siInput, THERMISTER_TYPE2);
		else if(ucFunction == AI_RANGE_50K_THERMISTOR)
		{
			siAdcResult = look_up_table(siInput, THERMISTER_TYPE50K);
			if(siAdcResult < 70)
				siAdcResult = 70;
		}
 		else
			siAdcResult = look_up_table(siInput, THERMISTER_TYPE3);
		//MHF:01-02-06,Added minus temperature display
		if(siAdcResult & 0x8000)
		siResult = -(int16)(siAdcResult & 0x7fff);
		else
		siResult = siAdcResult;
		//analog_input[i] = adc_result;
		
		if(EEP_DEGCorF)  
			siResult = (siResult * 9)/5 +320; 

		// Add the calibration term to the input.
		siResult = siResult + uiCal;//- CALIBRATION_OFFSET;

		 	
	}
	//-----------0-100%---------------
	else if(ucFunction == AI_RANGE_PERCNETAGE)  //MHF: Feb 24th 2005 new range setting for analog inputs
	{
		#ifdef TSTAT7_ARM
		siResult = (float)(siInput)*100/1024;
		#else
		siResult = (float)(siInput)*0.104;//0.159;//297/2046;
		#endif
	}
	
	else if(ucFunction == AI_RANGE_PERCNETAGE_10V) //cc add 10V precentage range for new tstat ARM mcu 
	{ 
		siResult = (float)(siInput)*0.117;//0.0955;//297 * 3/10230;
	}
		
	//-----------ON/OFF---------------
	else if((ucFunction == AI_RANGE_ON_OFF) || (ucFunction == AI_RANGE_OFF_ON)
		|| (ucFunction == AI_RANGE_OPEN_CLOSE) || (ucFunction == AI_RANGE_CLOSE_OPEN)
		|| (ucFunction == AI_RANGE_OCCUPIED_UNOCCUPIED) || (ucFunction == AI_RANGE_UNOCCUPIED_OCCUPIED)	)
	{
		siAdcResult = siInput;//(float)(siInput)/1023*50;
		#ifdef TSTAT7_ARM
		if(siAdcResult <= 512)//2.5V
		#else
		if(siAdcResult <= 482)//2.5V
		#endif
		{	
			if((ucFunction == AI_RANGE_OFF_ON)||(ucFunction == AI_RANGE_CLOSE_OPEN)||(ucFunction == AI_RANGE_UNOCCUPIED_OCCUPIED))
			{
				bAnalogInputStatus = 0; 
				#ifdef TSTAT8_HUNTER
				switch(i)
				{
					case 0:
						AI1_LED_OFF;
					break;
					case 1:
						AI2_LED_OFF;
					break;
					case 2:
						AI3_LED_OFF;
					break;
					case 3:
						AI4_LED_OFF;
					break;
					case 4:
						AI5_LED_OFF;
					break;
					case 5:
						AI6_LED_OFF;
					break;
					case 6:
						AI7_LED_OFF;
					break;
					default:
					break;
				}
				#endif//TSTAT8_HUNTER
			}
			else 
			{
				bAnalogInputStatus = 1;
				#ifdef TSTAT8_HUNTER
				switch(i)
				{
					case 0:
						AI1_LED_ON;
					break;
					case 1:
						AI2_LED_ON;
					break;
					case 2:
						AI3_LED_ON;
					break;
					case 3:
						AI4_LED_ON;
					break;					
					case 4:
						AI5_LED_ON;
					break;				
					case 5:
						AI6_LED_ON;
					break;	
					case 6:
						AI7_LED_ON;
					break;	
					
					default:
					break;
				}
				#endif//TSTAT8_HUNTER
			}				
		}
		#ifdef TSTAT7_ARM
		else if(siAdcResult > 512)//2.5V
		#else
		else if(siAdcResult > 482)//2.5V
		#endif
		{
			if((ucFunction == AI_RANGE_ON_OFF)||(ucFunction == AI_RANGE_OPEN_CLOSE)||(ucFunction == AI_RANGE_OCCUPIED_UNOCCUPIED))
			{
				bAnalogInputStatus = 0; 
				#ifdef TSTAT8_HUNTER
				switch(i)
				{
					case 0:
						AI1_LED_OFF;
					break;
					case 1:
						AI2_LED_OFF;
					break;
					case 2:
						AI3_LED_OFF;
					break;
					case 3:
						AI4_LED_OFF;
					break;
					case 4:
						AI5_LED_OFF;
					break;
					case 5:
						AI6_LED_OFF;
					break;
					case 6:
						AI7_LED_OFF;
					break;
					default:
					break;
				}
				#endif//TSTAT8_HUNTER				
			}
			else
			{
				bAnalogInputStatus = 1; 
				#ifdef TSTAT8_HUNTER
				switch(i)
				{
					case 0:
						AI1_LED_ON;
					break;
					case 1:
						AI2_LED_ON;
					break;
					case 2:
						AI3_LED_ON;
					break;
					case 3:
						AI4_LED_ON;
					break;				
					case 4:
						AI5_LED_ON;
					break;					
					case 5:
						AI6_LED_ON;
					break;
					case 6:
						AI7_LED_ON;
					break;						
					default:
					break;
				}
				#endif//TSTAT8_HUNTER				
			}
		}
		
		
		
		siResult = (uint16)(bAnalogInputStatus);
	}
	
	
	else if((ucFunction == AI_RANGE_ON_OFF_10V) || (ucFunction == AI_RANGE_OFF_ON_10V)
		|| (ucFunction == AI_RANGE_OPEN_CLOSE_10V) || (ucFunction == AI_RANGE_CLOSE_OPEN_10V)
		|| (ucFunction == AI_RANGE_OCCUPIED_UNOCCUPIED_10V) || (ucFunction == AI_RANGE_UNOCCUPIED_OCCUPIED_10V)	)
	{
		siAdcResult = siInput;//(float)(siInput)/1023*50;
		if(siAdcResult <= 500)
		{
			if((ucFunction == AI_RANGE_OFF_ON_10V)||(ucFunction == AI_RANGE_CLOSE_OPEN_10V)||(ucFunction == AI_RANGE_UNOCCUPIED_OCCUPIED_10V))
				bAnalogInputStatus = 0; 
			else 
				bAnalogInputStatus = 1; 
		}
		else if(siAdcResult > 500)
		{
			if((ucFunction == AI_RANGE_ON_OFF_10V)||(ucFunction == AI_RANGE_OPEN_CLOSE_10V)||(ucFunction == AI_RANGE_OCCUPIED_UNOCCUPIED_10V))
				bAnalogInputStatus = 0; 
			else
				bAnalogInputStatus = 1; 
		}
		siResult = (uint16)(bAnalogInputStatus);
	}	
	
	
	
	//-----------Custom Sensor---------------
	//else if(ucFunction == 4 && ucI < 3)
	else if((ucFunction == AI_RANGE_USER)||(ucFunction == AI_RANGE_USER_10V))  //extend customer table to all channels
		{
		if(ucFunction == AI_RANGE_USER)
			siResult = siInput*518/1000;
		else if(ucFunction == AI_RANGE_USER_10V)
		{
			#ifdef ROLY
				siResult = (int32)siInput*1200/1023;
			#else
			if((HardwareVersion < HW_VERSION_OVERSHOOT)&&(co2_present != 1))
				siResult = (int32)siInput*879/1000;
			else
				siResult = (int32)siInput*1200/1023;
			#endif
		}
		
		//	siResult = (int32)siInput*879/1000;
		//siResult = siInput;
		siResult *= InputVoltageTerm;
		siResult /= 100;
		if(ucFunction == AI_RANGE_USER)		
			siAdcResult = look_up_customtable(0 ,siResult,slope_type[0],AI_RANGE_USER);
		else if(ucFunction == AI_RANGE_USER_10V)
			siAdcResult = look_up_customtable(0 ,siResult,slope_type[0],AI_RANGE_USER_10V);
//		if(ucFunction == AI_RANGE_USER_10V)
//		{
//			siAdcResult *= 17;
//			siAdcResult /= 10;
//		}
		
//		if(table1_flag == ucI)
//			siAdcResult = look_up_customtable(EEP_TABLE1_ZERO ,siInput,slope_type[0]);
//		else
//			siAdcResult = look_up_customtable(EEP_TABLE1_ZERO + 22,siInput,slope_type[1]);			 
		// Add the calibration term to the input.
		siResult = siAdcResult + uiCal;// - CALIBRATION_OFFSET;
		}

	else if((ucFunction == AI_RANGE_CUSTOMER_SENSOR)||(ucFunction == AI_RANGE_CUSTOMER_SENSOR_10V))  //extend customer table to all channels
		{
		if(ucFunction == AI_RANGE_CUSTOMER_SENSOR)
			siResult = siInput*518/1000;
		else if(ucFunction == AI_RANGE_CUSTOMER_SENSOR_10V)
		{
			#ifdef ROLY
				siResult = (int32)siInput*1200/1023;
			#else			
			if((HardwareVersion < HW_VERSION_OVERSHOOT)&&(co2_present != 1))
				siResult = (int32)siInput*879/1000;
			else
				siResult = (int32)siInput*1200/1023;
			#endif
		}
		//	siResult = (int32)siInput*879/1000;
		siResult *= InputVoltageTerm;
		siResult /= 100;
		//siInput = siResult;
		if(ucFunction == AI_RANGE_CUSTOMER_SENSOR)		
			siAdcResult = look_up_customtable(0,siResult,slope_type[1],AI_RANGE_CUSTOMER_SENSOR);
		else if(ucFunction == AI_RANGE_CUSTOMER_SENSOR_10V)
			siAdcResult = look_up_customtable(0,siResult,slope_type[1],AI_RANGE_CUSTOMER_SENSOR_10V);
		siResult = siAdcResult + uiCal;// - CALIBRATION_OFFSET;
		}
		
	else if(ucFunction == AI_RANGE_MA)  //4-20mA
		{
		#ifdef TSTAT7_ARM
		siResult = 0;// tstat7 not support 4-20mA range
		#else			
		siResult = (float)siInput * 0.49;
		#endif
    if(siResult < 400)
			siResult = 400;
		siResult = get_4to20ma_value(siResult);
		
		
		}	
				
			
 	return siResult;
}


int16 get_tem(uint8 num)
{
	int16 tem;
	switch(num)
	{
		case PID_AI1:
		case PID_AI2:	
		case PID_AI3:
		case PID_AI4:
		case PID_AI5:
		case PID_AI6:
		case PID_AI7:
		case PID_AI8:		
			tem = mul_analog_input[num - PID_AI1];
		break;
		case PID_INTERNAL_TEMPERATURE:				
			tem = temperature_internal;
		break;

		default:
			tem = 0;
		break;
	}
  return tem;
}

void CalTemperature(void)
{
	int16 tem_temp;
	int16 delta_source1,delta_source2;
	tem_temp = ADC_getChannal(ADC1, 15);
	tem_temp = Trans_ADC(tem_temp);//temperature_internal * 1023 / 4095;	
	tem_temp = Filter(10, tem_temp);
	if(EEP_TempSelect == THERMISTER_TYPE3)	
	{
		tem_temp = look_up_table(tem_temp, THERMISTER_TYPE3);
	}
	else if(EEP_TempSelect == THERMISTER_TYPE50K)
		tem_temp = look_up_table(tem_temp, THERMISTER_TYPE50K);
	else
		tem_temp = look_up_table(tem_temp, THERMISTER_TYPE2);
 
//#ifdef TSTAT_CO2
//  temperature_c = tem_of_co2;
//#else	
	temperature_c = tem_temp;
//#endif	
	
	temperature_f = tem_temp * 9 /5 + 320;
	
	if(EEP_DEGCorF) //degreee F
		temperature_internal = temperature_f;		
	else
		temperature_internal = temperature_c;	

  temperature_org = temperature_internal;	

	temperature_internal = temperature_internal + internal_calibration;
	
	if(GetByteBit(&EEP_InterThermistorManual,0))//manual mode	
		
		temperature = (int16)ManualInternalSensor_HI*256 + ManualInternalSensor_LO;
	
	else//auto mode
		{
		if(EEP_TempSelect == 0)  //not used. still use internal	
					temperature = temperature_internal ;//not used. still use internal

//		else if(EEP_TempSelect == 1)  // external not used. still use internal
//			temperature = temperature_internal;
		else if((EEP_TempSelect == THERMISTER_TYPE2) || (EEP_TempSelect == THERMISTER_TYPE3)) // internal thermoster sensor
			temperature = temperature_internal;// + internal_calibration;
		
		else if(EEP_TempSelect == TEMP_HUM_TEMPERATURE)  // use temperature of hum sensor
			temperature = htu_temp + Hum_T_calibration;

		else if(EEP_TempSelect == TEMP_AVERAGE_TEMPERATURE_INTERNAL_AI1) //select average value of internal sensor and AI1
			temperature = (temperature_internal + mul_analog_input[0]) >> 1;//divided by 2,get the average

		else if(EEP_TempSelect == TEMP_AVERAGE_TEMPERATURE_AI1_TO_AI2) //select average value of AI1 and AI2
			temperature = (mul_analog_input[0] + mul_analog_input[1]) >> 1;//divided by 2,get the average

		else if(EEP_TempSelect == TEMP_AVERAGE_TEMPERATURE_AI1_TO_AI3) //select average value of AI1 and AI2 and AI3 
			temperature = (mul_analog_input[0] + mul_analog_input[1] + mul_analog_input[2]) / 3;//divided by 3,get the average

		else if(EEP_TempSelect == TEMP_AVERAGE_TEMPERATURE_AI1_TO_AI4) //select average value of AI1 TO AI4
			temperature = (mul_analog_input[0] + mul_analog_input[1] + mul_analog_input[2] + mul_analog_input[3]) /4;//divided by 4,get the average

		else if((EEP_TempSelect >= TEMP_AI1) && (EEP_TempSelect <= TEMP_AI8))//
			temperature = mul_analog_input[EEP_TempSelect - TEMP_AI1];
		else if (EEP_TempSelect == TEM_COMPATIBLE_AI1)
			temperature = mul_analog_input[0];
		
		else if(EEP_TempSelect == PID_DELTA_TEMPERATURE)
		{
			delta_source1 = get_tem(Delta_tem_source1);
			delta_source2 = get_tem(Delta_tem_source2);
			temperature = delta_source1 -  delta_source2;		
		}

		else
			temperature = temperature_internal;
		
		if(EEP_Input1Select == PID_DELTA_TEMPERATURE)
		{
			delta_source1 = get_tem(Delta_tem_source1);
			delta_source2 = get_tem(Delta_tem_source2);
			delta_temperature = delta_source1 -  delta_source2;		
		}
		
		
		if ( temperature < MIN_TEMP )  //Clip to MIN/MAX in Deg C, always. 
				temperature =	MIN_TEMP;
		if ( temperature > MAX_TEMP )  //TBD: check array instead of these defines
			temperature =MAX_TEMP; 
		}
//	AI_Present_Value[0] = temperature;

}

//#define AI_RANGE_UNUSED										0
//#define AI_RANGE_10K_THERMISTOR_TYPE2			1
//#define AI_RANGE_PERCNETAGE								2
//#define AI_RANGE_ON_OFF										3
//#define AI_RANGE_USER											4
//#define AI_RANGE_OFF_ON										5
//#define AI_RANGE_CUSTOMER_SENSOR			  	6		
//#define AI_RANGE_OPEN_CLOSE								7
//#define AI_RANGE_CLOSE_OPEN								8
//#define AI_RANGE_OCCUPIED_UNOCCUPIED		  9
//#define AI_RANGE_UNOCCUPIED_OCCUPIED			10
//#define AI_RANGE_10K_THERMISTOR_TYPE3			11
//#define AI_RANGE_MA												12

//00 0-20mA
//01 0-5V
//10 0-10V
//11 10K NTC

void set_input_range( uint8 range)
{
switch(range)
	{									
	case AI_RANGE_10K_THERMISTOR_TYPE2:	
	case AI_RANGE_10K_THERMISTOR_TYPE3:		
	case AI_RANGE_50K_THERMISTOR:	
	case AI_RANGE_ON_OFF:	
	case AI_RANGE_OFF_ON:										
	case AI_RANGE_CUSTOMER_SENSOR:			  		
	case AI_RANGE_OPEN_CLOSE:								
	case AI_RANGE_CLOSE_OPEN:								
	case AI_RANGE_OCCUPIED_UNOCCUPIED:	  
	case AI_RANGE_UNOCCUPIED_OCCUPIED:		
		INPUT_TYPE1_OUT(1);
		INPUT_TYPE0_OUT(1);
	break;
	//0-5V input type
	case AI_RANGE_UNUSED:	
  case AI_RANGE_VOLTAGE_5V:		
	case AI_RANGE_PERCNETAGE:								
	//case AI_RANGE_ON_OFF:										
	case AI_RANGE_USER:											
//	case AI_RANGE_OFF_ON:										
//	case AI_RANGE_CUSTOMER_SENSOR:			  		
//	case AI_RANGE_OPEN_CLOSE:								
//	case AI_RANGE_CLOSE_OPEN:								
//	case AI_RANGE_OCCUPIED_UNOCCUPIED:	  
//	case AI_RANGE_UNOCCUPIED_OCCUPIED:	
	
		INPUT_TYPE1_OUT(0);
		INPUT_TYPE0_OUT(1);
	break;	
 //0-10V input range
	case AI_RANGE_UNUSED_10V:	
	case AI_RANGE_VOLTAGE_10V:			
	case AI_RANGE_PERCNETAGE_10V:								
	case AI_RANGE_ON_OFF_10V:										
	case AI_RANGE_USER_10V:											
	case AI_RANGE_OFF_ON_10V:										
	case AI_RANGE_CUSTOMER_SENSOR_10V:			  		
	case AI_RANGE_OPEN_CLOSE_10V:								
	case AI_RANGE_CLOSE_OPEN_10V:								
	case AI_RANGE_OCCUPIED_UNOCCUPIED_10V:	  
	case AI_RANGE_UNOCCUPIED_OCCUPIED_10V:	
	
		INPUT_TYPE1_OUT(1);
		INPUT_TYPE0_OUT(0);
	break;	
	
	case AI_RANGE_MA:
		INPUT_TYPE1_OUT(0);
		INPUT_TYPE0_OUT(0);
	break;
	}

}
void sel_channel(uint8 channel)
{
switch(channel)
	{
	case SEL_AI1:
		CHSEL2_OUT(0);
		CHSEL1_OUT(0);
		CHSEL0_OUT(0);
	break;

	case SEL_AI2:
		CHSEL2_OUT(0);
		CHSEL1_OUT(0);
		CHSEL0_OUT(1);		
	break;	
	
	case SEL_AI3:
		CHSEL2_OUT(0);
		CHSEL1_OUT(1);
		CHSEL0_OUT(0);		
	break;	

	case SEL_AI4:
		CHSEL2_OUT(0);
		CHSEL1_OUT(1);
		CHSEL0_OUT(1);		
	break;	

	case SEL_AI5:
		CHSEL2_OUT(1);
		CHSEL1_OUT(0);
		CHSEL0_OUT(0);		
	break;	

	case SEL_AI6:
		CHSEL2_OUT(1);
		CHSEL1_OUT(0);
		CHSEL0_OUT(1);		
	break;	

	case SEL_AI7:
		CHSEL2_OUT(1);
		CHSEL1_OUT(1);
		CHSEL0_OUT(0);		
	break;	

	case SEL_AI8:
		CHSEL2_OUT(1);
		CHSEL1_OUT(1);
		CHSEL0_OUT(1);		
	break;		

	default:
	break;
	}
}

uint8 Get_AI_Function(uint8 channel)
{
	uint8 aif;
	switch(channel)
	{
		case 1: 
			aif = AI_Function1;
		break;

		case 2: 
			aif = AI_Function2;
		break;
		case 3: 
			aif = AI_Function3;
		break;
		case 4: 
			aif = AI_Function4;
		break;
		case 5: 
			aif = AI_Function5;
		break;
		case 6: 
			aif = AI_Function6;
		break;
		case 7: 
			aif = AI_Function7;
		break;
		case 8: 
			aif = AI_Function8;
		break;		
		default:
		break;
	}
return aif;
}

void CalInput(void)
{
	uint8 i,j,k,m;
	uint32 pir_temp = 0;//[6] = {0,0,0,0,0,0};
	uint16 adc_buf[20];
	uint32 adc_sum;
	static uint8 rs485_wrn_cnt = 0;
//MAX_INPUT_CHANNEL
	for(i=0;i<MAX_INPUT_CHANNEL;i++)//
		{	
    AI_MODE_OUT(1);	
		
		if(GetByteBit(&EEP_InputManuEnable,i)) 
			
			mul_analog_input[i] = ((int16)ManualAI_HI(i) << 8) + ManualAI_LO(i);
		
		else if(Get_AI_Function(i+1) == AI_FUNCTION_OUTTEM)
			{
			mul_analog_input[i] = outside_tem;
			}

		else
			{ 
			#ifdef TSTAT7_ARM			
			mul_analog_in_buffer[i] = ADC_getChannal(ADC1, channel[i]);	
			#else
			set_input_range(AI_Range(i));
			
		  sel_channel(i);	
			delay_ms(10);	
//mul_analog_in_buffer[i] = ADC_getChannal(ADC1, 11);
			adc_sum = 0;
				/*
			for(j=0;j<20;j++)
			{				
				adc_buf[j] = ADC_getChannal(ADC1, 11);
				adc_sum += adc_buf[j];
        delay_us(200);
			}	 
			mul_analog_in_buffer[i] = adc_sum / 20;	
				*/
				
			mul_analog_in_buffer[i] = ADC_getChannal(ADC1, 11);
			

			#endif	
			if(AI_Range(i) != AI_RANGE_MA)	//ma do not have enough resolution
				mul_analog_in_buffer[i] = Trans_ADC(mul_analog_in_buffer[i]);	

			if(AI_Range(i) == AI_RANGE_ON_OFF || AI_Range(i) == AI_RANGE_OFF_ON || ((AI_Range(i) >=AI_RANGE_OPEN_CLOSE)&&(AI_Range(i) <=AI_RANGE_UNOCCUPIED_OCCUPIED)))
				;//mul_analog_input[i] = mul_analog_in_buffer[i];//ON/OFF range,no filter
			else if(AI_Range(i) == AI_RANGE_ON_OFF_10V || AI_Range(i) == AI_RANGE_OFF_ON_10V || ((AI_Range(i) >=AI_RANGE_OPEN_CLOSE_10V)&&(AI_Range(i) <=AI_RANGE_UNOCCUPIED_OCCUPIED_10V)))
				;//mul_analog_input[i] = mul_analog_in_buffer[i];
			else
				mul_analog_in_buffer[i] = Filter(i,mul_analog_in_buffer[i]);

			//cc extend customer table to all channels
			if((AI_Range(i) == AI_RANGE_USER)||(AI_Range(i) == AI_RANGE_USER_10V))//if range is set to customer sensor
			{
				if(table1_flag < MAX_INPUT_CHANNEL)//AI1
					table2_flag = i;
				else
					table1_flag = i;										
			}
			mul_analog_in_buffer[i] = RangeConverter(AI_Range(i),mul_analog_in_buffer[i],i,0);
						
			
			mul_analog_input[i] = mul_analog_in_buffer[i] + mul_analog_cal[i];  
			}	
			//AI_MODE_OUT(0);
			//delay_ms(20);
		}
//     #ifdef TSTAT_CO2
			aq_value = ADC_getChannal(ADC1, 10);			

		#ifdef TSTAT_CO2
		if(hum_sensor_type == 0)
		{
			htu21d_trigger_measurement_no_hold_master(SEL_HUM);
			delay_ms(20);
			mul_analog_in_buffer[8] = read_hum_sensor(SEL_HUM);
      //delay_ms(20);
			htu21d_trigger_measurement_no_hold_master(SEL_TEMP);
			delay_ms(50);		
		  htu_temp = read_hum_sensor(SEL_TEMP);		
		}	
		
		else
		{
			SHT3X_GetTempAndHumi(&tem_org, &hum_org, REPEATAB_HIGH, MODE_POLLING, 50);
			htu_temp = tem_org;
			mul_analog_in_buffer[8] = hum_org;
		}
		
		#else
		
		if(hum_sensor_type == 1)
		{
			SHT3X_GetTempAndHumi(&tem_org, &hum_org, REPEATAB_HIGH, MODE_POLLING, 50);
			htu_temp = tem_org;
			mul_analog_in_buffer[8] = hum_org;
		}
		else
		{
			htu21d_trigger_measurement_no_hold_master(SEL_HUM);
			delay_ms(20);
			mul_analog_in_buffer[8] = read_hum_sensor(SEL_HUM);
      //delay_ms(20);
			htu21d_trigger_measurement_no_hold_master(SEL_TEMP);
			delay_ms(50);		
		  htu_temp = read_hum_sensor(SEL_TEMP);
		}
		#endif
		
		if(hum_manual_enable == 0)	  //hum_manaul_disable
			{

			if(HUM_Filter == 0)
				org_humidity = mul_analog_in_buffer[8];
			else
				org_humidity = Filter(9, mul_analog_in_buffer[8]);
			
			#ifdef TSTAT_CO2
			
			co2_data =  Filter(8, co2_data_org);
			co2_data = co2_data + co2_calibration_data;
			if(co2_data < 400)
				co2_data = 400;
			
			if(hum_sensor_type == 1)
				humidity = org_humidity;
			else
				humidity = CalculateTemperatureCompensation(org_humidity, htu_temp);

			#else	
			if(HardwareVersion >= 6)
				humidity = org_humidity;
			else
				humidity = CalculateTemperatureCompensation(org_humidity, htu_temp);
			#endif
			
			humidity = humidity + humidity_calibration;

			
			}
		else
			{
			humidity = read_eeprom(EEP_HUM_INPUT_MANUAL_VALUE+1);
			humidity = humidity * 256;
			humidity += read_eeprom(EEP_HUM_INPUT_MANUAL_VALUE);
			}	
		if(humidity > 1000) 
			humidity = 1000;
		if(humidity < 0)
			humidity = 0;
		dew_data = get_dew_pt(htu_temp,humidity);	
			
		light_sensor = ADC_getChannal(ADC1, 8);	
		
		for(i=0;i<20;i++)
		{		
			pir_temp += ADC_getChannal(ADC1, 9);	
			delay_ms(1);			
		}	
		pir_temp = pir_temp/20;
		//if((pir_temp > 500) && (pir_temp < 3400))
		pir_value = pir_temp *100 / 4096 * 3;	
		pir_temp = 0;	

		ao1_fdbk = ADC_getChannal(ADC1, 1);	
		ao1_fdbk = (uint32)ao1_fdbk * 909/4096;
		ao2_fdbk = ADC_getChannal(ADC1, 4);				
		ao2_fdbk = (uint32)ao2_fdbk * 909/4096;
		
		rs485_p_voltage = ADC_getChannal(ADC1, 12)*14/1000; //(3*ADC_getChannal(ADC1, 12)/4096)*1000/53;
		rs485_n_voltage = ADC_getChannal(ADC1, 13)*14/1000;;
		if((rs485_p_voltage > 15)||(rs485_n_voltage > 15))//if voltage over 15V
		{
			if(rs485_wrn_cnt < 10)
				rs485_wrn_cnt += 5;
			if(rs485_wrn_cnt >= 10)
			  voltage_overshoot = 1;
		}
		else
		{
			if(rs485_wrn_cnt > 0)
				rs485_wrn_cnt--;
			if(rs485_wrn_cnt == 0)
				voltage_overshoot = 0;
		}
}

uint16 Trans_ADC(uint16 adc_value)//translate ADC value from 12 bits to 10 bits
{
	uint16 temp_adc;
	temp_adc = adc_value/4;	
	return temp_adc;
}




void inputs_init(void) 
{
	inputs_io_init();
	inputs_adc_init();
	//dma_adc_init();	
}
int16 tem_select(uint8 id)
{
	int16 temp = 0;;
	switch(id)
	{
		case FRC_SRC_AI1:
		case FRC_SRC_AI2:
		case FRC_SRC_AI3:
		case FRC_SRC_AI4:
		case FRC_SRC_AI5:
		case FRC_SRC_AI6:	
		case FRC_SRC_AI7:
		case FRC_SRC_AI8:			
			temp = mul_analog_input[id - FRC_SRC_AI1];
		break;
		
		case FRC_SRC_INTERNAL:
			temp = temperature_internal;
		break;	
		default:
			break;
	}
	
	return temp;
}

void frc_input(void) //calculate free cooling input value
{
	check_frc_config();
 if(FrcEnable == ENABLE_FRC)
 {
	 if(outdoortem.value < spacetem.value)//outside have cooler air, then goto frc mode
	 {
	    fan_speed_user = FAN_AUTO;
			fanspeedbuf = FAN_AUTO;
			//icon.fan = 1;		 
	 }
	 else
	 {
			fan_speed_user = FAN_OFF;
			fanspeedbuf = FAN_OFF; 
	 }
	 
	 if(spacetem.source == FRC_LOCAL_SENSOR)
	 {
		 spacetem.value = tem_select(spacetem.id);
	 }
	 
	 if(supplytem.source == FRC_LOCAL_SENSOR)
	 {
		 supplytem.value = tem_select(supplytem.id);
	 }
	
	 if(outdoortem.source == FRC_LOCAL_SENSOR)
	 {
		 outdoortem.value = tem_select(outdoortem.id);
	 }	
	
	if(FrcBaseSelect == FRC_ENTHALPY_BASE)
	{
		if(outdoorhum.source == FRC_LOCAL_SENSOR)
		{
			outdoorhum.value = humidity;
		}
		
		if(indoorhum.source == FRC_LOCAL_SENSOR)
		{
			indoorhum.value = humidity;
		}
	}
	
 }

}


void check_frc_config(void)//check if each block in free cooling function is legal 
{
 //space temperature block
	frc_block_config = FRC_CONFIG_PASS;//initial state is OK, if any block config is wrong, then change state to wrong
	if(spacetem.source == FRC_LOCAL_SENSOR)//if select local sensor, value should be less than AI8
	{
		if(spacetem.id > FRC_SRC_AI8)
		{
			spacetem.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			spacetem.config = FRC_CONFIG_PASS;
	}
		
  if(spacetem.source == FRC_REMOTE_SENSOR) 
	{
		if(FrcSpaceTemUnit != EEP_DEGCorF) //local unit is different with remote panel 
		{
			spacetem.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			spacetem.config = FRC_CONFIG_PASS;
		
		if(spacetem.update > 0)
			spacetem.config = FRC_CONFIG_PASS;
		else
			spacetem.config = FRC_CONFIG_FAIL;
	}
	


 //supply temperature block-------------------------------
	if(supplytem.source == FRC_LOCAL_SENSOR)//if select local sensor, value should be less than 
	{
		if(supplytem.id > FRC_SRC_AI8)//internal/hum tem/AI1-AI8, AI8 is the last one
		{
			supplytem.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			supplytem.config = FRC_CONFIG_PASS;
	}
		
  if(supplytem.source == FRC_REMOTE_SENSOR) 
	{
		if(FrcSupplyTemUnit != EEP_DEGCorF) //local unit is different with remote panel 
		{
			supplytem.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			supplytem.config = FRC_CONFIG_PASS;
		
		if(supplytem.update >0)
			supplytem.config = FRC_CONFIG_PASS;
		else
			supplytem.config = FRC_CONFIG_FAIL;
	}
	
	
				
 //outdoor temperature block-----------------------------------
	if(outdoortem.source == FRC_LOCAL_SENSOR)//if select local sensor, value should be less than 
	{
		if(outdoortem.id > FRC_SRC_INTERNAL)
		{
			outdoortem.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			outdoortem.config = FRC_CONFIG_PASS;
	}
		
  if(outdoortem.source == FRC_REMOTE_SENSOR) 
	{
		if(FrcOutdoorTemUnit != EEP_DEGCorF) //local unit is different with remote panel 
		{
			outdoortem.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			outdoortem.config = FRC_CONFIG_PASS;
		
		if(outdoortem.update > 0)
			outdoortem.config = FRC_CONFIG_PASS;
		else
			outdoortem.config = FRC_CONFIG_FAIL;
	}
 //indoor humidity block----------------------------------
	if(indoorhum.source == FRC_LOCAL_SENSOR)//if select local sensor, value should be less than 
	{
		if(indoorhum.id != FRC_SRC_HUMIDITY)
		{
			indoorhum.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			indoorhum.config = FRC_CONFIG_PASS;
	}
  if(indoorhum.source == FRC_REMOTE_SENSOR)
	{		
		if(indoorhum.id != FRC_SRC_REMOTEHUM)
		{
			indoorhum.config = FRC_CONFIG_FAIL;	
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			indoorhum.config = FRC_CONFIG_PASS;	
		
		if(indoorhum.update > 0)
			indoorhum.config = FRC_CONFIG_PASS;
		else
			indoorhum.config = FRC_CONFIG_FAIL;		
	}
 //outdoor humidity block	---------------------------------------
	if(outdoorhum.source == FRC_LOCAL_SENSOR)//if select local sensor, value should be less than 
	{
		if(outdoorhum.id != FRC_SRC_HUMIDITY)
		{
			outdoorhum.config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			outdoorhum.config = FRC_CONFIG_PASS;	
	}	
  if(outdoorhum.source == FRC_REMOTE_SENSOR)
	{		
		if(outdoorhum.id != FRC_SRC_REMOTEHUM)
		{
			outdoorhum.config = FRC_CONFIG_FAIL;	
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			outdoorhum.config = FRC_CONFIG_PASS;
		if(outdoorhum.update > 0)
			outdoorhum.config = FRC_CONFIG_PASS;
		else
			outdoorhum.config = FRC_CONFIG_FAIL;			
	}
 //output block  -------------------------------------------
	if(frc_output_select == FRC_OUTPUT_AO1)
	{
		if(EEP_PidOutput6 != 2)
		{
			frc_output_config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			frc_output_config = FRC_CONFIG_PASS;
	}
	if(frc_output_select == FRC_OUTPUT_AO2)
	{
		if(EEP_PidOutput7 != 2)
		{
			frc_output_config = FRC_CONFIG_FAIL;	
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			frc_output_config = FRC_CONFIG_PASS;	
	}
 //pid block check if pid2 has the input from supply sensor
	if(supplytem.source == FRC_LOCAL_SENSOR)
	{
		if(EEP_Input1Select != supplytem.id)
		{			
			frc_pid2_config = FRC_CONFIG_FAIL;
			frc_block_config = FRC_CONFIG_FAIL;
		}
		else
			frc_pid2_config = FRC_CONFIG_PASS;
	}
	
}

void frc_mode_check(void)
{
	if(FrcEnable == ENABLE_FRC)
	{
		if(frc_block_config == FRC_CONFIG_PASS)//
		{
			if(FrcBaseSelect == FRC_TEMPERATURE_BASE)
			{
				if(outdoortem.value < (spacetem.value - (UniversalCoolDB_HI * 256 + UniversalCoolDB_LO)))
					frc_mode = FRC_FUNCTION_START;
				else
					frc_mode = FRC_FUNCTION_NOT_START;
			}
			else
			{
				if(outdoortem.value < (spacetem.value - (UniversalCoolDB_HI * 256 + UniversalCoolDB_LO)))
					frc_mode = FRC_FUNCTION_START;
				else
					frc_mode = FRC_FUNCTION_NOT_START;			
			}
		}
		else
			frc_mode = FRC_FUNCTION_NOT_START;
	}
	else
	{
		frc_mode = FRC_FUNCTION_NOT_START;
	}
	
	if(frc_mode == FRC_FUNCTION_START)
	{
		loop_setpoint[PID_LOOP2] = loop_setpoint[PID_LOOP1];
		EEP_HEAT_TABLE2 = 0; //set 0 heating stage
		EEP_COOL_TABLE2 = 1; //set 1 cooling stage 		
	}
	
}

u16 ADC_getChannal(ADC_TypeDef* ADCx, u8 channal)
{
         uint16_t tem = 0;
				 ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
         ADC_RegularChannelConfig(ADCx, channal, 1, ADC_SampleTime_55Cycles5);
         ADC_SoftwareStartConvCmd(ADCx, ENABLE);         
         while(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) == RESET);
         tem = ADC_GetConversionValue(ADCx);
         return tem;        
}
//------------------------------
int16 get_4to20ma_value(uint16 current)
{
	//y =kx+b
	//k= (y2-y1)/(x2-x1)
	//b= (x2y1-x1y2)/(x2-x1)
	//x1 = 40   x2 = 200
	//y1 = bottom_of_4to20ma   y2 = top_of_4to20ma 
	float temp; 
  float k;
  float b;	
	
	temp = top_of_4to20ma - bottom_of_4to20ma;
	k = temp/160;
	
	temp = 200*bottom_of_4to20ma - 40*top_of_4to20ma;
	b = temp/160;
	
	temp = k*current + b*10; 
	temp /= 10;
	return (int16)temp;
}







