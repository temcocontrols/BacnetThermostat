#ifndef _TCPIP_H
#define _TCPIP_H

#include <string.h>
#include "stm32f10x.h"
//#include "delay.h"
//#include "dma.h"
//#include "vmalloc.h"
//#include "enc28j60.h"
//#include "timerx.h"
//#include "uip.h"
//#include "uip_arp.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "tapdev.h"
//#include "resolv.h"


#define	TCPIP_CONNECT_CANCEL		0
#define TCPIP_CONNECT_WAIT			1
#define TCPIP_CONNECT_ACTIVE		2
#define TCPIP_CONNECT_XMIT_COMPLETE	3
#define TCPIP_CONNECT_BUSY			0xf1

#define BACNET_PORT 47808


#define REMOTE_SERVER 1

extern u16 Test[50];

extern u8 DNSC_flag;

extern u8 update_firmware;

extern u8 ip_addr[4];
extern u8 gateway[4];
extern u16 tcp_port;

extern u8 tcp_server_sendbuf[500];
extern u16 tcp_server_sendlen;

void dhcpc_configured(const struct dhcpc_state *s);



void vNETTask( void *pvParameters );

// UDP CLIENT SCAN 

#define UDP_SCAN_LPORT 1234
#define UDP_BACNET_LPORT 47808
void udp_scan_init(void);
void bip_Init(void);
void UDP_bacnet_APP(void);
void UDP_SCAN_APP(void);
void tcp_server_appcall(void);

// UDP SERVER TIMESERVER


// UDP CLIENT BACNET


//

// TCP SERVER MODBUS

// TCP SERVER WEBPAGE
#endif

