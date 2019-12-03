#ifndef __MYIIC_H
#define __MYIIC_H

#include "bitmap.h"
   	   		   
////IO方向设置
//#define SDA_IN()	{GPIOB->CRL &= 0XFFFFFFF0; GPIOB->CRL |= ((u32)8 << 0);}
//#define SDA_OUT()	{GPIOB->CRL &= 0XFFFFFFF0; GPIOB->CRL |= ((u32)3 << 0);}

////IO操作函数	 
//#define IIC_SCL		PBout(1)	//SCL
//#define IIC_SDA		PBout(0)	//SDA	 
//#define READ_SDA	PBin(0)		//输入SDA 
//#define IIC_WP		PCout(5)

////IO方向设置
//#define SDA_IN()	{GPIOA->CRL &= 0XFFFFFFF0; GPIOA->CRL |= ((u32)8 << 0);}
//#define SDA_OUT()	{GPIOA->CRL &= 0XFFFFFFF0; GPIOA->CRL |= ((u32)3 << 0);}

////IO操作函数	 
//#define IIC_SCL		PAout(3)	//SCL
//#define IIC_SDA		PAout(2)	//SDA	 
//#define READ_SDA	PAin(2)		//输入SDA 
//#define IIC_WP		PAout(0)



//IIC所有操作函数
void SDA_OUT(void);
void SDA_IN(void);
void SCL_OUT(void);
void SCL_IN(void);


void IIC_Init(void);				//初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);				//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void);				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);
//void SDA_IN(void);
u8 READ_SDA(void);
u8 READ_SCL(void);
void IIC_SDA(u8 status);
void IIC_SCL(u8 status);
#endif
