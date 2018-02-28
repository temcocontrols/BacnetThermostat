#ifndef __BACNET_H__
#define __BACNET_H__

#include <string.h>
#include "types.h"
#include "config.h"

#include "dlmstp.h"
#include "datalink.h"
#include "device.h"
#include "handlers.h"
#include "whois.h"
#include "address.h"
#include "client.h"



#define MAX_AVS  32    
#define MAX_AIS  32
#define MAX_AOS  12
#define MAX_BIS  5
#define MAX_BOS  12




// for ASIX
void switch_to_modbus(void);   // receive modbus frame when current protocal is mstp,switch to modbus
//void UART_Init(U8_T port);


#define BAC_PRIVATE	1
#define BAC_COMMON  1

// select current chip
//#define ASIX				1
#define ARM					1



uint8_t RS485_Get_Baudrate();

#if ASIX

#include 	"reg80390.h"                                                                 

#define TXEN  P2_0

extern U16_T far hsurRxCount;
extern U16_T far Test[50];3

void TCPIP_UdpClose(uint8_t);
void BIP_Receive_Handler(U8_T xdata * pData, U16_T length, U8_T id);
U8_T TCPIP_Bind(U8_T (* )(U32_T xdata*, U16_T, U8_T), void (* )(U8_T, U8_T), void (* )(U8_T xdata*, U16_T, U8_T));
U8_T TCPIP_UdpListen(U16_T, U8_T);
void TCPIP_UdpSend(U8_T, U8_T*,U8_T, U8_T*, U16_T);

void uart_send_string(U8_T *p, U16_T length,U8_T port);

#endif

#if ARM
#include "bitmap.h"


#define far 
#define xdata 

#define TXEN		PAout(8)

extern u16 far Test[50];
extern u8 uart_send[512] ;
extern uint16_t send_count;

extern uint16_t uip_len;

void USART_SendDataString(u16 );


#endif


extern uint8_t TransmitPacket[600];
extern uint8_t TransmitPacket_panel;
extern uint8_t Send_Private_Flag;
extern uint8_t MSTP_Transfer_OK;
char* get_label(uint8_t type,uint8_t num);
char* get_description(uint8_t type,uint8_t num);
char get_range(uint8_t type,uint8_t num);
void wirte_bacnet_value_to_buf(uint8_t type,uint8_t priority,uint8_t i);

#if BAC_PRIVATE

typedef enum
{
 AV,AI,AO,BI,BO
}BACNET_type;


//#include "user_data.h"
extern uint8_t far MSTP_Rec_buffer[600];
extern uint8_t MSTP_Write_OK;
extern uint8_t MSTP_Transfer_OK;
extern uint8_t MSTP_Transfer_Len;



void Tansfer_Mstp_pdu( uint8_t * pdu,uint16_t pdu_len);

void handler_conf_private_trans_ack( 
    uint8_t * service_request,
    uint16_t service_len,
    uint8_t * apdu,
    int apdu_len);

	void Handler_Complex_Ack(
    uint8_t * apdu,
    int apdu_len       /* total length of the apdu */
    );
		
uint8_t Send_Mstp(uint8_t flag,uint8_t *type);		

#endif


#if BAC_COMMON


extern uint8_t  AVS;
extern uint8_t  AIS;
extern uint8_t  AOS;
extern uint8_t  BIS;
extern uint8_t  BOS;

extern uint16_t AI_Present_Value[MAX_AIS];
extern uint16_t AO_Present_Value[MAX_AOS][BACNET_MAX_PRIORITY];
extern uint16_t AV_Present_Value[MAX_AVS];
extern BACNET_BINARY_PV BI_Present_Value[MAX_BIS];
extern BACNET_BINARY_PV BO_Present_Value[MAX_BOS][BACNET_MAX_PRIORITY];

#endif



#endif



