#include "output.h"
#include "define.h"
#include "stm32f10x_gpio.h"
#include "types.h"
#include "TstatFunctions.h"
#include "eepdefine.h"
#include "inputs.h"
#include "stdlib.h"
#include "control_logic.h"
#include "bacnet.h"
#include "24cxx.h"
#include "modbus.h"
#include "usart.h"
#include "rs485.h"
//#include "user_data.h"
//uint8 output_floating_flag = 0;
//uint16_t AO_Present_Value[MAX_AOS][BACNET_MAX_PRIORITY];
//BACNET_BINARY_PV BO_Present_Value[MAX_BOS][BACNET_MAX_PRIORITY];

uint8 pwm_flag = 0;
uint16 pwm_duty = 700;
//uint8  output6_function = 0;
//uint8  output7_function = 0;
uint8 first_output = 0;
//uint16 current_temp = 0;
//uint16 crnt_crnt;
uint8 xdata relay_to_be_switched;
uint8 xdata dtime[7];
uint16 OUT_PINS_1TO5;//    	D5		D4  	D3		D2		D1			    hi			lo 
PTBDSTR OUTPUT_1TO5;
PTBDSTR RELAY_1TO5;
PTBDSTR OFF_FIRST ;//use for operating the relays ,the ON to OFF has the highest priority

void output_ini(void)
{
			//degital output initial	

	
//	GPIO_InitTypeDef OUTPUT_LED2;
//	GPIO_InitTypeDef OUTPUT_LED3;
//	GPIO_InitTypeDef OUTPUT_LED4;
//	GPIO_InitTypeDef OUTPUT_LED5;
//	GPIO_InitTypeDef OUTPUT_LED6;
//	GPIO_InitTypeDef OUTPUT_LED7;
//	
//	GPIO_InitTypeDef INPUT_LED1;
//	GPIO_InitTypeDef INPUT_LED2;
//	GPIO_InitTypeDef INPUT_LED3;
//	GPIO_InitTypeDef INPUT_LED4;
//	GPIO_InitTypeDef INPUT_LED5;
//	GPIO_InitTypeDef INPUT_LED6;
//	GPIO_InitTypeDef INPUT_LED7;

	
	GPIO_InitTypeDef OUTPUT_RELAY1;
	GPIO_InitTypeDef OUTPUT_RELAY2;
	GPIO_InitTypeDef OUTPUT_RELAY3;
	GPIO_InitTypeDef OUTPUT_RELAY4;
	GPIO_InitTypeDef OUTPUT_RELAY5;
	
	GPIO_InitTypeDef AO_GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef AO_TIM_TimeBaseStructure;
	TIM_OCInitTypeDef AO_TIM_OCInitStructure;	
//#ifdef TSTAT_OCC
//	if(HardwareVersion >= HW_VERSION)
//	{
	GPIO_InitTypeDef DI12_GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef DI12_TIM_TimeBaseStructure;
	TIM_OCInitTypeDef DI12_TIM_OCInitStructure;
	
	GPIO_InitTypeDef DI1234_GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef DI1234_TIM_TimeBaseStructure;
	TIM_OCInitTypeDef DI1234_TIM_OCInitStructure;		
	
	
	GPIO_InitTypeDef DI34_GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef DI34_TIM_TimeBaseStructure;
	TIM_OCInitTypeDef DI34_TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);//enable GPIO clock	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);//enable GPIO clock	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//enable GPIO clock	

#ifdef TSTAT7_ARM
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
#else
	if(HardwareVersion >= HW_VERSION)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
//relay 34
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	}
#endif//TSTAT7_ARM
	
	
#ifdef TSTAT7_ARM
	DI1234_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9;  //TIM3_CH1-4
#else	
	if(HardwareVersion >= HW_VERSION)
	{
	DI12_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 ;  //TIM3_CH1-4
	DI34_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 ;  //TIM3_CH1-4
	}

	else
	{
	DI1234_GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9;  //TIM3_CH1-4
	}
#endif// TSTAT7_ARM	
	
#ifdef TSTAT7_ARM
	DI1234_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	DI1234_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &DI1234_GPIO_InitStructure);
#else	
	if(HardwareVersion >= HW_VERSION)
	{
	DI12_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	DI12_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &DI12_GPIO_InitStructure);
	
	DI34_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	DI34_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &DI34_GPIO_InitStructure);
	}
	else
	{
	DI1234_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	DI1234_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &DI1234_GPIO_InitStructure);
	}
#endif//#ifdef TSTAT7_ARM	


#ifndef TSTAT7_ARM		
	if(HardwareVersion >= HW_VERSION)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
	}
#endif

#ifdef TSTAT7_ARM	
	DI1234_TIM_TimeBaseStructure.TIM_Period = 1000;
	DI1234_TIM_TimeBaseStructure.TIM_Prescaler = 2;
	DI1234_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	DI1234_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &DI1234_TIM_TimeBaseStructure);
		
	DI1234_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	DI1234_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	DI1234_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
#else	
	if(HardwareVersion >= HW_VERSION)
	{
	DI12_TIM_TimeBaseStructure.TIM_Period = 1000;
	DI12_TIM_TimeBaseStructure.TIM_Prescaler = 2;
	DI12_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	DI12_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &DI12_TIM_TimeBaseStructure);
		
	DI12_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	DI12_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	DI12_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	DI34_TIM_TimeBaseStructure.TIM_Period = 1000;
	DI34_TIM_TimeBaseStructure.TIM_Prescaler = 2;
	DI34_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	DI34_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &DI34_TIM_TimeBaseStructure);
		
	DI34_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	DI34_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	DI34_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;	
	}
	else
	{
	DI1234_TIM_TimeBaseStructure.TIM_Period = 1000;
	DI1234_TIM_TimeBaseStructure.TIM_Prescaler = 2;
	DI1234_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	DI1234_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &DI1234_TIM_TimeBaseStructure);
		
	DI1234_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	DI1234_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	DI1234_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	}
#endif//#ifdef TSTAT7_ARM	

#ifdef TSTAT7_ARM
	TIM_OC1Init(TIM8, &DI1234_TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &DI1234_TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &DI1234_TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &DI1234_TIM_OCInitStructure);
#else	
	if(HardwareVersion >= HW_VERSION)
	{
	TIM_OC1Init(TIM8, &DI12_TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &DI12_TIM_OCInitStructure);
	
	TIM_OC1Init(TIM3, &DI34_TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &DI34_TIM_OCInitStructure);
	}

	else
	{
	TIM_OC1Init(TIM8, &DI1234_TIM_OCInitStructure);
	TIM_OC2Init(TIM8, &DI1234_TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &DI1234_TIM_OCInitStructure);
	TIM_OC4Init(TIM8, &DI1234_TIM_OCInitStructure);
	}
#endif//#ifdef TSTAT7_ARM
	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	

#ifdef TSTAT7_ARM
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);	
#else	
	if(HardwareVersion >= HW_VERSION)
	{
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	}
	else
	{
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
	}
#endif// TSTAT7_ARM
	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE); 
//analog output initial	
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

#ifndef TSTAT7_ARM	
	if(HardwareVersion >= HW_VERSION)
	{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	}
#endif	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE); //TIM3_CH1->PC6, TIM3_CH2->PC7, TIM3_CH3->PC8, TIM3_CH4->PC9	
	AO_GPIO_InitStructure.GPIO_Pin =  (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);  //TIM3_CH1-4
	AO_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	AO_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &AO_GPIO_InitStructure);
	
	AO_TIM_TimeBaseStructure.TIM_Period = 1000;
	AO_TIM_TimeBaseStructure.TIM_Prescaler = 2;
	AO_TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	AO_TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &AO_TIM_TimeBaseStructure);
		
	AO_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	AO_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	AO_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OC1Init(TIM4, &AO_TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &AO_TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &AO_TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_Cmd(TIM4, ENABLE);
	
}


void set_output (TIM_TypeDef* TIMx, u8 channel, u16 duty)
{
	//uint8 i = 0;
	u16 compare_value = duty;
	// calculate the compare value and then set it to change the pwm duty
	
	switch(channel)
	{
		case 1:  
			TIM_SetCompare1(TIMx, compare_value);
		break;
		case 2:
			TIM_SetCompare2(TIMx, compare_value);
		break;
		case 3:
			TIM_SetCompare3(TIMx, compare_value);
		break;
		case 4:
			TIM_SetCompare4(TIMx, compare_value);
		break;
	}
}

void relay_ctl_pwm(uint8 relay_num, uint16 duty)
{
	switch(relay_num)
	{
		//#ifdef TSTAT_OCC
		

		case 3:
			#ifdef TSTAT7_ARM
			set_output(TIM8,3, duty);
		#else
			if(HardwareVersion >= HW_VERSION)
				set_output(TIM3,1, duty);
			else
				set_output(TIM8,3, duty);
			#endif
		break;
		
		case 4:
			#ifdef TSTAT7_ARM
				set_output(TIM8,4, duty);
			#else
			if(HardwareVersion >= HW_VERSION)
				set_output(TIM3,2, duty);
			else
				set_output(TIM8,4, duty);
			#endif
		break;

		

		case 1:
			set_output(TIM8,1, duty);
		break;

		case 2:
			set_output(TIM8,2, duty);
		break;

		case 5:
			set_output(TIM4,1, duty);
		break;

		default:
			break;
	}	
}
void refresh_output(void)
{
int16 ao_temp;
uint8 relay_temp = 0;	
uint8 i = 0;
	if(sequence_of_operation==TEST_MODE)	//SOP=0
	{
			if(valve[0]<200)
			{
				relay_ctl_pwm(1,1000);
				relay_ctl_pwm(2,0);
				relay_ctl_pwm(3,0);
				relay_ctl_pwm(4,0);
				relay_ctl_pwm(5,0);
			}
				//OUTPUT_1TO5 = 0x01;
			else if(valve[0]<400)
			{
				relay_ctl_pwm(1,0);
				relay_ctl_pwm(2,1000);
				relay_ctl_pwm(3,0);
				relay_ctl_pwm(4,0);
				relay_ctl_pwm(5,0);
			}
			else if(valve[0]<600)
			{
				relay_ctl_pwm(1,0);
				relay_ctl_pwm(2,0);
				relay_ctl_pwm(3,1000);
				relay_ctl_pwm(4,0);
				relay_ctl_pwm(5,0);
			}
			else if(valve[0]<800)
			{
				relay_ctl_pwm(1,0);
				relay_ctl_pwm(2,0);
				relay_ctl_pwm(3,0);
				relay_ctl_pwm(4,1000);
				relay_ctl_pwm(5,0);
			}
			else 
			{
				relay_ctl_pwm(1,0);
				relay_ctl_pwm(2,0);
				relay_ctl_pwm(3,0);
				relay_ctl_pwm(4,0);
				relay_ctl_pwm(5,1000);
			}
			
			set_output(TIM4,2, valve[0]);
			set_output(TIM4,3, valve[1]);	
	}
 
	else
	{		
		for(i=1;i<6;i++)	
		{		
			relay_routine(i);

			if (relay_to_be_switched)
			{
				if (relay_to_be_switched == 1 || OFF_FIRST.Bits.b0)
				{		 		 
						RELAY_1TO5.Bits.b0 = !OUTPUT_1TO5.Bits.b0;    
						OFF_FIRST.Bits.b0 = 0 ; 		
				}  
				else if (relay_to_be_switched == 2 || OFF_FIRST.Bits.b1)
				{
						RELAY_1TO5.Bits.b1 = !OUTPUT_1TO5.Bits.b1;
						OFF_FIRST.Bits.b1 = 0 ;
				}
				else if (relay_to_be_switched == 3 || OFF_FIRST.Bits.b2)
				{
						RELAY_1TO5.Bits.b2 = !OUTPUT_1TO5.Bits.b2;
						OFF_FIRST.Bits.b2 = 0 ;
					}
				else if (relay_to_be_switched == 4 || OFF_FIRST.Bits.b3)
				{
						RELAY_1TO5.Bits.b3 = !OUTPUT_1TO5.Bits.b3;
						OFF_FIRST.Bits.b3 = 0 ;
				}
				else if (relay_to_be_switched == 5 || OFF_FIRST.Bits.b4)
				{				
						RELAY_1TO5.Bits.b4 = !OUTPUT_1TO5.Bits.b4;
						OFF_FIRST.Bits.b4 = 0 ;
				}
				
				relay_to_be_switched = 0;	
			}	
		}

		
		
		
		if(Sys_Sequence == TRANSDUCER_MODE)//in transducer mode, relay1 is indicating occupied/unoccupied
		{
		 if(fan_speed_user == FAN_OFF)//unoccupied
		 {
		 	relay_ctl_pwm(1,0);
			pwm_flag &= 0xfe;	
		 } 
		 else
		 {
			if((pwm_flag&0x01) == 0x00)
				{
				relay_ctl_pwm(1,1000);
				pwm_flag |= 0x01; 	
				}
			else
				relay_ctl_pwm(1,pwm_duty);
		 }
		}
		else
		{	
			for(i=0;i<5;i++)
			{
				if(GetByteBit(&EEP_OutputManuEnable,i))   //USER SET
				{ 
					SetByteBit(&RELAY_1TO5.BYTE,i,!GetByteBit(&ManualRelayAll,i));//CC 2011/01/18 RELAY SET
				//	OUTPUT_1TO5 = read_eeprom(MANUAL_RELAY)&0x1f;	 
				}
				if(deadmaster_triggered == 1)
				{
					if(DeadMsater_AutoManual == 2)//deadmaster manual mode, outputs will follow demaster setting 
						SetByteBit(&RELAY_1TO5.BYTE,i,!GetByteBit(&DeadMsater_OutputState,i));//CC 2011/01/18 RELAY SET
					else if(DeadMsater_AutoManual == 1)//deadmaster auto mode, outputs will follow pid control
					{
						EEP_OutputManuEnable = 0;  //trun to auto mod
						write_eeprom(EEP_OUTPUT_MANU_ENABLE, 0);
						if(i == 4)
							deadmaster_triggered = 3;
					}
				}				
				
				
				
			}
			if(!RELAY_1TO5.Bits.b0)
					{
					if((pwm_flag&0x01) == 0x00)
						{
						relay_ctl_pwm(1,1000);
						pwm_flag |= 0x01; 	
						}
					else
						relay_ctl_pwm(1,pwm_duty);
					RELAY1_LED_ON					
					}
			else
					{
					relay_ctl_pwm(1,0);
					pwm_flag &= 0xfe;	
					RELAY1_LED_OFF
					}
		}		
				
				
		if(!RELAY_1TO5.Bits.b1)
				{
				if((pwm_flag&0x02) == 0x00)
					{
					relay_ctl_pwm(2,1000);
					pwm_flag |= 0x02; 	
					}
				else
					relay_ctl_pwm(2,pwm_duty);	

				RELAY2_LED_ON				
				}
		else
				{
				relay_ctl_pwm(2,0);
				pwm_flag &= 0xfd;	
					RELAY2_LED_OFF
				}

		if(!RELAY_1TO5.Bits.b2)
				{
				if((pwm_flag&0x04) == 0x00)
					{
					relay_ctl_pwm(3,1000);
					pwm_flag |= 0x04; 	
					}
				else
					relay_ctl_pwm(3,pwm_duty);
				RELAY3_LED_ON				
				}
		else
				{
				relay_ctl_pwm(3,0);
				pwm_flag &= 0xfb;
				RELAY3_LED_OFF					
				}
				
		if(!RELAY_1TO5.Bits.b3)
				{
				if((pwm_flag&0x08) == 0x00)
					{
					relay_ctl_pwm(4,1000);
					pwm_flag |= 0x08; 	
					}
				else
					relay_ctl_pwm(4,pwm_duty);
				RELAY4_LED_ON				
				}
		else
				{
				relay_ctl_pwm(4,0);
				pwm_flag &= 0xf7;
				RELAY4_LED_OFF						
				}
				
		if(!RELAY_1TO5.Bits.b4)
				{
				if((pwm_flag&0x10) == 0x00)
					{
					relay_ctl_pwm(5,1000);
					pwm_flag |= 0x10; 	
					}
				else
					relay_ctl_pwm(5,pwm_duty);
				RELAY5_LED_ON					
				}
		else
				{
				relay_ctl_pwm(5,0);
				pwm_flag &= 0xef;	
				RELAY5_LED_OFF	
				}

		if(deadmaster_triggered)
		{
			if(DeadMsater_AutoManual == 2)//deadmaster manual mode, outputs will follow demaster setting 
			{
				SetByteBit(&EEP_OutputManuEnable,5,1);
				SetByteBit(&EEP_OutputManuEnable,6,1);
			}		
		}

			if(GetByteBit(&EEP_OutputManuEnable, 5))//analog output1 manual mode 
				{
					if((deadmaster_triggered) && (DeadMsater_AutoManual == 2))
					{
						ao_temp = DeadMsater_CoolOutput_HI ;
						ao_temp *= 256;
						ao_temp += DeadMsater_CoolOutput_LO ;
					
					}
					else
					{
						ao_temp = ManualAO1_HI;
						ao_temp *= 256;
						ao_temp += ManualAO1_LO;
						
					}	
					
					if(EEP_Output1Scale == OUTPUT_ANALOG_RANGE_ONOFF)
					{
						#ifdef TSTAT8_HUNTER
							 if(ao_temp == 1)
							 {
								 set_output(TIM4,2, 0);
								 DO1_LED_ON
							 }
							 else
							 {
								 set_output(TIM4,2, 1000);
								 DO1_LED_OFF	
							 }

						#else
							if(ao_temp == 1)
								set_output(TIM4,2, 1000);
							else
								set_output(TIM4,2,0);
						#endif
					}
					else
						set_output(TIM4,2, ao_temp);						
				}
			else
				{
				switch(EEP_Output1Scale)
					{	
						
					case OUTPUT_ANALOG_RANGE_ONOFF:
					#ifdef TSTAT8_HUNTER
							if(OUTPUT_1TO5.Bits.b5)  //output on /high
								{
								relay_6(RELAY_OFF);
								DO1_LED_ON	
								}
							else
								{
								relay_6(RELAY_ON);
								DO1_LED_OFF	
								}
						
							
						#else
						
							if(OUTPUT_1TO5.Bits.b5)  //output on /high
								{
								relay_6(RELAY_ON);
								}
							else
								{
								relay_6(RELAY_OFF);
								}
						#endif
					break;
					
					case OUTPUT_RANGE_TRANSDUCER:
					case OUTPUT_ANALOG_RANGE_0TO10V: 
						 if(FrcEnable == ENABLE_FRC)
						 {
							if(valve[0] < FrcMinFreshAir*10)
								valve[0] = FrcMinFreshAir*10;
						 }
						set_output(TIM4,2, valve[0]);
					break;
						
			//		case OUTPUT_ANALOG_RANGE_PWM:
			//				//not use 
			//		break;
					
					default:
						set_output(TIM4,2, valve[0]);
					break;	
					
					}
				}
		
			if(GetByteBit(&EEP_OutputManuEnable, 6))//analog output2 manua2 mode 
			{
				if((deadmaster_triggered) && (DeadMsater_AutoManual == 2))
					{
						ao_temp = DeadMsater_HeatOutput_LO;
						ao_temp *= 256;
						ao_temp += DeadMsater_HeatOutput_LO;
					}
					else
					{
						ao_temp = ManualAO2_HI;
						ao_temp *= 256;
						ao_temp += ManualAO2_LO;								
					}
					
					if(EEP_Output2Scale == OUTPUT_ANALOG_RANGE_ONOFF)
					{
						#ifdef TSTAT8_HUNTER
							 if(ao_temp == 1)
							 {
								 set_output(TIM4,3, 0);
								 DO2_LED_ON
							 }
							 else
							 {
								 set_output(TIM4,3, 1000);
								 DO2_LED_OFF
							 }
						
						#else
							if(ao_temp == 1) 
								set_output(TIM4,3, 1000);
							else
								set_output(TIM4,3,0);						
						#endif
					}
					else
						set_output(TIM4,3, ao_temp);
			}
			else
			{
			switch(EEP_Output2Scale)
				{
				case OUTPUT_ANALOG_RANGE_ONOFF:
					#ifdef TSTAT8_HUNTER
						if(OUTPUT_1TO5.Bits.b6)  //output on /high
						{
							relay_7(RELAY_OFF );
							DO2_LED_ON
						}
						else
						{
							relay_7(RELAY_ON);
							DO2_LED_OFF
						}					
					
					#else
											
						if(OUTPUT_1TO5.Bits.b6)  //output on /high
						{
							relay_7(RELAY_ON);
						}
						else
						{
							relay_7(RELAY_OFF);
						}
					#endif
				break;

				case OUTPUT_RANGE_TRANSDUCER:
				case OUTPUT_ANALOG_RANGE_0TO10V:
					

					set_output(TIM4,3, valve[1]);		

				break;
				
				case OUTPUT_ANALOG_RANGE_PWM:
					
				break;

				default:
					set_output(TIM4,3, valve[1]);
				break;	
				
				}
			}
		

	
	}		
}


void relay_routine(uint8 relay_number)   //relay number from 1 to 7 according to relay 1 to relay5 and analog1 and analog2
{		      //soon we will create more versions of the tstat, some analog, some digital, some analog+digital, some universal (changeable by the customer)
 	uint8 state_bit;//,delay_enable_bit=0;  //need check this :  delay_enable_bit
 	uint8 OUTX_TEMP,RELAYX_TEMP;         //OUTX_TEMP IS THE TEMPORARY OF OUTX, RELAYX_TEMP IS THE TEMPORARY OF RELAYX
 	uint8 OFF_OUTX_TEMP;                 //OFF_OUTX_TEMP IS THE TEMPORARY OF OUTX
	uint8 OUT_PINS_1TO5_BIT_L;           //OUT_PINS_1TO5_BIT IS THE TEMPORARY OF OUT_PINS_1TO5 BITS, _H = 1 MEANS IF NEED TO CHANGE, _L MEANS CURRENT STATE
 	uint8 OUT_PINS_1TO5_BIT_H;            //RELAY1-RELAY5 CORRESPONDING TO 0,2,4,6,8
	uint8  relay_timer_number;
 	

 	  switch(relay_number)
 	  {
//				  relay_timer_number=0; //timer numbers start at 0 , and relay number starts at 1
// 	        OUTX_TEMP  =OUT1;
// 	        OFF_OUTX_TEMP = OFF_OUT1;
// 	        RELAYX_TEMP = RELAY1;
// 	        OUT_PINS_1TO5_BIT_L = 0; 
//					OUT_PINS_1TO5_BIT_H = 1; 
 	   case 1 :   //relay1 routine
	        relay_timer_number=0; //timer numbers start at 0 , and relay number starts at 1
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b0;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b0;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b0;
 	        OUT_PINS_1TO5_BIT_L = 0; 
			OUT_PINS_1TO5_BIT_H = 1; 
			break;
 	   case 2 :   //relay2 routine
	        relay_timer_number=1;
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b1;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b1;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b1;
 	        OUT_PINS_1TO5_BIT_L = 2; 
			OUT_PINS_1TO5_BIT_H = 3; 
			break;
 	   case 3 :   //relay3 routine
	        relay_timer_number=2;
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b2;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b2;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b2;
 	        OUT_PINS_1TO5_BIT_L = 4;
			OUT_PINS_1TO5_BIT_H = 5; 
			break;
 	   case 4 :   //relay4 routine
	        relay_timer_number=3;
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b3;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b3;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b3;
 	        OUT_PINS_1TO5_BIT_L = 6;
			OUT_PINS_1TO5_BIT_H = 7;
			break;
 	   case 5 :   //relay5 routine
	        relay_timer_number=4;
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b4;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b4;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b4;
 	        OUT_PINS_1TO5_BIT_L = 8;
			OUT_PINS_1TO5_BIT_H = 9;
			break;

	  case 6:
	  		relay_timer_number=5;
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b5;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b5;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b5;
 	        OUT_PINS_1TO5_BIT_L = 10;
			OUT_PINS_1TO5_BIT_H = 11;
			break;

	  case 7:
	  		relay_timer_number=6;
 	        OUTX_TEMP  = OUTPUT_1TO5.Bits.b6;
 	        OFF_OUTX_TEMP = OFF_FIRST.Bits.b6;
 	        RELAYX_TEMP = RELAY_1TO5.Bits.b6;
 	        OUT_PINS_1TO5_BIT_L = 12;
			OUT_PINS_1TO5_BIT_H = 13;
			break;

		default:
		break;
 	  }
 	
 	
	if(RELAYX_TEMP == OUTX_TEMP) // only do something if the relay does not match the output register
	{
		state_bit = get_bit(OUT_PINS_1TO5,OUT_PINS_1TO5_BIT_L);	// check bool 0 and store in bool 1
		
		// only do something if the old state bool does not match the new state
		// i.e. only if the state has changed.
		if((!OUTX_TEMP) != state_bit)	
			{
			set_bit(&OUT_PINS_1TO5,OUT_PINS_1TO5_BIT_L,!OUTX_TEMP); // set state bool to match !OUT1

			if( state_bit )   // OFF to ON transition
			  { 
			   dtime[relay_timer_number] = Output_Delay_OfftoOn(relay_timer_number);//read_eeprom(EEP_OUTPUT_DELAY_OFF_TO_ON + relay_timer_number);
//			   relay_on_or_off_check_flag[relay_timer_number] = RELAY_OFF_TO_ON;	
			  }
			else // ON to OFF transition
			  {		
				dtime[relay_timer_number] = Output_Delay_OntoOff(relay_timer_number);//read_eeprom(EEP_OUTPUT_DELAY_ON_TO_OFF + relay_timer_number);  //TBD :need translte this to avariable
//				relay_on_or_off_check_flag[relay_timer_number] = RELAY_ON_TO_OFF;
			  }
				
			if(dtime[relay_timer_number] > 0)  // if the delay is active, set the delay bool and start the timer
				{
				set_bit(&OUT_PINS_1TO5,OUT_PINS_1TO5_BIT_H,RELAY_ON); //TBD : check if ON is actualy 1, we assumed it is here
//				start_timer(OUTPUT1_TIMER + relay_timer_number , DEFAULT_TIMEOUT); //timer start of the table is output1 , no common start point,n need define
				}

			}
		
		// if the delay is off, set the relay
		if (!get_bit(OUT_PINS_1TO5,OUT_PINS_1TO5_BIT_H))
		{
			relay_to_be_switched = relay_number ;

			if(!RELAYX_TEMP)
			OFF_OUTX_TEMP = RELAY_ON ;
		 

      switch (relay_number)
      {
      case 1: 	
      	OFF_FIRST.Bits.b0 = OFF_OUTX_TEMP;
		break;
      case 2: 	
      	OFF_FIRST.Bits.b1 = OFF_OUTX_TEMP;
		break;
      case 3: 	
      	OFF_FIRST.Bits.b2 = OFF_OUTX_TEMP;
		break;
      case 4: 	
      	OFF_FIRST.Bits.b3 = OFF_OUTX_TEMP;
		break;
      case 5: 	
      	OFF_FIRST.Bits.b4 = OFF_OUTX_TEMP;      	      	      	      	
		break;
	  case 6: 	
      	OFF_FIRST.Bits.b5 = OFF_OUTX_TEMP;      	      	      	      	
		break;
	  case 7: 	
      	OFF_FIRST.Bits.b6 = OFF_OUTX_TEMP;      	      	      	      	
		break;
		
	  default:
	  break;	
      }
 
		}
	}					
}

//void wirte_bacnet_value_to_buf(uint8_t type,uint8_t priority,uint8_t i)
//{
//switch(type)
//{
//	case AV:

//	break;
//	
//	case AI:
//		if((i<9)&&(i>0))//AI1-AI8
//		{
//			if(GetByteBit(&EEP_InputManuEnable,i))//manual mode
//			{
//				write_eeprom(MANUAL_ANALOG1_HI, (AI_Present_Value[i]>>8)&0xff);
//				write_eeprom(MANUAL_ANALOG1_LO, (AI_Present_Value[i]&0xff));
//				ManualAI_HI(i-1) = (AI_Present_Value[i]>>8) & 0xff;	
//				ManualAI_LO(i-1) =  AI_Present_Value[i] & 0xff;				
//			}
//			else
//			{
//				mul_analog_cal[i-1] =  AI_Present_Value[i] - mul_analog_input[i-1];
//				write_eeprom(EEP_CALIBRATION_ANALOG1 + (i-1)*2, mul_analog_cal[i-1]&0xFF );
//				write_eeprom(EEP_CALIBRATION_ANALOG1 + (i-1)*2 + 1, (mul_analog_cal[i-1]>> 8) & 0xff );
//			}
//		}
//		else if(i==0)//internal temperature
//		{
//				if(GetByteBit(&b.eeprom[EEP_INTERNAL_SENSOR_MANUAL],0))
//				{
//					write_eeprom(MANUAL_INTERNAL_HI, (AI_Present_Value[i]>>8)&0xff);
//					write_eeprom(MANUAL_INTERNAL_LO, AI_Present_Value[i]&0xff);
//				}
//				else
//				{
//					if((EEP_TempSelect == THERMISTER_TYPE2)||(EEP_TempSelect == THERMISTER_TYPE3))
//					{
//						internal_calibration = AI_Present_Value[i] - temperature_internal;
//						write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR , internal_calibration&0xFF ) ;
//						write_eeprom( EEP_CALIBRATION_INTERNAL_THERMISTOR + 1, internal_calibration>>8 ) ;
//						Calibration_Internal_LO = internal_calibration&0xFF ;
//						Calibration_Internal_HI = internal_calibration>>8; 
//					}
//				}		
//		}
//		else if(i == 9)//humidity
//		{
//			;
//		}
//		else if(i == 10)
//		{
//			;
//		}

//		else
//		{
//			;
//		}
//	break;
//		
//	case AO:
//		if(priority < 8)
//		{
//			if(i==0)
//			{
//				ManualAO1_HI = AO_Present_Value[0][7] >> 8 & 0xff;
//				ManualAO1_LO = AO_Present_Value[0][7] & 0xff;					
//				write_eeprom(MANUAL_COOL_VALVE_HI,ManualAO1_HI);				
//				write_eeprom(MANUAL_COOL_VALVE_LO,ManualAO1_LO);	
//			}
//			if(i==1)
//			{
//				ManualAO2_HI = AO_Present_Value[1][7] >> 8 & 0xff;
//				ManualAO2_LO = AO_Present_Value[1][7] & 0xff;					
//				write_eeprom(MANUAL_HEAT_VALVE_HI,ManualAO2_HI);				
//				write_eeprom(MANUAL_HEAT_VALVE_LO,ManualAO2_LO);	
//			}
//		}
//	break;

//	case BO:
//			ManualRelay(i) = (uint8)BO_Present_Value[i][7];
//			write_eeprom(EEP_MANU_RELAY1 + i, ManualRelay(i));	
//	break;

//	default:
//	break;		
//}

//}


void relay_6(uint8 state)
{
	if(state)  //output on /high
		{
		set_output(TIM4,2, 1000);
//		AO_Present_Value[0][15] = 1000;	
		}
	else
		{
		set_output(TIM4,2, 0); //output off/low	
//		AO_Present_Value[0][15] = 0;	
		}

}

void relay_7(uint8 state)
{
	if(state)  //output on /high
	{
		set_output(TIM4,3, 1000);
//		AO_Present_Value[1][15] = 1000;
	}
	else
	{
		set_output(TIM4,3, 0); //output off/low	
//		AO_Present_Value[1][15] = 0;
	}
}




