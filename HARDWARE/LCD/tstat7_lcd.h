#ifndef	 _TSTAT7_LCD_H_
#define	 _TSTAT7_LCD_H_

#include "define.h"
#include "bitmap.h"


#ifdef TSTAT7_ARM
 
#define DATA_HT1621	 		PDout(3)
#define CLOCK_HT1621	  	PDout(2) 
#define CS_HT1621		  	PDout(0)

#define RST	 		 		PDout(15)
#define SCL 				PAout(3)	 
#define SDA 				PAout(2)	 

#define LED_TEST 		    PAout(11)

#ifndef OFF
	#define OFF   	0
#endif
#ifndef ON
	#define ON		1
#endif

#ifndef UNIT_C		
	#define UNIT_C	0
#endif
#ifndef	UNIT_F
	#define	UNIT_F	1
#endif

#ifndef UNOCCUPID		
	#define UNOCCUPID	0
#endif
#ifndef	OCCUPID
	#define	OCCUPID	1
#endif

#ifndef HEAT_MODE
	#define HEAT_MODE	2
#endif
#ifndef COOL_MODE		
	#define COOL_MODE	1
#endif
#ifndef NONE_MODE
	#define NONE_MODE	0
#endif


#define ZERO	0xff00	//put the some segments don't screen
#define MON		0x0040	//Monday
#define TUE		0x0020	//Tuesday
#define WED		0x0010	//Wednesday
#define THU		0x0110	//Thursdat
#define FRI		0x0120	//Friday
#define SAT		0x0140	//Saturday
#define SUN		0x0180	//Sunday
#define COL		0x0580	//toggle point every second	:
#define DP		0x1410	//point						 .
#define DT		0x1880	//1
#define DT1		0x1680	//ACTUAL
#define DT2		0x0a20	//snow	
#define DT3		0x0b80	//sunshine
#define DT4		0x0c80	//moon
#define DT5		0x0b40	//house
#define DT6		0x0b20	//human
#define DT7		0x0b10	//running human
#define DT8		0x0a10	//fire
#define DT9		0x0a40	//-------
#define DT10	0x0780	//low fan	
#define DT11	0x0380	//mid fan	
#define DT12	0x0a80	//high fan
#define DT13	0x0e80	//SETTING
#define DT14	0x1080	//%RH
#define DT15	0x1420	//°„F
#define DT16	0x1440	//°„c
void putText(unsigned char Part,unsigned char *pStr);
void Screen1_Display(unsigned char *pStr);
void Screen2_Display(unsigned char *pStr);

 
void Tstat7_ClearScreen( void );
void Tstat7_FullScreen( void );
void Tstat7_InitScreen( void );
void updateCurrentIcon(void);
void Fan1Icon(void);
void Fan2Icon(void);
void Fan3Icon(void);
void NoFanIcon(void);
void DisplayDecimalIcon(uint8 status);
void DisplayFanSpeedIcon(uint8 mode);
void DisplayHeat_CoolIcon(uint8 mode);
void putTime(uint8 hr,uint8 min,uint8 flop_col,uint8 flop_hour,uint8 flop_min );
void putWeek(uint8 week);
void putDigitals1(uint16 uiNum,uint8 units);
void putDigitals2(uint16 uiNum);
void Tstat7_Restart( void );
void Tstat7_lcd_gpio_Init(void);
void HT1621_Test(short int pos);
void Digital_Test(short int pos,unsigned char data);
void DisplayIcon(uint16 CMD,uint8 status); 

/*******************NEW LCD ST7565R  ROUTINE *******************/
void Tstat7_lcd_gpio_Init_REV22(void);
void Tstat7_InitScreen_HT1621( void );
void LCDInit();
void Newlcdtest(void); 
void LCD_WRITE_CMD(uint8 dat);
void LCD_WRITE_DAT(uint8 dat);
void WRITE_STRING86(uint8 plat,uint8 column,uint8 *p);
/*******************END OF ST7565R  ROUTINE *******************/



 #define CS_0             GPIO_ResetBits(GPIOD, GPIO_Pin_6)  //LCD????
 #define RES_1            GPIO_SetBits(GPIOD, GPIO_Pin_9)    //LCD?????,?????????
 #define RES_0            GPIO_ResetBits(GPIOD, GPIO_Pin_9)  //LCD??
 #define A0_1             GPIO_SetBits(GPIOD, GPIO_Pin_10)   //Ao=1,?LCD????
 #define A0_0             GPIO_ResetBits(GPIOD, GPIO_Pin_10) //Ao=0,?LCD????
 #define WR_1             GPIO_SetBits(GPIOD, GPIO_Pin_11)   //LCD???
 #define WR_0             GPIO_ResetBits(GPIOD, GPIO_Pin_11) //LCD???
 #define RD_1             GPIO_SetBits(GPIOD, GPIO_Pin_12)   //LCD???
 #define RD_0             GPIO_ResetBits(GPIOD, GPIO_Pin_12) //LCD???
 

 void Lcd12232delay(unsigned int Time);//????
 void LCD_WriteLByte(uint8 B);//?PC???8?,?8???
 void w_com(unsigned char B);//?LCD???
 void w_data(unsigned char data);//?LCD???
 void SetStartPage(u8 StartPageAddress);//????????
 void SetStartColumn(u8 StartColumnAddress);//???????
 void SetStartLine(u8 StartLineAddress);//???????
 void clrscr(void);//??
 void ClrOnePage(unsigned StartPage);//????
 void ClrNumColumn(unsigned char StartPage,unsigned char StartColumn,unsigned Num);//??StartPage??StartColumn????????Num?
 void Displayhz(unsigned char num,const unsigned char *p);//?(0,0)????num?16*16???(??????)
 void DisplayHz(unsigned char StartPage,unsigned char column,unsigned char Num,const unsigned char *p);//?StartPage???Column?????????Num???
 void DisplayZf(unsigned char StartPage,unsigned char column,unsigned char Num,const unsigned char *p);//?StartPage???Column?????????Num?8*16???
 void DisplayZF(unsigned char num,const unsigned char *p);//?(0,0)????num?8*16???(??????)
 void display_map(const unsigned char *p);//??128*32???,p????????
 void LCD_Init();//LCD???



#endif//TSTAT7_ARM



#endif

