

#include "tstat7_lcd.h"
#include "delay.h"  
#include "math.h" 
#include "stdlib.h"
#include "TstatFunctions.h"
#include "eepdefine.h"
#include "define.h"
#include "setpoint.h"


#ifdef TSTAT7_ARM
#define ICON_ROW  	26
static uint8 icon_data[ICON_ROW];
extern void watchdog(void);
const unsigned char tab2[]={
 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,

 };



char const ALLData[128][8] = 
{
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	0
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	10
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	20
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	30
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x04,0x00,0x00,0x04,0x04,0x04,0x04,0x04},//!	33
{0x00,0x00,0x00,0x00,0x00,0x09,0x09,0x0b},//"
{0x05,0x05,0x0a,0x1f,0x0a,0x1f,0x0a,0x05},//#
{0x04,0x0f,0x15,0x14,0x1f,0x15,0x14,0x04},//$
{0x09,0x15,0x09,0x07,0x18,0x12,0x15,0x12},//%
{0x16,0x09,0x15,0x02,0x02,0x0c,0x00,0x00},//&
{0x00,0x00,0x00,0x00,0x0c,0x08,0x08,0x08},//'
{0x02,0x02,0x01,0x01,0x01,0x01,0x02,0x02},//(	40
{0x08,0x08,0x10,0x10,0x10,0x10,0x08,0x08},//)
{0x00,0x0b,0x04,0x1f,0x04,0x0b,0x00,0x00},//*
{0x04,0x04,0x04,0x1f,0x04,0x04,0x04,0x00},//+
{0x04,0x0c,0x08,0x18,0x10,0x00,0x00,0x00},//,
{0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x00},//-
{0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00},//.
{0x10,0x10,0x10,0x08,0x08,0x04,0x04,0x02},///
{0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x0e},//0	48
{0x1f,0x04,0x04,0x04,0x04,0x04,0x04,0x06},//1 	
{0x1f,0x11,0x02,0x04,0x08,0x10,0x11,0x1e},//2  	
{0x0e,0x11,0x10,0x10,0x0c,0x10,0x11,0x0e},//3 	
{0x1c,0x08,0x1f,0x0a,0x0a,0x0c,0x0c,0x08},//4  	
{0x0e,0x11,0x10,0x10,0x0e,0x02,0x02,0x1e},//5  	
{0x0e,0x11,0x11,0x11,0x0f,0x01,0x02,0x1c},//6  	
{0x04,0x04,0x08,0x08,0x08,0x10,0x11,0x1f},//7	
{0x0e,0x11,0x11,0x11,0x0e,0x11,0x11,0x0e},//8  	
{0x07,0x08,0x10,0x0e,0x11,0x11,0x11,0x0e},//9 	57
{0x00,0x0c,0x0c,0x00,0x00,0x0c,0xc0,0x00},//:
{0x02,0x04,0x0c,0x00,0x00,0x0c,0x0c,0x00},//;
{0x10,0x08,0x06,0x01,0x06,0x08,0x10,0x00},//<
{0x00,0x00,0x1f,0x00,0x1f,0x00,0x00,0x00},//=
{0x01,0x02,0x0c,0x10,0x0c,0x02,0x01,0x00},//>
{0x06,0x00,0x04,0x08,0x10,0x12,0xc0,0x00},//?
{0x0e,0x01,0x19,0x15,0x15,0x19,0x11,0x0e},//@
{0x11,0x11,0x11,0x11,0x1f,0x11,0x11,0x0e},//A 	65
{0x0f,0x11,0x11,0x11,0x0f,0x11,0x11,0x0f},//B
{0x1e,0x01,0x01,0x01,0x01,0x01,0x01,0x1e},//C
{0x0f,0x11,0x11,0x11,0x11,0x11,0x11,0x0f},//D
{0x1f,0x01,0x01,0x01,0x1f,0x01,0x01,0x1f},//E
{0x01,0x01,0x01,0x01,0x1f,0x01,0x01,0x1f},//F
{0x0e,0x11,0x11,0x11,0x0d,0x01,0x01,0x0e},//G
{0x11,0x11,0x11,0x11,0x1f,0x11,0x11,0x11},//H
{0x1f,0x04,0x04,0x04,0x04,0x04,0x04,0x1f},//I
{0x06,0x05,0x05,0x04,0x04,0x04,0x04,0x1f},//J
{0x11,0x05,0x03,0x01,0x03,0x05,0x09,0x11},//K
{0x1f,0x02,0x02,0x02,0x02,0x02,0x02,0x07},//L
{0x11,0x11,0x11,0x11,0x15,0x1b,0x11,0x00},//M
{0x11,0x19,0x11,0x15,0x11,0x13,0x11,0x11},//N 
{0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x0e},//O
{0x01,0x01,0x01,0x01,0x0f,0x11,0x11,0x0f},//P
{0x1e,0x0d,0x09,0x09,0x09,0x09,0x09,0x06},//Q
{0x19,0x05,0x03,0x01,0x07,0x09,0x09,0x07},//R
{0x0f,0x11,0x10,0x10,0x0e,0x01,0x11,0x0e},//S
{0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x1f},//T 
{0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x11},//U 
{0x04,0x04,0x0a,0x0a,0x0a,0x11,0x11,0x11},//V 
{0x11,0x1b,0x15,0x11,0x11,0x11,0x11,0x00},//W
{0x11,0x11,0x0a,0x04,0x04,0x0a,0x11,0x11},//X
{0x0e,0x04,0x04,0x04,0x0a,0x0a,0x11,0x11},//Y 
{0x1f,0x11,0x02,0x04,0x04,0x08,0x11,0x1f},//Z 	90
{0x0e,0x02,0x02,0x02,0x02,0x02,0x02,0x0e},//[
{0x18,0x18,0x18,0x14,0x14,0x12,0x12,0x11},//\
{0x0e,0x08,0x08,0x08,0x08,0x08,0x08,0x0e},//]
{0x00,0x00,0x00,0x00,0x11,0x0a,0x04,0x04},//^
{0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//_
{0x00,0x00,0x00,0x00,0x00,0x04,0x02,0x01},//`
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x16,0x09,0x09,0x09,0x0e,0x08,0x07,0x00},//a 	97 
{0x0f,0x11,0x11,0x13,0x0d,0x01,0x01,0x01},//b
{0x0e,0x11,0x01,0x01,0x11,0x0e,0x00,0x00},//c
{0x1e,0x11,0x11,0x11,0x1e,0x10,0x10,0x10},//d  
{0x1e,0x11,0x01,0x1f,0x11,0x0e,0x00,0x00},//e
{0x02,0x02,0x02,0x02,0x02,0x1f,0x02,0x1c},//f
{0x0e,0x11,0x11,0x10,0x0e,0x11,0x11,0x0e},//g 
{0x11,0x11,0x11,0x11,0x13,0x0d,0x01,0x01},//h 
{0x0e,0x04,0x04,0x04,0x06,0x00,0x04,0x00},//i 
{0x06,0x08,0x08,0x08,0x08,0x0e,0x00,0x08},//j  
{0x19,0x05,0x03,0x05,0x09,0x01,0x01,0x01},//k  
{0x0e,0x04,0x04,0x04,0x04,0x04,0x04,0x00},//l 
{0x1f,0x15,0x15,0x15,0x15,0x06,0x00,0x00},//m  
{0x13,0x12,0x12,0x12,0x16,0x16,0x00,0x00},//n  
{0x0e,0x11,0x11,0x11,0x11,0x0e,0x00,0x00},//o  
{0x1f,0x02,0x02,0x0e,0x12,0x12,0x0f,0x00},//p 
{0x18,0x10,0x10,0x1e,0x11,0x11,0x1e,0x00},//q  
{0x0f,0x02,0x02,0x02,0x06,0x1b,0x00,0x00},//r 
{0x0f,0x10,0x10,0x0e,0x01,0x1e,0x00,0x00},//s 
{0x1c,0x02,0x02,0x02,0x02,0x1f,0x02,0x00},//t  
{0x16,0x19,0x11,0x11,0x11,0x1b,0x00,0x00},//u  
{0x04,0x0a,0x0a,0x11,0x11,0x11,0x00,0x00},//v  
{0x0a,0x15,0x15,0x15,0x11,0x11,0x00,0x00},//w  
{0x1b,0x0a,0x04,0x04,0x0a,0x1b,0x00,0x00},//x
{0x0f,0x10,0x10,0x1c,0x12,0x12,0x12,0x00},//y  
{0x1f,0x12,0x04,0x08,0x11,0x1f,0x00,0x00},//z 	122
{0x08,0x08,0x04,0x06,0x06,0x04,0x08,0x08},//{
{0x40,0x40,0x40,0x40,0x04,0x04,0x04,0x04},//|
{0x02,0x02,0x04,0x0c,0x0c,0x04,0x02,0x02},//}
{0x00,0x00,0x00,0x0d,0x12,0x00,0x00,0x00},//~
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//	127
};


uint16 const ArrayData[10][22] =
{
//0			  1				2			  3				4			  5				6			  7				8			  9				A(U)
0x17f0,0x1850,0x1760,0x1800,0x17d0,0x1830,0x17f0,0x1820,0x1760,0x1860,0x17b0,0x1860,0x17b0,0x1870,0x17e0,0x1800,0x17f0,0x1870,0x17f0,0x1860,0x17e0,0x1870,//1 Segment
0x15f0,0x1650,0x1560,0x1600,0x15d0,0x1630,0x15f0,0x1620,0x1560,0x1660,0x15b0,0x1660,0x15b0,0x1670,0x15e0,0x1600,0x15f0,0x1670,0x15f0,0x1660,0x1570,0x1650,//2 Segment
0x12f0,0x1350,0x1260,0x1300,0x12d0,0x1330,0x12f0,0x1320,0x1260,0x1360,0x12b0,0x1360,0x12b0,0x1370,0x12e0,0x1300,0x12f0,0x1370,0x12f0,0x1360,0x1200,0x1300,//3 Segment
0x0df0,0x0c50,0x0d60,0x0c00,0x0db0,0x0c60,0x0df0,0x0c20,0x0d60,0x0c30,0x0dd0,0x0c30,0x0dd0,0x0c70,0x0d70,0x0c00,0x0df0,0x0c70,0x0df0,0x0c30,0x0df0,0x0c30,//4 Segment
0x0ff0,0x0e50,0x0f60,0x0e00,0x0fb0,0x0e60,0x0ff0,0x0e20,0x0f60,0x0e30,0x0fd0,0x0e30,0x0fd0,0x0e70,0x0f70,0x0e00,0x0ff0,0x0e70,0x0ff0,0x0e30,0x0ff0,0x0e30,//5 Segment
0x11f0,0x1050,0x1160,0x1000,0x11b0,0x1060,0x11f0,0x1020,0x1160,0x1030,0x11d0,0x1030,0x11d0,0x1070,0x1170,0x1000,0x11f0,0x1070,0x11f0,0x1030,0x11f0,0x1030,//6 Segment
0x08f0,0x0950,0x0860,0x0900,0x08d0,0x0930,0x08f0,0x0920,0x0860,0x0960,0x08b0,0x0960,0x08b0,0x0970,0x08e0,0x0900,0x08f0,0x0970,0x08f0,0x0960,0x08f0,0x0960,//7 Segment
0x06f0,0x0750,0x0660,0x0700,0x06d0,0x0730,0x06f0,0x0720,0x0660,0x0760,0x06b0,0x0760,0x06b0,0x0770,0x06e0,0x0700,0x06f0,0x0770,0x06f0,0x0760,0x06f0,0x0760,//8 Segment
0x04f0,0x0550,0x0460,0x0500,0x04d0,0x0530,0x04f0,0x0520,0x0460,0x0560,0x04b0,0x0560,0x04b0,0x0570,0x04e0,0x0500,0x04f0,0x0570,0x04f0,0x0560,0x04f0,0x0560,//9 Segment
0x02f0,0x0350,0x0260,0x0300,0x02d0,0x0330,0x02f0,0x0320,0x0260,0x0360,0x02b0,0x0360,0x02b0,0x0370,0x02e0,0x0300,0x02f0,0x0370,0x02f0,0x0360,0x02f0,0x0360//10 Segment
};


 
//---------------------------------------
/*********************ST7565 driver*******************/
 
/**************************???*************************/ 
//=========================LCD??????====================== 
//sbit LCDA0=P2^5;  //????
//sbit LCDSK=P2^6;  //????
//sbit LCDSI=P2^7;  //????
//sbit LCDRS=P2^4;  //????
//=========================????============================= 
void LCDInit();        //????? 
void LCDWriteByte(char dat,char command);  //??????,????/????(0??,1??) 
void LCDShiftWrite(char dat);  //??????? 
void LCDSetXY(char x, char y); //?????? 


#define LCD_col   128 //128?
#define LCD_plat  64 //64?
#define TX_col   16 //16????
#define ZF_col    21    //21????
#define PAGE_NUM  8     //???
#define Page0    0xb0  //page 0 address
#define Display_Sta_line  0x40 // display ram H0 address

/*****************************************
FUNCTION NAME:I2CStart
FUNCTION:Start I2C bus
INPUT:none
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void I2CStart( void )
{ 
	SDA = 1;
	SCL = 1;
	delay_us(4);
 	SDA = 0;	//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SCL = 0;	//钳住I2C总线，准备发送或接收数据 
}

/*****************************************
FUNCTION NAME:I2CStop
FUNCTION:Stop I2C bus
INPUT:none
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void I2CStop( void )
{ 
	SCL = 0;
	SDA = 0;	//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SCL = 1;
	SDA = 1;
	delay_us(4);
}

/*****************************************
FUNCTION NAME:I2CSendByte
FUNCTION:Send a byte on I2C bus
INPUT:ucWRData
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void I2CSendByte( unsigned char ucWRData )
{
	unsigned char   i;
	SCL = 0;		//拉低时钟开始数据传输 
	for(i = 0;i < 8;i++)
	{
		SDA = ( ucWRData & 0x80 ) >> 7;
		ucWRData <<= 1;	
		delay_us(2);
		SCL = 1;
		delay_us(2);
		SCL = 0;
	}
	SDA = 1;
	delay_us(2);
	SCL = 1;
	delay_us(2);
	SCL = 0;
}


/*****************************************
FUNCTION NAME:WriteCommand
FUNCTION:Write command to HT1621
INPUT:Data
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void WriteCommand( unsigned char Data )
{
	unsigned char   i;
	unsigned char  RegData;

	RegData = 0x80;

	CS_HT1621 = 1;
	delay_us(2);
	CS_HT1621 = 0;

	for( i = 0; i < 3; i++ )
	{
		DATA_HT1621  =( RegData & 0x80 ) >> 7; 
		CLOCK_HT1621 = 0;
		delay_us(2);//		_nop_();_nop_();_nop_();_nop_();
		CLOCK_HT1621 = 1;
		delay_us(2);
		RegData    <<= 1;
	}
	
	RegData = Data;
	for( i = 0; i < 8; i++ )
	{
		DATA_HT1621  = ( RegData & 0x80 ) >> 7;
		CLOCK_HT1621 = 0;
		delay_us(2);//		_nop_();_nop_();_nop_();_nop_();
		CLOCK_HT1621 = 1;
		delay_us(2);
		RegData    <<= 1;
	}
	
	CLOCK_HT1621 = 0;
	delay_us(2);//	_nop_();_nop_();_nop_();_nop_();
	CLOCK_HT1621 = 1;
	delay_us(2);
	CS_HT1621 = 1;
}

/*****************************************
FUNCTION NAME:WriteData
FUNCTION:Write data to HT1621
INPUT:Addr,Data
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
static void WriteData( unsigned char Addr,unsigned char Data )
{
	unsigned char   i;
	unsigned char  RegData;

	RegData = 0xa0;

	CS_HT1621 = 1;
	delay_us(2);
	CS_HT1621 = 0;

	for( i = 0; i < 3; i++ )
	{
		DATA_HT1621  = ( RegData & 0x80 ) >> 7;
		CLOCK_HT1621 = 0;
		delay_us(2);//_nop_();_nop_();_nop_();_nop_();
		CLOCK_HT1621 = 1;
		delay_us(2);
		RegData    <<= 1;
	}
	
	RegData = Addr;
	RegData <<= 2;
	for( i = 0; i < 6; i++ )
	{
		DATA_HT1621  = ( RegData & 0x80 ) >> 7;
		CLOCK_HT1621 = 0;
		delay_us(2);//		_nop_();_nop_();_nop_();_nop_();
		CLOCK_HT1621 = 1;
		delay_us(2);
		RegData    <<= 1;
	}

	RegData = Data;
	for( i = 0; i < 4; i++ )
	{
		DATA_HT1621  = ( RegData & 0x80 )>>7;
		CLOCK_HT1621 = 0;
		delay_us(2);//		_nop_();_nop_();_nop_();_nop_();
		CLOCK_HT1621 = 1;
		delay_us(2);
		RegData    <<= 1;
	}
	
	CLOCK_HT1621 = 0;
	delay_us(2);//	_nop_();_nop_();_nop_();_nop_();
	CLOCK_HT1621 = 1;
	delay_us(2);
	CS_HT1621 = 1;
}


 
/*****************************************
FUNCTION NAME:putText
FUNCTION:Get text and put them on part
INPUT:Part, *pStr
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void putText(unsigned char Part,unsigned char *pStr)
{
	unsigned char  i,j;
	unsigned char  Loop;
	unsigned char  Data[8][8];
	uint8 reverse_buf[5][8];//for ST7565

	for( i = 0; i < 8; i++ )
	{
		for( j = 0; j < 8; j++ )
		{
			Data[i][j] = ALLData[*(pStr+i)][j];
		}
	}	

 
	i = 0;
	j = 8;

	for( i = 0; i < 8; i++ )
	{Data[0][i] = Data[0][i] | (Data[1][i] << 5);}
	
	for( i = 0; i < 8; i++ )
	{Data[1][i] = (Data[1][i] >> 3) | (Data[2][i] << 2) | (Data[3][i] << 7);}

	for( i = 0; i < 8; i++ )
	{Data[2][i] = (Data[3][i] >> 1) | (Data[4][i] << 4);}

	for( i = 0; i < 8; i++ )
	{Data[3][i] = (Data[4][i] >> 4) | (Data[5][i] << 1) | (Data[6][i] << 6);}

	for( i = 0; i < 8; i++ )
	{Data[4][i] = (Data[6][i] >> 2) | (Data[7][i] << 3);}

	if(HardwareVersion <= 21)
	{
		for( Loop = 0; Loop < 5; Loop++ )
		{	
			I2CStart();
			I2CSendByte( 0x78 );//chip address	
			I2CSendByte( 0x00 );
			I2CSendByte( 0x30 );
			I2CSendByte( 0x40 + Loop );
			if( Part == 0 )	
				I2CSendByte( 0x88 );
			else
				I2CSendByte( 0x80 );
			I2CStop();

			I2CStart();	
			I2CSendByte( 0x78 );//chip address	
			I2CSendByte( 0x40 );
			for(i = 0;i < j;i++)
			{
				I2CSendByte( Data[Loop][i] );	
			}
			I2CStop();
		}
	}
	else//if hardware version is > 21, ST7565 chip
	{
		for( Loop = 0; Loop <5; Loop++ )
		{
			for(j=0;j<8;j++)
			{
				for(i=0;i<8;i++)
				{
					if(GetByteBit(&Data[Loop][j],i))
						SetByteBit(&reverse_buf[4-Loop][j],7-i,1);
					else
						SetByteBit(&reverse_buf[4-Loop][j],7-i,0);
				}
			}
		}	
		for( Loop = 0; Loop <5; Loop++ )
		{	
			 if( Part == 0 )
			 {
			 LCD_WRITE_CMD(0xb1+Loop);
			 LCD_WRITE_CMD(0x17);//diaplay colum high 4 bits
			 LCD_WRITE_CMD(0x04);//diaplay colum low 4 bit 
			 }
			else
			{
			 LCD_WRITE_CMD(0xb1+Loop);
			 LCD_WRITE_CMD(0x17);//diaplay colum high 4 bits
			 LCD_WRITE_CMD(0x0c);//diaplay colum low 4 bit 
			}			

			for(i = 8;i > 0;i--)
			{
				LCD_WRITE_DAT(reverse_buf[Loop][i-1] );			
			}
		}	
	}
}


void Screen1_Display(unsigned char *pStr)
{
	putText(0,pStr);
}
void Screen2_Display(unsigned char *pStr)
{
	putText(1,pStr);
}


/*****************************************
FUNCTION NAME:DisplaySegment
FUNCTION:Display each segment
INPUT:Num,Data
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
//static void DisplaySegment( uint16 Data )
//{
//	WriteData( (unsigned char)( Data >> 8 ),(unsigned char)Data );
//}

void DisplayIcon(uint16 CMD,uint8 status)
{
	uint8 addr, data;
	addr = (unsigned char)( CMD >> 8 );
	data = (unsigned char)CMD;
	if(addr < ICON_ROW)
	{
		if(status == ON)
		{
			icon_data[addr] |= data;
		}
		else
		{
			icon_data[addr] &= (~data);
		}
		
		WriteData(addr,icon_data[addr]);
	}
}
static void DispalyDigital(uint16 CMD)
{
	uint8 addr, data;
	addr = (unsigned char)( CMD >> 8 );
	data = (unsigned char)CMD;
	if(addr < ICON_ROW)
	{ 
		icon_data[addr] |= data; 
		WriteData(addr,icon_data[addr]);
	}
}


static void clean_digital(uint8 pos)
{
		 
	    switch (pos)
		{
			case 0:
				icon_data[23] = 0;
				icon_data[24] &= 0x80;
			    WriteData(23,icon_data[23]);
				WriteData(24,icon_data[24]);
				break;
			case 1:
				icon_data[21] = 0;
				icon_data[22] &= 0x80;
				WriteData(21,icon_data[21]);
				WriteData(22,icon_data[22]);
				break;
			case 2:
				icon_data[18] = 0;
				icon_data[19] &= 0x80;
				WriteData(18,icon_data[18]);
				WriteData(19,icon_data[19]);
				break;
			case 3:
				icon_data[13] = 0;
				icon_data[12] &= 0x80;
				WriteData(13,icon_data[13]);
				WriteData(12,icon_data[12]);
				break;
			case 4:
				icon_data[15] = 0;
				icon_data[14] &= 0x80;
				WriteData(15,icon_data[15]);
				WriteData(14,icon_data[14]);
				break;
			case 5:
				icon_data[17] = 0;
				icon_data[16] &= 0x80;
			    WriteData(17,icon_data[17]);
				WriteData(16,icon_data[16]);
				break;
			case 6:
				icon_data[8] = 0;
				icon_data[9] &= 0x80;
				WriteData(8,icon_data[8]);
				WriteData(9,icon_data[9]);
				break;
			case 7:
				icon_data[6] = 0;
				icon_data[7] &= 0x80;
				WriteData(6,icon_data[6]);
				WriteData(7,icon_data[7]);
				break;
			case 8:
				icon_data[4] = 0;
				icon_data[5] &= 0x80;
				WriteData(4,icon_data[4]);
				WriteData(5,icon_data[5]);
				break;
			case 9:
				icon_data[2] = 0;
				icon_data[3] &= 0x80;
				WriteData(2,icon_data[2]);
				WriteData(3,icon_data[3]);
				break;
			default:
				break;
		
		}
		
}

/*****************************************
FUNCTION NAME:ClareScreen
FUNCTION:Clear HT1621 & NT7533 screen
INPUT:none
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void Tstat7_ClearScreen( void )
{
	uint8  i;
	
	for( i = 0; i < 26; i++ )
	{
		WriteData( i,0 );
	} 
	
	for( i = 0; i < ICON_ROW; i++ )
	{
		icon_data[i] = 0x00;
	}
	putText(0,(uint8 *)"        ");
	putText(1,(uint8 *)"        "); 	
}

void Tstat7_FullScreen( void )
{
	//unsigned char xdata temp[8]= {0x0,0x0,0x0,0x0,0x0,0,0,0};
 
	uint8  i;
	
	for( i = 0; i < 26; i++ )
	{
		WriteData( i,0xFF );
	}
	for( i = 0; i < ICON_ROW; i++ )
	{
		icon_data[i] = 0xff;
	}
	putText(0,(uint8 *)"OOOOOOOO");
	putText(1,(uint8 *)"OOOOOOOK");
}

/*****************************************
FUNCTION NAME:InitScreen
FUNCTION:Init HT1621 & NT7533
INPUT:none
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void Tstat7_InitScreen( void )
{
	Tstat7_lcd_gpio_Init();
	
	delay_us( 100 );
	RST = 0;
	delay_us( 100 );
	RST = 1;
	delay_us( 100 );

//HT1621
	WriteCommand( 0x01 );//Turn on system oscillator
	WriteCommand( 0x03 );//Turn on LCD outputs
	WriteCommand( 0x18 );//System on-chip RC oscillator,RC 256K
	WriteCommand( 0x29 );//1/3bias,4 commons
	WriteCommand( 0xe3 );//Normal mode
 
  
//NT7533
	if(HardwareVersion <= 21)
	{
	I2CStart();	
	I2CSendByte( 0x78 );//chip address
	I2CSendByte( 0x00 );
	I2CSendByte( 0x30 );//Function Set Use basic instruction set
	I2CSendByte( 0x12 );//Set HV-gen.stages
	delay_us( 100 );
	I2CSendByte( 0x05 );//Set the range of VLCD
	I2CSendByte( 0x31 );//Function Set Use extended instruction set
	I2CSendByte( 0x15 );//System Bias:1/6 Bias
	I2CSendByte( 0x0c );//Display configuration
	I2CSendByte( 0x04 );//Set Temperature coefficient
	I2CSendByte( 0x93 );//Set VOP = 7.8V
	I2CSendByte( 0x30 );//Function Set Use basic instruction set
	I2CSendByte( 0x0c );//Display Control Normal Display
	I2CStop();
	}
	else
	{
	 LCDInit();

	 clrscr();	
	}
}

void Tstat7_InitScreen_HT1621( void )
{
	Tstat7_lcd_gpio_Init_REV22();
	
	delay_us( 100 );
	RST = 0;
	delay_us( 100 );
	RST = 1;
	delay_us( 100 );

//HT1621
	WriteCommand( 0x01 );//Turn on system oscillator
	WriteCommand( 0x03 );//Turn on LCD outputs
	WriteCommand( 0x18 );//System on-chip RC oscillator,RC 256K
	WriteCommand( 0x29 );//1/3bias,4 commons
	WriteCommand( 0xe3 );//Normal mode
}


 


void updateCurrentIcon(void)
{
//	uint8 i;  	
//	for( i = 0; i < ICON_ROW; i++ )
//	{
//		WriteData( i,icon_data[i] );
//	}  
}

void Fan1Icon(void)
{	 
	DisplayIcon(DT11,OFF);
	DisplayIcon(DT12,OFF);
	DisplayIcon(DT10,ON);
}
void Fan2Icon(void)
{	 
	DisplayIcon(DT10,OFF);
	DisplayIcon(DT12,OFF);
	DisplayIcon(DT11,ON);
}
void Fan3Icon(void)
{	 
	DisplayIcon(DT10,OFF);
	DisplayIcon(DT11,OFF);
	DisplayIcon(DT12,ON);  
}
void NoFanIcon(void)
{	 
	DisplayIcon(DT10,OFF);
	DisplayIcon(DT11,OFF);
	DisplayIcon(DT12,OFF);  
}

void DisplayDecimalIcon(uint8 status)
{
	if(status == ON) 	DisplayIcon(DP,ON);
	else				DisplayIcon(DP,OFF);
		
}

void DisplayFanSpeedIcon(uint8 mode)//unoccupid mode / occupid mode
{
	DisplayIcon(DT5,ON); //show house
	DisplayIcon(DT9,ON); //show ------
	
if(schedule_on_off == 1)
		{
			switch(mode)
			{
				case DHOME_SP://morning at home/wake up
					DisplayIcon(DT7,OFF);//unshow run human
					DisplayIcon(DT4,OFF);//unshow moon	
					DisplayIcon(DT6,ON); //show human
					DisplayIcon(DT3,ON); //show sunshine			
				break;
				
				case NIGHT_SP://WORK
					DisplayIcon(DT7,ON);//unshow run human
					DisplayIcon(DT4,OFF);//unshow moon	
					DisplayIcon(DT6,OFF); //show human
					DisplayIcon(DT3,ON); //show sunshine	
//				disp_icon(45, 45, sunicon, 		ICON_XPOS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//				disp_icon(45, 45, offhome, 		ICON_XPOS+60,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);				
				break;

				case NHOME_SP://morning at home/wake up
					DisplayIcon(DT7,OFF);//unshow run human
					DisplayIcon(DT4,ON);//unshow moon	
					DisplayIcon(DT6,ON); //show human
					DisplayIcon(DT3,OFF); //show sunshine						
//				disp_icon(45, 45, moonicon, 		ICON_XPOS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//				disp_icon(45, 45, athome, 		ICON_XPOS+60,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);				
				break;
				
				case SLEEP_SP_LIB://SLEEP
					DisplayIcon(DT7,ON);//show run human
					DisplayIcon(DT4,ON);//show moon	
					DisplayIcon(DT6,OFF); //show human
					DisplayIcon(DT3,OFF); //show sunshine	
//				disp_icon(45, 45, moonicon, 		ICON_XPOS,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);
//				disp_icon(45, 45, offhome, 		ICON_XPOS+60,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);				
				break;

				case HOLIDAY_SP_LIB://SLEEP
					DisplayIcon(DT7,ON);//show run human
					DisplayIcon(DT4,OFF);//show moon	
					DisplayIcon(DT6,OFF); //show human
					DisplayIcon(DT3,OFF); //show sunshine	
//					disp_null_icon(45, 45, 0, ICON_XPOS ,ICON_POS,TSTAT8_BACK_COLOR, TSTAT8_BACK_COLOR);
//					disp_null_icon(45, 45, 0, ICON_XPOS+60 ,ICON_POS,TSTAT8_BACK_COLOR, TSTAT8_BACK_COLOR);			
				break;			
			}
		}
		else
		{
			if(override_timer_time)
			{
					DisplayIcon(DT7,OFF);//unshow run human
					DisplayIcon(DT4,OFF);//unshow moon	
					DisplayIcon(DT6,ON); //show human
					DisplayIcon(DT3,OFF); //show sunshine	
//			disp_null_icon(45, 45, 0, ICON_XPOS ,ICON_POS,TSTAT8_BACK_COLOR, TSTAT8_BACK_COLOR);
//			disp_icon(45, 45, athome, 		ICON_XPOS+60,	ICON_POS,TSTAT8_CH_COLOR, TSTAT8_BACK_COLOR);		
			}
			else
			{
				if(fan_speed_user == FAN_OFF)
				{
					DisplayIcon(DT7,ON);//show run human
					DisplayIcon(DT4,OFF);//show moon	
					DisplayIcon(DT6,OFF); //show human
					DisplayIcon(DT3,OFF); //show sunshine
				}
			  else
				{
					DisplayIcon(DT7,OFF);//show run human
					DisplayIcon(DT4,OFF);//show moon	
					DisplayIcon(DT6,ON); //show human
					DisplayIcon(DT3,OFF); //show sunshine
				}
			}
		}		
	
//	if(mode == OCCUPID)
//	{
//		DisplayIcon(DT7,OFF);//unshow run human
//    DisplayIcon(DT4,OFF);//unshow moon	
//		DisplayIcon(DT6,ON); //show human
//		DisplayIcon(DT3,ON); //show sunshine
//	}
//	else
//	{
//		DisplayIcon(DT6,OFF);//unshow human
//		DisplayIcon(DT3,OFF);//unshow sunshine
//		DisplayIcon(DT7,ON); //show run human
//    DisplayIcon(DT4,ON); //show moon			
//	}
}




void DisplayHeat_CoolIcon(uint8 mode)
{
	if(mode == HEAT_MODE)	
	{
		DisplayIcon(DT2,OFF);//unshow snow	
		DisplayIcon(DT8,ON); //show fire
	}
	else if(mode == COOL_MODE)	
	{
		DisplayIcon(DT2,ON);  //show snow	
		DisplayIcon(DT8,OFF); //unshow fire
	}
	else
	{
		DisplayIcon(DT2,OFF);//unshow snow
		DisplayIcon(DT8,OFF); //unshow fire		
	}
}



/*****************************************
FUNCTION NAME:putTime
FUNCTION:display time 
INPUT:hr,min,flop
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
 
void putTime(uint8 hr,uint8 min,uint8 flop_col,uint8 flop_hour,uint8 flop_min )
{
	uint8 hrH,hrL,minH,minL;
 
	hrH = hr/10;
	hrL = hr%10;
	minH = min/10;
	minL = min%10; 
	clean_digital(6);
	clean_digital(7);
	clean_digital(8);
	clean_digital(9); 
	if( hrH == 0 )
	{
		;			//		DispalyDigital( ArrayData[6][hrH*2] & ZERO );
					//		DispalyDigital( ArrayData[6][hrH*2+1] & ZERO );
	}
	else
	{
		if(!flop_hour)
		{	
			DispalyDigital( ArrayData[6][hrH*2] );
			DispalyDigital( ArrayData[6][hrH*2+1] );
	
		}
	
	}
	if(!flop_hour)
	{
		DispalyDigital( ArrayData[7][hrL*2] );  
		DispalyDigital( ArrayData[7][hrL*2 + 1] );  
	}
	if(!flop_min)
	{	
		DispalyDigital( ArrayData[8][minH*2] );
		DispalyDigital( ArrayData[8][minH*2 + 1] );
		
		DispalyDigital( ArrayData[9][minL*2] );
		DispalyDigital( ArrayData[9][minL*2 + 1]);
	}
	//	delay_us(100);
	if( !flop_col)
	{
		DisplayIcon( COL,ON );
	}
	else
		DisplayIcon( COL,OFF);
	updateCurrentIcon();
}
 

/*****************************************
FUNCTION NAME:putWeek
FUNCTION:display everyday of each week
INPUT:week
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
 
void putWeek(uint8 week)
{
	//close all the week days first.
	DisplayIcon( MON , OFF); 
	DisplayIcon( TUE , OFF);
	DisplayIcon( WED , OFF);
	DisplayIcon( THU , OFF);
	DisplayIcon( FRI , OFF);
	DisplayIcon( SAT , OFF);
	DisplayIcon( SUN , OFF);
	//show the week day we need.
	switch( week )
	{ 
		case 1: 
			DisplayIcon( MON , ON); 
			break;
		case 2: 
			DisplayIcon( TUE , ON); 
			break;
		case 3: 
			DisplayIcon( WED , ON); 
			break;
		case 4: 
			DisplayIcon( THU , ON); 
			break;
		case 5: 
			DisplayIcon( FRI , ON);
			break;
		case 6: 
			DisplayIcon( SAT , ON); 
			break;
		case 0: 
			DisplayIcon( SUN , ON); 
			break;
		default:
			break;
	}
}
 



void putDigitals1(uint16 uiNum,uint8 units)
{
	 
	uint8 DataHigh,DataMid,DataLow,DataDec;
	clean_digital(0);
	clean_digital(1);
	clean_digital(2); 
	if(uiNum < 2000) // the maximal value lcd can display.
	{
		DataHigh = uiNum/1000;
		DataMid  = uiNum%1000/100;
		DataLow  = uiNum%100/10;
		DataDec  = uiNum%10;
		 
		if(DataHigh == 0) DisplayIcon(DT,OFF);
		else DisplayIcon(DT,ON);
		
		if((DataHigh == 0)&&(DataMid == 0)) 
		{
			;
//			DispalyDigital( ArrayData[0][DataMid*2] & ZERO );
//			DispalyDigital( ArrayData[0][DataMid*2 + 1] & ZERO );
		}
		else
		{
			DispalyDigital( ArrayData[0][DataMid*2]);
			DispalyDigital( ArrayData[0][DataMid*2 + 1]);
		}
		
		DispalyDigital( ArrayData[1][DataLow*2]);
		DispalyDigital( ArrayData[1][DataLow*2 + 1]);
		
		DispalyDigital( ArrayData[2][DataDec*2]);
		DispalyDigital( ArrayData[2][DataDec*2 + 1]);  		 	 
	}
	else
	{ 
		DataMid	 = 10;
		DataLow  = 10;
		DataDec  = 10;
		DisplayIcon(DT,OFF);
		
		DispalyDigital( ArrayData[0][DataMid*2]);
		DispalyDigital( ArrayData[0][DataMid*2 + 1]); 
		
		DispalyDigital( ArrayData[1][DataLow*2]);
		DispalyDigital( ArrayData[1][DataLow*2 + 1]);
		
		DispalyDigital( ArrayData[2][DataDec*2]);
		DispalyDigital( ArrayData[2][DataDec*2 + 1]);  	
	} 	   
	if( units == UNIT_C )
	{ 
		DisplayIcon( DT15, OFF );  //disable unit F.
		DisplayIcon( DT16, ON );   //enable unit C .
	}
	else  
	{ 
		DisplayIcon( DT16, OFF );  //disable unit C.
		DisplayIcon( DT15, ON );   //enable unit F .
	}
	
	updateCurrentIcon();
}

/*****************************************
FUNCTION NAME:putDigitals2
FUNCTION:display data 
INPUT:uiNum
OUTPUT:none
CREATE BY:Jackie Cao
BREATE DATE:2007,12,26
MODIFY DATE:
*****************************************/
void putDigitals2(uint16 uiNum)
{
	uint8 DataHigh,DataMid,DataLow; 
	clean_digital(3);
	clean_digital(4);
	clean_digital(5); 
	if(uiNum < 1000) // the maximal value lcd can display.
	{
		DataHigh = uiNum/100;
		DataMid  = uiNum%100/10; 
		DataLow  = uiNum%10;
		 
		if(DataHigh == 0)
		{
			;
//			DispalyDigital( ArrayData[3][DataMid*2] & ZERO );
//			DispalyDigital( ArrayData[3][DataMid*2 + 1] & ZERO );
		}
		else
		{
			DispalyDigital( ArrayData[3][DataMid*2]);
			DispalyDigital( ArrayData[3][DataMid*2 + 1]);
		} 
		
		if((DataHigh == 0)&&(DataMid == 0)) 
		{
			;
//			DispalyDigital( ArrayData[4][DataMid*2] & ZERO );
//			DispalyDigital( ArrayData[4][DataMid*2 + 1] & ZERO );
		}
		else
		{
			DispalyDigital( ArrayData[4][DataMid*2]);
			DispalyDigital( ArrayData[4][DataMid*2 + 1]);
		}
		
		DispalyDigital( ArrayData[5][DataLow*2]);
		DispalyDigital( ArrayData[5][DataLow*2 + 1]); 		 	 
	}
	else
	{ 
		DataHigh  = 10; 
		DataMid	 = 10;
		DataLow  = 10;
		DispalyDigital( ArrayData[3][DataHigh*2]);
		DispalyDigital( ArrayData[3][DataHigh*2 + 1]);   
		
		DispalyDigital( ArrayData[4][DataMid*2]);
		DispalyDigital( ArrayData[4][DataMid*2 + 1]); 
		
		DispalyDigital( ArrayData[5][DataLow*2]);
		DispalyDigital( ArrayData[5][DataLow*2 + 1]);  		
	}  
	
	DisplayIcon( DT13, ON ); //show setting
	
	updateCurrentIcon();
 
} 
 
 
 
void Tstat7_Restart( void )
{ 
	putText(0,(uint8 *)"restart_");
	putText(1,(uint8 *)"ing...");   
}
	


void Tstat7_lcd_gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_11);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_15);
}
	 
void Tstat7_lcd_gpio_Init_REV22(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_11);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);
	GPIO_ResetBits(GPIOD, GPIO_Pin_1|GPIO_Pin_6);
} 



const uint16 icon[27] = {ZERO,MON,TUE,WED,THU,FRI,SAT,SUN,COL,DP,DT,
						DT1,DT2,DT3,DT4,DT5,DT6,DT7,DT8,DT9,DT10,DT11,DT12,DT13,DT14,DT15,DT16}; 

							
void HT1621_Test(short int  pos)
{
	unsigned   temp;  
	temp = abs(pos); 
	if(pos< 0)
		DisplayIcon( icon[temp],OFF);  
	else
		DisplayIcon( icon[pos],ON);
}	


void Digital_Test(short int pos,unsigned char data)
{
	unsigned char temp;
	temp = abs(pos);
	clean_digital(temp);
	if(pos < 0)
	{
		;
//		DispalyDigital( ArrayData[temp][data*2]  & ZERO); 
//		DispalyDigital( ArrayData[temp][data*2+1] & ZERO);

	}
	else
	{
		DispalyDigital( ArrayData[pos][data*2]  ); 
		DispalyDigital( ArrayData[pos][data*2+1] );
	}
}




 void LCD_WriteLByte(u8 B)//?PC???8?,?8???
 {
   u16 Data_PAL;
   Data_PAL = GPIO_ReadOutputData(GPIOD);
   Data_PAL = Data_PAL&0xFF00;//???8???
   Data_PAL = Data_PAL | B;//???8???
   GPIO_Write(GPIOD,Data_PAL );
 }
 
 void clrscr(void)//??
 {
   u8 i,page;
   for(page=0xb0;page<0xb8;page++)
   {
     LCD_WRITE_CMD(page);
     LCD_WRITE_CMD(0x17);//???,????????,??0???
     LCD_WRITE_CMD(0x04);//???,??0???
     for(i=0;i<16;i++)
     {LCD_WRITE_DAT(0);}
   }
 }

 

//***************************????******************************* 
void WRITE_STRING86(uint8 plat,uint8 column,uint8 *p)
{
 uint8 page,col,col_h,col_l;
 uint8 i;
 
 page=plat+Page0;
 col=column*6;
 col_h=((col>>4)|0xf0)&0x1f;
 col_l=col&0x0f;
 LCD_WRITE_CMD(0);
 LCD_WRITE_CMD(0);
 LCD_WRITE_CMD(page);
 

 LCD_WRITE_DAT(0x55);


}
//========================?????????================================= 
void LCDInit() 
{ 
	 LCDCS = 0; 
	 LCDRS=0;       //LCD ????(L) 
	 delay_ms(100); // ??100ms , Datasheet ??????1us
	 LCDRS = 1;    //LCD ????(H)                                     

	 delay_ms(100); //????

	 LCD_WRITE_CMD(0xe2);//软件复位
	 LCD_WRITE_CMD(0xaf);//显示开
	 LCD_WRITE_CMD(0x40);//显示RAM行地址  0x40为第0行
	 LCD_WRITE_CMD(0xa0);//LCD正向显示
	 LCD_WRITE_CMD(0xa6);//正常显示
	 LCD_WRITE_CMD(0xa5);//display all off
	 LCD_WRITE_CMD(0xa2);//偏压比设定
	 LCD_WRITE_CMD(0xc8);//0xc8第一行在屏幕上方
	 LCD_WRITE_CMD(0x2f);//开关内部电源
	 LCD_WRITE_CMD(0x24);//对比度设置
	 LCD_WRITE_CMD(0x81);//进入细调设命令
	 LCD_WRITE_CMD(0x24);
	 
	 delay_ms(1000);
	 LCD_WRITE_CMD(0xa4);//display all oN
      
} 

//========================?????(???????)============================= 
void LCDWriteByte(char dat,char command) 
{   
//                 LCDCS = 0;                         //CS=0.?:??????CS,???????.   
	 if(command == 0)       
	 {   
		 LCDA0 = 0;                           //A0=0,????   
		 LCDShiftWrite(dat);   
	 }   
	 else                  
	 {   
		 LCDA0 = 1;                          //A0=1,????   
		 LCDShiftWrite(dat);     
	 }   
//        LCDCS = 1;                                 //CS=1.?:??????CS,???????. 
} 


void LCD_WRITE_CMD(uint8 dat)
{
		LCDA0 = 0;                           //A0=0,????   
    LCDShiftWrite(dat);   
}

void LCD_WRITE_DAT(uint8 dat)
{
		LCDA0 = 1;                           //A0=0,????   
    LCDShiftWrite(dat);   
}
//===========================??(??)??????================================ 
void LCDShiftWrite(char dat)   
{    
     unsigned char i;                                      //?????? 
     unsigned char Series,Temp;                           //?????? 
       
     LCDSK = 0;                                           //SCL=0   
     Series = dat;   
     for(i=0; i<8; i++)   
     {   
			 LCDSK = 0;                              //SCL=0   
			 Temp=Series & 0x80;                    //???7?   
			 if(Temp)                               //??Temp=1,??SI=1.??Temp+0,??SI=0 
			 {   
							 LCDSI = 1;                    //SI=1   
			 }   
			 else    
			 {   
							 LCDSI = 0;                   //SI=0   
			 } 
			 LCDSK = 1;                          //SCL=1????      
			 Series = Series << 1;               //??1?    
     }   
} 

/********************************end of ST7565 driver******************************/
void putTextST7565(unsigned char Part,unsigned char *pStr)
{
	unsigned char  i,j;
	unsigned char  Loop;
	unsigned char  Data[8][8];

	uint8 reverse_buf[5][8];//for ST7565

	for( i = 0; i < 8; i++ )
	{
		for( j = 0; j < 8; j++ )
		{
			Data[i][j] = ALLData[*(pStr+i)][j];
		}
	}	 
	i = 0;
	j = 8;

	for( i = 0; i < 8; i++ )
	{Data[0][i] = Data[0][i] | (Data[1][i] << 5);}
	
	for( i = 0; i < 8; i++ )
	{Data[1][i] = (Data[1][i] >> 3) | (Data[2][i] << 2) | (Data[3][i] << 7);}

	for( i = 0; i < 8; i++ )
	{Data[2][i] = (Data[3][i] >> 1) | (Data[4][i] << 4);}

	for( i = 0; i < 8; i++ )
	{Data[3][i] = (Data[4][i] >> 4) | (Data[5][i] << 1) | (Data[6][i] << 6);}

	for( i = 0; i < 8; i++ )
	{Data[4][i] = (Data[6][i] >> 2) | (Data[7][i] << 3);}
	

	for( Loop = 0; Loop <5; Loop++ )
	{
		for(j=0;j<8;j++)
		{
			for(i=0;i<8;i++)
			{
				if(GetByteBit(&Data[Loop][j],i))
					SetByteBit(&reverse_buf[4-Loop][j],7-i,1);
				else
					SetByteBit(&reverse_buf[4-Loop][j],7-i,0);
			}
		}
	}		
	

	for( Loop = 0; Loop <5; Loop++ )
	{	
//		I2CStart();
//		I2CSendByte( 0x78 );//chip address	
//		I2CSendByte( 0x00 );
//		I2CSendByte( 0x30 );
//		I2CSendByte( 0x40 + Loop );
//		if( Part == 0 )	
//			I2CSendByte( 0x88 );
//		else
//			I2CSendByte( 0x80 );
//		I2CStop();

//		I2CStart();	
//		I2CSendByte( 0x78 );//chip address	
//		I2CSendByte( 0x40 );
  	 if( Part == 0 )
		 {
		 LCD_WRITE_CMD(0xb1+Loop);
		 LCD_WRITE_CMD(0x17);//diaplay colum high 4 bits
		 LCD_WRITE_CMD(0x04);//diaplay colum low 4 bit 
		 }
		else
		{
		 LCD_WRITE_CMD(0xb1+Loop);
		 LCD_WRITE_CMD(0x17);//diaplay colum high 4 bits
		 LCD_WRITE_CMD(0x0c);//diaplay colum low 4 bit 
		}			

		for(i = 8;i > 0;i--)
		{
      LCD_WRITE_DAT(reverse_buf[Loop][i-1] );			
		}
	}
}

void Newlcdtest(void) 
{                          
 u8 i,page;

	 LCDInit();

	clrscr();

	putText(0,(uint8 *)"TEMPAUTO"); 
	putText(1,(uint8 *)"12345678"); 
} 





#endif

