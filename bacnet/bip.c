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


#if ARM
#ifdef BIP
//#include "tcpip.h"
#endif
#endif

/* NAMING CONSTANT DECLARATIONS */
#define BIP_MAX_CONNS			4
#define BIP_NO_NEW_CONN			0xFF

#define BIP_STATE_FREE			0
#define	BIP_STATE_WAIT			1
#define	BIP_STATE_CONNECTED		2


//int socklen_t;

//xSemaphoreHandle sembip;
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


		
typedef struct _BIP_CONN
{
	U8_T  State;
	U8_T  UdpSocket;

} BIP_CONN;


#if ASIX


U8_T *xdata bip_Data;
U16_T xdata bip_len;
uint8_t far PDUBuffer_BIP[MAX_APDU];

U8_T  bip_InterAppId;
static BIP_CONN bip_Conns[BIP_MAX_CONNS];

static U8_T bip_NewConn(U32_T xdata* pip, U16_T remotePort, U8_T socket)
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
	bip_set_socket(bip_Conns[0].UdpSocket);
//	vSemaphoreCreateBinary(sembip);
//	qBip = xQueueCreate(4, 1);
} /* End of GUDPBC_Init() */
#endif

#if ARM

#ifdef BIP

uint8_t PDUBuffer_BIP[MAX_APDU];
uint8_t * bip_Data;
uint16_t  bip_len;

void bip_Init(void)
{
	struct uip_udp_conn *conn;
	uip_ipaddr_t addr;
	bip_set_socket(0);
	bip_set_addr(0);
	bip_set_port(47808); 	
	// udp server
	uip_listen(HTONS(UDP_BACNET_LPORT));
	uip_udp_bind(&uip_udp_conns[1], HTONS(UDP_BACNET_LPORT));
} 

void UDP_bacnet_APP(void)
{
	/* check the status */
	u8 len;
	u8  n = 0;
	
	struct uip_udp_conn *conn;
	uip_ipaddr_t addr;
	uint16_t pdu_len = 0;  
	BACNET_ADDRESS far src; /* source address */

	
	if(uip_poll())
	{  // auto send
		
	}
	
	if(uip_newdata())
	{  // receive data
		
		bip_Data = PDUBuffer_BIP;
		memcpy(bip_Data,uip_appdata,uip_len);
		bip_len = uip_len;

		uip_ipaddr_copy(addr, uip_udp_conn->ripaddr);
		conn = uip_udp_new(&addr, uip_udp_conn->rport);
		pdu_len = datalink_receive(&src, &PDUBuffer_BIP[0], sizeof(PDUBuffer_BIP), 0,BAC_IP);
	  {
		    if(pdu_len) 
				{ 
	        npdu_handler(&src, &PDUBuffer_BIP[0], pdu_len,BAC_IP);	
	      }			
		}
		uip_udp_remove(conn);
	}	
}
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

/** Function to send a packet out the BACnet/IP socket (Annex J).
 * @ingroup DLBIP
 *
 * @param dest [in] Destination address (may encode an IP address and port #).
 * @param npdu_data [in] The NPDU header (Network) information (not used).
 * @param pdu [in] Buffer of data to be sent - may be null (why?).
 * @param pdu_len [in] Number of bytes in the pdu buffer.
 * @return Number of bytes sent on success, negative number on failure.
 */
uint8_t far mtu[MAX_MPDU_IP] = { 0 };
extern void USB_Send_GSM(U8_T * str,U16_T len);

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
 //   (void) npdu_data;
    /* assumes that the driver has already been initialized */
	

	if(protocal == BAC_IP)
	{
		if (BIP_Socket < 0) 
		{
		return 0;//BIP_Socket;
		} 
	}
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
#if USB_HOST
		if(protocal == BAC_IP) 		
			
        /* invalid address */
#endif
		  
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
#if USB_HOST
	if((protocal == BAC_GSM) && (dest->mac_len != 6))
	{
		mtu_len = 26;
		mtu[0] = 0x81;
		mtu[1] = 0x0b;
		mtu[2] = 0x00;
		mtu[3] = 0x19;
		mtu[4] = 0x01;
		mtu[5] = 0x20;
		mtu[6] = 0xff;
		mtu[7] = 0xff;
		mtu[8] = 0x00;
		mtu[9] = 0xff;
		mtu[10] = 0x10;
		mtu[11] = 0x00;
		mtu[12] = 0xc4;
		mtu[13] = 0x02;
		mtu[14] = 0x00;
		mtu[15] = (U8_T)(Instance >> 8);
		mtu[16] = (U8_T)(Instance);
		mtu[17] = 0x22;
		mtu[18] = 0x02;
		mtu[19] = 0x58;
		mtu[20] = 0x19;
		mtu[21] = 0x91;
		mtu[22] = 0x03;
		mtu[23] = 0x22;
		mtu[24] = 0x01;
		mtu[25] = 0x04;
	}
#endif
	if(protocal == BAC_IP)
	{
#if ASIX
			TCPIP_UdpSend(BIP_Socket, 0, 0, mtu, mtu_len);	
#endif
		
#if ARM
#ifdef BIP
		uip_send((char *)mtu, mtu_len);
#endif
#endif
	}
#if USB_HOST
	else if(protocal == BAC_GSM)
	{
		USB_Send_GSM(mtu,mtu_len);
	}
#endif
	memset(mtu,0,MAX_MPDU_IP);
	mtu_len = 0;

    return bytes_sent;	 
}

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

#ifdef BIP
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
    uint16_t i = 0;
    int function = 0;
//		uint16_t time_to_live = 0;
//		uint16_t result_code = 0;
//	bool status = false;
    /* Make sure the socket is open */


	if(protocal == BAC_IP)
	if (BIP_Socket < 0)
	{		
     return 0;
	}


	
	received_bytes = bip_len;
  bip_Data = pdu;


    /* See if there is a problem */
	if(protocal == BAC_IP)
	{
	    if (received_bytes < 0)        
				return 0;
	    
	    /* no problem, just no bytes */
	    if (received_bytes == 0)
        	return 0;
	}

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
		sin.sin_addr.s_addr = 0;
		sin.sin_port = 0;
		original_sin.sin_addr.s_addr = 0;
		original_sin.sin_port = 0;
		dest.sin_addr.s_addr = 0;
		dest.sin_port = 0;
		if(function == BVLC_REGISTER_FOREIGN_DEVICE)
		{
		decode_unsigned32(&pdu[6], (uint32_t *)&sin.sin_addr);
		decode_unsigned16(&pdu[6 + 4], &sin.sin_port);	
		}
		else
		{
		decode_unsigned32(&pdu[pdu_len],(uint32_t *)&sin.sin_addr);
		decode_unsigned16(&pdu[pdu_len + 4], &sin.sin_port);
		}			

    /* subtract off the BVLC header */
    pdu_len -= 4;

		switch (function) {
////        case BVLC_RESULT:
////            /* Upon receipt of a BVLC-Result message containing a result code
////               of X'0000' indicating the successful completion of the
////               registration, a foreign device shall start a timer with a value
////               equal to the Time-to-Live parameter of the preceding Register-
////               Foreign-Device message. At the expiration of the timer, the
////               foreign device shall re-register with the BBMD by sending a BVLL
////               Register-Foreign-Device message */
////            /* Clients can now get this result */
////            (void) decode_unsigned16(&pdu[4], &result_code);
////            BVLC_Result_Code = (BACNET_BVLC_RESULT) result_code;
//////            debug_printf("BVLC: Result Code=%d\n", BVLC_Result_Code);
////            /* not an NPDU */
////            npdu_len = 0;
////            break;
//        case BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE:
////            debug_printf("BVLC: Received Write-BDT.\n");
//            /* Upon receipt of a BVLL Write-Broadcast-Distribution-Table
//               message, a BBMD shall attempt to create or replace its BDT,
//               depending on whether or not a BDT has previously existed.
//               If the creation or replacement of the BDT is successful, the BBMD
//               shall return a BVLC-Result message to the originating device with
//               a result code of X'0000'. Otherwise, the BBMD shall return a
//               BVLC-Result message to the originating device with a result code
//               of X'0010' indicating that the write attempt has failed. */
//						if(Modbus.bbmd_en == 0)
//						{
//							 pdu_len = 0;
//							break;
//						}	
//            status = bvlc_create_bdt(&pdu[4], pdu_len);
//            if (status) {
//                bvlc_send_result(&sin, BVLC_RESULT_SUCCESSFUL_COMPLETION);
//            } else {
//                bvlc_send_result(&sin,
//                    BVLC_RESULT_WRITE_BROADCAST_DISTRIBUTION_TABLE_NAK);
//            }
//            /* not an NPDU */
//            pdu_len = 0;
//            break;
//        case BVLC_READ_BROADCAST_DIST_TABLE:
////            debug_printf("BVLC: Received Read-BDT.\n");
//            /* Upon receipt of a BVLL Read-Broadcast-Distribution-Table
//               message, a BBMD shall load the contents of its BDT into a BVLL
//               Read-Broadcast-Distribution-Table-Ack message and send it to the
//               originating device. If the BBMD is unable to perform the
//               read of its BDT, it shall return a BVLC-Result message to the
//               originating device with a result code of X'0020' indicating that
//               the read attempt has failed. */
//						if(Modbus.bbmd_en == 0)
//						{
//							 pdu_len = 0;
//							break;
//						}	
//            if (bvlc_send_bdt(&sin) <= 0) {
//                bvlc_send_result(&sin,
//                    BVLC_RESULT_READ_BROADCAST_DISTRIBUTION_TABLE_NAK);
//            }
//            /* not an NPDU */
//            pdu_len = 0;
//            break;
////        case BVLC_READ_BROADCAST_DIST_TABLE_ACK:
//////            debug_printf("BVLC: Received Read-BDT-Ack.\n");
////            /* FIXME: complete the code for client side read */
////            /* not an NPDU */
////            pdu_len = 0;
////            break;
//        case BVLC_FORWARDED_NPDU:
//            /* Upon receipt of a BVLL Forwarded-NPDU message, a BBMD shall
//               process it according to whether it was received from a peer
//               BBMD as the result of a directed broadcast or a unicast
//               transmission. A BBMD may ascertain the method by which Forwarded-
//               NPDU messages will arrive by inspecting the broadcast distribution
//               mask field in its own BDT entry since all BDTs are required
//               to be identical. If the message arrived via directed broadcast,
//               it was also received by the other devices on the BBMD's subnet. In
//               this case the BBMD merely retransmits the message directly to each
//               foreign device currently in the BBMD's FDT. If the
//               message arrived via a unicast transmission it has not yet been
//               received by the other devices on the BBMD's subnet. In this case,
//               the message is sent to the devices on the BBMD's subnet using the
//               B/IP broadcast address as well as to each foreign device
//               currently in the BBMD's FDT. A BBMD on a subnet with no other
//               BACnet devices may omit the broadcast using the B/IP
//               broadcast address. The method by which a BBMD determines whether
//               or not other BACnet devices are present is a local matter. */
//            /* decode the 4 byte original address and 2 byte port */
//						if(Modbus.bbmd_en == 0)
//						{
//							 pdu_len = 0;
//							break;
//						}	
//            bvlc_decode_bip_address(&pdu[4], &original_sin.sin_addr,
//                &original_sin.sin_port);
//            pdu_len -= 6;
//            /*  Broadcast locally if received via unicast from a BDT member */
//            if (bvlc_bdt_member_mask_is_unicast(&sin)) {
//                dest.sin_addr.s_addr = bip_get_broadcast_addr();
//                dest.sin_port = bip_get_port();		
//                bvlc_send_mpdu(&dest, &pdu[4 + 6], pdu_len);
//            }
//            /* use the original addr from the BVLC for src */
//            dest.sin_addr.s_addr = original_sin.sin_addr.s_addr;
//            dest.sin_port = original_sin.sin_port;
//            bvlc_fdt_forward_npdu(&dest, &pdu[4 + 6], pdu_len);
////            debug_printf("BVLC: Received Forwarded-NPDU from %s:%04X.\n",
// //               inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
//            bvlc_internet_to_bacnet_address(src, &dest);
//            if (pdu_len < max_pdu) {
//                /* shift the buffer to return a valid PDU */
//                for (i = 0; i < pdu_len; i++) {
//                    pdu[i] = pdu[4 + 6 + i];
//                }
//            } else {
//                /* ignore packets that are too large */
//                /* clients should check my max-apdu first */
//                pdu_len = 0;
//            }
//            break;
//        case BVLC_REGISTER_FOREIGN_DEVICE:
//            /* Upon receipt of a BVLL Register-Foreign-Device message, a BBMD
//               shall start a timer with a value equal to the Time-to-Live
//               parameter supplied plus a fixed grace period of 30 seconds. If,
//               within the period during which the timer is active, another BVLL
//               Register-Foreign-Device message from the same device is received,
//               the timer shall be reset and restarted. If the time expires
//               without the receipt of another BVLL Register-Foreign-Device
//               message from the same foreign device, the FDT entry for this
//               device shall be cleared. */
//						if(Modbus.bbmd_en == 0)
//						{
//							 pdu_len = 0;
//							 break;
//						}	
//						Test[40]++;
//            (void) decode_unsigned16(&pdu[4], &time_to_live);
//						
//            if (bvlc_register_foreign_device(&sin, time_to_live)) {
//							
//                bvlc_send_result(&sin, BVLC_RESULT_SUCCESSFUL_COMPLETION);
////                debug_printf("BVLC: Registered a Foreign Device.\n");
//            } else {
//                bvlc_send_result(&sin,
//                    BVLC_RESULT_REGISTER_FOREIGN_DEVICE_NAK);
////                debug_printf("BVLC: Failed to Register a Foreign Device.\n");
//            }
//            /* not an NPDU */
//            pdu_len = 0;
//            break;
//        case BVLC_READ_FOREIGN_DEVICE_TABLE:
////            debug_printf("BVLC: Received Read-FDT.\n");
//            /* Upon receipt of a BVLL Read-Foreign-Device-Table message, a
//               BBMD shall load the contents of its FDT into a BVLL Read-
//               Foreign-Device-Table-Ack message and send it to the originating
//               device. If the BBMD is unable to perform the read of its FDT,
//               it shall return a BVLC-Result message to the originating device
//               with a result code of X'0040' indicating that the read attempt has
//               failed. */
//						if(Modbus.bbmd_en == 0)
//						{
//							 pdu_len = 0;
//							 break;
//						}	
//							
//            if (bvlc_send_fdt(&sin) <= 0) {
//                bvlc_send_result(&sin,
//                    BVLC_RESULT_READ_FOREIGN_DEVICE_TABLE_NAK);
//            }
//            /* not an NPDU */
//            pdu_len = 0;
//            break;
////        case BVLC_READ_FOREIGN_DEVICE_TABLE_ACK:
//////            debug_printf("BVLC: Received Read-FDT-Ack.\n");
////            /* FIXME: complete the code for client side read */
////            /* not an NPDU */
////            npdu_len = 0;
////            break;
////        case BVLC_DELETE_FOREIGN_DEVICE_TABLE_ENTRY:
//////            debug_printf("BVLC: Received Delete-FDT-Entry.\n");
////            /* Upon receipt of a BVLL Delete-Foreign-Device-Table-Entry
////               message, a BBMD shall search its foreign device table for an entry
////               corresponding to the B/IP address supplied in the message. If an
////               entry is found, it shall be deleted and the BBMD shall return a
////               BVLC-Result message to the originating device with a result code
////               of X'0000'. Otherwise, the BBMD shall return a BVLCResult
////               message to the originating device with a result code of X'0050'
////               indicating that the deletion attempt has failed. */
////            if (bvlc_delete_foreign_device(&pdu[4])) {
////                bvlc_send_result(&sin, BVLC_RESULT_SUCCESSFUL_COMPLETION);
////            } else {
////                bvlc_send_result(&sin,
////                    BVLC_RESULT_DELETE_FOREIGN_DEVICE_TABLE_ENTRY_NAK);
////            }
////            /* not an NPDU */
////            npdu_len = 0;
////            break;
//        case BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK:
////          debug_printf
////               ("BVLC: Received Distribute-Broadcast-to-Network from %s:%04X.\n",
////                inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
//            /* Upon receipt of a BVLL Distribute-Broadcast-To-Network message
//               from a foreign device, the receiving BBMD shall transmit a
//               BVLL Forwarded-NPDU message on its local IP subnet using the
//               local B/IP broadcast address as the destination address. In
//               addition, a Forwarded-NPDU message shall be sent to each entry
//               in its BDT as described in the case of the receipt of a
//               BVLL Original-Broadcast-NPDU as well as directly to each foreign
//               device currently in the BBMD's FDT except the originating
//               node. If the BBMD is unable to perform the forwarding function,
//               it shall return a BVLC-Result message to the foreign device
//               with a result code of X'0060' indicating that the forwarding
//               attempt was unsuccessful */

//						
//            bvlc_forward_npdu(&sin, &pdu[4], pdu_len);
//            bvlc_bdt_forward_npdu(&sin, &pdu[4], pdu_len);
//            bvlc_fdt_forward_npdu(&sin, &pdu[4], pdu_len);
//            /* not an NPDU */
//            pdu_len = 0;
//            break;
        case BVLC_ORIGINAL_UNICAST_NPDU:
//            debug_printf("BVLC: Received Original-Unicast-NPDU.\n");
            /* ignore messages from me */
            if ((sin.sin_addr.s_addr == bip_get_addr()) &&
                (sin.sin_port == bip_get_port())) {
                pdu_len = 0;
            } else {
                bvlc_internet_to_bacnet_address(src, &sin);
                if (pdu_len < max_pdu) {
                    /* shift the buffer to return a valid PDU */
                    for (i = 0; i < pdu_len; i++) {
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
#endif

extern Byte	Station_NUM;

void bip_get_my_address(
    BACNET_ADDRESS * my_address)
{
    int i = 0;

    if (my_address) {
        my_address->mac_len = 6;
        memcpy(&my_address->mac[0], &BIP_Address.s_addr, 4);
        memcpy(&my_address->mac[4], &BIP_Port, 2);

	//	my_address->mac[0] = Station_NUM;

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


