/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2005 Steve Karg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307, USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/

#include <stdint.h>     /* for standard integer types uint8_t etc. */
#include <stdbool.h>    /* for the standard bool type. */
#include "bacdcode.h"
#include "bacint.h"
#include "bacnet.h"
#include "bip.h"
#include "bvlc.h"
//#include "uip.h"
//#include "tcpip.h"
#include "net.h"        /* custom per port */	  
#include "types.h"
#include <string.h>
//#if PRINT_ENABLED
#include <stdio.h>      /* for standard i/o, like printing */
//#endif
#include "datalink.h"
#include "handlers.h"


#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI )
#ifdef BIP

extern U32_T  far	Instance;
#include "tcpip.h"
//#include "uip.h"
//#include "user_data.h"
#include "define.h"


#if ARM_TSTAT_WIFI
void uart_send_string(U8_T *p, U16_T length,U8_T port);

extern uint8_t bacnet_wifi_buf[500];
extern uint16_t bacnet_wifi_len;

void uip_send(const void * ptr,int len)
{
	
}

int bip_send_pdu_client(
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len,uint8_t protocal
	)
{

}
			
#endif

#endif

#endif

/* NAMING CONSTANT DECLARATIONS */
#define BIP_MAX_CONNS			4
#define BIP_NO_NEW_CONN			0xFF

#define BIP_STATE_FREE			0
#define	BIP_STATE_WAIT			1
#define	BIP_STATE_CONNECTED		2


//int socklen_t;

//extern xSemaphoreHandle sembip;
//xQueueHandle qBip;

/** @file bip.c  Configuration and Operations for BACnet/IP */

static int BIP_Socket = -1;
/* port to use - stored in network byte order */
static uint16_t BIP_Port = 0;   /* this will force initialization in demos */
/* IP Address - stored in network byte order */
static struct in_addr BIP_Address;
/* Broadcast Address - stored in network byte order */
static struct in_addr BIP_Broadcast_Address;

/** Setter for the BACnet/IP socket handle.
 *
 * @param sock_fd [in] Handle for the BACnet/IP socket.
 */

#if 0
static int BIP_SEND_Socket = -1;




BIP_SEND_CONN BIP_Send_Conns;
U8_T BIP_Send_InterAppId;
#endif 

typedef struct _BIP_CONN
{
	U8_T  State;
	U8_T  UdpSocket;

} BIP_CONN;


#if (ASIX_MINI || ASIX_CM5)


U8_T *xdata bip_Data;
U16_T xdata bip_len;
uint8_t far PDUBuffer_BIP[MAX_APDU];

U8_T  bip_InterAppId;
static BIP_CONN bip_Conns[BIP_MAX_CONNS];

//void reset_lose_bip(void);

U8_T bip_NewConn(U32_T xdata* pip, U16_T remotePort, U8_T socket)
{
	U8_T	i;

	for (i = 0; i < BIP_MAX_CONNS; i++)
	{
		if (bip_Conns[i].State == BIP_STATE_FREE)
		{
			bip_Conns[i].State = BIP_STATE_CONNECTED;
			bip_Conns[i].UdpSocket = socket;
			return i;
		}
	}
	if(i == BIP_MAX_CONNS)
	{	 
		bip_Conns[0].State = BIP_STATE_FREE;
		TCPIP_UdpClose(bip_Conns[0].UdpSocket);		

		bip_Conns[0].State = BIP_STATE_CONNECTED;
		bip_Conns[0].UdpSocket = socket;
	
		return 0;
	}
		
	return BIP_NO_NEW_CONN;	
}


 
void bip_Event(U8_T id, U8_T event)
{		
	bip_Conns[id].State = event;
} 

void BIP_Receive_Handler(U8_T xdata * pData, U16_T length, U8_T id)
{	
	uint16_t pdu_len = 0;  
	BACNET_ADDRESS far src; /* source address */
	
	bip_Data = PDUBuffer_BIP;
	memcpy(bip_Data,pData,length);
	bip_len = length;
	bip_set_socket(bip_Conns[id].UdpSocket);

	pdu_len = datalink_receive(&src, &PDUBuffer_BIP[0], sizeof(PDUBuffer_BIP), 0,BAC_IP);
	{
			if(pdu_len) 
			{ 
				npdu_handler(&src, &PDUBuffer_BIP[0], pdu_len,BAC_IP);	
			}			
	}

}

void bip_Init(U16_T localPort)
{
	U8_T	i;	 
	
	for (i = 0; i < BIP_MAX_CONNS; i++)
	{
		bip_Conns[i].State = BIP_STATE_FREE;
	}
	bip_InterAppId = TCPIP_Bind(bip_NewConn, bip_Event, BIP_Receive_Handler);
	/* unicast packet */
	TCPIP_UdpListen(localPort, bip_InterAppId);
//	bip_set_socket(bip_Conns[0].UdpSocket);
} /* End of GUDPBC_Init() */
#endif

#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI)

#ifdef BIP
extern U8_T count_bip_connect;
uint8_t far PDUBuffer_BIP[MAX_APDU];
uint8_t * bip_Data;
uint16_t  bip_len;

#if (ARM_MINI || ASIX_MINI || ARM_CM5)

void bip_Init(void)
{
	struct uip_udp_conn *conn;
	uip_ipaddr_t addr;
	bip_set_socket(0);
	bip_set_addr(0);
	bip_set_port(UDP_BACNET_LPORT); 	
	// udp server
	uip_listen(HTONS(UDP_BACNET_LPORT));
	
	uip_ipaddr_copy(addr, 0xffffffff);
	conn = uip_udp_new(&addr, HTONS(UDP_BACNET_LPORT)); // des port
	if(conn != NULL) 
	{ 
		uip_udp_bind(conn,HTONS(UDP_BACNET_LPORT));  // src port					
	}
} 

U8_T bip_send_mstp_rport;

void UDP_bacnet_APP(void)
{
	/* check the status */
//	u8 len;
//	u8  n = 0;
	static u32 t1 = 0;
	static u32 t2 = 0;
	struct uip_udp_conn *bip_server_conn;

	uip_ipaddr_t addr;
	uint16_t pdu_len = 0;  
	BACNET_ADDRESS far src; /* source address */

	if(uip_poll())
	{  // auto send
		t1 = uip_timer;
		if(t1 - t2 >= 10)
		{
			if(send_mstp_index1 < rec_mstp_index1)
			{
			//if(Send_mstp_Flag){ 
				uip_send(mstp_bac_buf1[send_mstp_index1].buf,mstp_bac_buf1[send_mstp_index1].len);	
				//Send_mstp_Flag = 0;
				send_mstp_index1++;
			//}
			}		
			t2 = uip_timer;
		}
	}
	if(uip_newdata())
	{  // receive data
		
		bip_Data = PDUBuffer_BIP;
		memcpy(bip_Data,uip_appdata,uip_len);
		bip_len = uip_len;

		// if bbmd
//		if(bbmd_en == 1)
//		{
//			Test[10]++;
//			pdu_len = datalink_receive(&src, &PDUBuffer_BIP[0], sizeof(PDUBuffer_BIP), 0,BAC_BVLC);
//			Test[11]++;
//			if(pdu_len) 
//			{
//				npdu_handler(&src, &PDUBuffer_BIP[0], pdu_len,BAC_BVLC);	
//			}			
//		}
//		else
		{
			pdu_len = datalink_receive(&src, &PDUBuffer_BIP[0], sizeof(PDUBuffer_BIP), 0,BAC_IP);
			if(pdu_len) 
			{
				npdu_handler(&src, &PDUBuffer_BIP[0], pdu_len,BAC_IP);	
			}			
		
		}
	  
//		uip_udp_remove(bip_server_conn);
	}	
}
#endif

#endif

#endif
void bip_set_socket(
    int sock_fd)
{
    BIP_Socket = sock_fd;
}

/** Getter for the BACnet/IP socket handle.
 *
 * @return The handle to the BACnet/IP socket.
 */
int bip_socket(
    void)
{
    return BIP_Socket;
}

bool bip_valid(
    void)
{
    return (BIP_Socket != -1);
}

void bip_set_addr(
    uint32_t net_address)
{       /* in network byte order */
    BIP_Address.s_addr = net_address;
}

/* returns network byte order */
uint32_t bip_get_addr(
    void)
{
    return BIP_Address.s_addr;
}

void bip_set_broadcast_addr(
    uint32_t net_address)
{       /* in network byte order */
    BIP_Broadcast_Address.s_addr = net_address;
}

#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI)
void Set_broadcast_bip_address(void)
{
#if (ARM_MINI || ASIX_MINI || ARM_CM5)
	Send_bip_address[0] = BIP_Broadcast_Address.s_addr >> 24;
	Send_bip_address[1] = BIP_Broadcast_Address.s_addr >> 16;
	Send_bip_address[2] = BIP_Broadcast_Address.s_addr >> 8;
	Send_bip_address[3] = BIP_Broadcast_Address.s_addr;
	Send_bip_address[4] = BIP_Port >> 8;
	Send_bip_address[5] = BIP_Port;
#endif
}
#endif

/* returns network byte order */
uint32_t bip_get_broadcast_addr(
    void)
{
    return BIP_Broadcast_Address.s_addr;
}


void bip_set_port(
    uint16_t port)
{       /* in network byte order */
    BIP_Port = port;
}

/* returns network byte order */
uint16_t bip_get_port(
    void)
{
    return BIP_Port;
}

static int bip_decode_bip_address(
    BACNET_ADDRESS * bac_addr,
    struct in_addr *address,    /* in network format */
    uint16_t * port)
{       /* in network format */
    int len = 0;

    if (bac_addr) {
        memcpy(&address->s_addr, &bac_addr->mac[0], 4);
        memcpy(port, &bac_addr->mac[4], 2);
        len = 6;
    }

    return len;
}



void bip_send_mpdu(struct sockaddr_in *dest,
    uint8_t * mtu,
    uint16_t mtu_len)
{
#if (ARM_MINI || ASIX_MINI)
#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI)
	Send_bip_Flag = 1;
//	count_send_bip = 0;	
	Send_bip_count = 5;	
	Set_broadcast_bip_address();
#endif
	bip_send_pdu(0,0,mtu,mtu_len,BAC_IP);
#endif	
}



/** Function to send a packet out the BACnet/IP socket (Annex J).
 * @ingroup DLBIP
 *
 * @param dest [in] Destination address (may encode an IP address and port #).
 * @param npdu_data [in] The NPDU header (Network) information (not used).
 * @param pdu [in] Buffer of data to be sent - may be null (why?).
 * @param pdu_len [in] Number of bytes in the pdu buffer.
 * @return Number of bytes sent on success, negative number on failure.
 */
extern void USB_Send_GSM(U8_T * str,U16_T len);
extern U8_T Send_bip_address[6];
uint8_t far mtu[MAX_MPDU_IP] = { 0 };


int bip_send_pdu(
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len,uint8_t protocal
	)
{       /* number of bytes of data */  
    struct sockaddr_in bip_dest;
    int mtu_len = 0;
    int bytes_sent = 0;
    /* addr and port in host format */
    struct in_addr address;
    uint16_t port = 0;
		uint8_t far mtu[MAX_MPDU_IP] = { 0 };

 //   (void) npdu_data;
    /* assumes that the driver has already been initialized */
#if !(ARM_TSTAT_WIFI)
	if(protocal == BAC_IP)
	{
		if (BIP_Socket < 0) 
		{
		return 0;//BIP_Socket;
		} 
	}
#endif
    mtu[0] = BVLL_TYPE_BACNET_IP;
    bip_dest.sin_family = AF_INET;
    if ((dest->net == BACNET_BROADCAST_NETWORK) || ((dest->net > 0) &&
			(dest->len == 0)) || (dest->mac_len == 0)) {
        /* broadcast */
				
        address.s_addr = BIP_Broadcast_Address.s_addr;
        port = BIP_Port;
        mtu[1] = BVLC_ORIGINAL_BROADCAST_NPDU;
    } else if (dest->mac_len == 6) {
        bip_decode_bip_address(dest, &address, &port);
        mtu[1] = BVLC_ORIGINAL_UNICAST_NPDU;
    } else {

        return -1;

		// if GSM, do not return
    }
    bip_dest.sin_addr.s_addr = address.s_addr;
    bip_dest.sin_port = port;
		
    memset(&(bip_dest.sin_zero), '\0', 8);
    mtu_len = 2;
    mtu_len += encode_unsigned16(&mtu[mtu_len],
        (uint16_t) (pdu_len + 4 /*inclusive */ ));
    memcpy(&mtu[mtu_len], pdu, pdu_len);
    mtu_len += pdu_len;

	if(protocal == BAC_IP)
	{
#if (ASIX_MINI || ASIX_CM5)	
		TCPIP_UdpSend(BIP_Socket, 0, 0, mtu, mtu_len);	
#endif
		
#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI)
		
#if (ARM_MINI || ASIX_MINI || ARM_CM5)
#ifdef BIP
		uip_send((char *)mtu, mtu_len);
#endif
#endif
#if ARM_TSTAT_WIFI 
//		uart_send_string((char *)mtu, mtu_len,3);
		memcpy(bacnet_wifi_buf,mtu,mtu_len);
		bacnet_wifi_len = mtu_len;
#endif		

#endif
	}

	bytes_sent = mtu_len;
	
	memset(mtu,0,MAX_MPDU_IP);
	mtu_len = 0;
  return bytes_sent;	 
}

#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI)
#if (ARM_MINI || ARM_CM5)
struct uip_udp_conn * bip_send_client_conn;

int bip_send_pdu_client(
    BACNET_ADDRESS * dest,      /* destination address */
    BACNET_NPDU_DATA * npdu_data,       /* network information */
    uint8_t * pdu,      /* any data to be sent - may be null */
    unsigned pdu_len,uint8_t protocal
	)
{       /* number of bytes of data */  

    struct sockaddr_in bip_dest;
    int mtu_len = 0;
    int bytes_sent = 0;
    /* addr and port in host format */
    struct in_addr address;
    uint16_t port = 0;
		uint8_t far mtu1[MAX_MPDU_IP] = { 0 };

 //   (void) npdu_data;
    /* assumes that the driver has already been initialized */
    mtu1[0] = BVLL_TYPE_BACNET_IP;
    bip_dest.sin_family = AF_INET;
    if ((dest->net == BACNET_BROADCAST_NETWORK) || ((dest->net > 0) &&
			(dest->len == 0)) || (dest->mac_len == 0)) {
        /* broadcast */
				
        address.s_addr = BIP_Broadcast_Address.s_addr;
        port = BIP_Port;
        mtu1[1] = BVLC_ORIGINAL_BROADCAST_NPDU;
    } else if (dest->mac_len == 6) {
        bip_decode_bip_address(dest, &address, &port);
        mtu1[1] = BVLC_ORIGINAL_UNICAST_NPDU;
    } else {
        return -1;

    }
		
    bip_dest.sin_addr.s_addr = address.s_addr;
    bip_dest.sin_port = port;
		
    memset(&(bip_dest.sin_zero), '\0', 8);
    mtu_len = 2;
    mtu_len += encode_unsigned16(&mtu1[mtu_len],
        (uint16_t) (pdu_len + 4 /*inclusive */ ));
    memcpy(&mtu1[mtu_len], pdu, pdu_len);
    mtu_len += pdu_len;

		if(Send_bip_Flag)
		{
			uip_ipaddr_t addr;
			if(bip_send_client_conn != NULL) 
			{		
				uip_udp_remove(bip_send_client_conn);
			}
			
			Send_bip_count = MAX_RETRY_SEND_BIP;
			uip_ipaddr(addr,Send_bip_address[0],Send_bip_address[1],Send_bip_address[2],Send_bip_address[3]);	
			bip_send_client_conn = uip_udp_new(&addr, HTONS(Send_bip_address[4] * 256 + Send_bip_address[5])); // des port
			if(bip_send_client_conn != NULL) 
			{ 
				uip_udp_bind(bip_send_client_conn,HTONS(UDP_BIP_SEND_LPORT));  // src port					
			}
			memcpy(bip_bac_buf.buf,&mtu1, mtu_len);			
			bip_bac_buf.len = mtu_len;
			
		}
//		else
//			uip_send((char *)mtu, mtu_len);


	
	bytes_sent = mtu_len;
	
	memset(mtu1,0,MAX_MPDU_IP);
	mtu_len = 0;
  return bytes_sent;	

}
#endif 
#endif

/** Implementation of the receive() function for BACnet/IP; receives one
 * packet, verifies its BVLC header, and removes the BVLC header from
 * the PDU data before returning.
 *
 * @param src [out] Source of the packet - who should receive any response.
 * @param pdu [out] A buffer to hold the PDU portion of the received packet,
 * 					after the BVLC portion has been stripped off.
 * @param max_pdu [in] Size of the pdu[] buffer.
 * @param timeout [in] The number of milliseconds to wait for a packet.
 * @return The number of octets (remaining) in the PDU, or zero on failure.
 */


uint16_t bip_receive(
    BACNET_ADDRESS * src,       /* source address */
    uint8_t * pdu,      /* PDU data */
    uint16_t max_pdu,   /* amount of space available in the PDU  */
    unsigned protocal)
{
    int received_bytes = 0;
    uint16_t pdu_len = 0;       /* return value */
//    fd_set read_fds;
 //   int max = 0;
 //   struct timeval select_timeout;
    struct sockaddr_in sin = {0};
		struct sockaddr_in original_sin = {0};
		struct sockaddr_in dest = {0};
  //  socklen_t sin_len = sizeof(sin);
		uint16_t npdu_len = 0;      /* return value */
    uint16_t i = 0;
    int function = 0;
		uint16_t time_to_live = 0;
		uint16_t result_code = 0;
	bool status = false;
    /* Make sure the socket is open */
#if !(ARM_TSTAT_WIFI)
	if(protocal == BAC_IP)
	if (BIP_Socket < 0)
	{		
     return 0;
	}
	
#if (ASIX_MINI || ASIX_CM5) 	
	received_bytes = bip_len;
  bip_Data = pdu;
#endif 
	
#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI )
#ifdef BIP
	received_bytes = bip_len;
  bip_Data = pdu;
#endif
	
#endif
	
    /* See if there is a problem */
	if(protocal == BAC_IP)
	{
	    if (received_bytes < 0)        
				return 0;
	    
	    /* no problem, just no bytes */
	    if (received_bytes == 0)
        	return 0;
	}
#endif
	received_bytes = 0;
  /* the signature of a BACnet/IP packet */
		if (pdu[0] != BVLL_TYPE_BACNET_IP)
		{ 	  
				return 0;
		}

		function = pdu[1]; 
    /* decode the length of the PDU - length is inclusive of BVLC */
    (void) decode_unsigned16(&pdu[2], &pdu_len);
		// get sin address, added by chelsea
		//????????????????
		sin.sin_addr.s_addr = 0;
		sin.sin_port = 0;
		original_sin.sin_addr.s_addr = 0;
		original_sin.sin_port = 0;
		dest.sin_addr.s_addr = 0;
		dest.sin_port = 0;
		
		// changed by chelsea
		// pdu[0]. ... 81 0b 00 18 01 20 ff ff ff 00 10 00, cant get sin.sin_addr
//		if(function == BVLC_REGISTER_FOREIGN_DEVICE)
//		{
//		decode_unsigned32(&pdu[6], (uint32_t *)&sin.sin_addr);
//		decode_unsigned16(&pdu[6 + 4], &sin.sin_port);	
//		}
//		else
//		{
//		decode_unsigned32(&pdu[pdu_len],(uint32_t *)&sin.sin_addr);
//		decode_unsigned16(&pdu[pdu_len + 4], &sin.sin_port);
//		}	
#if (ARM_MINI || ARM_CM5)
		sin.sin_port = uip_udp_conn->rport;
		memcpy(&sin.sin_addr.s_addr,uip_udp_conn->ripaddr,4);
#endif
    /* subtract off the BVLC header */
    pdu_len -= 4;
		switch (function) {
        case BVLC_RESULT:
            /* Upon receipt of a BVLC-Result message containing a result code
               of X'0000' indicating the successful completion of the
               registration, a foreign device shall start a timer with a value
               equal to the Time-to-Live parameter of the preceding Register-
               Foreign-Device message. At the expiration of the timer, the
               foreign device shall re-register with the BBMD by sending a BVLL
               Register-Foreign-Device message */
            /* Clients can now get this result */
#if (ARM_MINI || ARM_CM5)
            (void) decode_unsigned16(&pdu[4], &result_code);
            BVLC_Result_Code = (BACNET_BVLC_RESULT) result_code;
//            debug_printf("BVLC: Result Code=%d\n", BVLC_Result_Code);
            /* not an NPDU */
            npdu_len = 0;
#endif
            break;
        case BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE:
//            debug_printf("BVLC: Received Write-BDT.\n");
            /* Upon receipt of a BVLL Write-Broadcast-Distribution-Table
               message, a BBMD shall attempt to create or replace its BDT,
               depending on whether or not a BDT has previously existed.
               If the creation or replacement of the BDT is successful, the BBMD
               shall return a BVLC-Result message to the originating device with
               a result code of X'0000'. Otherwise, the BBMD shall return a
               BVLC-Result message to the originating device with a result code
               of X'0010' indicating that the write attempt has failed. */
#if (ARM_MINI || ARM_CM5)
						if(bbmd_en == 0)
						{
							 pdu_len = 0;
							break;
						}	
#endif
            status = bvlc_create_bdt(&pdu[4], pdu_len);
            if (status) {
                bvlc_send_result(&sin, BVLC_RESULT_SUCCESSFUL_COMPLETION);
            } else {
                bvlc_send_result(&sin,
                    BVLC_RESULT_WRITE_BROADCAST_DISTRIBUTION_TABLE_NAK);
            }
            /* not an NPDU */
            pdu_len = 0;
            break;
        case BVLC_READ_BROADCAST_DIST_TABLE:
//            debug_printf("BVLC: Received Read-BDT.\n");
            /* Upon receipt of a BVLL Read-Broadcast-Distribution-Table
               message, a BBMD shall load the contents of its BDT into a BVLL
               Read-Broadcast-Distribution-Table-Ack message and send it to the
               originating device. If the BBMD is unable to perform the
               read of its BDT, it shall return a BVLC-Result message to the
               originating device with a result code of X'0020' indicating that
               the read attempt has failed. */
#if (ARM_MINI || ARM_CM5)
						if(bbmd_en == 0)
						{
							 pdu_len = 0;
							break;
						}	
#endif
            if (bvlc_send_bdt(&sin) <= 0) {
                bvlc_send_result(&sin,
                    BVLC_RESULT_READ_BROADCAST_DISTRIBUTION_TABLE_NAK);
            }
            /* not an NPDU */
            pdu_len = 0;
            break;
//        case BVLC_READ_BROADCAST_DIST_TABLE_ACK:
////            debug_printf("BVLC: Received Read-BDT-Ack.\n");
//            /* FIXME: complete the code for client side read */
//            /* not an NPDU */
//            pdu_len = 0;
//            break;
        case BVLC_FORWARDED_NPDU:
            /* Upon receipt of a BVLL Forwarded-NPDU message, a BBMD shall
               process it according to whether it was received from a peer
               BBMD as the result of a directed broadcast or a unicast
               transmission. A BBMD may ascertain the method by which Forwarded-
               NPDU messages will arrive by inspecting the broadcast distribution
               mask field in its own BDT entry since all BDTs are required
               to be identical. If the message arrived via directed broadcast,
               it was also received by the other devices on the BBMD's subnet. In
               this case the BBMD merely retransmits the message directly to each
               foreign device currently in the BBMD's FDT. If the
               message arrived via a unicast transmission it has not yet been
               received by the other devices on the BBMD's subnet. In this case,
               the message is sent to the devices on the BBMD's subnet using the
               B/IP broadcast address as well as to each foreign device
               currently in the BBMD's FDT. A BBMD on a subnet with no other
               BACnet devices may omit the broadcast using the B/IP
               broadcast address. The method by which a BBMD determines whether
               or not other BACnet devices are present is a local matter. */
            /* decode the 4 byte original address and 2 byte port */
#if (ARM_MINI || ARM_CM5)
						if(bbmd_en == 0)
						{
							 pdu_len = 0;
							break;
						}	
#endif
            bvlc_decode_bip_address(&pdu[4], &original_sin.sin_addr,
                &original_sin.sin_port);
            pdu_len -= 6;
            /*  Broadcast locally if received via unicast from a BDT member */
            if (bvlc_bdt_member_mask_is_unicast(&sin)) {
                dest.sin_addr.s_addr = bip_get_broadcast_addr();
                dest.sin_port = bip_get_port();		
                bvlc_send_mpdu(&dest, &pdu[4 + 6], pdu_len);
            }
            /* use the original addr from the BVLC for src */
            dest.sin_addr.s_addr = original_sin.sin_addr.s_addr;
            dest.sin_port = original_sin.sin_port;
            bvlc_fdt_forward_npdu(&dest, &pdu[4 + 6], pdu_len);
//            debug_printf("BVLC: Received Forwarded-NPDU from %s:%04X.\n",
 //               inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
            bvlc_internet_to_bacnet_address(src, &dest);
            if (pdu_len < max_pdu) {
                /* shift the buffer to return a valid PDU */
                for (i = 0; i < pdu_len; i++) {
                    pdu[i] = pdu[4 + 6 + i];
                }
            } else {
                /* ignore packets that are too large */
                /* clients should check my max-apdu first */
                pdu_len = 0;
            }
            break;
        case BVLC_REGISTER_FOREIGN_DEVICE:
            /* Upon receipt of a BVLL Register-Foreign-Device message, a BBMD
               shall start a timer with a value equal to the Time-to-Live
               parameter supplied plus a fixed grace period of 30 seconds. If,
               within the period during which the timer is active, another BVLL
               Register-Foreign-Device message from the same device is received,
               the timer shall be reset and restarted. If the time expires
               without the receipt of another BVLL Register-Foreign-Device
               message from the same foreign device, the FDT entry for this
               device shall be cleared. */
#if (ARM_MINI || ARM_CM5)
						if(bbmd_en == 0)
						{
							 pdu_len = 0;
							 break;
						}	
#endif					
            (void) decode_unsigned16(&pdu[4], &time_to_live);
						
            if (bvlc_register_foreign_device(&sin, time_to_live)) {
							
                bvlc_send_result(&sin, BVLC_RESULT_SUCCESSFUL_COMPLETION);
//                debug_printf("BVLC: Registered a Foreign Device.\n");
            } else {
                bvlc_send_result(&sin,
                    BVLC_RESULT_REGISTER_FOREIGN_DEVICE_NAK);
//                debug_printf("BVLC: Failed to Register a Foreign Device.\n");
            }
            /* not an NPDU */
            pdu_len = 0;
            break;
        case BVLC_READ_FOREIGN_DEVICE_TABLE:
//            debug_printf("BVLC: Received Read-FDT.\n");
            /* Upon receipt of a BVLL Read-Foreign-Device-Table message, a
               BBMD shall load the contents of its FDT into a BVLL Read-
               Foreign-Device-Table-Ack message and send it to the originating
               device. If the BBMD is unable to perform the read of its FDT,
               it shall return a BVLC-Result message to the originating device
               with a result code of X'0040' indicating that the read attempt has
               failed. */
#if (ARM_MINI || ARM_CM5)
						if(bbmd_en == 0)
						{
							 pdu_len = 0;
							 break;
						}	
#endif							
            if (bvlc_send_fdt(&sin) <= 0) {
                bvlc_send_result(&sin,
                    BVLC_RESULT_READ_FOREIGN_DEVICE_TABLE_NAK);
            }
            /* not an NPDU */
            pdu_len = 0;
            break;
#if (ARM_MINI || ARM_CM5)
        case BVLC_READ_FOREIGN_DEVICE_TABLE_ACK:
//            debug_printf("BVLC: Received Read-FDT-Ack.\n");
            /* FIXME: complete the code for client side read */
            /* not an NPDU */
            npdu_len = 0;
            break;
        case BVLC_DELETE_FOREIGN_DEVICE_TABLE_ENTRY:
//            debug_printf("BVLC: Received Delete-FDT-Entry.\n");
            /* Upon receipt of a BVLL Delete-Foreign-Device-Table-Entry
               message, a BBMD shall search its foreign device table for an entry
               corresponding to the B/IP address supplied in the message. If an
               entry is found, it shall be deleted and the BBMD shall return a
               BVLC-Result message to the originating device with a result code
               of X'0000'. Otherwise, the BBMD shall return a BVLCResult
               message to the originating device with a result code of X'0050'
               indicating that the deletion attempt has failed. */
            if (bvlc_delete_foreign_device(&pdu[4])) {
                bvlc_send_result(&sin, BVLC_RESULT_SUCCESSFUL_COMPLETION);
            } else {
                bvlc_send_result(&sin,
                    BVLC_RESULT_DELETE_FOREIGN_DEVICE_TABLE_ENTRY_NAK);
            }
            /* not an NPDU */
            npdu_len = 0;
            break;
#endif
        case BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK:
//          debug_printf
//               ("BVLC: Received Distribute-Broadcast-to-Network from %s:%04X.\n",
//                inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
            /* Upon receipt of a BVLL Distribute-Broadcast-To-Network message
               from a foreign device, the receiving BBMD shall transmit a
               BVLL Forwarded-NPDU message on its local IP subnet using the
               local B/IP broadcast address as the destination address. In
               addition, a Forwarded-NPDU message shall be sent to each entry
               in its BDT as described in the case of the receipt of a
               BVLL Original-Broadcast-NPDU as well as directly to each foreign
               device currently in the BBMD's FDT except the originating
               node. If the BBMD is unable to perform the forwarding function,
               it shall return a BVLC-Result message to the foreign device
               with a result code of X'0060' indicating that the forwarding
               attempt was unsuccessful */

						
            bvlc_forward_npdu(&sin, &pdu[4], pdu_len);
            bvlc_bdt_forward_npdu(&sin, &pdu[4], pdu_len);
            bvlc_fdt_forward_npdu(&sin, &pdu[4], pdu_len);
            /* not an NPDU */
            pdu_len = 0;
            break;

        case BVLC_ORIGINAL_UNICAST_NPDU:
//            debug_printf("BVLC: Received Original-Unicast-NPDU.\n");
            /* ignore messages from me */
#if (ARM_MINI || ARM_CM5)
            if ((sin.sin_addr.s_addr == bip_get_addr()) &&
                (sin.sin_port == bip_get_port())) {
                pdu_len = 0;
            } else
#endif								
						{	
                bvlc_internet_to_bacnet_address(src, &sin);
                if (pdu_len < max_pdu) {
                    /* shift the buffer to return a valid PDU */
                    for (i = 0; i < pdu_len + 4; i++) { 
											// FIXED by chelsea, +4
                        pdu[i] = pdu[4 + i];
                    }
                } else {
                    /* ignore packets that are too large */
                    /* clients should check my max-apdu first */
                    pdu_len = 0;
                }
            }
            break;
        case BVLC_ORIGINAL_BROADCAST_NPDU:
//            debug_printf("BVLC: Received Original-Broadcast-NPDU.\n");
            /* Upon receipt of a BVLL Original-Broadcast-NPDU message,
               a BBMD shall construct a BVLL Forwarded-NPDU message and
               send it to each IP subnet in its BDT with the exception
               of its own. The B/IP address to which the Forwarded-NPDU
               message is sent is formed by inverting the broadcast
               distribution mask in the BDT entry and logically ORing it
               with the BBMD address of the same entry. This process
               produces either the directed broadcast address of the remote
               subnet or the unicast address of the BBMD on that subnet
               depending on the contents of the broadcast distribution
               mask. See J.4.3.2.. In addition, the received BACnet NPDU
               shall be sent directly to each foreign device currently in
               the BBMD's FDT also using the BVLL Forwarded-NPDU message. */
						
            bvlc_internet_to_bacnet_address(src, &sin);
            if (pdu_len < max_pdu) {
                /* shift the buffer to return a valid PDU */
                for (i = 0; i < pdu_len; i++) {
                    pdu[i] = pdu[4 + i];
                }
                /* if BDT or FDT entries exist, Forward the NPDU */
//								if(Modbus.bbmd_en)
//								{
//									bvlc_bdt_forward_npdu(&sin, &pdu[0], pdu_len);
//									bvlc_fdt_forward_npdu(&sin, &pdu[0], pdu_len);
//								}
            } else {
                /* ignore packets that are too large */
                pdu_len = 0;
            }
            break;
        default:
            break;
    }
    return pdu_len;
}


//extern uint8_t * bip_Data_client;
//extern uint16_t  bip_len_client;

//uint16_t bip_receive_client(
//    BACNET_ADDRESS * src,       /* source address */
//    uint8_t * pdu,      /* PDU data */
//    uint16_t max_pdu,   /* amount of space available in the PDU  */
//    unsigned protocal)
//{
//    int received_bytes = 0;
//    uint16_t pdu_len = 0;       /* return value */
////    fd_set read_fds;
// //   int max = 0;
// //   struct timeval select_timeout;
//    struct sockaddr_in sin = {0};
//		struct sockaddr_in original_sin = {0};
//		struct sockaddr_in dest = {0};
//  //  socklen_t sin_len = sizeof(sin);
//    uint16_t i = 0;
//    int function = 0;
////		uint16_t time_to_live = 0;
////		uint16_t result_code = 0;
////	bool status = false;
//    /* Make sure the socket is open */


//	if(protocal == BAC_IP)
//	if (BIP_Socket < 0)
//	{		
//     return 0;
//	}

//	
//#if (ARM_MINI || ARM_CM5 || ARM_TSTAT_WIFI)
//#ifdef BIP
//	received_bytes = bip_len_client;
//  bip_Data_client = pdu;
//#endif
//	
//#endif

//    /* See if there is a problem */
//	if(protocal == BAC_IP)
//	{
//	    if (received_bytes < 0)        
//				return 0;
//	    
//	    /* no problem, just no bytes */
//	    if (received_bytes == 0)
//        	return 0;
//	}

//	received_bytes = 0;
//	
//  /* the signature of a BACnet/IP packet */
//		if (pdu[0] != BVLL_TYPE_BACNET_IP)
//		{ 	  
//				return 0;
//		}

//		function = pdu[1]; 
//    /* decode the length of the PDU - length is inclusive of BVLC */
//    (void) decode_unsigned16(&pdu[2], &pdu_len);
//		
//		// get sin address, added by chelsea
//		//????????????????
//		sin.sin_addr.s_addr = 0;
//		sin.sin_port = 0;
//		original_sin.sin_addr.s_addr = 0;
//		original_sin.sin_port = 0;
//		dest.sin_addr.s_addr = 0;
//		dest.sin_port = 0;
//		
//		
//		// changed by chelsea
//		// pdu[0]. ... 81 0b 00 18 01 20 ff ff ff 00 10 00, cant get sin.sin_addr
////		if(function == BVLC_REGISTER_FOREIGN_DEVICE)
////		{
////		decode_unsigned32(&pdu[6], (uint32_t *)&sin.sin_addr);
////		decode_unsigned16(&pdu[6 + 4], &sin.sin_port);	
////		}
////		else
////		{
////		decode_unsigned32(&pdu[pdu_len],(uint32_t *)&sin.sin_addr);
////		decode_unsigned16(&pdu[pdu_len + 4], &sin.sin_port);
////		}	
//		
//		sin.sin_port = uip_udp_conn->rport;
//		memcpy(&sin.sin_addr.s_addr,uip_udp_conn->ripaddr,4);
//    /* subtract off the BVLC header */
//    pdu_len -= 4;

//		switch (function) {
//        case BVLC_ORIGINAL_UNICAST_NPDU:
////            debug_printf("BVLC: Received Original-Unicast-NPDU.\n");
//            /* ignore messages from me */
//            if ((sin.sin_addr.s_addr == bip_get_addr()) &&
//                (sin.sin_port == bip_get_port())) {
//                pdu_len = 0;
//            } else {
//                bvlc_internet_to_bacnet_address(src, &sin);
//                if (pdu_len < max_pdu) {
//                    /* shift the buffer to return a valid PDU */
//                    for (i = 0; i < pdu_len; i++) {
//                        pdu[i] = pdu[4 + i];
//                    }
//                } else {
//                    /* ignore packets that are too large */
//                    /* clients should check my max-apdu first */
//                    pdu_len = 0;
//                }
//            }
//            break;
//        case BVLC_ORIGINAL_BROADCAST_NPDU:
////            debug_printf("BVLC: Received Original-Broadcast-NPDU.\n");
//            /* Upon receipt of a BVLL Original-Broadcast-NPDU message,
//               a BBMD shall construct a BVLL Forwarded-NPDU message and
//               send it to each IP subnet in its BDT with the exception
//               of its own. The B/IP address to which the Forwarded-NPDU
//               message is sent is formed by inverting the broadcast
//               distribution mask in the BDT entry and logically ORing it
//               with the BBMD address of the same entry. This process
//               produces either the directed broadcast address of the remote
//               subnet or the unicast address of the BBMD on that subnet
//               depending on the contents of the broadcast distribution
//               mask. See J.4.3.2.. In addition, the received BACnet NPDU
//               shall be sent directly to each foreign device currently in
//               the BBMD's FDT also using the BVLL Forwarded-NPDU message. */
//						
//            bvlc_internet_to_bacnet_address(src, &sin);
//            if (pdu_len < max_pdu) {
//                /* shift the buffer to return a valid PDU */
//                for (i = 0; i < pdu_len; i++) {
//                    pdu[i] = pdu[4 + i];
//                }
//                /* if BDT or FDT entries exist, Forward the NPDU */
////								if(Modbus.bbmd_en)
////								{
////									bvlc_bdt_forward_npdu(&sin, &pdu[0], pdu_len);
////									bvlc_fdt_forward_npdu(&sin, &pdu[0], pdu_len);
////								}
//            } else {
//                /* ignore packets that are too large */
//                pdu_len = 0;
//            }
//            break;
//        default:
//            break;
//    }
//		
//    return pdu_len;
//}

extern Byte	Station_NUM;

void bip_get_my_address(
    BACNET_ADDRESS * my_address)
{
    int i = 0;

    if (my_address) {
        my_address->mac_len = 6;
        memcpy(&my_address->mac[0], &BIP_Address.s_addr, 4);
        memcpy(&my_address->mac[4], &BIP_Port, 2);

        my_address->net = 0;    /* local only, no routing */
        my_address->len = 0;    /* no SLEN */
        for (i = 0; i < MAX_MAC_LEN; i++) {
            /* no SADR */
            my_address->adr[i] = 0;
        }
    }

    return;
}

void bip_get_broadcast_address(
    BACNET_ADDRESS * dest)
{       /* destination address */
    int i = 0;  /* counter */
    if (dest) {
        dest->mac_len = 6;
        memcpy(&dest->mac[0], &BIP_Broadcast_Address.s_addr, 4);
        memcpy(&dest->mac[4], &BIP_Port, 2);
        dest->net = BACNET_BROADCAST_NETWORK;
        dest->len = 0;  /* no SLEN */
        for (i = 0; i < MAX_MAC_LEN; i++) {
            /* no SADR */
            dest->adr[i] = 0;
        }
				
    }

    return;
}


//uint8_t AI_Index_To_Instance[1];
//uint8_t BI_Index_To_Instance[1];
//uint8_t AO_Index_To_Instance[1];
//uint8_t BO_Index_To_Instance[1];
//uint8_t AV_Index_To_Instance[1];
//uint8_t BV_Index_To_Instance[1];

//uint8_t AI_Instance_To_Index[1];
//uint8_t BI_Instance_To_Index[1];
//uint8_t AO_Instance_To_Index[1];
//uint8_t BO_Instance_To_Index[1];
//uint8_t AV_Instance_To_Index[1];
//uint8_t BV_Instance_To_Index[1];


//U8_T Get_index_by_AOx(U8_T ao_index,U8_T *out_index)
//{}
//U8_T Get_index_by_BOx(U8_T do_index,U8_T *out_index)
//{}
//U8_T Get_index_by_AIx(U8_T ai_index,U8_T *in_index)
//{}
//U8_T Get_index_by_BIx(U8_T bi_index,U8_T *in_index)
//{}
//U8_T Get_index_by_AVx(U8_T av_index,U8_T *var_index)
//{}
//U8_T Get_index_by_BVx(U8_T bv_index,U8_T *var_index)
//{}

//U8_T Get_AOx_by_index(U8_T index,U8_T *ao_index)
//{}
//U8_T Get_BOx_by_index(U8_T index,U8_T *bo_index)
//{}
//U8_T Get_AIx_by_index(U8_T index,U8_T *ai_index)
//{}
//U8_T Get_BIx_by_index(U8_T index,U8_T *bi_index)
//{}
//U8_T Get_AVx_by_index(U8_T index,U8_T *av_index)
//{}
//U8_T Get_BVx_by_index(U8_T index,U8_T *bv_index)
//{}
	
//    BACNET_POLARITY Binary_Input_Polarity(
//        uint32_t object_instance)
//	{}

//    bool Binary_Input_Polarity_Set(
//        uint32_t object_instance,
//        BACNET_POLARITY polarity)
//		{}

		


//uint8_t Send_Mstp(uint8_t flag,uint8_t *type)
//{}

//void Transfer_Bip_To_Mstp_pdu( uint8_t * pdu,uint16_t pdu_len)
//{}
//void Transfer_Mstp_To_Bip_pdu( uint8_t src, uint8_t * pdu,uint16_t pdu_len)
//{}


//		void Handler_Complex_Ack(
//    uint8_t * apdu,
//    int apdu_len,       /* total length of the apdu */
//   uint8_t protocal )
//		{}
//			
//			void Send_whois_to_mstp(void)
//			{}
uint16_t count_suspend_mstp;
uint8_t flag_suspend_mstp;

//void handler_conf_private_trans_ack(
//    uint8_t * service_request,
//    uint16_t service_len,
//    uint8_t * apdu,
//    int apdu_len,
//		uint8_t protocal	)
//{}

//void handler_private_transfer( 	
//	uint8_t * apdu,
//  unsigned apdu_len,
//	BACNET_ADDRESS * src,uint8_t protocal)
//{}

BACNET_POLARITY Binary_Value_Polarity(
    uint32_t object_instance)
	{}
		
bool Binary_Value_Polarity_Set(
    uint32_t object_instance,
    BACNET_POLARITY polarity)
{}
	
//uint8_t header_len;	
//uint16_t transfer_len;	
uint8_t MSTP_Transfer_Len;	
	
uint8 Get_State_Text_Len(uint8_t i)
{}

char * Get_State_Text(uint8_t i,uint8_t j)
{}

void write_value_state(uint32_t object_instance,
    uint32_t state_index,
    char *new_name)
{}
	
	
	
	
	
		
		
		
			
			