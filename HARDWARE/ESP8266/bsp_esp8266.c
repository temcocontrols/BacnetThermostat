#include "bsp_esp8266.h"
//#include "common.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "stm32f10x_iwdg.h"
#include "delay.h"
#include "productModel.h"
#include "define.h"
#include "revision.h"
#include "eepdefine.h"
#include "store.h"

extern uint8 update_flag;
//WIFI_STR AQ;
STR_MODBUS Modbus;
STR_SCAN_CMD Infor[20];
STR_SCAN_CMD Scan_Infor;
uint8 transactionID[6];


unsigned char STA_ip[4];

static void                   ESP8266_GPIO_Config                 ( void );
static void                   ESP8266_USART_Config                ( void );
static void                   ESP8266_USART_NVIC_Configuration    ( void );



struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

//#include "common.h"
#include "stm32f10x.h"
#include <stdarg.h>



char *  itoa ( int value, char * string, int radix );

/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];

	
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //换行符
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//十进制
				d = va_arg(ap, int);
				
				itoa(d, buf, 10);
				
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;
				
				default:
				Data++;
				
				break;
				
			}		 
		}
		
		else USART_SendData(USARTx, *Data++);
		
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
		
	}
}


/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */
char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */





/**
  * @brief  ESP8266初始化函数
  * @param  无
  * @retval 无
  */
void ESP8266_Init ( void )
{
	ESP8266_GPIO_Config (); 
	
	ESP8266_USART_Config (); 
	
	
	macESP8266_RST_HIGH_LEVEL();

//	macESP8266_CH_DISABLE();
	
	
}


/**
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
static void ESP8266_GPIO_Config ( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 配置 CH_PD 引脚*/
//	macESP8266_CH_PD_APBxClock_FUN ( macESP8266_CH_PD_CLK, ENABLE ); 
//											   
//	GPIO_InitStructure.GPIO_Pin = macESP8266_CH_PD_PIN;	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

//	GPIO_Init ( macESP8266_CH_PD_PORT, & GPIO_InitStructure );	 


	
	/* 配置 RST 引脚*/
	macESP8266_RST_APBxClock_FUN ( macESP8266_RST_CLK, ENABLE ); 
											   
	GPIO_InitStructure.GPIO_Pin = macESP8266_RST_PIN;	

	GPIO_Init ( macESP8266_RST_PORT, & GPIO_InitStructure );	 


}


/**
  * @brief  初始化ESP8266用到的 USART
  * @param  无
  * @retval 无
  */
static void ESP8266_USART_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	/* config USART clock */
	macESP8266_USART_APBxClock_FUN ( macESP8266_USART_CLK, ENABLE );
	macESP8266_USART_GPIO_APBxClock_FUN ( macESP8266_USART_GPIO_CLK, ENABLE );
	
	/* USART GPIO config */
	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  macESP8266_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macESP8266_USART_TX_PORT, &GPIO_InitStructure);  
  
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = macESP8266_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macESP8266_USART_RX_PORT, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = macESP8266_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(macESP8266_USARTx, &USART_InitStructure);
	
	
	/* 中断配置 */
	USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断 
	USART_ITConfig ( macESP8266_USARTx, USART_IT_IDLE, ENABLE ); //使能串口总线空闲中断 	

	ESP8266_USART_NVIC_Configuration ();
	
	
	USART_Cmd(macESP8266_USARTx, ENABLE);
	
	
}


/**
  * @brief  配置 ESP8266 USART 的 NVIC 中断
  * @param  无
  * @retval 无
  */
static void ESP8266_USART_NVIC_Configuration ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_2 );

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = macESP8266_USART_IRQ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


/*
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 ESP8266_AT_Test 调用
 */
void ESP8266_Rst ( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
	 macESP8266_RST_LOW_LEVEL();
	 delay_ms ( 500 ); 
	 macESP8266_RST_HIGH_LEVEL();
	#endif

}

bool ESP8266_Cmd_unit8 ( uint8_t * cmd, char * reply1, char * reply2, uint16_t len,u32 waittime )
{    
	uint16_t i;
	uint16_t count;
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

//	macESP8266_Usart ( "%s\r\n", cmd );
	
	for(i = 0;i < len;i++)
	{	
		USART_SendData(UART4, cmd[i]);
		while( USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET );
	}
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	count = 0;
	while (( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag)  && (count++ < 10))
	{
		delay_ms(50) ;
		IWDG_ReloadCounter();
	}
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
//	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
//		return true;
//	
//	delay_ms ( waittime );                 //延时
//	
//	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

//	macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else 
		if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}
/*
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
//	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包
	uint16 count;
	macESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
//	delay_ms ( waittime );                 //延时
//	
//	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	count = 0;
	while ( (! strEsp8266_Fram_Record .InfBit .FramFinishFlag) && (count++ < 10))
	{
		delay_ms(50) ;
		IWDG_ReloadCounter();
	}
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
//	macPC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
  
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}


/*
 * 函数名：ESP8266_AT_Test
 * 描述  ：对WF-ESP8266模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
//void ESP8266_AT_Test ( void )
//{
//	macESP8266_RST_HIGH_LEVEL();
//	
//	delay_ms ( 1000 ); 
//	
//	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) ESP8266_Rst ();  	

//}
char ESP8266_AT_Test ( void )
{
	char count=0;
	
	macESP8266_RST_HIGH_LEVEL();	
	delay_ms ( 1000 );
	while ( count < 10 )
	{
		if( ESP8266_Cmd ( "AT", "OK", NULL, 500 ) ) 
			return 2;
		ESP8266_Rst();
		++ count;
	}
	if(count == 10)
		return 0;
	else
		return 1;
}


/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	switch ( enumMode )
	{
		case STA:
			return ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2500 ); 
		
	  case AP:
		  return ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2500 ); 
		
		case STA_AP:
		  return ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2500 ); 
		
	  default:
		  return false;
  }
	
}


/*
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 5000 );
	
}

bool ESP8266_JoinAP_DEF( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP_DEF=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_Cmd ( cCmd, "OK", NULL, 5000 );
	
}
extern u16 far Test[50];
bool ESP8266_CIPSTA_DEF (void)
{
	char i;
	char cStr [100] = { 0 }, cCmd [120];
	char strip[4][4];
	char strsubnet[4][4];
	char strgetway[4][4];
	
	
	for(i=0;i<4;i++)
	{
		itoa(SSID_Info.ip_addr[i],strip[i],10);
	}
	for(i=0;i<4;i++)
		itoa(SSID_Info.net_mask[i],strsubnet[i],10);
	for(i=0;i<4;i++)
		itoa(SSID_Info.getway[i],strgetway[i],10);
  
	//sprintf ( cStr, "\"%s.%s.%s.%s\",\"192.168.10.1\",\"255.255.255.0\"", strip[0],strip[1],strip[2],strip[3]);	
	sprintf (cStr, "\"%s.%s.%s.%s\",\"%s.%s.%s.%s\",\"%s.%s.%s.%s\"", 
	strip[0],strip[1],strip[2],strip[3],
	strgetway[0],strgetway[1],strgetway[2],strgetway[3],
	strsubnet[0],strsubnet[1],strsubnet[2],strsubnet[3]);

  sprintf ( cCmd, "AT+CIPSTA_DEF=%s",cStr);
//	sprintf ( cCmd, "AT+CIPSTA_DEF=\"192.168.10.100\",\"192.168.10.1\",\"255.255.255.0\"");

	return ESP8266_Cmd (cCmd, "OK",0, 4000 );
	
}
/*
 * 函数名：ESP8266_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	
	return ESP8266_Cmd ( cCmd, "OK", 0, 1000 );
	
}


/*
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char cStr [20];
	
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_Cmd ( cStr, "OK", 0, 500 );
	
}


/*
 * 函数名：ESP8266_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
}

bool ESP8266_Link_UDP( char * ip, uint16_t remoteport, uint16_t localport, uint8_t udpmode,ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];


	sprintf ( cStr, "\"%s\",\"%s\",%d,%d,%d", "UDP", ip, remoteport, localport,udpmode);


  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
}
/*
 * 函数名：ESP8266_StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
 */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ));
//		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );
//		
//		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 500 ) &&
//						 ESP8266_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}


/*
 * 函数名：ESP8266_Get_LinkStatus
 * 描述  ：获取 WF-ESP8266 的连接状态，较适合单端口时使用
 * 输入  ：无
 * 返回  : 2，已连接AP,获得IP
 *         3，已建立连接
 *         4，断开网络连接
 *				 5，未连接AP
 *         0，获取状态失败
 *  			 6, WIFI_SSID_FAIL
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;	
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:5\r\n" ) )
			return 5;	 // no link

	}
	
	return 6;
	
}


/*
 * 函数名：ESP8266_Get_IdLinkStatus
 * 描述  ：获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
 * 输入  ：无
 * 返回  : 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	

	}
	
	return ucIdLinkStatus;
	
}


/*
 * 函数名：ESP8266_Inquire_ApIp
 * 描述  ：获取 F-ESP8266 的 AP IP
 * 输入  ：pApIp，存放 AP IP 的数组的首地址
 *         ucArrayLength，存放 AP IP 的数组的长度
 * 返回  : 0，获取失败
 *         1，获取成功
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Inquire_ApIp (uint8_t * pStamac, uint8_t * pStaIp,uint8_t ucArrayLength )
{
	char uc;
	
	char * pCh;
	char i;
	
	char pos;
	char datlen;
	uint8_t ip[4];
	uint8_t mac[6];
	
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pos = 0;
	datlen = 0;
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STAIP,\"" );
	
	if ( pCh )
		pCh += 7;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		if(*( pCh + uc) == '.')
		{
			if(datlen == 1)
				pStaIp[pos] = ip[0];
			else if(datlen == 2)
				pStaIp[pos] = ip[0] * 10 + ip[1];
			else if(datlen == 3)
				pStaIp[pos] = ip[0] * 100 + ip[1] * 10 + ip[2];
			
			pos++;
			datlen = 0;
		}
		else
		{
			if((pos == 3) && (* ( pCh + uc) == '\"'))
				break;
			
			ip[datlen++] = * ( pCh + uc) - '0';
		}		

		
	}
	
	if(datlen == 1)
		pStaIp[pos] = ip[0];
	else if(datlen == 2)
		pStaIp[pos] = ip[0] * 10 + ip[1];
	else if(datlen == 3)
		pStaIp[pos] = ip[0] * 100 + ip[1] * 10 + ip[2];
	
// GET MAC address
	ESP8266_Cmd ( "AT+CIPSTAMAC?", "OK", 0, 500 );	
	pos = 0;
	datlen = 0;
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTAMAC:\"" );
	
//	AT+CIPSTAMAC?

//+CIPSTAMAC:"ec:fa:bc:40:e0:f0"

//OK
	if ( pCh )
		pCh += 11;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		if(*( pCh + uc) == ':')
		{
			if(datlen == 2)
				pStamac[pos] = mac[0] * 16 + mac[1];
			
			pos++;
			datlen = 0;
		}
		else
		{
			if((pos == 5) && (* ( pCh + uc) == '\"'))
				break;
			
			if((* ( pCh + uc) >= '0') && (* ( pCh + uc) <= '9'))
				mac[datlen++] = * ( pCh + uc) - '0';
			else if((* ( pCh + uc) >= 'a') && (* ( pCh + uc) <= 'f'))
				mac[datlen++] = * ( pCh + uc) - 'a' + 10;
		}	
		
	}
	
	if(datlen == 2)
		pStamac[pos] = mac[0] * 16 + mac[1];	
	return 1;
	
}



/*
 * 函数名：ESP8266_UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_UnvarnishSend ( void )
{
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) )
		return false;
	
	return 
	  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 500 );
	
}


/*
 * 函数名：ESP8266_ExitUnvarnishSend
 * 描述  ：配置WF-ESP8266模块退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_ExitUnvarnishSend ( void )
{
	delay_ms ( 1000 );
	
	macESP8266_Usart ( "+++" );
	
	delay_ms ( 500 ); 
	
}


/*
 * 函数名：ESP8266_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, uint8_t * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	uint8_t cStr [540];
	bool bRet = false;
	
	if(ulStrLength > 500) 
		return 0;
	if ( enumEnUnvarnishTx )
	{
		macESP8266_Usart ( "%s", pStr );
		bRet = true;		
	}
	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength);
		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength);
		
		macESP8266_Usart ( "%s\r\n", cStr );
		delay_ms ( 200 );
		//bRet = ESP8266_Cmd ( cStr, "> ", 0, 500 );
		//printf("bRet = %d\r\d",bRet);
		//bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 1000 );
		bRet = ESP8266_Cmd_unit8 ( pStr, "SEND OK", 0, ulStrLength,100 );
		
  }
	
	return bRet;

}


/*
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
 */
uint8_t * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	uint8_t * pRecStr = NULL;
	uint16_t count;
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	memset(&strEsp8266_Fram_Record,0,sizeof(strEsp8266_Fram_Record));
	
	count = 0;
	while ( (! strEsp8266_Fram_Record .InfBit .FramFinishFlag) && (count++ < 100))
	{
		delay_ms(2) ;
		if(update_flag == 24)
	 {		 	 
		 if(ESP8266_CIPSTA_DEF())
		 {
			SSID_Info.ip_addr[0] = IPnum(0);
			SSID_Info.ip_addr[1] = IPnum(1);
			SSID_Info.ip_addr[2] = IPnum(2);
			SSID_Info.ip_addr[3] = IPnum(3);	
			ESP8266_Rst();	
		  update_flag = 0;
		 }
	 }
	 else if(update_flag == 25)
	 {
		 if(ESP8266_CWDHCP_DEF())
		 {
			ESP8266_Rst();	
		  update_flag = 0;
		 }	 
	 }
	 else if(update_flag	== 22)	//factory setting
	 {
	  if(ESP8266_Cmd ( "AT+RESTORE", "OK", 0, 500))
		update_flag = 0;
		// Clear SSID
			memset(SSID_Info.name,0,64);
			memset(SSID_Info.password,0,32);
			write_page_en[WIFI_TYPE] = 1; 
			Flash_Write_Mass();
	 }	
		
		IWDG_ReloadCounter();
	}
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';

	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
//	memset(&strEsp8266_Fram_Record,0,sizeof(strEsp8266_Fram_Record));
	return pRecStr;
	
}



uint16_t check_packet(uint8_t * str,uint8_t * dat)
{
	//  +IPD,0,12:...
	uint16_t len = 0;
	uint16_t i;
	if(str[8] == ',')
	{
		if((str[10] == ':') && (str[9] >= '0' && str[9] <= '9'))
		{
			len = str[9] - '0';
			for(i = 0;i < len;i++)
				*dat++ = str[11 + i];
			return len;
		}
		else if(str[11] == ':')
		{
			if(str[9] >= '0' && str[9] <= '9' && str[10] >= '0' && str[10] <= '9')
			{
				len = (str[9] - '0') * 10 + (str[10] - '0');
				for(i = 0;i < len;i++)
					*dat++ = str[12 + i];
				return len;
			}
		}
		else if(str[12] == ':')
		{
			if(str[9] >= '0' && str[9] <= '9' && str[10] >= '0' && str[10] <= '9' && str[11] >= '0' && str[11] <= '9')
			{
				len = (str[9] - '0') * 100 + (str[10] - '0') * 10 + (str[11] - '0');
				for(i = 0;i < len;i++)
					*dat++ = str[13 + i];
				return len;
			}
		}
		return 0;
	}
	return 0;
}
//-----------------------------------------------------
void UdpData(unsigned char type)
{
	// header 2 bytes
	memset(&Scan_Infor,0,sizeof(STR_SCAN_CMD));
	if(type == 0)
		Scan_Infor.cmd = 0x0065;
	else if(type == 1)
		Scan_Infor.cmd = 0x0067;

	Scan_Infor.len = 0x001d;
	
	//serialnumber 4 bytes
	Scan_Infor.own_sn[0] = SerialNumber(0);//(unsigned short int)Modbus.serialNum[0];
	Scan_Infor.own_sn[1] = SerialNumber(1);//(unsigned short int)Modbus.serialNum[1];
	Scan_Infor.own_sn[2] = SerialNumber(2);//(unsigned short int)Modbus.serialNum[2];
	Scan_Infor.own_sn[3] = SerialNumber(3);//(unsigned short int)Modbus.serialNum[3];
	
	Scan_Infor.product = PM_TSTAT8;//PRODUCT_MINI_ARM;  // only for test now

	//modbus address
	Scan_Infor.address = laddress;//(unsigned short int)Modbus.address;
	
	//Ip
	Scan_Infor.ipaddr[0] = (unsigned short int)SSID_Info.ip_addr[0];
	Scan_Infor.ipaddr[1] = (unsigned short int)SSID_Info.ip_addr[1];
	Scan_Infor.ipaddr[2] = (unsigned short int)SSID_Info.ip_addr[2];
	Scan_Infor.ipaddr[3] = (unsigned short int)SSID_Info.ip_addr[3];
	
	//port
	Scan_Infor.modbus_port = SSID_Info.modbus_port;  //tbd :???????????????? 

	// software rev
	Scan_Infor.firmwarerev = EEPROM_VERSION;
	// hardware rev
	Scan_Infor.hardwarerev = HardwareVersion;
	
	Scan_Infor.instance_low = HTONS(Instance); // hight byte first
	Scan_Infor.panel_number = laddress; //  36	
	Scan_Infor.instance_hi = HTONS(Instance >> 16); // hight byte first
	
	Scan_Infor.bootloader = 0;  // 0 - app, 1 - bootloader, 2 - wrong bootloader
	Scan_Infor.BAC_port = SSID_Info.bacnet_port;//((Modbus.Bip_port & 0x00ff) << 8) + (Modbus.Bip_port >> 8);  // 
	Scan_Infor.zigbee_exist = 0; // 0 - inexsit, 1 - exist
	Scan_Infor.subnet_protocal = 0;
	state = 1;
//	scanstart = 0;

}
//---------------------------------------------

void Set_transaction_ID(uint8_t *str, unsigned short int id, unsigned short int num)
{
	str[0] = (U8_T)(id >> 8);		//transaction id
	str[1] = (U8_T)id;

	str[2] = 0;						//protocol id, modbus protocol = 0
	str[3] = 0;

	str[4] = (U8_T)(num >> 8);
	str[5] = (U8_T)num;
}
//-------------------------------------------------


bool ESP8266_CWDHCP_DEF (void)
{	
	char cStr [100] = { 0 };
  char cCmd [20];
  sprintf ( cCmd, "AT+CWDHCP_DEF=1,1",cStr);

	return ESP8266_Cmd (cCmd, "OK",0, 4000 );
	
}
