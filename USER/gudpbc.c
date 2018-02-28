#include "gudpbc.h"
#include "stm32f10x.h"




/*
 * ----------------------------------------------------------------------------
 * Function Name: GUDPBC_Receive
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
//void GUDPBC_Receive(U8_T XDATA* pData, U16_T length, U8_T id)
//{
//	U8_T n, state;
////	if((pData[0] == 100) && (pData[1] == 0))
//	if(pData[0] == 0x64)
//	{
//		state = 1;
//		for(n = 0; n < (U8_T)length / 4; n++)
//        { 
////        	if((pData[4*n+1] == ip_address[0]) && (pData[4*n+2] == ip_address[1]) && (pData[4*n+3] == ip_address[2]) && (pData[4*n+4] == ip_address[3]))
////            { 
////                state=0;
////                break;
////            }
//        }
////        if(state)
////			gudpbc_HandleSearchReq(pData, id, 0);
//	}
////	else if((pData[0] == 0x66) && (pData[1] == ip_address[0]) && (pData[2] == ip_address[1]) && (pData[3] == ip_address[2]) && (pData[4] == ip_address[3]))
////	{
////		n = 5;

////		/*ip_mode = 0;

////		ip_address[0] = pData[n++];
////		ip_address[1] = pData[n++];
////		ip_address[2] = pData[n++];
////		ip_address[3] = pData[n++];

////		subnet_mask_address[0] = pData[n++];
////		subnet_mask_address[1] = pData[n++];
////		subnet_mask_address[2] = pData[n++];
////		subnet_mask_address[3] = pData[n++];

////		gateway_address[0] = pData[n++];
////		gateway_address[1] = pData[n++];
////		gateway_address[2] = pData[n++];
////		gateway_address[3] = pData[n++];

////		ChangeIP = 1;*/

////		gudpbc_HandleSearchReq(pData, id, 1);
////	}
////	else if((pData[0] == 0xee) && (pData[1] == 0x10))
////	{   
////		gudpbc_HandleSearchReq(pData, id, 0); 
////	    IntFlashErase(ERA_RUN, 0x60000);
////	    AX11000_SoftReboot(); 	
////	}
//}