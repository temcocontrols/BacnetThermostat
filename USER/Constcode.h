

#include "define.h"

//extern uint8 const poll_co2_cmd[3][8];
extern unsigned int short const baudrate[9];
extern uint8 const poll_co2_cmd[9];
extern  DISPTEMP const code ion[4];

extern  DISPTEMP const code clock_week[7];
extern  DISPTEMP const code clock_month[12];
extern  DISPTEMP const code fan_speed[6];

//extern  DISPTEMP  const code menu[TOTAL_MENU_PARAMETERS + 1 + 2 + 1 ];
extern MENUNAME const menu[TOTAL_MENU_PARAMETERS + 1 + 2 + 1];
extern unsigned int short  const code table1_custom_default[ ];
extern unsigned int short  const code table2_custom_default[ ];

extern uint8 const code extern_operation_sop1[ ];
extern uint8 const code extern_operation_customer[ ];
extern uint8 const code extern_operation_default[ ];
extern uint8 const code extern_operation_sop4[ ];
extern uint8 const code extern_operation_sop5[ ];
extern uint8 const code default_valve_table[21];
extern uint8 const code default_pwm_table[7];

extern uint8 const code default_valve_table[21];
extern uint8 const code default_pwm_table[7];

extern uint8 code  fan_table[2][4] ;

extern uint8 const code def_tab[11];
extern uint8 const code def_tab_pic[19];
extern uint8 const code def_tab_type3[19] ;
extern unsigned int short const code def_tab_type50K[19];

//extern uint8 const code PWM_def_tab[51] ;
extern uint8 const code SP_ITEM[3][8];
extern uint8 const code LCD_LINE1[13][9];
extern uint8 const code LCD_LINE2[9][9];
extern uint8 const code MODE[8][9];


extern uint8  const code init_big_ticks[ TOTAL_TIMERS ];

extern uint8 const code parameter_array[2][ TOTAL_EE_PARAMETERS - MAXEEPCONSTRANGE  ][2] ;
extern uint8 const code parameter_default[2][ TOTAL_EE_PARAMETERS - MAXEEPCONSTRANGE ];
extern uint8 const code extern_operation_default[ ];
extern uint8 const code OUTPUT_DELAY_TABLE[2][7];
//extern uint8 const code  auchCRCLo[256];
//extern uint8 const code auchCRCHi[256];
extern unsigned int short const code tbl_hummity_freq[10][2];


#ifdef RTC8563
extern void I2CStartCLOCK( void );
//*****************************************
extern void I2CStopCLOCK( void );
//*****************************************
extern void I2CSendByteCLOCK( uint8 ucWRData );

//*****************************************
extern uint8 I2CReadByteCLOCK( void );
//*****************************************
extern uint8 read_clock(uint8 address);
//*****************************************
extern void write_clock(uint8 address,uint8 value);
//*****************************************
extern void InitClock(void);
//*****************************************uint8 HexToBcd(uint8 byte);
////////////////////////////////////////////
extern uint8 DealBCD(uint8 BCDcode);
////////////////////////////////////////////
extern uint8 HexToBcd(uint8 byte);

#endif

extern void one_second_pause(void);
extern void (*Reset)(void);




