#include "define.h"
//#include "LibDisplay.h"


//#include "constcode.h"
//DISPTEMP const fan_speed_4c[5] = {
//	{0xC3,"OFF"},
//	{0xC3,"HEAT"},
//	{0xC3,"COOL"},
//	{0xC3,"GAS"},
//	{0xC3,"AUTO"}, 
//} ;

//uint8 const poll_co2_cmd[3][8] = 
//{
//	{0xff, 0x03, 0x00, 0x6b, 0x00, 0x01, 0xe0, 0x08},	//reg 107:  temperature
//	{0xff, 0x03, 0x00, 0x6c, 0x00, 0x01, 0x31, 0xc9},	//reg 108:humidity
//	{0xff, 0x03, 0x00, 0x6d, 0x00, 0x01, 0x00, 0x09}	//reg 109:co2	
//};
//#define BAUDRATE_9600						0
//#define BAUDRATE_19200					1
//#define BAUDRATE_38400          2
//#define BAUDRATE_57600					3
//#define BAUDRATE_115200					4
//#define BAUDRATE_76800					5
//#define BAUDRATE_1200						6
//#define BAUDRATE_4800						7
//#define BAUDRATE_14400					8
unsigned int short const baudrate[9]={96,192,384,576,1152,768,12,48,144};

uint8 const poll_co2_cmd[9]={0xFF, 0x01, 0x86, 0x00,0x00,0x00,0x00,0x00,0X79};

uint8 const code tem_unit[2] = {'C','F'};


uint16  const code table1_custom_default[ ] =
   {150,85,60,45,35,25,15,5,65531,65521,65486};

uint16  const code table2_custom_default[ ] =
   {0,300,600,900,1200,1500,1800,2100,2400,2700,3000};

uint8 const code default_valve_table[21] =    // [mode_operation]
{
	// 8bit

	//XX XX XX XX
	//v4 v3 v2 v1

//XX	00		01				10				11
//		0%		0-100%			50-100%			100%
//
//  coast, 	cool1,	cool2,	cool3,	heat1,	heat2,	heat3
	0x00,	0x11,	0x33,	0x33,	0x14,	0x3c,	0x3c,   // PID1 ON ANALOG OUTPUT_1TO5 TABLE
	0x00,	0x11,	0x33,	0x33,	0x14,	0x3c,	0x3c,   // PID2 ON ANALOG OUTPUT_1TO5 TABLE
	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00    // PID1 OFF ANALOG OUTPUT_1TO5 TABLE

} ;

uint8 const code default_pwm_table[7] =    // [mode_operation]
{
//  coast, 	cool1,	cool2,	cool3,	heat1,	heat2,	heat3
	0,		0X40,		0X30,		0X10,	0X04,		0X03,		0X01
} ;

uint8 code  fan_table[2][4] = {{0,2,2,4},{1,3,3,4}};

uint8 const code def_tab[11] =
			{					 // 10k termistor GREYSTONE -40 to 120 Deg.C or -40 to 248 Deg.F 
			 192, 209, 206, 187, 161, 131, 103, 79, 61, 45, 155
			};
//MHF:12-30-05,Added 4 values to make the tstat can measure minus degree
uint8 const code def_tab_pic[19] =
			{					 // 10k termistor GREYSTONE -40 to 120 Deg.C or -40 to 248 Deg.F 
			 25, 39, 61, 83, 102, 113, 112, 101, 85, 67, 51, 38, 28, 21, 15,11, 8, 6, 23
 			};

uint8 const code def_tab_type3[19] = //10K thernistor type 3
			{ 
			 27, 42, 62, 86, 96, 104, 103,  94, 80,  64, 51, 40, 31, 23, 17,13,10, 8, 30
			};
//uint8 const code def_tab_type3_old[15] = //10K thernistor type 3
//			{ 
//			 29, 45, 63, 81, 96, 104, 103,  94, 80, 65, 51, 40, 30, 23, 77
//			};

uint16 const code def_tab_type50K[10] = //NTC 50K for Chen
			{ 
				12,13,15,14,18,18,19,24,19,791
			 //57,57,55,52,49,45,40,36,31,27,24,21,18,15,13,11,10,8,57
			};



//uint8 const code PWM_def_tab[51] =
//			{				
//				5,4,5,5,5,6,6,6,7,7,8,8,8,9,10,10,11,11,13,13,15,16,17,18,21,26,30,35,35,36,41,43,46,47,47,43,40,36,33,29,27,25,23,21,19,19,17,16,15,15,14
//			};
#ifndef TSTAT7_ARM
uint8 const code LCD_LINE1[15][9]=
{
	"        ",
	"  TEMP  ",
	"SETPOINT",
	" INPUT1 ",
	" INPUT2 ",
	" INPUT3 ",
	" INPUT4 ",
	" INPUT5 ",
	" INPUT6 ",
	" INPUT7 ",
	" INPUT8 ",
	" MODE   ",
	" USER   ",
	"CO2  ppm",
	"HUMIDITY"

};

DISPTEMP const code ion[4] = {
{0xC3,"LOF"},
{0xC3,"LON"},
{0xC3,"POF"},
{0xC3,"PON"},
};


// Display fan speed stages on LCD
DISPTEMP const code clock_month[12] =
{
{0xC3,"Jan"}, 											
{0xC3,"Feb"}, 											
{0xC3,"Mar"},											
{0xC3,"Apr"}, 											
{0xC3,"May"},											
{0xC3,"Jun"},										
{0xC3,"Jul"},											
{0xC3,"Aug"}, 											
{0xC3,"Sep"},											
{0xC3,"Oct"},
{0xC3,"Nov"},											
{0xC3,"Dec"},
};

DISPTEMP const code clock_week[7] =
{
{0xC3,"Mon "}, 											
{0xC3,"Tues"}, 											
{0xC3,"Wed "},											
{0xC3,"Thur"}, 											
{0xC3,"Fri "},											
{0xC3,"Sat "},										
{0xC3,"Sun "},											
};
DISPTEMP const code fan_speed[6] = {
	{0xC3,"OFF"},
	{0xC3,"-1-"},
	{0xC3,"-2-"},
	{0xC3,"-3-"},
	{0xC3,"AUTO"},
	{0xC3,"ON"},
 
} ;
//Menu information on LCD display

MENUNAME const menu[TOTAL_MENU_PARAMETERS + 1 + 2 + 1] = {//Added setpoint and fan mode to menu
{"Temperatur","Calibrate"},		//00 CAL single point calibration of temperature
{"Temperatur","Select"},		//01 Temperature Sensor Select
{"Temperatur","Filter"},		//02 Temperature filter
{"Analog","Input1"},		//03 Analog input 1
{"Analog","Input2"},		//04 Analog input 2
//{0x83,"dI1"},		//05 Digital input 1  //cc not digital input any more
{"Override","Timer"},		//06 Override timer
{"DigitalOut","Calibrate"},		//07 Digital to analog output calibration
{"Baudrate","Select "},		//08 Baudrate 
{"ShortCycle","Delay"},		//09 Short cycle delay
{"ChangeOver","Delay"},		//10 Changeover delay
{"Proportion","nal Term"},		//11 Proportional term
{"Integral","Term"},		//12 Integral term
{"Operation","Sequency"},		//13 Different sequence of operations

{"HeatCool","Config"},		//14 Heating and cooling config
{"Cooling","Deadband"},		//15 Cooling deadband
{"Heating","Deadband"},		//16 Heating deadband
{"Degree","C/F"},		//17 Degree C or degree F
{"FanSpeed","Select"},		//19 Select fan speed number
{"NightHeat","Deadband"},		//19 Night heating deadband

{"NightCool","Deadband"},		//20 Night cooling deadband
{"NightHeat","Setpoint"},		//21 Night heating setpoint
{"NotUsed"," "},		//21 Night heating setpoint	//NOT USE
{"NightCool","Setpoint"},		//22 Night cooling setpoint	//NOT USE
{"NotUsed"," "},		//22 Night cooling setpoint	//NOT USE

{"Applicatio","Mode"},		//23 Application mode ,hotel or office
{"PowerUp","Setpoint"},		//24 Power up cooling setpoint
{"PowerUp","On/Off"},		//25 Power up on or off

{"KeyPad","Select"},		//26 Keypad select
{"AutoFan","Control"},		//27 Allow user to set auto fan or not
{"AnalogOut1","Calibrate"},		//28 Calibration analog output channel 1
{"AnalogOut2","Calibrate"},		//29 Calibration analog output channel 2
{"Max","Setpoint"},		//30 Max setpoint
{"Min","Setpoint"},		//31 Min setpoint

{"MenuLock","mode"},		//32 Menu lock mode
{"LedMode","NotUsed"},		//33 Led display mode
{"ValveTravl","Time"},		//34 Full valve travel time
{"RS485/ZGB","Select"},
{"MODBUS","BACNET"},

{"",""},
{"",""},
{"",""},
{"",""},
{"",""},
{"",""},
{"",""},

{"","Workday"},
{"","Workday"},
{"","Workday"},
{"","Workday"},
{"","Weekend"},
{"","Weekend"},
{"","Weekend"},
{"Protocol","Select"},

{"Save as","Default"},		//35 Store current set as factory default
{"Factory","Default"},		//36 Factory default


{"Modbus","Address"},		//37 Addrerss
//when LOCK=3,the user have to into menu to adjust temperature setpoiont and fan mode
{"Setpoint"," "},		//56 Setpoint

{"Fan","Speed"},		//57 Fan speed
}; 


//DISPTEMP  const code menu[TOTAL_MENU_PARAMETERS + 1 + 2 + 1] = {//Added setpoint and fan mode to menu
//{0x83,"CAL"},		//00 CAL single point calibration of temperature
//{0x83,"tSS"},		//01 Temperature Sensor Select
//{0x83,"FIL"},		//02 Temperature filter
//{0x83,"AI1"},		//03 Analog input 1
//{0x83,"AI2"},		//04 Analog input 2
////{0x83,"dI1"},		//05 Digital input 1  //cc not digital input any more
//{0x83,"Ort"},		//06 Override timer
//{0x83,"dAC"},		//07 Digital to analog output calibration
//{0x83,"bAU"},		//08 Baudrate 
//{0x83,"dSC"},		//09 Short cycle delay
//{0x83,"dCH"},		//10 Changeover delay
//{0x83,"PPr"},		//11 Proportional term
//{0x83,"PIn"},		//12 Integral term
//{0x83,"SOP"},		//13 Different sequence of operations

//{0x83,"HC"},		//14 Heating and cooling config
//{0x83,"Cdb"},		//15 Cooling deadband
//{0x83,"Hdb"},		//16 Heating deadband
//{0x83,"C_F"},		//17 Degree C or degree F
//{0x83,"FAn"},		//19 Select fan speed number
//{0x83,"nHd"},		//19 Night heating deadband

//{0x83,"nCd"},		//20 Night cooling deadband
//{0x83,"nSp"},		//21 Night heating setpoint
//{0x83,"nHSH"},		//21 Night heating setpoint	//NOT USE
//{0x83,"nCS"},		//22 Night cooling setpoint	//NOT USE
//{0x83,"nCSH"},		//22 Night cooling setpoint	//NOT USE

//{0x83,"APP"},		//23 Application mode ,hotel or office
//{0x83,"POS"},		//24 Power up cooling setpoint
//{0x83,"POn"},		//25 Power up on or off

//{0x83,"PAd"},		//26 Keypad select
//{0x83,"AUt"},		//27 Allow user to set auto fan or not
//{0x83,"OU1"},		//28 Calibration analog output channel 1
//{0x83,"OU2"},		//29 Calibration analog output channel 2
//{0x83,"SHI"},		//30 Max setpoint
//{0x83,"SLO"},		//31 Min setpoint

//{0x83,"LOC"},		//32 Menu lock mode
//{0x83,"dIS"},		//33 Led display mode
//{0x83,"Vtt"},		//34 Full valve travel time
//{0X82,"485/ZGB"},
//{0x83,"DEMAND"},

//{0x83,""},
//{0x82,""},
//{0x83,""},
//{0x83,""},
//{0x83,""},
//{0x82,""},
//{0x82,""},

//{0x81,"Workday"},
//{0x81,"Workday"},
//{0x81,"Workday"},
//{0x81,"Workday"},
//{0x81,"Weekend"},
//{0x81,"Weekend"},
//{0x81,"Weekend"},
//{0x81,"Protocol"},

//{0x83,"DEF"},		//35 Store current set as factory default
//{0x83,"FAC"},		//36 Factory default


//{0x83,"Add"},		//37 Addrerss
////when LOCK=3,the user have to into menu to adjust temperature setpoiont and fan mode
//{0x83,"SEt"},		//38 Setpoint

//{0x83,"FAS"},		//39 Fan speed
// 

//}; 

uint8 const code SP_ITEM[3][8]=
{
" TEM_SP ",
" CO2_SP ",
" HUM_SP "

};


uint8 const code LCD_LINE2[9][9]=
{ 
	"        ",
	"      C ",
	"      F ",
	"      % ",
	"      RH",
	"     AMP",
	"      KW",
	"      KA",
	"     ppm",
};
uint8 const code MODE[8][9]=
{ 
	"COASTING",
	"COOLING",
	"HEATING",
	"FAN OFF",
	"FAN -1-",
	"FAN -2-",
	"FAN -3-",
	"FAN AUTO",
};

#endif //TSTAT7_ARM



uint8  const code init_big_ticks[ TOTAL_TIMERS ] =
{
  5 ,    //00 update temperature timer  300 msec
  10 ,   //01 temperature display  1 sec  cc:original = 10
  10 ,   //02 control logic
  10 ,   //03 update PID update every 1 sec, update_pid() ,
  10,    //   chech the PID value each seconds to decide if need to change valve position
   4 ,   //04 REFRESH_OUTPUTS_TIMER , // Refresh the analog outputs
  10 ,   //05 blink timer      1000 ms
  30,    //06 keypad activity  5000 ms
  4	,	 //07 eight led timer
  9 ,    //08 dealwith the data received from serialport.
  10,		 // output1
  10,        // output2
  10,	     // output3
  10,	     // output4
  10,		 // output5
  10,		 // output6
  10,		 // output7
  100,		// state switch timer
  100,		// cooling lockout timer
  100,		// heating lockout timer
  91,		// override timer
  10,       // open new relays ,delay one second then control use PWM
  100,		// turn relay on if the temperature less than freeze protect setpoint in 10 seconds
  10,		// one second timer
  100,		// 10s timer for lcd turn off delay
  3,		// 2 second timer for LCD screen rotate  //CC original =20
  100,      // run schedule routine, every 10s 
  5,		//clock menu "date" blink timer
  5,		//clock menu "time" blink timer
  5,		//schedule menu "time" blink timer 
//  10		//rs485/wireless detect timer
} ;


uint8 const code parameter_array[2][ TOTAL_EE_PARAMETERS - MAXEEPCONSTRANGE][2] =
{
		{
		//    LOWER  UPPER
		 {0 ,    20    }, //01 EEP_TEMP_SELECT //temperature sensor select,3 = average internal sensor and external sensor
		 {0 ,    100  }, //02 EEP_FILTER //temperature filter
		 {0 ,    5    }, //03 EEP_ANALOG_IN1 //analog in 1
		 {0 ,    5    }, //04 EEP_ANALOG_IN2 //analog in 2
		 {0 ,    255  }, //05 EEP_OVERRIDE_TIMER //OVERRIDE_TIMER
		 {0 ,    255  }, //06 EEP_DAC_OFFSET //DAC OFFSET
		 {0 ,    8  },   //07 EEP_BAUDRATEbaud rate
		 {0 ,    20  },  //08 EEP_CYCLING_DELAY
		 {0 ,    200  }, //09 EEP_CHANGOVER_DELAY               
		 {0 ,    255},  //10 EEP_COOLING_PTERM //COOLING_PTERM  =  2C
		 {0 ,    255   },//11 EEP_COOLING_ITERM //COOLING_ITEMR = 10
		 {0 ,    TSTAT_SOP_TOTAL-1  }, //12EEP_SEQUENCE //SEQUENCE for different sequences of operation 
		 {0 ,    5  },   //13 EEP_HEAT_COOL_CONFIG  	
		 {1 ,    255  }, //14 EEP_COOLING_DEADBAND //COOLING_DEADBAND  = 1 deg c	
		 {1 ,    255  }, //15 EEP_HEATING_DEADBAND //HEATING_DEADBAND  = 1 deg c
		 {0 ,    1    }, //16 EEP_DEGC_OR_F //temperature units  0 = DegC, 1 = DegF 
		 {0 ,    4    }, //17 EEP_FAN_MODE 
		 {0 ,    255   }, //18 EEP_NIGHT_HEATING_DEADBAND  
		 {0 ,    255   }, //19 EEP_NIGHT_COOLING_DEADBAND 
		 {0,    255   }, //20 EEP_NIGHT_HEATING_SETPOINT LOW BYTE
		 {0,    255   }, // EEP_NIGHT_HEATING_SETPOINT HIGH BYTE
		 {0 ,   255   }, // EEP_NIGHT_COOLING_SETPOINT LOW BYTE
		 {0 ,   255   }, // EEP_NIGHT_COOLING_SETPOINT HIGH BYTE
		 {0 ,    1    }, // EEP_APPLICATION // HOTEL, OFFICE, HOME, ....
		 {0,    255   }, // 25EEP_POWERUP_SETPOINT //POS power on setpoint= cooling_day_setpoint, used for SOP=4 and future modes
		 {0 ,    3    }, // EEP_POWERUP_ON_OFFPOMer on mode setting, on or off. Some customers will ask for options on this 
		 {0 ,    8    }, // EEP_KEYPAD_SELECT,6 =2C,the lower two buttons used to adjust setpoint;7 = 6C,the upper two buttons use to control lighting
		 {0 ,    2    }, // EEP_AUTO_ONLY ,
		 {0,    255     }, // EEP_OUTPUT1_SCALE
		 {0,    255     }, // 30EEP_OUTPUT2_SCALE
		 {0,    255   }, // EEP_MAX_SETPOINT //max setpoint
		 {0,    255   }, // EEP_MIN_SETPOINT //min setpoint
		 {0 ,    4    },  	//  EEP_SPECIAL_MENU_LOCK//3,user can not do anything from keypad,and only can do somthign from menu;4,part of menu items for gridpoint
		 {0 ,    6    },	//  EEP_DISPLAY; 6,display nothing except menu mode		
 		 {10 ,    255 }, 	//35  EEP_VALVE_TRAVEL_TIME
		 {0,	101	  },	//  EEP_RS485_MODERS //485
		 {0,	2	  },  //EEP_DEMAND_RESPONSE //demand response
		 {0,    99   },  //clock year
		 {1,    12    },  //clock month
		 {1,    31     },  //clock day
		 {1,    7    },  	//clock week
		 {0,    23    },  //clock hour
		 {0,    60    },  //clock minute
		 {0,    60    },  //clock second

		 {0,    60    },  //schedule work day time hour
		 {0,    60    },  //schedule work day time minute
		 {0,    60    },  //schedule work night time hour
		 {0,    60    },  //schedule work night time minute
		 {0,    60    },  //schedule workend day time hour
		 {0,    60    },  //schedule workend day time minute
		 {0,    60    },  //schedule workend night time hour
		 {0,    60    },  //schedule workend night time minute
 		 {0,      255 },  //EEP_STORE_DEFAULTSv //store the default
		 {0,    255   }, // EEP_FACTORY_DEFAULTS //FAC
		 {0,    255   }, // 40EEP_DAY_COOLING_SETPOINT low 
		 {0,    255   }, // EEP_DAY_COOLING_SETPOINT high
		 {0,    255   }, // EEP_DAY_HEATING_SETPOINT 
		 {0,    255   }, // EEP_DAY_HEATING_SETPOINT
		 {0,    255   }, // EEP_NIGHT_SETPOINT 
		 {0,    255   }, // EEP_NIGHT_SETPOINT
		 {0 ,    4    }, // EEP_FAN_SPEED //User fan setting	
		 {0 ,    6    }, //heat Stages in the  PID3 table
		 {0,     6    }, //cool stages in the  PID3 table
		 {0 ,    6    }, // EEP_HEAT_UNIVERSAL_TABLE //heat Stages in the universal PID table
		 {0,     6    }, // EEP_COOL_UNIVERSAL_TABLE //cool stages in the uinversal PID table
		 {0 ,    6    }, // EEP_HEAT_ORIGINAL_TABLE //heat Stages in the universal PID table
		 {0,     6    }, // 50EEP_COOL_ORIGINAL_TABLE //cool stages in the uinversal PID table	 
		 {0,     255  }, // EEP_INPUT1_SELECT // Select which input chanel to be used in PID caculation '1' analog input1 '2' analog input2,3 = humidity
		 {0,    255   }, // EEP_UNIVERSAL_PTERM //Lower  byte P term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_PTERM //higher byte P term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_ITERM //Lower  byte I term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_ITERM //higher byte I term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_SET //Lower  byte setpoint,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_SET //higher byte setpoint,don't use it
         {0,    255   }, // EEP_UNIVERSAL_NIGHTSET //Lower  byte night setpoint,don't use it
         {0,    255   }, // EEP_UNIVERSAL_NIGHTSET //higher byte night setpoint,don't use it
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // 60EEP_LED1_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED2_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED3_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED4_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED5_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED6_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED7_CONTROL
         {0 ,   3     }, //EEP_PID_OUTPUT1 //select which PI to control the output.OUT1
		 {0 ,   3     }, //EEP_PID_OUTPUT2 //OUT2
		 {0 ,   3     }, //60EEP_PID_OUTPUT3 //OUT3
		 {0 ,   3     }, //70EEP_PID_OUTPUT4 //OUT4
		 {0 ,   3     }, //EEP_PID_OUTPUT5 //70 OUT5
		 {0 ,   3     }, //EEP_PID_OUTPUT6 //OUT6
		 {0 ,   3     }, //EEP_PID_OUTPUT7 //OUT7
		 {0 ,   3     }, //EEP_RANGE_OUTPUT1 //OUT1  ON/OFF PWM FLOATING
		 {0 ,   3     }, //EEP_RANGE_OUTPUT2 //OUT2  ON/OFF PWM FLOATING
		 {0 ,   3     }, //EEP_RANGE_OUTPUT3 //OUT3  ON/OFF PWM FLOATING
		 {0 ,   3     }, //EEP_RANGE_OUTPUT4 //OUT4  ON/OFF PWM FLOATING 
		 {0 ,   3     }, //EEP_RANGE_OUTPUT5 //OUT5  ON/OFF PWM FLOATING 
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT1 //interlock items MHF:2006-3-17  Added two items .timer or and timer and
		 {0,    7     }, //80EEP_INTERLOCK_OUTPUT2
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT3
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT4
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT5	
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT6
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT7
		 {1,    10    }, //EEP_SETPOINT_INCREASE //the incresement each temperature setpoint increase 
		 {0,    255   }, //EEP_FREEZE_TEMP_SETPOINT //freeze protect temperature setpoint
		 {5,    255   }, //EEP_FREEZE_DELAY_ON //defrost after delay xx seconds 
		 {5,    255   }, //EEP_FREEZE_DELAY_OFF //heating xx minutes
		 {0,    10     }, //90EEP_ANALOG1_FUNCTION //analog1 function 
		 {0,    10     }, //EEP_ANALOG2_FUNCTION // analog2 function
		 {0,    255   }, //EEP_TIMER_ON //timer on ,2 bytes
		 {0,    255   }, //EEP_TIMER_ON
		 {0,    255   }, //EEP_TIMER_OFF //timer off ,2 bytes
		 {0,    255   }, //EEP_TIMER_OFF
		 {0,    255   }, //EEP_MIN_ADDRESS //minimum address be allowed ,2 bytes
		 {0,    255   }, //EEP_MIN_ADDRESS
		 {0,    255   }, //EEP_MAX_ADDRESS //maximum address be allowed ,2 bytes
		 {0,    255   }, //EEP_MAX_ADDRESS
		 {0,    2     }, //100EEP_TIMER_UNITS //timer units
		 {0,	255   }, //EEP_INPUT_MANU_ENABLE // input manual enable ,2bytes 
		 {0,	255   }, //EEP_INTERNAL_SENSOR_MANU_ENABLE
		 {0,	255   }, //EEP_OUTPUT_MANU_ENABLE output manual enable
		 {0,	1	  }, //EEP_BASE_ADDRESS //BASE ADDRESS SELECT 
		 {0,	255	  }, //EEP_DEAD_MASTER //dead master counter
		 {0,	5	  }, //EEP_ROUND_DISPLAY //rounding for display
		 {0,	1	  }, //EEP_CONFIGURATION //config flag 
		 {0,	3	  }, //EEP_TIMER_SELECT //select that user timer is for interval timer or rotation timer,2 = INTERLOCK TIMER  ,3 = PWM FEATURE
		 {0,	3	  }, //EEP_OUTPUT1_FUNCTION //rotation flag,each bool from LST represent the corresponding output if be put into totation group.use this before version 32.1.
		 {0,	3	  }, //110EEP_OUTPUT2_FUNCTION //use one byte to store the function for each output
	     {0,	3	  }, //EEP_OUTPUT3_FUNCTION //0 = normal,1 = rotation,2 = lighting , 3 = PWM	
		 {0,	3	  }, //EEP_OUTPUT4_FUNCTION
		 {0,	3	  }, //EEP_OUTPUT5_FUNCTION
		 {0,	255	  }, //EEP_DEFAULT_SETPOINT
		 {0,    2     }, //EEP_SETPOINT_CONTROL
		 {0,	1	  }, //EEP_PIR_STAGE PIR stage,0 = disable pir,1 = enable pir
         {0,    1     },// EEP_SYSTEM_TIME_FORMAT //system time format  0 = 12h, 1 = 24h
		 {0,	255	  },// EEP_ANALOG1_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = CUSTOMER,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG2_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = CUSTOMER,5 = OFF/ON
		 {0,	255	  },// 120EEP_ANALOG3_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG4_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG5_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG6_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG7_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG8_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
	     {0,	255   },// EEP_LCD_TURN_OFF //LCD turn off delay,0 - 255 minutes
	     {0,	255   },// EEP_FREE_COOL_CONFIG //free cool configure
	     {0,	255   },// EEP_LOCK_REGISTER //Lock register
	     {0,	255   },// EEP_CUSTOM_ADDRESS //Store the data customer write
		 {0,	15	  },// 130EEP_LCD_SCREEN1 //What message shows on LCD screen 1,0 = none,1 = temp,2 = setpoint,3 = IN1...10 = IN8,11 = MODE	 13 = user info
 		 {0,	15	  },// EEP_LCD_SCREEN2 //What message shows on LCD screen 2,0 = none,1 = temp,2 = setpoint,3 = IN1...10 = IN8,11 = MODE	 13 = user info
		 {0,	2     },//SUN icon control,0 = auto,1=  turn off,2 = turn on
		 {0,    2     },//MOON icon control,0 = auto,1=  turn off,2 = turn on
		 {0,		1 },// 132EEP_DAYSETPOINT_OPTION  //day setpoint option,1 =hotel,0 = office	
		 {0,      3}, //heating/cooling/auto mode select
		 {0,	255},//analog input1 filter	
		 {0,	255},//analog input2 filter	
		 {0,	255},//analog input3 filter	
		 {0,	255},//analog input4 filter	
		 {0,	255},//analog input5 filter	
		 {0,	255},//analog input6 filter	
		 {0,	255},//analog input7 filter	
		 {0,	255},//analog input8 filter
		 {0,	255},//CO2 filter
		 {0,	255},//hum filter
		 {0,	255},//co2 cilibration data high 8bits
		 {0,	255},//co2 cilibration data low 8bits
		} ,
		{
		//    LOWER  UPPER
		 {0 ,    20    }, //01 EEP_TEMP_SELECT //temperature sensor select,3 = average internal sensor and external sensor
		 {0 ,    100  }, //02 EEP_FILTER //temperature filter
		 {0 ,    5    }, //03 EEP_ANALOG_IN1 //analog in 1
		 {0 ,    5    }, //04 EEP_ANALOG_IN2 //analog in 2
		 {0 ,    255  }, //05 EEP_OVERRIDE_TIMER //OVERRIDE_TIMER
		 {0 ,    255  }, //06 EEP_DAC_OFFSET //DAC OFFSET
		 {0 ,    8  },   //07 EEP_BAUDRATE //baud rate
		 {0 ,    20  },  //08 EEP_CYCLING_DELAY
		 {0 ,    200  }, //09 EEP_CHANGOVER_DELAY               
		 {0 ,   255},  //10 EEP_COOLING_PTERM //COOLING_PTERM  =  2C
		 {0 ,    255   },//11 EEP_COOLING_ITERM //COOLING_ITEMR = 10
		 {0 ,    TSTAT_SOP_TOTAL-1  }, //12 EEP_SEQUENCE //SEQUENCE for different sequences of operation 
		 {0 ,    5  },   //13 EEP_HEAT_COOL_CONFIG  	
		 {1 ,    255  }, //14 EEP_COOLING_DEADBAND //COOLING_DEADBAND  = 1 deg c	
		 {1 ,    255  }, //15 EEP_HEATING_DEADBAND //HEATING_DEADBAND  = 1 deg c
		 {0 ,    1    }, //16 EEP_DEGC_OR_F //temperature units  0 = DegC, 1 = DegF 
		 {0 ,    4    }, //17 EEP_FAN_MODE 
		 {0 ,    95   }, //18 EEP_NIGHT_HEATING_DEADBAND //night heating setback 
		 {0 ,    99   }, //19 EEP_NIGHT_COOLING_DEADBAND //night cooling setback 
		 {0,    255   }, //20 EEP_NIGHT_HEATING_SETPOINT LOW BYTE
		 {0,    255   }, // EEP_NIGHT_HEATING_SETPOINT HIGH BYTE
		 {0 ,   255   }, // EEP_NIGHT_COOLING_SETPOINT LOW BYTE
		 {0 ,   255   }, // EEP_NIGHT_COOLING_SETPOINT HIGH BYTE
		 {0 ,    1    }, // EEP_APPLICATION // HOTEL, OFFICE, HOME, ....
		 {0,    255   }, // EEP_POWERUP_SETPOINT //POS power on setpoint= cooling_day_setpoint, used for SOP=4 and future modes
		 {0 ,    3    }, // EEP_POWERUP_ON_OFFPOMer on mode setting, on or off. Some customers will ask for options on this 
		 {0 ,    8    }, // EEP_KEYPAD_SELECT,6 =2C,the lower two buttons used to adjust setpoint;7 = 6C,the upper two buttons use to control lighting
		 {0 ,    2    }, // EEP_AUTO_ONLY ,
 		 {0,    255   }, // EEP_OUTPUT1_SCALE
		 {0,    255   }, //30 EEP_OUTPUT2_SCALE
		 {0,    255   },   	//  EEP_MAX_SETPOINT //max setpoint
		 {0,    255   },   	//  EEP_MIN_SETPOINT //min setpoint
		 {0 ,    4    } ,  	// EEP_SPECIAL_MENU_LOCK//3,user can not do anything from keypad,and only can do somthign from menu;4,part of menu items for gridpoint
		 {0 ,    6    },	// EEP_DISPLAY; 6,display nothing except menu mode		
 		 {10 ,    255 }, 	// EEP_VALVE_TRAVEL_TIME
		 {0,	101	  },	// EEP_RS485_MODERS //485
		 {0,	2	  },  //EEP_DEMAND_RESPONSE //demand response
		 {0,    99   },  //clock year
		 {1,    12    },  //clock month
		 {1,    7     },  //clock week
		 {1,    31    },  //clock day
		 {0,    23    },  //clock hour
		 {0,    60    },  //clock minute
		 {0,    60    },  //clock second

		 {0,    60    },  //schedule work day time hour
		 {0,    60    },  //schedule work day time minute
		 {0,    60    },  //schedule work night time hour
		 {0,    60    },  //schedule work night time minute
		 {0,    60    },  //schedule workend day time hour
		 {0,    60    },  //schedule workend day time minute
		 {0,    60    },  //schedule workend night time hour
		 {0,    60    },  //schedule workend night time minute
 		 {0,      255 },  //EEP_STORE_DEFAULTSv //store the default
		 {0,    255   }, // EEP_FACTORY_DEFAULTS //FAC
		 {0,    255   }, //40 EEP_DAY_COOLING_SETPOINT LOW BYTE
		 {0,    255   }, // EEP_DAY_COOLING_SETPOINT HIGH BYTE
		 {0,    255   }, //EEP_DAY_HEATING_SETPOINT 
		 {0,    255   }, // EEP_DAY_HEATING_SETPOINT
		 {0,    255   }, // EEP_NIGHT_SETPOINT LOW BYTE
		 {0,    255   }, // EEP_NIGHT_SETPOINT HIGH BYTE
		 {0 ,    4    }, // EEP_FAN_SPEED //User fan setting
		 {0 ,    6    }, //heat Stages in the  PID3 table
		 {0,     6    }, //cool stages in the  PID3 table	
		 {0 ,    6    }, // EEP_HEAT_UNIVERSAL_TABLE //heat Stages in the universal PID table
		 {0,     6    }, // EEP_COOL_UNIVERSAL_TABLE //cool stages in the uinversal PID table
		 {0 ,    6    }, // EEP_HEAT_ORIGINAL_TABLE //heat Stages in the universal PID table
		 {0,     6    }, //50 EEP_COOL_ORIGINAL_TABLE //cool stages in the uinversal PID table	 
		 {0,     255  }, // EEP_INPUT1_SELECT // Select which input chanel to be used in PID caculation '1' analog input1 '2' analog input2,3 = humidity
		 {0,    255   }, // EEP_UNIVERSAL_PTERM //Lower  byte P term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_PTERM //higher byte P term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_ITERM //Lower  byte I term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_ITERM //higher byte I term,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_SET //Lower  byte setpoint,don't use it
		 {0,    255   }, // EEP_UNIVERSAL_SET //higher byte setpoint,don't use it
         {0,    255   }, // EEP_UNIVERSAL_NIGHTSET //Lower  byte night setpoint,don't use it
         {0,    255   }, // EEP_UNIVERSAL_NIGHTSET //higher byte night setpoint,don't use it
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, //60 EEP_LED1_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED2_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED3_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED4_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED5_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED6_CONTROL
		 {0 ,    TOTAL_LED_CONTROL_STATES-1  }, // EEP_LED7_CONTROL
         {0 ,   3     }, //EEP_PID_OUTPUT1 //select which PI to control the output.OUT1
		 {0 ,   3     }, //EEP_PID_OUTPUT2 //OUT2
		 {0 ,   3     }, //EEP_PID_OUTPUT3 //OUT3
		 {0 ,   3     }, //70EEP_PID_OUTPUT4 //OUT4
		 {0 ,   3     }, //EEP_PID_OUTPUT5 //70 OUT5
		 {0 ,   3     }, //EEP_PID_OUTPUT6 //OUT6
		 {0 ,   3     }, //EEP_PID_OUTPUT7 //OUT7
		 {0 ,   3     }, //EEP_RANGE_OUTPUT1 //OUT1  ON/OFF PWM FLOATING
		 {0 ,   3     }, //EEP_RANGE_OUTPUT2 //OUT2  ON/OFF PWM FLOATING
		 {0 ,   3     }, //EEP_RANGE_OUTPUT3 //OUT3  ON/OFF PWM FLOATING
		 {0 ,   3     }, //EEP_RANGE_OUTPUT4 //OUT4  ON/OFF PWM FLOATING 
		 {0 ,   3     }, //EEP_RANGE_OUTPUT5 //OUT5  ON/OFF PWM FLOATING 	 
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT1 //interlock items MHF:2006-3-17  Added two items .timer or and timer and
		 {0,    7     }, //80EEP_INTERLOCK_OUTPUT2
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT3
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT4
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT5	
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT6
		 {0,    7     }, //EEP_INTERLOCK_OUTPUT7
		 {1,    10    }, //EEP_SETPOINT_INCREASE //the incresement each temperature setpoint increase 
		 {0,    255   }, //EEP_FREEZE_TEMP_SETPOINT //freeze protect temperature setpoint
		 {5,    255   }, //EEP_FREEZE_DELAY_ON //defrost after delay xx seconds 
		 {5,    255   }, //EEP_FREEZE_DELAY_OFF //heating xx minutes
		 {0,    10     }, //90EEP_ANALOG1_FUNCTION //analog1 function 
		 {0,    10     }, //EEP_ANALOG2_FUNCTION // analog2 function
		 {0,    255   }, //EEP_TIMER_ON //timer on ,2 bytes
		 {0,    255   }, //EEP_TIMER_ON
		 {0,    255   }, //EEP_TIMER_OFF //timer off ,2 bytes
		 {0,    255   }, //EEP_TIMER_OFF
		 {0,    255   }, //EEP_MIN_ADDRESS //minimum address be allowed ,2 bytes
		 {0,    255   }, //EEP_MIN_ADDRESS
		 {0,    255   }, //EEP_MAX_ADDRESS //maximum address be allowed ,2 bytes
		 {0,    255   }, //EEP_MAX_ADDRESS
		 {0,    2     }, //100EEP_TIMER_UNITS //timer units
		 {0,	255   }, //EEP_INPUT_MANU_ENABLE // input manual enable ,2bytes 
		 {0,	255   }, //EEP_INTERNAL_SENSOR_MANU_ENABLE
		 {0,	255   }, //EEP_OUTPUT_MANU_ENABLE output manual enable
		 {0,	1	  }, //EEP_BASE_ADDRESS //BASE ADDRESS SELECT 
		 {0,	255	  }, //EEP_DEAD_MASTER //dead master counter
		 {0,	5	  }, //EEP_ROUND_DISPLAY //rounding for display
		 {0,	1	  }, //EEP_CONFIGURATION //config flag 
		 {0,	3	  }, //EEP_TIMER_SELECT //select that user timer is for interval timer or rotation timer,2 = INTERLOCK TIMER  ,3 = PWM FEATURE
		 {0,	3	  }, //EEP_OUTPUT1_FUNCTION //rotation flag,each bool from LST represent the corresponding output if be put into totation group.use this before version 32.1.
		 {0,	3	  }, //110EEP_OUTPUT2_FUNCTION //use one byte to store the function for each output
	     {0,	3	  }, //EEP_OUTPUT3_FUNCTION //0 = normal,1 = rotation,2 = lighting , 3 = PWM	
		 {0,	3	  }, //EEP_OUTPUT4_FUNCTION
		 {0,	3	  }, //EEP_OUTPUT5_FUNCTION
		 {0,	255	  }, //EEP_DEFAULT_SETPOINT
		 {0,    2     }, //EEP_SETPOINT_CONTROL
		 {0,	1	  }, //EEP_PIR_STAGE PIR stage,0 = disable pir,1 = enable pir
         {0,    1     },// EEP_SYSTEM_TIME_FORMAT //system time format  0 = 12h, 1 = 24h
		 {0,	255	  },// EEP_ANALOG1_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = CUSTOMER,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG2_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = CUSTOMER,5 = OFF/ON
		 {0,	255	  },//120 EEP_ANALOG3_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG4_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG5_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG6_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG7_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
		 {0,	255	  },// EEP_ANALOG8_RANGE.0 = raw data,1 = thermistor,2 = %,3 = ON/OFF,4 = N/A,5 = OFF/ON
	     {0,	255   },// EEP_LCD_TURN_OFF //LCD turn off delay,0 - 255 minutes
	     {0,	255   },// EEP_FREE_COOL_CONFIG //free cool configure
	     {0,	255   },// EEP_LOCK_REGISTER //Lock register
	     {0,	255   },// EEP_CUSTOM_ADDRESS //Store the data customer write
		 {0,	15	  },//130 EEP_LCD_SCREEN1 //What message shows on LCD screen 1,0 = none,1 = temp,2 = setpoint,3 = IN1...10 = IN8,11 = MODE	,13 = user info
 		 {0,	15	  },// EEP_LCD_SCREEN2 //What message shows on LCD screen 2,0 = none,1 = temp,2 = setpoint,3 = IN1...10 = IN8,11 = MODE	,13 = user info
		 {0,	2     },//SUN icon control,0 = auto,1=  turn off,2 = turn on
		 {0,    2     },//MOON icon control,0 = auto,1=  turn off,2 = turn on
		 {0,		1 },//123 EEP_DAYSETPOINT_OPTION  //day setpoint option,1 =hotel,0 = office
		 {0,      3},
		 {0,	255},//analog input1 filter	
		 {0,	255},//analog input2 filter	
		 {0,	255},//analog input3 filter	
		 {0,	255},//analog input4 filter	
		 {0,	255},//analog input5 filter	
		 {0,	255},//analog input6 filter	
		 {0,	255},//analog input7 filter	
		 {0,	255},//analog input8 filter	
		 {0,	255},//CO2 filter
		 {0,	255},//hum filter
		 {0,	255},//co2 cilibration data high 8bits
		 {0,	255},//co2 cilibration data low 8bits
		 } 
};
uint8 const code parameter_default[2][ TOTAL_EE_PARAMETERS - MAXEEPCONSTRANGE ] =
{
 /*  1                                                                                                        10                     																								20			                                                                                                                30                                                                                                                                             40                                                   																									    50																																									60																																	  70																																   80																																											90																																																	100																																				110																																			120																																								      130																							*/
	{2,/*TSS*/	DEFAULT_FILTER,/*FIL*/	 0,/*AI1*/	 0,/*AI2*/	  60,/*Ort*/ 100,/*dAC*/ 1,/*bau*/  0,/*dSC*/  0,/*dCH*/   60,/*CPr*/    50,/*CIn*/  1,/*SOP*/   0,/*HC*/	10,/*CDb*/ 10,/*HDb*/ 0,/*C_F*/  3,/*FAn*/  10,/*nHd*/ 10,/*nCd*/	190,/*nHSl*/ 0,/*nHSh*/   210,/*nCSl*/ 0x00/*nCSh*/,    0,/*APP*/  20,/*POS*/ 2,/*POn*/  1,/*PAD*/  0,/*AUT*/  1,/*OU1*/   1,/*OU2*/	 50,/*MaxS*/   15,/*MinS*/   	0,/*LOC*/   0,/*dIS*/  90,/*Vtt*/    COMMUNICATION_MODE,/*RS485*/	  0,/*DEMAND*/ 0,0,0,0,0,0,0,/*clock*/  0,0,0,0,0,0,0,0,/*schedule*/    0,/*DEF*/     0,/*FAC*/    210,/*DCSL*/    0,/*DCSH*/     190,/*DHSL*/   0,/*DHSH*/      200,/*NSl*/     0,/*NSh*/      0,/*FAS */  3,/*PID3HSTG*/ 3,/*PID3cSTG*/
	3,/*UHSTG*/ 		3,/*UCSTG*/  	3,/*HOT*/		    3,/*COT*/  	 3,/*InTYPE*/         0xe8,/*PLO*/   	0x03,/*PHI*/    0x0a,/*ILO*/     0,/*IHI*/     0xd0,/*uSLO*/   0x07,/*uSHI*/   0xb8,/*uNSLO*/    0x0b,/*uNSHI*/      3,/*LD1*/    2,/*LD2*/    1,/*LD3*/  15,/*LD4*/  23,/*LD5*/  24,/*LD6*/  19,/*LD7*/   0,/*PIO1*/ 	   0,/*PIO2*/     	0,/*PIO3*/ 	  0,/*PIO4*/    0,/*PIO5*/    0,/*PIO6*/    0,/*PIO7*/     0,/*O1*/ 0,/*O2*/ 0,/*O3*/ 0,/*O4*/ 0,/*O5*/ 				  0,/*ItLkO1*/ 0,/*ItLkO2*/ 0,/*ItLkO3*/ 0,/*ItLkO4*/ 0,/*ItLkO5*/ 0,/*ItLkO6*/ 0,/*ItLkO7*/  10,/*SP_INCRESE*/          5,/*freezeSP*/       10,/*delayON*/       30,/*delayOFF*/    		0,/*AI1*/  0,/*AI2*/     0,/*timer_onH*/         0,/*timer_onL*/		0,/*timer_offH*/          0,/*timer_offL*/			1,/*MINidL*/           0,/*MINidH*/	     254,/*MAXidL*/	  0,/*MAXidH*/	 0,/*units*/		0,/*MANUAL_AI*/      0,/*MANUAL_INTERNAL*/     0,/*OUen*/	   0,/*BASE*/	0,	/*DeadM*/   1,/*round*/	  0,/*con*/   0,/*TS*/ 	 0,/*OF1*/	0,/*OF2*/	0,/*OF3*/	0,/*OF4*/	0,/*OF5*/	 20,/*defSP*/	0,/*ConReg*/	   0,/*Pir*/	0,/*SysF*/    	  0,/*AO1RNG*/ 0,/*AO2RNG*/ 0,/*AO3RNG*/	0,/*AO4RNG*/	0,/*AO5RNG*/	0,/*AO6RNG*/	0,/*AO7RNG*/	0,/*AO8RNG*/		255,/*LCDO/F*/	   	0,/*FRC*/	 0,/*LOCK*/    100,/*CUSTOM*/	   1,/*LCD1*/	 0,/*LCD2*/	  0,/*SUN*/  0,/*MOON*/   1/*DSPT*/, 1,/*mode*/ 
	DEFAULT_FILTER,/*IN1_filter*/ DEFAULT_FILTER,/*IN2_filter*/ DEFAULT_FILTER,/*IN3_filter*/ DEFAULT_FILTER,/*IN4_filter*/ DEFAULT_FILTER,/*IN5_filter*/ DEFAULT_FILTER,/*IN6_filter*/ DEFAULT_FILTER,/*IN7_filter*/ DEFAULT_FILTER,/*IN8_filter*/ DEFAULT_FILTER,/*CO2_filter*/ DEFAULT_FILTER,/*HUm_filter*/ 0,/*co2_cal_l*/ 0/*co2_cal_h*/},  // deg C 
	{2,/*TSS*/	DEFAULT_FILTER,/*FIL*/	 0,/*AI1*/	 0,/*AI2*/	  60,/*Ort*/ 100,/*dAC*/ 4,/*bau*/  0,/*dSC*/  0,/*dCH*/   60,/*CPr*/    50,/*CIn*/  1,/*SOP*/   0,/*HC*/	10,/*CDb*/ 10,/*HDb*/ 1,/*C_F*/  3,/*FAn*/  10,/*nHd*/ 10,/*nCd*/	0x96,/*nHSl*/0x02,/*nHSh*/0xba,/*nCSl*/ 0x02/*nCSh*/,    0,/*APP*/  68,/*POS*/ 2,/*POn*/  1,/*PAD*/  0,/*AUT*/  1,/*OU1*/   1,/*OU2*/	 99,/*MaxS*/   55,/*MinS*/   	0,/*LOC*/   0,/*dIS*/  90,/*Vtt*/    COMMUNICATION_MODE,/*RS485*/	  0,/*DEMAND*/ 0,0,0,0,0,0,0,/*clock*/  0,0,0,0,0,0,0,0,/*schedule*/    0,/*DEF*/     0,/*FAC*/    0xba,/*DCSL*/   2,/*DCSH*/     0x96,/*DHSL*/  2,/*DHSH*/      0Xa8,/*NSl*/    2,/*NSh*/      0,/*FAS */  3,/*PID3HSTG*/ 3,/*PID3cSTG*/
    3,/*UHSTG*/  	    3,/*UCSTG*/  	3,/*HOT*/		    3,/*COT*/  	 3,/*InTYPE*/         0xe8,/*PLO*/   	0x03,/*PHI*/    0x0a,/*ILO*/     0,/*IHI*/     0xd0,/*uSLO*/   0x07,/*uSHI*/   0xb8,/*uNSLO*/    0x0b,/*uNSHI*/      3,/*LD1*/    2,/*LD2*/    1,/*LD3*/  15,/*LD4*/  23,/*LD5*/  24,/*LD6*/  19,/*LD7*/   0,/*PIO1*/ 	   0,/*PIO2*/    	0,/*PIO3*/ 	  0,/*PIO4*/    0,/*PIO5*/    0,/*PIO6*/    0,/*PIO7*/     0,/*O1*/ 0,/*O2*/ 0,/*O3*/ 0,/*O4*/ 0,/*O5*/ 				  0,/*ItLkO1*/ 0,/*ItLkO2*/ 0,/*ItLkO3*/ 0,/*ItLkO4*/ 0,/*ItLkO5*/ 0,/*ItLkO6*/ 0,/*ItLkO7*/  10,/*SP_INCRESE*/          40,/*freezeSP*/      10,/*delayON*/       30,/*delayOFF*/    		0,/*AI1*/  0,/*AI2*/     0,/*timer_onH*/         0,/*timer_onL*/		0,/*timer_offH*/          0,/*timer_offL*/			1,/*MINidL*/           0,/*MINidH*/	     254,/*MAXidL*/	  0,/*MAXidH*/	 0,/*units*/		0,/*MANUAL_AI*/      0,/*MANUAL_INTERNAL*/     0,/*OUen*/	   0,/*BASE*/	0,	/*DeadM*/   1,/*round*/	  0,/*con*/   0,/*TS*/	 0,/*OF1*/	0,/*OF2*/	0,/*OF3*/	0,/*OF4*/	0,/*OF5*/	 68,/*defSP*/	0,/*ConReg*/	   0,/*Pir*/	0,/*SysF*/    	  0,/*AO1RNG*/ 0,/*AO2RNG*/ 0,/*AO3RNG*/	0,/*AO4RNG*/	0,/*AO5RNG*/	0,/*AO6RNG*/	0,/*AO7RNG*/	0,/*AO8RNG*/		255,/*LCDO/F*/	  	0,/*FRC*/	 0,/*LOCK*/    100,/*CUSTOM*/	   1,/*LCD1*/	 0,/*LCD2*/	  0,/*SUN*/  0,/*MOON*/   1/*DSPT*/, 1,/*mode*/
	DEFAULT_FILTER,/*IN1_filter*/ DEFAULT_FILTER,/*IN2_filter*/ DEFAULT_FILTER,/*IN3_filter*/ DEFAULT_FILTER,/*IN4_filter*/ DEFAULT_FILTER,/*IN5_filter*/ DEFAULT_FILTER,/*IN6_filter*/ DEFAULT_FILTER,/*IN7_filter*/ DEFAULT_FILTER,/*IN8_filter*/ DEFAULT_FILTER,/*CO2_filter*/ DEFAULT_FILTER,/*HUm_filter*/ 0,/*co2_cal_l*/ 0/*co2_cal_h*/}, // deg F

};

 

// MDF 5/3/04 Changed extern_operation_default table so that OUT4 and OUT5 correspond to table.
//uint8 const code extern_operation_default[ ] = 
////		Coast Cool1 Cool2 Cool3 Heat1 Heat2 Heat3
//	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
//		0x01, 0x01, 0x09, 0x09, 0x01, 0x01, 0x11 , //Fan1
//		0x02, 0x02, 0x0a, 0x0a, 0x02, 0x02, 0x12 , //Fan2
//		0x04, 0x04, 0x0c, 0x0c, 0x04, 0x04, 0x14 , //Fan3
//		0x01, 0x41, 0xca, 0xcc, 0x41, 0xd2, 0xd4 ,  //Fan4
//		0x00, 0x41, 0xca, 0xcc, 0x41, 0xd2, 0xd4 ,  //only fan auto ,use for universal PID
//    };

#ifdef SANJAY
uint8 const code extern_operation_sop1[ ] = 
//		Coast Cool1 Cool2 Cool3 Heat1 Heat2 Heat3
	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
		0x00, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00 , //Fan1  //cooling output + fan_low on
		0x00, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00 , //Fan2  //cooling output + fan_med on
		0x00, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00 , //Fan3  //cooling output + fan_high on
		0x00, 0x41, 0xca, 0xcc, 0x41, 0xd2, 0xd4 , //Fan4
		0x00, 0x41, 0xca, 0xcc, 0x41, 0xd2, 0xd4   //only fan auto ,use for universal PID
    };
#else
uint8 const code extern_operation_sop1[ ] = 
//		Coast Cool1 Cool2 Cool3 Heat1 Heat2 Heat3
	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
		0x01, 0x01, 0x09, 0x09, 0x01, 0x01, 0x11 , //Fan1
		0x02, 0x02, 0x0a, 0x0a, 0x02, 0x02, 0x12 , //Fan2
		0x04, 0x04, 0x0c, 0x0c, 0x04, 0x04, 0x14 , //Fan3
		0x01, 0x41, 0xca, 0xcc, 0x41, 0xd2, 0xd4 , //Fan4
		0x00, 0x41, 0xca, 0xcc, 0x41, 0xd2, 0xd4   //only fan auto ,use for universal PID
    };
#endif
uint8 const code extern_operation_customer[ ] = 
//		Coast Cool1 Cool2 Heat1 Heat2 Spare Spare
	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
		0x00, 0x11, 0x31, 0x03, 0x07, 0x00, 0x00 , //Fan1
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan2
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan3
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan4
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   //only fan auto ,use for universal PID
    };

uint8 const code extern_operation_default[ ] = 	 //standar 
//		Coast Cool1 Cool2 Heat1 Heat2 Spare Spare
	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
		0x00, 0x09, 0x19, 0x03, 0x07, 0x00, 0x00 , //Fan1
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan2
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan3
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan4
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   //only fan auto ,use for universal PID
    };


uint8 const code extern_operation_sop4[ ] = 	 //heat	pump
//		Coast Cool1 Cool2 Heat1 Heat2 Spare Spare
	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
		0x00, 0x09, 0x19, 0x05, 0x07, 0x00, 0x00 , //Fan1
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan2
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan3
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan4
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   //only fan auto ,use for universal PID
    };

uint8 const code extern_operation_sop5[ ] = 	//cool pump
//		Coast Cool1 Cool2 Heat1 Heat2 Spare Spare
	{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan0
		0x00, 0x0d, 0x1d, 0x01, 0x03, 0x00, 0x00 , //Fan1
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan2
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan3
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , //Fan4
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   //only fan auto ,use for universal PID
    };


uint8 const code OUTPUT_DELAY_TABLE[2][7]= 
{
  {0,0,0,0,0,0,0},			      // 1 = 0.1s
  {0,0,0,0,0,0,0}
};

#ifndef TSTAT_ARM
uint8 const code auchCRCHi[256] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;	
	/* Table of CRC values for high¨Corder byte */


uint8 const code  auchCRCLo[256] = {

0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40

} ;	
#endif
//////////////////////////////////////////////////////////////////////////////////////////////
//uint16 const code tbl_hummity_freq[10][2]={
//		{HUMCOUNT1_H,HUMRH1_H},
//		{HUMCOUNT2_H,HUMRH2_H},
//		{HUMCOUNT3_H,HUMRH3_H},
//		{HUMCOUNT4_H,HUMRH4_H},
//		{HUMCOUNT5_H,HUMRH5_H},
//		{HUMCOUNT6_H,HUMRH6_H},
//		{HUMCOUNT7_H,HUMRH7_H},
//		{HUMCOUNT8_H,HUMRH8_H},	
//		{HUMCOUNT9_H,HUMRH9_H},
//		{HUMCOUNT10_H,HUMRH10_H}
//};
///////////////////////////////////////////////////////////////////////////////////////////////
//uint8 const code user_message[2][8] = {
//	{0x20,0x20,0x55,0x73,0x65,0x72,0x20,0x20},	//==User
//	{0x20,0x4D,0X65,0X73,0X73,0X61,0x67,0x65}	//==Message
//};



