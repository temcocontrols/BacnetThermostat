#include "types.h"
#include "define.h"
#include "Constcode.h"

#define uint16		unsigned short int
#define uint8		unsigned char
#define uint32		unsigned long
	
#define int16		signed short int
#define int8		signed char
#define int32		signed long
	
#ifdef TSTAT_ZIGBEE
extern uint8 read_zgb_index;
extern uint8 zigbee_rssi;
extern uint8 zigbee_index;
extern uint8 zigbee_point_info[48];
#endif

extern uint16_t flash_buf[2];
extern uint8 icon_flag[9];
extern uint8 deadmaster_triggered;
extern uint16 deadmaster_timer;
extern uint8 pre_setpoint_select;
extern uint8 setpoint_select;
extern uint8 pre_mode;
extern uint8 fanspeedbuf;
extern uint8 cooltest;
extern uint8 out6buf;
extern int16 setpoint_buf;
extern uint8  buffer_occupied;
extern uint8  back_buffer_occupied;
extern uint8 update_flag;
extern uint8 outsidetem_health;
extern uint16_t outsidetem_timer;
extern uint8 outside_tem_flag;
extern uint8 fan_name_check;
extern uint8 blink_flag;
extern uint8 sp_blink_timer;
extern uint8 fan_blink_timer;
extern uint8 hc_blink_timer;
extern uint16_t lcd_turn_off_timer;
extern void SoftReset(void);
extern void save_user_def(void);
//extern uint8 manual_icon_control;
extern uint8  disp_item_queue[15];
extern int16_t eep_get_value(uint8 hibyte, uint8 lobyte);
extern uint16 get_day_of_year( void );


extern int16_t pir_timer ;
extern uint8 menu_addi;
#ifndef TSTAT7_ARM
extern uint16_t item_to_adjust;
#endif
extern uint8 xdata analog_output_type;
//extern uint8 xdata id_write_protect;
extern uint8 menu_timer;
extern uint8 time_data[9];
extern uint8 xdata demand_response;  //0:function disabled 1: NO  2:YES 
extern uint8 occupied; 
extern uint8 fan_speed_user;
extern uint8 backup_fan_speed; 
extern uint8 override_timer;
//extern uint8 show_id_state;
extern uint16_t xdata show_id_cnt;
//extern uint8 blink_flag;
extern uint8 dis6_display;
extern uint8 dis6;
extern uint8 display_blink_flag; 
extern uint8 gbDisplayChanged;
extern uint8 override_timer_flag;
extern uint8 occupancy;
extern uint32 override_timer_time;
extern uint8 xdata one_minute_counter;
extern uint8 menu_mode;
extern uint8 part_lock;
extern uint8 menu_addi;
extern int16_t humidity_calibration;
extern int16_t org_humidity;
extern int16_t htu_temp;

uint8 get_bit(uint16 c,uint8 pos);
void SetByteBit(uint8* c,uint8 pos,uint8 vb);  
uint8 GetByteBit(uint8* c,uint8 pos);
void InitialRAM(void);
void initialize_eeprom(void);
void ConverteCF(void);
void refresh_setpoint(uint8 day_night);


int32 mul( signed	long x1	, int32 x2 );
void set_bit(uint16_t* c,uint8 pos, uint8 on_or_off);	
void init_extern_operation_eeprom(uint8 sop_num);
void init_valve_operation_eeprom(void);
void init_output_delay_eeprom(void);
void init_table_custom_eeprom(void);
void init_universal_db_eeprom(void);
void init_pwm_operation_eeprom(void);

#ifndef TSTAT7_ARM
void decrease_cooling_setpoint(void) ;
//void decrease_fan_speed(void) ;
void increase_sysmode(void);
void decrease_parameter(void ) ;
void exit_menu_mode(void) ;
void increase_cooling_setpoint(void) ;
void increase_fan_speed(void) ;
void increase_parameter(void) ;
void start_setpoint_mode(void) ;
void start_normal_mode(void) ;
void start_menu_mode(void) ;
void start_next_menu(void) ;
void start_previous_menu(void) ;
void show_parameter(void) ;
void start_menu(void) ;
void accept_new_setting(void) ;
void DealWithKey(uint8 key_id);
void accept_fan_setting(void);
void accept_parameter(void);
#endif
//void backlight_ctrl(uint8 onoff);

void DisRestart(void);
void SetRTCTime(uint16_t item, uint16_t value);
int16 GetRTCTime(uint16_t item);
void one_second_pause(void);
void RunSchedule(void);

uint16_t read_hum_sensor(uint8 item);
uint8 htu21d_soft_reset(void);
int8 htu21d_trigger_measurement_no_hold_master(int8 type);
uint8 htu21d_read_user_register(void);
int16_t CalculateTemperatureCompensation(int16_t hummity_back,int16_t temp_back ) ;
int16_t get_dew_pt(int16 para_t,int16_t para_h);
//void icon_control_output(uint8 icon_control_mode, uint8 icon_name);
void icon_control_output(uint8 icon_name);
//void refresh_icon_output(void);
//float PID_realize(float point,float ActPoint);
//void PID_init();

//typedef struct {
//float SetPoint; //?????
//float ActualPoint; //?????
//float err; //?????
//float err_last; //????????
//float Kp,Ki,Kd; //????????????
//float result; //?????(????????)
//float integral; //?????
//}STRCTPID;

//struct _pid{
//float SetSpeed; //?????
//float ActualSpeed; //?????
//float err; //?????
//float err_last; //????????
//float Kp,Ki,Kd; //????????????
//float voltage; //?????(????????)
//float integral; //?????
//}pid; 

//extern STRCTPID newpid;






















