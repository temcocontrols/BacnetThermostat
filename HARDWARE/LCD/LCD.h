
#ifndef	__LCD_H__

#define	__LCD_H__



#include "bitmap.h"
#include "define.h"



#ifndef	TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif


//#define FORM48X120		0
#define FORM32X64 		0
#define FORM15X30			1


#ifndef TSTAT7_ARM

extern uint8 *scroll;
extern uint8 scroll_ram[20][15];
extern uint8 fan_flag;
extern uint8 display_flag;
extern uint8 schedule_hour_minute; //indicate current display item is "hour" or "minute"
extern uint8 blink_parameter;
extern uint8 clock_blink_flag;
void LCDtest(void);
extern uint8 const chlib[];
extern uint8 const chlibsmall[];
extern uint16 const athome[];
extern uint16 const offhome[];
extern uint16 const sunicon[];
extern uint16 const moonicon[];
extern uint16 const heaticon[]; 
extern uint16 const coolicon[];
extern uint16 const fanspeed0a[];
extern uint16 const fanspeed1a[];
extern uint16 const fanspeed2a[];
extern uint16 const fanspeed3a[];
extern uint16 const fanbladeA[];
extern uint16 const fanbladeB[];

extern uint16 const degree_o[];
//extern uint16 const therm_meter[];
extern uint16 const leftup[];
extern uint16 const leftdown[];
extern uint16 const rightdown[];
extern uint16 const rightup[];
extern uint16 const cmnct_send[]; 
extern uint16 const cmnct_rcv[]; 

typedef struct   
{
 uint8 unit;
 uint8 setpoint;
 uint8 fan;
 uint8 sysmode;
 uint8 occ_unocc;
 uint8 heatcool;
 uint8 fanspeed;
 uint8 cmnct_send;
 uint8 cmnct_rcv; 	
} DISP_CHANGE; 
extern DISP_CHANGE icon;
//extern uint16 const angle[];
void draw_tangle(uint8 xpos, uint16 ypos);
void ClearScreen(unsigned int bColor);
void disp_ch(uint8 form, uint16 x, uint16 y,uint8 value,uint16 dcolor,uint16 bgcolor);		
void disp_icon(uint16 cp, uint16 pp, uint16 const *icon_name, uint16 x,uint16 y,uint16 dcolor, uint16 bgcolor);
void disp_null_icon(uint16 cp, uint16 pp, uint16 const *icon_name, uint16 x,uint16 y,uint16 dcolor, uint16 bgcolor);
//void disp_str(uint8 form, uint16 x,uint16 y,uint8 *str,uint16 dcolor,uint16 bgcolor);	
void display_SP(int16 setpoint);
void display_fanspeed(int16 speed);
void display_mode(void);
void display_fan(void);
void display_icon(void);
void display_value(uint16 pos,int16 disp_value, uint8 disp_unit);
//void display_menu(uint16 pos, uint8 *item);
void display_menu (uint8 *item1, uint8 *item2);
void clear_line(uint8 linenum);
void clear_lines(void);
//void display_clock_date(int8 item, int16 value);
//void display_clock_time(int8 item, int16 value);
void display_scroll(void);
void display_schedule_time(int8 schedule_time_sel, uint8 hour_minute);
void Top_area_display(uint8 item, int16 value, uint8 unit);

#endif //TSTAT7_ARM

#endif

