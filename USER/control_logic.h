#include "types.h"
#include "define.h"
#include "eepdefine.h"
#include "output.h"
#define xdata 
#define idata
#define bdata
extern uint16 giPwmTimeOn[2];
extern uint8 pid_ctrl_bit[7];
extern uint8 pwm1_percent[2];
extern void  TimerSwitch(uint8 times);
extern void CalDesireTime( uint16 timevalue,uint8 units,uint8 type);
extern uint8  timer_changed_status;
extern uint8  mode_stage_changed[3];
extern uint8  lighting_stage;
extern uint16 time_counter;
extern uint8 timer_switch_status[2];
extern uint8 timer_enable;
extern uint8 output_floating_flag;
extern int16 valve[3];
//extern uint8 read_eeprom( uint16 addr );
//extern void write_eeprom( uint16 addr, uint8 ch );
extern uint16 COOLING_MODE;
extern uint16 HEATING_MODE;
extern uint8 freeze_delay_off;
extern uint16 xdata freeze_timer_counter;
extern uint8 stateswitchflag;
extern uint8	init_delay_flag;
extern uint16 xdata HEATING_LOCKOUT;
extern uint16 xdata COOLING_LOCKOUT;
extern uint8 xdata heating_lockout_time[3];
extern uint8 xdata cooling_lockout_time[3];
extern uint8 xdata heating_lock_10s_counter;
extern uint8 xdata cooling_lock_10s_counter;
extern uint16 xdata STATE_LOCK;
extern uint8 flash_or_eeprom ;
extern uint8 get_bit(uint16 c,uint8 pos) ;
extern void set_bit(uint16* c,uint8 pos,uint8 on_or_off) ;
//extern uint8 xdata temperature_filter_bypass;
//extern uint8 xdata pic_version;
//extern uint8 internal_thermistor;
extern uint8 occupied;
extern uint8 xdata fan_speed_user;
extern uint8 xdata pid_setpoint[3];
extern et_sequence_of_operation   sequence_of_operation; 
extern et_mode_of_operation  current_mode_of_operation[3];
//extern uint8 pulse_flag ;
//extern uint16  low_volts ; 
//extern uint16  high_volts ; 
//extern void start_timer( et_timer timer_no , uint8 timeout );
//extern void setpoint(void) ;
extern int16 idata temperature ; // global temperature x 10, filtered, 
extern int16 xdata analog_output[2] ; 
extern int16 xdata analog_input[4] ; 
extern int16 xdata mul_analog_input[10] ;
extern int16 xdata mul_analog_in_buffer[10];
extern int16 xdata pre_mul_analog_input[10]; //used to filter  readings
//extern uint8 bdata OUTPUT_1TO5 ;
extern PTBDSTR OUTPUT_1TO5;

extern PTBDSTR RELAY_1TO5;

extern PTBDSTR OFF_FIRST;

extern uint16 xdata cooling_db ;   
extern uint16 xdata heating_db ;
extern int16 idata loop_setpoint[3] ;

uint8 get_cureent_stage(void);
uint8 get_current_mode(void);




//extern int32 div( int32 dividend , int32 divisor );
int32 mul( int32 x1 , int32 x2 ) ;
//extern void watchdog( void );
void control_logic(void);  
//int8 FlashRead_Absolute(uint16 location, uint8 *value);
#define VALVE_CLOSED  	0  //00
#define PID1 			2  //01  valve opens in stage1 cooling or heating
#define PID2 			3  //10  valve opens in stage2 cooling or heating
#define VALVE_OPEN 		1  //11  valve is open
//use the MSB 4 bits at the same byte
#define PID3			4  //01  0---50% valve open












