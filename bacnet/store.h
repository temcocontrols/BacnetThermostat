#ifndef  _STORE_H
#define  _STORE_H 
#include "bitmap.h"

#define FLASH_APP1_ADDR		0x08008000 
#define STM_SECTOR_SIZE	2048


// page 125 0x0803 e800  - 0x0803 efff    2K  OUT
// page 126 0x0803 f000  - 0x0803 f7ff    2K  IN
// page 127 0x0803 f800  - 0x0803 ffff    2K  VAR

// page 250 0x0807 d000  - 0x0807 d7ff    2K  OUT
// page 251 0x0807 d800  - 0x0807 dfff    2K
// page 252 0x0807 e000  - 0x0807 e7ff    2K  IN
// page 253 0x0807 e800  - 0x0807 efff    2K  
// page 254 0x0807 f000  - 0x0807 f7ff    2K  VAR
// page 255 0x0807 f800  - 0x0807 ffff    2K  

#define FLASH_MODBUS_ID      		 0x8007700//use this to store modbus ID
#define FLASH_SERIAL_NUM_LO      0x8007710//use this to store serial number infomation 
#define FLASH_SERIAL_NUM_HI      0x8007720//use this to store serial number infomation

#define WR_PAGE_FLAG					0X0803d800
#define AR_PAGE_FLAG    			0X0803e000
#define OUT_PAGE_FLAG					0x0803e800//0x807d000
#define IN_PAGE_FLAG 					0x0803f000//0x807e000	
#define AV_PAGE_FLAG 					0x0803f800//0x807f000  //FLASH_APP1_ADDR + 254*STM_SECTOR_SIZE
 

//#define INFO_PAGE	(OUT_PAGE_FLAG+2)	
#define OUT_PAGE	(OUT_PAGE_FLAG+2)	
#define IN_PAGE		(IN_PAGE_FLAG+2)
#define AV_PAGE		(AV_PAGE_FLAG+2) 

#define WR_PAGE   (WR_PAGE_FLAG + 2)
#define AR_PAGE   (AR_PAGE_FLAG + 2)

enum{
	OUT_TYPE,
	IN_TYPE,
	VAR_TYPE,
	WR_TYPE,
	AR_TYPE,
	MAX_TYPE,
};
#pragma pack(push)
#pragma pack(1)
typedef struct
{
	int8_t description[21];	      /*  (21 uint8_ts; string)*/
	int8_t label[9];		      /*  (9 uint8_ts; string)*/

	float value;		      /*  (4 uint8_ts; float)*/

	uint8_t auto_manual;  /*  (1 bit; 0=auto, 1=manual)*/
	uint8_t digital_analog;  /*  (1 bit; 1=analog, 0=digital)*/
	uint8_t control	;
	uint8_t unused	;
	uint8_t range ; /*  (1 uint8_t ; variable_range_equate)*/

	
}	Str_variable_point; /* 21+9+4+1+1 = 36*/
#pragma pack(pop)

extern Str_variable_point var[] ;


#pragma pack(push)
#pragma pack(1)
typedef struct
{
	int8_t description[21]; 	       /* (21 uint8_ts; string)*/
	int8_t label[9];		       /* (9 uint8_ts; string)*/

	int32_t value;		       /* (4 uint8_ts; int32_t) */

	int8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog;  /* (1 bit; 0=digital, 1=analog)*/
	int8_t switch_status/*access_level*/;  /* (3 bits; 0-5)*/
	int8_t control ;  /* (1 bit; 0=off, 1=on)*/
	int8_t read_remote;//digital_control;  /* (1 bit)*/
	int8_t decom;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	int8_t range;	/* (1 uint8_t ; output_range_equate)*/

	uint8_t sub_id;//	uint8_t m_del_low;  /* (1 uint8_t ; if analog then low)*/
	uint8_t sub_product;//	uint8_t s_del_high; /* (1 uint8_t ; if analog then high)*/
	uint8_t sub_number;
	//uint8_t count;//uint16_t count ;//delay_timer;      /* (2 uint8_ts;  seconds,minutes)*/
	
	uint8_t pwm_period;
} Str_out_point;  /* 45 */
#pragma pack(pop)

extern Str_out_point outputs[];

#pragma pack(push)
#pragma pack(1)
typedef struct
{
	int8_t description[21]; 	      /* (21 uint8_ts; string)*/
	int8_t label[9];		      	/* (9 uint8_ts; string)*/

	int32_t value;		     						/* (4 uint8_ts; int32_t)*/

	uint8_t  filter;  /* */
	int8_t decom;/* (1 bit; 0=ok, 1=point decommissioned)*/  // high 4 bits input type, low 4 bits for old decom status
	uint8_t sub_id;//int8_t sen_on;/* (1 bit)*/
	uint8_t sub_product;//int8_t sen_off;  /* (1 bit)*/
	int8_t control; /*  (1 bit; 0=OFF, 1=ON)*/
	int8_t auto_manual; /* (1 bit; 0=auto, 1=manual)*/
	int8_t digital_analog ; /* (1 bit; 1=analog, 0=digital)*/
	int8_t calibration_sign; /* (1 bit; sign 0=positiv 1=negative )*/
	uint8_t sub_number;//int8_t calibration_increment; /* (1 bit;  0=0.1, 1=1.0)*/
	uint8_t calibration_hi; /* (5 bits - spare )*/ 
	uint8_t calibration_lo;  /* (8 bits; -25.6 to 25.6 / -256 to 256 )*/

	uint8_t range;	      /* (1 uint8_t ; input_range_equate)*/

} Str_in_point; /* 46 */
#pragma pack(pop)
extern Str_in_point inputs[];

void Flash_Write_Mass(void) ;

void mass_flash_init(void) ;


extern uint8_t write_page_en[MAX_TYPE] ;










#endif

