#include "key.h"
#include "delay.h"
#include "stdio.h"
#include "gpio_define.h"
#include "eepdefine.h"
#include "TstatFunctions.h"
#include "24cxx.h"
#include "output.h"
#include "lcd.h"
#include "define.h"
#ifdef TSTAT7_ARM
#include "tstat7_menu.h"
#endif
#include "setpoint.h"
#include "eepdefine.h"
extern uint8 scheduleoff;
uint8 item_menu_timer = 0; 
uint8 item_menu = 0;
uint8 current_item = 0;
uint8 prev_keypad_state;
uint8 setpoint_adjust_flag = 0; 
uint8 down_key = 0;
uint8 keypad_value;
uint16 xdata lastkey_counter = 0;
uint8 key_pressed;
uint8 xdata keypad_count ;	//winter menu control
PTBDSTR KEY_TEMP;
uint8 menu_id;
uint8 pre_menu_id = 0;
void keypad_ini(void)
{
//GPIO_InitTypeDef GPIO_KEYPAD_HIGH;
GPIO_InitTypeDef GPIO_KEY1; 
GPIO_InitTypeDef GPIO_KEY2; 
GPIO_InitTypeDef GPIO_KEY3; 
GPIO_InitTypeDef GPIO_KEY4; 
#ifdef TSTAT7_ARM
	GPIO_InitTypeDef GPIO_KEY5; 
	GPIO_InitTypeDef GPIO_KEY6; 
#endif	
	
//GPIO_KEYPAD_HIGH.GPIO_Pin = gpio_map[KEYPAD_HIGH].GPIO_Pin_X;
//GPIO_KEYPAD_HIGH.GPIO_Mode = GPIO_Mode_Out_PP;
//GPIO_KEYPAD_HIGH.GPIO_Speed = GPIO_Speed_50MHz;
//GPIO_Init(gpio_map[KEYPAD_HIGH].GPIOX, &GPIO_KEYPAD_HIGH);

GPIO_KEY1.GPIO_Pin = gpio_map[KEY1].GPIO_Pin_X;
GPIO_KEY1.GPIO_Mode = GPIO_Mode_IPU;
GPIO_KEY1.GPIO_Speed = GPIO_Speed_2MHz;	 	
GPIO_Init(gpio_map[KEY1].GPIOX, &GPIO_KEY1);

GPIO_KEY2.GPIO_Pin = gpio_map[KEY2].GPIO_Pin_X;
GPIO_KEY2.GPIO_Mode = GPIO_Mode_IPU;
GPIO_KEY2.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(gpio_map[KEY2].GPIOX, &GPIO_KEY2);

GPIO_KEY3.GPIO_Pin = gpio_map[KEY3].GPIO_Pin_X;
GPIO_KEY3.GPIO_Mode = GPIO_Mode_IPU; 	
GPIO_KEY3.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(gpio_map[KEY3].GPIOX, &GPIO_KEY3);

GPIO_KEY4.GPIO_Pin = gpio_map[KEY4].GPIO_Pin_X;
GPIO_KEY4.GPIO_Mode = GPIO_Mode_IPU; 
GPIO_KEY4.GPIO_Speed = GPIO_Speed_2MHz;	
GPIO_Init(gpio_map[KEY4].GPIOX, &GPIO_KEY4);
#ifdef TSTAT7_ARM
		GPIO_KEY5.GPIO_Pin = gpio_map[KEY5].GPIO_Pin_X;
		GPIO_KEY5.GPIO_Mode = GPIO_Mode_IPU; 
		GPIO_KEY5.GPIO_Speed = GPIO_Speed_2MHz;	
		GPIO_Init(gpio_map[KEY5].GPIOX, &GPIO_KEY5);
		
		GPIO_KEY6.GPIO_Pin = gpio_map[KEY6].GPIO_Pin_X;
		GPIO_KEY6.GPIO_Mode = GPIO_Mode_IPU; 
		GPIO_KEY6.GPIO_Speed = GPIO_Speed_2MHz;	
		GPIO_Init(gpio_map[KEY6].GPIOX, &GPIO_KEY6);
#endif

}

//static void WRITE_KEYPAD_HIGH(u8 status)
//{
//	if(status) 
//		GPIO_SetBits(gpio_map[KEYPAD_HIGH].GPIOX, gpio_map[KEYPAD_HIGH].GPIO_Pin_X);
//	else
//		GPIO_ResetBits(gpio_map[KEYPAD_HIGH].GPIOX, gpio_map[KEYPAD_HIGH].GPIO_Pin_X); 
//}

static u8 READ_KEY()
{
	u8 status = 1;
//	uint8 keypad_temp = 0;
	status = GPIO_ReadInputDataBit(gpio_map[KEY1].GPIOX, gpio_map[KEY1].GPIO_Pin_X);
	if(status == LOW_LEVEL)
			KEY_TEMP.Bits.b2 = 1;//S3
	else
			KEY_TEMP.Bits.b2 = 0;//S3

	status = GPIO_ReadInputDataBit(gpio_map[KEY2].GPIOX, gpio_map[KEY2].GPIO_Pin_X);
	if(status == LOW_LEVEL)
			KEY_TEMP.Bits.b1 = 1; //S2
	else
			KEY_TEMP.Bits.b1 = 0; //S2

	status = GPIO_ReadInputDataBit(gpio_map[KEY3].GPIOX, gpio_map[KEY3].GPIO_Pin_X);
	if(status == LOW_LEVEL)
			KEY_TEMP.Bits.b0 = 1; //S1
	else
			KEY_TEMP.Bits.b0 = 0; //S1

	status = GPIO_ReadInputDataBit(gpio_map[KEY4].GPIOX, gpio_map[KEY4].GPIO_Pin_X);
	if(status == LOW_LEVEL)
			KEY_TEMP.Bits.b3 = 1;//S4
	else
			KEY_TEMP.Bits.b3 = 0;//S4

#ifdef TSTAT7_ARM
	status = GPIO_ReadInputDataBit(gpio_map[KEY5].GPIOX, gpio_map[KEY5].GPIO_Pin_X);
	if(status == LOW_LEVEL)
			KEY_TEMP.Bits.b4 = 1;// 
	else
			KEY_TEMP.Bits.b4 = 0;// 
	
	status = GPIO_ReadInputDataBit(gpio_map[KEY6].GPIOX, gpio_map[KEY6].GPIO_Pin_X);
	if(status == LOW_LEVEL)
			KEY_TEMP.Bits.b5 = 1;// 
	else
			KEY_TEMP.Bits.b5 = 0;// 
#endif
	return KEY_TEMP.BYTE;
}

void keypad_handler( void )		//
{
	//keypad_latch located at P3.7 , 
	//  -read keypad at P0 when latch is active low
	//  -then put latch back high to go to high impedance state	
//	uint8 temp_bit;
	uint8 keypad_state = 0;
//	uint8 i;	
	
	//KEYPAD_HIGH = 0; 
//	WRITE_KEYPAD_HIGH(LOW_LEVEL);
	//	P0 = 0xff ;
	delay_us(1); 	 			
	//	keypad_state = ~P0 ;	//save the kaypad state	to avoid repeating keypresses	
	keypad_state = READ_KEY();
	
	//	KEYPAD_HIGH = 1; 
//	WRITE_KEYPAD_HIGH(HIGH_LEVEL);
	 
#ifdef OCC_TEST
	if(((prev_keypad_state ^keypad_state) &	keypad_state) && (pir_end == 0))
#else
	if(((prev_keypad_state ^keypad_state) &	keypad_state) && (EEP_SpecialMenuLock !=3))
#endif
	{
		down_key = 1;
		menu_timer = 0;
	 	lcd_turn_off_timer = LCDTurnOff;
		lcd_turn_off_timer *=60;	
		if(lcd_turn_off_timer > 0)
			#ifdef TSTAT7_ARM
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);	
			#else
			GPIO_SetBits(GPIOA, GPIO_Pin_0);	//backlight_ctrl(BK_ON);
			#endif
	} 
	if((down_key == 1) && (prev_keypad_state ^	keypad_state) &	prev_keypad_state && (EEP_AutoOnly != 2))	//one event for	each button hit
		{
			down_key = 0;
			dis6 = 1;
			dis6_display = 100;
			keypad_value = prev_keypad_state;

      display_blink_flag = 0;
#ifdef TSTAT7_ARM
			Keypad_Receive(prev_keypad_state);
#else
			if(menu_mode == 1)//if it is in menu mode, redefine key functions
			{
				if(((pre_menu_id == INCREASE_PARAMETER)||(pre_menu_id == DECREASE_PARAMETER)) 
				&&((prev_keypad_state == SYS_MODE_CODE)||(prev_keypad_state == FAN_MODE_CODE)))
					{
					menu_id = ACCEPT_PARAMETER;
					pre_menu_id = 0;
					}
				else
					{
					switch(prev_keypad_state)
						{
						case COOL_DOWN_CODE:
							menu_id = DECREASE_PARAMETER;
						break;
						
						case COOL_UP_CODE:
							menu_id = INCREASE_PARAMETER;
							break;

						//case FAN_DOWN_CODE:
						case FAN_MODE_CODE:						
							menu_id = START_PREVIOUS_MENU;
							break;
				
						case SYS_MODE_CODE://lower right button 
							menu_id = START_NEXT_MENU;
							break;	
						}	
					pre_menu_id = menu_id;	
					}
			
			}
			else
			{
				if(item_menu == 0) //first time in to item menu					
				{
					switch(prev_keypad_state)
						{
						case COOL_DOWN_CODE:
						//menu_id = DECREASE_COOLING_SETPOINT;
						break;
					
						case COOL_UP_CODE:
						//menu_id = INCREASE_COOLING_SETPOINT;
						break;

						case FAN_MODE_CODE:												
						//menu_id = INCREASE_FAN_SPEED;//DECREASE_FAN_SPEED;
						menu_id = SELECT_ITEM_UP;
						item_menu = 1;
						current_item = 1;
						item_menu_timer = 5;
						break;
				
						case SYS_MODE_CODE://lower right button 
						//menu_id = INCREASE_SYS_MODE;
						menu_id = SELECT_ITEM_DOWN;
						item_menu = 1;
						current_item = 1;
						item_menu_timer = 5;
						break;	
						} //switch keypad_state
					}
				else
				{
					switch(prev_keypad_state)
						{
						case COOL_DOWN_CODE:
							if(current_item == 1)
								menu_id = DECREASE_COOLING_SETPOINT;
							else if(current_item == 2)
								menu_id = T8_DECREASE_FAN_SPEED;
							else if(current_item == 3)
								menu_id = DECREASE_SYS_MODE;
							
							item_menu_timer = 5;
						break;
					
						case COOL_UP_CODE:
							if(current_item == 1)
								menu_id = INCREASE_COOLING_SETPOINT;
							else if(current_item == 2)
								menu_id = INCREASE_FAN_SPEED;
							else if(current_item == 3)
								menu_id = INCREASE_SYS_MODE;
							
							item_menu_timer = 5;
						break;

						case FAN_MODE_CODE:												
						//menu_id = INCREASE_FAN_SPEED;//DECREASE_FAN_SPEED;
						menu_id = SELECT_ITEM_UP;
						item_menu = 1;
						if(current_item < 3)
							current_item++;
						else
							current_item = 1;
						
						item_menu_timer = 5;
						break;
				
						case SYS_MODE_CODE://lower right button 
						//menu_id = INCREASE_SYS_MODE;
						menu_id = SELECT_ITEM_DOWN;
						item_menu = 1;
						if(current_item >  1)
							current_item--;
						else
							current_item = 3;
						
						item_menu_timer = 5;
						break;	
						} //switch keypad_state
				}
				
					
			// MDF 12/06/04 Added Ort - OVERRIDE_TIMER to allow user to override the unoccupied mode of the clock
			// If any button has been pressed, check to see if the ORT should be turned on
			// if the tstat is unoccupied, the override timer should be turned on
			}
			if(prev_keypad_state == FAN_MODE_CODE || prev_keypad_state == SYS_MODE_CODE || prev_keypad_state == COOL_DOWN_CODE || prev_keypad_state == COOL_UP_CODE || EEP_KeypadSelect == 0)
			{
			//if current fan mode is OFF when any key is pressed,start override timer
				scheduleoff = 0;
//				if (!occupied)
//					{
//					if(EEP_OverrideTimer)
//						{
//						occupied = 1;
//						setpoint_select = DHOME_SP;	
//						info_byte |= 0x01;//
//						write_eeprom(EEP_INFO_BYTE,info_byte);
//						override_timer_time = (uint32)60 * EEP_OverrideTimer;
////						fan_speed_user = FAN_AUTO;
////						EEP_FanSpeed = FAN_AUTO;
////						write_eeprom(EEP_FAN_SPEED,FAN_AUTO);						
//						}
////					if(prev_keypad_state == FAN_MODE_CODE)
////						menu_id = 0; //if current mode is fan off, do not deal with the key, just switch to auto mode 
//					}
			
				key_pressed = 1;
				lastkey_counter = 1;//reset the timer if any key is pressed
				one_minute_counter  = 0;
			}
			 
		
#endif		
		
		} //if((prev_keypad_state ^	keypad_state) &	keypad_state)
		prev_keypad_state = keypad_state;	
		
		if((keypad_state == MENU_CODE)&&(!menu_mode) && (EEP_SpecialMenuLock != 1) && (EEP_SpecialMenuLock != 3))				//winter judge menu delay
			{                
			if(keypad_count++ > 30) 
				{ //if press two menu keypad, push menu after three seconds 
				//push_event(MENU_KEY);
				menu_id = START_MENU_MODE;
				menu_mode = 1;	
									
				}	
			}			
		else
			keypad_count = 0 ;

} //-----------end of keypad handler ---------------


