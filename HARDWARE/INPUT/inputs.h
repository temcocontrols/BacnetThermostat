#ifndef __INPUTS_H
#define __INPUTS_H

#include "bitmap.h"
#include "stm32f10x_adc.h"
#include "define.h"
#include "types.h"
#include "bacnet.h"
#include "store.h"
extern uint8 co2_autocal_disable;
extern uint8 co2_present;
extern int16 delta_temperature;
extern signed char Hum_T_calibration;
extern uint8 hum_sensor_type;
extern uint8 bacnet_detect;
extern uint8 bacnet_detect_timer;
extern float tem_org;
extern float hum_org;
extern uint8 voltage_overshoot;
extern uint16 rs485_p_voltage;
extern uint16 rs485_n_voltage;
extern uint16 aq_value;
extern int16 top_of_4to20ma;
extern int16 bottom_of_4to20ma;
extern uint16 tem_of_co2;
extern uint16 hum_of_co2;

extern uint8 frc_mode; //0:frc mode not start 1: frc mode start 
extern int16 temperature_org;
extern uint8 frc_pid2_config;
extern uint8 frc_output_select;
extern uint8 frc_output_config;
extern int16 frc_outdoorhum_enthalpy;
extern int16 frc_indoorhum_enthalpy;
void frc_mode_check(void);

extern STR_FRC_INPUT spacetem;
extern STR_FRC_INPUT supplytem;
extern STR_FRC_INPUT outdoortem;
extern STR_FRC_INPUT outdoorhum;
extern STR_FRC_INPUT indoorhum;

extern Str_in_point FRC_OUTDOORTEM;
extern Str_in_point FRC_INDOORHUM;
extern int16_t outside_tem;
extern uint16_t  light_sensor;
extern int16_t  co2_calibration_data;
extern uint8_t slope_type[2];
extern int16_t xdata analog_calibration[5];
extern uint8_t power_up_timer;
extern int16_t old_temperature;
extern int16_t mul_analog_cal[8];
//extern uint8_t mul_input_filter[10];
extern int16_t pre_mul_analog_input[10];
extern int16_t mul_analog_in_buffer[10];
extern int16_t mul_analog_input[10];
extern uint16_t co2_data;
extern int16_t humidity;									
extern int16_t temperature; // global temperature x 10, filtered
extern int16_t temperature_c; 
extern int16_t temperature_f; 
extern int16_t temperature_internal;
extern int16_t internal_calibration;
extern uint8_t xdata table1_flag;
extern uint8_t xdata table2_flag;
extern int16_t dew_data;
extern void CalTemperature(void);
extern void CalInput(void);
int16_t look_up_table(uint16_t count, int8_t type);
int16_t RangeConverter(uint8_t function, int16_t para,uint8_t i,int16_t cal);

void inputs_init(void) ;
void inpust_scan(void) ;
u16 ADC_getChannal(ADC_TypeDef* ADCx, u8 channal) ;
int16_t look_up_customtable(uint8 id,int16_t count,uint8_t slope,uint8 range );
int16_t Filter(uint8_t channel,signed input);
uint16_t Trans_ADC(uint16_t adc_value);

void frc_input(void);
void check_frc_config(void);
int16 get_4to20ma_value(uint16 current);

#ifdef TSTAT7_ARM
//extern vu16 AD_Value[MAX_AI_CHANNEL]; 
extern const uint8_t channel[MAX_AI_CHANNEL];
#endif
#endif































