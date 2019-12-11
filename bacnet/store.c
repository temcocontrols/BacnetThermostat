#include "config.h"
#include "store.h"
#include "bacnet.h"
#include "stmflash.h"
#include "bsp_esp8266.h"

#define PAGE_LENTH		MAX_AVS * sizeof(Str_variable_point)
//#define PAGE_LENTH		MAX_AIS * sizeof(Str_in_point) 
uint8_t write_page_en[MAX_TYPE];
Str_variable_point var[MAX_AVS] ;
Str_out_point  outputs[MAX_OUTS];
Str_in_point   inputs[MAX_AIS]; 

extern Str_weekly_routine_point weekly_routines[MAX_WR];
extern Str_annual_routine_point annual_routines[MAX_AR];  

//static uint8_t  tempbuf[1024] = {0};

 
 
//#define PAGE127     0x0803f800 
// 


void Flash_Write_Mass(void)
{ 
	uint16_t	len = 0 ;
//	uint16_t    loop1;	 
	uint8_t     tempbuf[PAGE_LENTH]; 
	if(write_page_en[OUT_TYPE] == 1)
	{
		write_page_en[OUT_TYPE] = 0 ;	
		STMFLASH_Unlock();  //½âËø	
		STMFLASH_ErasePage(OUT_PAGE_FLAG);				
//		for(loop1 = 0;loop1 < MAX_OUTS;loop1++)
//		{
//			memcpy(&tempbuf[sizeof(Str_out_point) * loop1],&outputs[loop1],sizeof(Str_out_point));					
//		}
//		len = sizeof(Str_out_point) *MAX_OUTS ;
//		iap_write_appbin(OUT_PAGE,(uint8_t*)tempbuf, len); 
		
		len = MAX_OUTS * sizeof(Str_out_point) ; 
		memcpy(tempbuf,(void*)&outputs[0].description[0],len); 
		iap_write_appbin(OUT_PAGE,(uint8_t*)tempbuf, len); 
		
		STMFLASH_WriteHalfWord(OUT_PAGE_FLAG, 10000) ;
		STMFLASH_Lock();	
 	} 
	
	if(write_page_en[IN_TYPE] == 1)
	{
		write_page_en[IN_TYPE] = 0 ;
		STMFLASH_Unlock();	
		STMFLASH_ErasePage(IN_PAGE_FLAG);
//		for(loop1 = 0;loop1 < MAX_INS;loop1++)
//		{
//			memcpy(&tempbuf[sizeof(Str_in_point) * loop1],&inputs[loop1],sizeof(Str_in_point));					
//		}
//		len = sizeof(Str_in_point)*MAX_INS ;
		len = MAX_AIS * sizeof(Str_in_point) ; 
		memcpy(tempbuf,(void*)&inputs[0].description[0],len);  
		iap_write_appbin(IN_PAGE,(uint8_t*)tempbuf, len); 
		
		STMFLASH_WriteHalfWord(IN_PAGE_FLAG, 10000) ;
		STMFLASH_Lock();	
	} 
	
	if(write_page_en[VAR_TYPE] == 1)
	{  
			
		STMFLASH_Unlock();
		STMFLASH_ErasePage(AV_PAGE_FLAG);
		STMFLASH_ErasePage(AV_PAGE_FLAG + 2048);
		 
		len = MAX_AVS * sizeof(Str_variable_point) ; 
		memcpy(tempbuf,(void*)&var[0].description[0],len); 
		iap_write_appbin(AV_PAGE,(uint8_t*)tempbuf, len); 
		
		STMFLASH_WriteHalfWord(AV_PAGE_FLAG, 10000) ;	
		STMFLASH_Lock();
		write_page_en[VAR_TYPE] = 0 ; 
	}								
	if(write_page_en[WIFI_TYPE] == 1)
	{  
		STMFLASH_Unlock();
		STMFLASH_ErasePage(WIFI_PAGE_FLAG);

		iap_write_appbin(WIFI_PAGE,(u8 *)(&SSID_Info),sizeof(STR_SSID)); 
		
		STMFLASH_WriteHalfWord(WIFI_PAGE_FLAG, 10000) ;	
		STMFLASH_Lock();
		write_page_en[WIFI_TYPE] = 0 ; 
	}	 

	if(write_page_en[WIFI_TYPE] == 1)
	{  
		STMFLASH_Unlock();
		STMFLASH_ErasePage(WIFI_PAGE_FLAG);

		iap_write_appbin(WIFI_PAGE,(u8 *)(&SSID_Info),sizeof(STR_SSID)); 
		
		STMFLASH_WriteHalfWord(WIFI_PAGE_FLAG, 10000) ;	
		STMFLASH_Lock();
		write_page_en[WIFI_TYPE] = 0 ; 
	}

}
const uint8 Var_label[MAX_AVS][9] = {
	
	"Baudrate ",   //0
	"StnNumer ",   //1
	"Protocol ", //2
	"Instance ",//3
	"Schedule ",  //4
	"DaySP    ",//5					 
	"NightSP  ",//6
	"FanMode  ",//7
	"FWVER    ",  //8
	"CrntMode ",//9
	"Unit     ", //10 
	"SysMode  ",  //11
	"MenuLock ",     //12
	"OvrdTmr  ",//13
	"Pid2DSP  ",//14  
	"Pid2NSP  ",//15
	"OUTManul ",//16 
	"ORTLeft  ",//17 
	"CoolDB   ",//18
	"HeatDB   ",//19  
};
const uint8 Var_Description[MAX_AVS][21] = {
	
	"baudrate select      ",   	//0
	"station number       ",   	//1
	"modbus/bacnet switch ",    //2
	"instance number      ",		//3
	"schedule enable      ",		//4
	"pid1 day setpoint    ",		//5		 
	"pid1 night setpoint  ",		//6
	"fan mode             ",		//7
	"firmware version     ", 		//8
	"current H/C mode     ",		//9 
	"temprature unit      ",//10
	"system mode          ",//11
	"Menu lock            ",//12
	"override timer       ",//13
	"pid2 day setpoint    ",//14
	"pid2 night setpoint  ",//15
	"output manual enable ",//16
	"override timer left  ",//17
	"cooling deadband     ",//18
	"heating deadband     ",//19
};

const uint8 Outputs_label[MAX_OUTS][9] = {
 	"AO1",
	"AO2",
	"DO1",
	"DO2",
	"DO3",
	"DO4",
	"DO5",
};

const uint8 WR_label[MAX_WR][9] = {
 	"WR1",
	"WR2"
};

const uint8 WR_Description[MAX_WR][21] = {
 	
 	"WEEKLY ROUTINE1",
	"WEEKLY ROUTINE2",
//	"WEEKLY ROUTINE3",
//	"WEEKLY ROUTINE4",
//	"WEEKLY ROUTINE5",
//	"WEEKLY ROUTINE6",
//	"WEEKLY ROUTINE7",
};

const uint8 AR_label[MAX_AR][9] = {
 	"AR1",
};

const uint8 AR_Description[MAX_AR][21] = {
 	
 	"ANNUAL ROUTINE1",
};

const uint8 Outputs_Description[MAX_OUTS][21] = {
 	
 	"Analog output1",
	"Analog output2",
	"Digital output1",
	"Digital output2",
	"Digital output2",
	"Digital output2",
	"Digital output2",
};




const uint8 Inputs_label[MAX_AIS][9] = {
 	"AI1",
	"AI2",
	"AI3",
	"AI4",
	"AI5",
	"AI6",
	"AI7",
	"AI8",
	"TEM",
	"HUM",
	"CO2"	
};
const uint8 Inputs_Description[MAX_AIS][21] = {
 	
 	"Analog Input1",
	"Analog Input2",
	"Analog Input3",
	"Analog Input4",
	"Analog Input5",
	"Analog Input6",
	"Analog Input7",
	"Analog Input8",
	"Temperature",
	"Humidity",	
};




//void mass_flash_init(void)
//{  
//	uint8 i;
//	uint16 pos;
//	write_page_en = 0; 
//    
//	if(AT24CXX_ReadOneByte(EEP_FACTORY_SET) == 0x55) 
//	{
//		pos = 0;
//		for(i = 0;i < MAX_VARS; i++) 
//		{	 
//			STMFLASH_Read_Page(PAGE127, tempbuf);
//			memcpy(var[i].description,&tempbuf[pos],21); 
//			pos += 21;
//			memcpy(var[i].label,&tempbuf[pos],9); 
//			pos += 9;
//		}
//		for(i = 0;i < MAX_AOS; i++) 
//		{
//			memcpy(outputs[i].description,&tempbuf[pos],21); 
//			pos += 21;
//			memcpy(outputs[i].label,&tempbuf[pos],9); 
//			pos += 9;			
//		}
//	}
//	else 
//	{
//		AT24CXX_WriteOneByte(EEP_FACTORY_SET,0x55);
//		for(i = 0;i < MAX_VARS; i++) 
//		{	 
//			memcpy(var[i].description,Var_Description[i],21);  
//			memcpy(var[i].label,Var_label[i],9); 
//			 
//		}
//		for(i = 0;i < MAX_AOS; i++) 
//		{
//			memcpy(outputs[i].description,Outputs_Description[i],21);  
//			memcpy(outputs[i].label,Outputs_label[i],9);   		
//		}
//		
//		Flash_Write_Mass();
//	}	 
//}


// page 125 0x0803 e800  - 0x0803 efff    2K  OUT
// page 126 0x0803 f000  - 0x0803 f7ff    2K  IN
// page 127 0x0803 f800  - 0x0803 ffff    2K  VAR

// page 250 0x0807 d000  - 0x0807 d7ff    2K  OUT
// page 251 0x0807 d800  - 0x0807 dfff    2K
// page 252 0x0807 e000  - 0x0807 e7ff    2K  IN
// page 253 0x0807 e800  - 0x0807 efff    2K  
// page 254 0x0807 f000  - 0x0807 f7ff    2K  VAR
// page 255 0x0807 f800  - 0x0807 ffff    2K  

void mass_flash_init(void)
{
	u16 temp = 0 ;
	u16 loop , j ;
	u16 len = 0;
	uint8_t  tempbuf[PAGE_LENTH];
// 	u16 temp2 = 0 ;
//	u8 label_buf[21] ;
	for(j = 0; j < MAX_TYPE; j++)
		write_page_en[j] = 0;
//---------------------------------OUT ROUTINE---------------------------- 
 	temp = STMFLASH_ReadHalfWord(OUT_PAGE_FLAG);
//	printf("temp=%x, %x\n\r", temp2, temp);
	if(1)//(temp != 10000)
	{
//		STMFLASH_Unlock();
//		STMFLASH_ErasePage(OUT_PAGE_FLAG);
		for(loop=0; loop<MAX_OUTS; loop++ )
		{
			memcpy(outputs[loop].description,Outputs_Description[loop],21);  
			memcpy(outputs[loop].label,Outputs_label[loop],9);  		
			outputs[loop].value = 0; 
			outputs[loop].auto_manual = 0 ;
			outputs[loop].digital_analog = 0 ;
			outputs[loop].switch_status = 0 ;
			outputs[loop].control = 0 ;
			outputs[loop].out_of_service = 0 ;
			outputs[loop].decom = 0 ;
			outputs[loop].range = 0 ;
			outputs[loop].sub_id = 0 ;
			outputs[loop].sub_product = 0 ;
			outputs[loop].pwm_period = 0 ;
		}
		len = MAX_OUTS * sizeof(Str_out_point) ;
//		if(len > PAGE_LENTH)
//		{
//			memcpy(tempbuf,(void *)&outputs[0].description[0],PAGE_LENTH); 
//			memcpy(tempbuf,(void *)(&outputs[0].description[0] + PAGE_LENTH),len - PAGE_LENTH);
//		}
//		else
//		memcpy(tempbuf,(void *)&outputs[0].description[0],len);  
//		iap_write_appbin(OUT_PAGE,(uint8_t*)tempbuf, len);	 
//		STMFLASH_WriteHalfWord(OUT_PAGE_FLAG, 10000) ;
//		STMFLASH_Lock();	
	}
//	else
//	{
//		len = MAX_OUTS * sizeof(Str_out_point) ;
//		STMFLASH_MUL_Read(OUT_PAGE,(void *)&outputs[0].description[0], len );	
//	}
//---------------------------------AV ROUTINE----------------------------	
	temp = STMFLASH_ReadHalfWord(AV_PAGE_FLAG);  
	if(1)//(temp != 10000)
	{
//		STMFLASH_Unlock();
//		STMFLASH_ErasePage(AV_PAGE_FLAG);
//		STMFLASH_ErasePage(AV_PAGE_FLAG + 2048);
		for(loop=0; loop<MAX_AVS; loop++ )
		{
			memcpy(var[loop].description,Var_Description[loop],21);  
			memcpy(var[loop].label,Var_label[loop],9); 
			var[loop].value = 0; 
			var[loop].auto_manual = 0 ;
			var[loop].digital_analog = 0 ;
			var[loop].control = 0 ;
			var[loop].unused = 0 ;
			var[loop].range = 0 ;
			var[loop].range = 0 ;
		}
//		len = MAX_AVS * sizeof(Str_variable_point);
//		memcpy(tempbuf,(void*)&var[0].description[0],len); 
//		iap_write_appbin(AV_PAGE,(uint8_t*)tempbuf, len); 
//		STMFLASH_WriteHalfWord(AV_PAGE_FLAG, 10000);	
//		STMFLASH_Lock();
	}
//	else
//	{
//		len = MAX_AVS * sizeof(Str_variable_point) ;
//		STMFLASH_MUL_Read(AV_PAGE,(void *)&var[0].description[0], len );
//	}
//---------------------------------AI ROUTINE----------------------------	
	temp = STMFLASH_ReadHalfWord(IN_PAGE_FLAG);
	if(1)//(temp != 10000)
	{
//		STMFLASH_ErasePage(IN_PAGE_FLAG);
		for(loop=0; loop<MAX_AIS; loop++ )
		{		 
			memcpy(inputs[loop].description, Inputs_Description[loop], 21);
			memcpy(inputs[loop].label, Inputs_label[loop], 9); 
			inputs[loop].value = 0; 
			inputs[loop].filter = 5 ;
			inputs[loop].decom = 0 ;
			inputs[loop].sub_id = 0 ;
			inputs[loop].sub_product = 0 ;
			inputs[loop].control = 0 ;
			inputs[loop].auto_manual = 0 ;
			inputs[loop].digital_analog = 0 ;
			inputs[loop].calibration_sign = 0 ;
			inputs[loop].sub_number = 0 ;
			inputs[loop].calibration_hi = (500>>8)&0xff ;
			inputs[loop].calibration_lo = 500 &0xff ;
			inputs[loop].range = 0 ; 
		}
//		len = MAX_AIS * sizeof(Str_in_point) ;
//		memcpy(tempbuf,(void*)&inputs[0], len);		
//		iap_write_appbin(IN_PAGE,(uint8_t*)tempbuf, len);	
//		STMFLASH_WriteHalfWord(IN_PAGE_FLAG, 10000) ;
	}
//	else
//	{
//		len = MAX_AIS * sizeof(Str_in_point) ;
//		STMFLASH_MUL_Read(IN_PAGE,(void *)&inputs[0].description[0], len ); 
//	}
//---------------------------------WEEKLY ROUTINE----------------------------
	temp = STMFLASH_ReadHalfWord(WR_PAGE_FLAG);
	if(temp != 10000)
	{
		STMFLASH_ErasePage(WR_PAGE_FLAG);
		for(loop=0; loop<MAX_WR; loop++ )
		{		 
			memcpy(weekly_routines[loop].description, WR_Description[loop], 21);
			memcpy(weekly_routines[loop].label, WR_label[loop], 9); 
			weekly_routines[loop].value = 0;  
		}
		len = MAX_WR * sizeof(Str_weekly_routine_point) ;
		memcpy(tempbuf,(void*)&weekly_routines[0], len);		
		iap_write_appbin(WR_PAGE,(uint8_t*)tempbuf, len);	
		STMFLASH_WriteHalfWord(WR_PAGE_FLAG, 10000);
	}
	else
	{
		len = MAX_WR * sizeof(Str_weekly_routine_point) ;
		STMFLASH_MUL_Read(WR_PAGE,(void *)&weekly_routines[0].description[0], len ); 
	}	
//----------------------------------ANNUAL ROUTINE-----------------------------------
	temp = STMFLASH_ReadHalfWord(AR_PAGE_FLAG);
	if(temp != 10000)
	{
		STMFLASH_ErasePage(AR_PAGE_FLAG);
		for(loop=0; loop<MAX_AR; loop++ )
		{		 
			memcpy(annual_routines[loop].description, AR_Description[loop], 21);
			memcpy(annual_routines[loop].label, AR_label[loop], 9); 
			annual_routines[loop].value = 0;  
		}
		len = MAX_AR * sizeof(Str_annual_routine_point) ;
		memcpy(tempbuf,(void*)&annual_routines[0], len);		
		iap_write_appbin(AR_PAGE,(uint8_t*)tempbuf, len);	
		STMFLASH_WriteHalfWord(AR_PAGE_FLAG, 10000) ;
	}
	else//
	{
		len = MAX_AR * sizeof(Str_annual_routine_point) ;
		STMFLASH_MUL_Read(AR_PAGE,(void *)&annual_routines[0].description[0], len ); 
	}	

	temp = STMFLASH_ReadHalfWord(WIFI_PAGE_FLAG);
	if(temp != 10000)
	{
		memset(&SSID_Info,0,sizeof(STR_SSID));
		STMFLASH_ErasePage(WIFI_PAGE_FLAG);
		iap_write_appbin(WIFI_PAGE,(void *)(&SSID_Info), sizeof(STR_SSID));	
		STMFLASH_WriteHalfWord(WIFI_PAGE_FLAG, 10000) ;
	}
	else//
	{
		STMFLASH_MUL_Read(WIFI_PAGE,(void *)(&SSID_Info),sizeof(STR_SSID));
	}		
}

