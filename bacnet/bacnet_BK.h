#ifndef __BACNET_H__
#define __BACNET_H__

#include <string.h>
#include "types.h"
#include "stdint.h"


#define BAC_MSTP 0
#define BAC_IP 1
#define BAC_GSM 2
#define MODBUS  	3



/* Enable the Gateway (Routing) functionality here, if desired. */
#if !defined(MAX_NUM_DEVICES)
#ifdef BAC_ROUTING
#define MAX_NUM_DEVICES 3       /* Eg, Gateway + two remote devices */
#else
#define MAX_NUM_DEVICES 1       /* Just the one normal BACnet Device Object */
#endif
#endif


/* Define your processor architecture as
   Big Endian (PowerPC,68K,Sparc) or Little Endian (Intel,AVR)
   ARM and MIPS can be either - what is your setup? */
#if !defined(BIG_ENDIAN)
#define BIG_ENDIAN 1
#endif

/* Define your Vendor Identifier assigned by ASHRAE */
#if !defined(BACNET_VENDOR_ID)
#define BACNET_VENDOR_ID 260
#endif
#if !defined(BACNET_VENDOR_NAME)
#define BACNET_VENDOR_NAME "Temco controls"
#endif

/* Max number of bytes in an APDU. */
/* Typical sizes are 50, 128, 206, 480, 1024, and 1476 octets */
/* This is used in constructing messages and to tell others our limits */
/* 50 is the minimum; adjust to your memory and physical layer constraints */
/* Lon=206, MS/TP=480, ARCNET=480, Ethernet=1476, BACnet/IP=1476 */
#if !defined(MAX_APDU)
    /* #define MAX_APDU 50 */
    /* #define MAX_APDU 1476 */


#define MAX_APDU 600//500//1476
/* #define MAX_APDU 128 enable this IP for testing readrange so you get the More Follows flag set */
#elif defined (BACDL_ETHERNET)
#define MAX_APDU 1476
#else
#define MAX_APDU 480
#endif //!defined(MAX_APDU)


	   

/* for confirmed messages, this is the number of transactions */
/* that we hold in a queue waiting for timeout. */
/* Configure to zero if you don't want any confirmed messages */
/* Configure from 1..255 for number of outstanding confirmed */
/* requests available. */
#if !defined(MAX_TSM_TRANSACTIONS)
#define MAX_TSM_TRANSACTIONS  20//255 //????????????? changed by chelsea
#endif
/* The address cache is used for binding to BACnet devices */
/* The number of entries corresponds to the number of */
/* devices that might respond to an I-Am on the network. */
/* If your device is a simple server and does not need to bind, */
/* then you don't need to use this. */
#if !defined(MAX_ADDRESS_CACHE)
#define MAX_ADDRESS_CACHE 255
#endif

/* some modules have debugging enabled using PRINT_ENABLED */
#if !defined(PRINT_ENABLED)
#define PRINT_ENABLED 0
#endif

/* BACAPP decodes WriteProperty service requests
   Choose the datatypes that your application supports */


#define BACAPP_NULL
#define BACAPP_BOOLEAN
#define BACAPP_UNSIGNED
#define BACAPP_SIGNED
#define BACAPP_REAL
#define BACAPP_DOUBLE
#define BACAPP_OCTET_STRING
#define BACAPP_CHARACTER_STRING
#define BACAPP_BIT_STRING
#define BACAPP_ENUMERATED
#define BACAPP_DATE
#define BACAPP_TIME
#define BACAPP_OBJECT_ID


#define MAX_BITSTRING_BYTES (15)


#ifndef MAX_CHARACTER_STRING_BYTES
#define MAX_CHARACTER_STRING_BYTES (MAX_APDU-6)
#endif

#ifndef MAX_OCTET_STRING_BYTES
#define MAX_OCTET_STRING_BYTES (MAX_APDU-6)
#endif


#define BACNET_SVC_I_HAVE_A    1
#define BACNET_SVC_WP_A        1
#define BACNET_SVC_RP_A        1
#define BACNET_SVC_RPM_A       1
#define BACNET_SVC_DCC_A       1
#define BACNET_SVC_RD_A        1
#define BACNET_SVC_TS_A        1
#define BACNET_SVC_SERVER      0
#define BACNET_USE_OCTETSTRING 1
#define BACNET_USE_DOUBLE      1
#define BACNET_USE_SIGNED      1


#include "dlmstp.h"
#include "datalink.h"
#include "device.h"
#include "handlers.h"
#include "whois.h"
#include "address.h"
#include "client.h"



#define MAX_AVS  50    
#define MAX_AIS  8
#define MAX_AOS  2
#define MAX_BIS  0
#define MAX_BOS  7


// for ASIX
void switch_to_modbus(void);   // receive modbus frame when current protocal is mstp,switch to modbus
//void UART_Init(U8_T port);



#define BAC_COMMON  1

// select current chip
#define ASIX				0
#define ARM					1
uint8_t RS485_Get_Baudrate();

#if ASIX

#include 	"reg80390.h"
#include "bip.h"

#define BACDL_ALL 	
#define BACDL_BIP
#define BACDL_MSTP

#define BBMD_ENABLED 1 
#define BAC_PRIVATE	1
#define TXEN  P2_0

extern U16_T far hsurRxCount;
extern U16_T far Test[50];

void TCPIP_UdpClose(uint8_t);
void BIP_Receive_Handler(U8_T xdata * pData, U16_T length, U8_T id);
U8_T TCPIP_Bind(U8_T (* )(U32_T xdata*, U16_T, U8_T), void (* )(U8_T, U8_T), void (* )(U8_T xdata*, U16_T, U8_T));
U8_T TCPIP_UdpListen(U16_T, U8_T);
void TCPIP_UdpSend(U8_T, U8_T*,U8_T, U8_T*, U16_T);

void uart_send_string(U8_T *p, U16_T length,U8_T port);
void UART_Init(U8_T port);
#endif //ASIX

#if ARM
#include "bitmap.h"
 
//#define BIP   // TSTAT dont have it

#define BACDL_ALL
#define BACDL_MSTP

#define BAC_PRIVATE	0


#ifdef BIP
#include "bip.h"
//#include "tcpip.h" 
#define BBMD_ENABLED 1
#define BACDL_BIP
#endif  //BIP

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
char get_range(uint8_t type,uint8_t num);//get unit
char get_AM_Status(uint8_t type,uint8_t num);//auto manual
U16_T Get_bacnet_value_from_buf(uint8_t type,uint8_t priority,uint8_t i);

void wirte_bacnet_value_to_buf(uint8_t type,uint8_t priority,uint8_t i,int16_t unit);
void write_bacnet_unit_to_buf(uint8_t type,uint8_t priority,uint8_t i,uint8_t unit);
void write_bacnet_name_to_buf(uint8_t type,uint8_t priority,uint8_t i,char* str);
void write_bacent_AM_to_buf(uint8_t type,uint8_t i,uint8_t am);
typedef enum
{
 AV,AI,AO,BI,BO
}BACNET_type;


#if BAC_PRIVATE



//#include "user_data.h"
extern uint8_t far MSTP_Rec_buffer[600];
extern uint8_t MSTP_Write_OK;
extern uint8_t MSTP_Transfer_OK;
extern uint8_t MSTP_Transfer_Len;
extern uint8_t remote_panel_num;

U8_T Get_current_panel(void);
void chech_mstp_collision(void);
void check_mstp_packet_error(void);
void check_mstp_timeout(void);

void Send_SUB_I_Am(uint8_t index);
//void add_remote_panel_db(uint32_t device_id,uint8_t panel);

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

#endif //BAC_PRIVATE


#if BAC_COMMON


extern uint8_t  AVS;
extern uint8_t  AIS;
extern uint8_t  AOS;
extern uint8_t  BIS;
extern uint8_t  BOS;


#endif //BAC_COMMON




extern unsigned char far Temp_Buf[MAX_APDU];

#endif //__BACNET_H__












