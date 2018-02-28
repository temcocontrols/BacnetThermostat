/***********************define short variable name instead of long one to make code easy to understand****************/
//TBD :global repalce all these things
#include "define.h"
//#ifdef TSTAT_ARM
//#define Serial_number(x)  								b.eeprom[EEP_SERIALNUMBER_LOWORD + x]
//#define laddress												b.eeprom[EEP_ADDRESS]
//#endif

//typedef struct
//	{ 
//	unsigned char eeprom[TOTAL_EEP_VARIABLES];
//	}LINK;

extern LINK b;
	
#ifdef TSTAT_ARM

//#define zigbee_index										b.eeprom[51]
//#define zigbee_point_info(x)						b.eeprom[52 + x]


#define ScheduleMondayEvent1(x)					b.eeprom[EEP_SCHEDULE_MONDAY_EVENT1_H + x]
#define ScheduleMondayEvent2(x)					b.eeprom[EEP_SCHEDULE_MONDAY_EVENT2_H + x]
#define ScheduleMondayEvent3(x)					b.eeprom[EEP_SCHEDULE_MONDAY_EVENT3_H + x]
#define ScheduleMondayEvent4(x)					b.eeprom[EEP_SCHEDULE_MONDAY_EVENT4_H + x]
#define ScheduleMondayEvent5(x)					b.eeprom[EEP_SCHEDULE_MONDAY_EVENT5_H + x]
#define ScheduleMondayEvent6(x)					b.eeprom[EEP_SCHEDULE_MONDAY_EVENT6_H + x]

#define ScheduleTuesdayEvent1(x)				b.eeprom[EEP_SCHEDULE_TUESDAY_EVENT1_H + x]
#define ScheduleWensdayEvent1(x)				b.eeprom[EEP_SCHEDULE_WENSDAY_EVENT1_H + x]
#define ScheduleThursdayEvent1(x)				b.eeprom[EEP_SCHEDULE_THURSDAY_EVENT1_H + x]
#define ScheduleFridayEvent1(x)					b.eeprom[EEP_SCHEDULE_FRIDAY_EVENT1_H + x]
#define ScheduleSatdayEvent1(x)					b.eeprom[EEP_SCHEDULE_SATDAY_EVENT1_H + x]
#define ScheduleSundayEvent1(x)					b.eeprom[EEP_SCHEDULE_SUNDAY_EVENT1_H + x]
#define ScheduleHolidayEvent1(x)				b.eeprom[EEP_SCHEDULE_HOLIDAY_EVENT1_H + x]
#define ScheduleHolidayEvent2(x)				b.eeprom[EEP_SCHEDULE_HOLIDAY_EVENT2_H + x]
#define ScheduleHolidayEvent3(x)				b.eeprom[EEP_SCHEDULE_HOLIDAY_EVENT3_H + x]
#define ScheduleHolidayEvent4(x)				b.eeprom[EEP_SCHEDULE_HOLIDAY_EVENT4_H + x]
#define ScheduleHolidayEvent5(x)				b.eeprom[EEP_SCHEDULE_HOLIDAY_EVENT5_H + x]
#define ScheduleHolidayEvent6(x)				b.eeprom[EEP_SCHEDULE_HOLIDAY_EVENT6_H + x]

#define ScheduleDay(x)									b.eeprom[EEP_SCHEDULE_DAY_BEGAIN + x]
#define ScheduleMondayFlag(x)	       		b.eeprom[EEP_SCHEDULE_MONDAY_FLAG + x]
#define ScheduleTuesdayFlag(x)				b.eeprom[EEP_SCHEDULE_TUESDAY_FLAG + x]
#define ScheduleWensdayFlag(x)				b.eeprom[EEP_SCHEDULE_WENSDAY_FLAG + x]
#define ScheduleThursdayFlag(x)				b.eeprom[EEP_SCHEDULE_THURSDAY_FLAG + x]
#define ScheduleFridayFlag(x)					b.eeprom[EEP_SCHEDULE_FRIDAY_FLAG + x]
#define ScheduleSatdayFlag(x)					b.eeprom[EEP_SCHEDULE_SATDAY_FLAG + x]
#define ScheduleSundayFlag(x)					b.eeprom[EEP_SCHEDULE_SUNDAY_FLAG + x]
#define InputVoltageTerm							b.eeprom[EEP_INPUT_VOLTAGE_TERM]

#define EEP_DaySpLo											b.eeprom[EEP_DAY_SETPOINT]
#define EEP_DaySpHi											b.eeprom[EEP_DAY_SETPOINT + 1]

#define SerialNumber(x)  								b.eeprom[EEP_SERIALNUMBER_LOWORD + x]             
//#define Serial_number[1]	 								b.eeprom[EEP_SERIALNUMBER_LOWORD + 1]
//#define Serial_number[2]  								b.eeprom[EEP_SERIALNUMBER_LOWORD + 2]
//#define Serial_number[3]  								b.eeprom[EEP_SERIALNUMBER_LOWORD + 3]
#define FirmwareVersion_LO  						b.eeprom[EEPROM_VERSION_NUMBER_LO]
#define FirmwareVersion_HI  						b.eeprom[EEPROM_VERSION_NUMBER_HI]
#define laddress												b.eeprom[EEP_ADDRESS]
#define ProductModel										b.eeprom[EEP_PRODUCT_MODEL]
#define HardwareVersion									b.eeprom[EEP_HARDWARE_REV]
#define protocol_select									b.eeprom[EEP_PROTOCOL_SEL]			
//#define Dterm														b.eeprom[EEP_DTERM]
#define info_byte												b.eeprom[EEP_INFO_BYTE]
#define EEPTestAddress									b.eeprom[EEPROM_TEST_ADDRESS]
#define UpdateState											b.eeprom[EEP_UPDATE_STATUS]
#define UpdateState_ptr_HI							b.eeprom[EEP_UPDATE_PTR_HI]
#define UpdateState_ptr_LO							b.eeprom[EEP_UPDATE_PTR_LO]
#define UpdateState_ptr_LO2							b.eeprom[EEP_UPDATE_PTR_LO + 1]//do not know what's this
#define UniversalHeatDB_LO							b.eeprom[EEP_UNIVERSAL_HEAT_DB]
#define UniversalHeatDB_HI							b.eeprom[EEP_UNIVERSAL_HEAT_DB + 1]
#define UniversalCoolDB_LO							b.eeprom[EEP_UNIVERSAL_COOL_DB]
#define UniversalCoolDB_HI							b.eeprom[EEP_UNIVERSAL_COOL_DB + 1]
#define UnitS1_HI_LO										b.eeprom[EEP_UNITS1_HIGH]
#define UnitS1_HI_HI										b.eeprom[EEP_UNITS1_HIGH + 1]
#define UnitS1_LO_LO										b.eeprom[EEP_UNITS1_LOW]
#define UnitS1_LO_HI										b.eeprom[EEP_UNITS1_LOW + 1]
#define UnitS2_HI_LO										b.eeprom[EEP_UNITS2_HIGH]
#define UnitS2_HI_HI										b.eeprom[EEP_UNITS2_HIGH + 1]
#define UnitS2_LO_LO										b.eeprom[EEP_UNITS2_LOW]
#define UnitS2_LO_HI										b.eeprom[EEP_UNITS2_LOW + 1]
#define Table1_ZERO_LO									b.eeprom[EEP_TABLE1_ZERO]
#define Table1_ZERO_HI									b.eeprom[EEP_TABLE1_ZERO + 1]
#define Table1_HalfOne_LO								b.eeprom[EEP_TABLE1_HALFONE]
#define Table1_HalfOne_HI								b.eeprom[EEP_TABLE1_HALFONE + 1]
#define Table1_One_LO										b.eeprom[EEP_TABLE1_ONE]
#define Table1_One_HI										b.eeprom[EEP_TABLE1_ONE + 1]
#define Table1_HalfTwo_LO								b.eeprom[EEP_TABLE1_HALFTWO]
#define Table1_HalfTwo_HI								b.eeprom[EEP_TABLE1_HALFTWO + 1]
#define Table1_Two_LO										b.eeprom[EEP_TABLE1_TWO]
#define Table1_Two_HI										b.eeprom[EEP_TABLE1_TWO + 1]
#define Table1_HalfThree_LO							b.eeprom[EEP_TABLE1_HALFTHREE]
#define Table1_HalfThree_HI							b.eeprom[EEP_TABLE1_HALFTHREE + 1]
#define Table1_Three_LO									b.eeprom[EEP_TABLE1_THREE]
#define Table1_Three_HI									b.eeprom[EEP_TABLE1_THREE + 1]
#define Table1_HalfFour_LO							b.eeprom[EEP_TABLE1_HALFFOUR]
#define Table1_HalfFour_HI							b.eeprom[EEP_TABLE1_HALFFOUR + 1]
#define Table1_Four_LO									b.eeprom[EEP_TABLE1_FOUR]
#define Table1_Four_HI									b.eeprom[EEP_TABLE1_FOUR + 1]
#define Table1_HalfFive_LO							b.eeprom[EEP_TABLE1_HALFFIVE]
#define Table1_HalfFive_HI							b.eeprom[EEP_TABLE1_HALFFIVE + 1]
#define Table1_Five_LO									b.eeprom[EEP_TABLE1_FIVE]
#define Table1_Five_HI									b.eeprom[EEP_TABLE1_FIVE + 1]
#define Table2_ZERO_LO									b.eeprom[EEP_TABLE2_ZERO]
#define Table2_ZERO_HI									b.eeprom[EEP_TABLE2_ZERO + 1]
#define Table2_HalfOne_LO								b.eeprom[EEP_TABLE2_HALFONE]
#define Table2_HalfOne_HI								b.eeprom[EEP_TABLE2_HALFONE + 1]
#define Table2_One_LO										b.eeprom[EEP_TABLE2_ONE]
#define Table2_One_HI										b.eeprom[EEP_TABLE2_ONE + 1]
#define Table2_HalfTwo_LO								b.eeprom[EEP_TABLE2_HALFTWO]
#define Table2_HalfTwo_HI								b.eeprom[EEP_TABLE2_HALFTWO + 1]
#define Table2_Two_LO										b.eeprom[EEP_TABLE2_TWO]
#define Table2_Two_HI										b.eeprom[EEP_TABLE2_TWO + 1]
#define Table2_HalfThree_LO							b.eeprom[EEP_TABLE2_HALFTHREE]
#define Table2_HalfThree_HI							b.eeprom[EEP_TABLE2_HALFTHREE + 1]
#define Table2_Three_LO									b.eeprom[EEP_TABLE2_THREE]
#define Table2_Three_HI									b.eeprom[EEP_TABLE2_THREE + 1]
#define Table2_HalfFour_LO							b.eeprom[EEP_TABLE2_HALFFOUR]
#define Table2_HalfFour_HI							b.eeprom[EEP_TABLE2_HALFFOUR + 1]
#define Table2_Four_LO									b.eeprom[EEP_TABLE2_FOUR]
#define Table2_Four_HI									b.eeprom[EEP_TABLE2_FOUR + 1]
#define Table2_HalfFive_LO							b.eeprom[EEP_TABLE2_HALFFIVE]
#define Table2_HalfFive_HI							b.eeprom[EEP_TABLE2_HALFFIVE + 1]
#define Table2_Five_LO									b.eeprom[EEP_TABLE2_FIVE]
#define Table2_Five_HI									b.eeprom[EEP_TABLE2_FIVE + 1]
#define Calibration_AI_LO(x)						b.eeprom[EEP_CALIBRATION_ANALOG1 + x]
#define Calibration_AI_HI(x)						b.eeprom[EEP_CALIBRATION_ANALOG1 + 1 + x]

#define Calibration_AI1_LO							b.eeprom[EEP_CALIBRATION_ANALOG1]
#define Calibration_AI1_HI							b.eeprom[EEP_CALIBRATION_ANALOG1 + 1]
#define Calibration_AI2_LO							b.eeprom[EEP_CALIBRATION_ANALOG2]
#define Calibration_AI2_HI							b.eeprom[EEP_CALIBRATION_ANALOG2 + 1]
#define Calibration_AI3_LO							b.eeprom[EEP_CALIBRATION_ANALOG3]
#define Calibration_AI3_HI							b.eeprom[EEP_CALIBRATION_ANALOG3 + 1]
#define Calibration_AI4_LO							b.eeprom[EEP_CALIBRATION_ANALOG4]
#define Calibration_AI4_HI							b.eeprom[EEP_CALIBRATION_ANALOG4 + 1]
#define Calibration_AI5_LO							b.eeprom[EEP_CALIBRATION_ANALOG5]
#define Calibration_AI5_HI							b.eeprom[EEP_CALIBRATION_ANALOG5 + 1]
#define Calibration_AI6_LO							b.eeprom[EEP_CALIBRATION_ANALOG6]
#define Calibration_AI6_HI							b.eeprom[EEP_CALIBRATION_ANALOG6 + 1]
#define Calibration_AI7_LO							b.eeprom[EEP_CALIBRATION_ANALOG7]
#define Calibration_AI7_HI							b.eeprom[EEP_CALIBRATION_ANALOG7 + 1]
#define Calibration_AI8_LO							b.eeprom[EEP_CALIBRATION_ANALOG8]
#define Calibration_AI8_HI							b.eeprom[EEP_CALIBRATION_ANALOG8 + 1]
#define Calibration_External_LO					b.eeprom[EEP_CALIBRATION_EXTERNAL]
#define Calibration_External_HI					b.eeprom[EEP_CALIBRATION_EXTERNAL + 1]
#define Calibration_Spare_LO					  b.eeprom[EEP_CALIBRATION_ANALOG_IN2]
#define Calibration_Spare_HI						b.eeprom[EEP_CALIBRATION_ANALOG_IN2 + 1]
#define Calibration_Internal_LO					b.eeprom[EEP_CALIBRATION_INTERNAL_THERMISTOR]
#define Calibration_Internal_HI					b.eeprom[EEP_CALIBRATION_INTERNAL_THERMISTOR + 1]
#define Calibration_LO									b.eeprom[EEP_CALIBRATION]
#define Calibration_HI									b.eeprom[EEP_CALIBRATION + 1]
#endif
/*************************************************************************************/
#define EEP_TempSelect			b.eeprom[EEP_TEMP_SELECT]
#define EEP_Filter					b.eeprom[EEP_FILTER]

#define EEP_AnalogIn1				b.eeprom[EEP_ANALOG1_RANGE]
#define EEP_AnalogIn2				b.eeprom[EEP_ANALOG2_RANGE]
#define EEP_AnalogIn3				b.eeprom[EEP_ANALOG3_RANGE]
#define EEP_AnalogIn4				b.eeprom[EEP_ANALOG4_RANGE]
#define EEP_AnalogIn5				b.eeprom[EEP_ANALOG5_RANGE]
#define EEP_AnalogIn6				b.eeprom[EEP_ANALOG6_RANGE]
#define EEP_AnalogIn7				b.eeprom[EEP_ANALOG7_RANGE]
#define EEP_AnalogIn8				b.eeprom[EEP_ANALOG8_RANGE]

#define AI_Range(x)					b.eeprom[EEP_ANALOG1_RANGE + x]

#define EEP_OverrideTimer			b.eeprom[EEP_OVERRIDE_TIMER ]
#define EEP_DacOffset				b.eeprom[EEP_DAC_OFFSET ]
#define EEP_Baudrate				b.eeprom[EEP_BAUDRATE ]
#define EEP_PTERM1					b.eeprom[EEP_COOLING_PTERM ]
#define EEP_ITERM1					b.eeprom[EEP_COOLING_ITERM ]
#define EEP_DTERM1					b.eeprom[EEP_DTERM ]
#define PidSampleTime				b.eeprom[EEP_PID_SAMPLE_TIME] 
#define Sys_Sequence				b.eeprom[EEP_SEQUENCE ]
#define EEP_HeatCoolConfig			b.eeprom[EEP_HEAT_COOL_CONFIG ]
#define EEP_DEGCorF					b.eeprom[EEP_DEGC_OR_F ]
#define EEP_FanMode 				b.eeprom[EEP_FAN_MODE ]
#define EEP_Application				b.eeprom[EEP_APPLICATION ]
#define EEP_PowerupSetpoint			b.eeprom[EEP_POWERUP_SETPOINT ]
#define EEP_PowerupOnOff			b.eeprom[EEP_POWERUP_ON_OFF ]
#define EEP_KeypadSelect			b.eeprom[EEP_KEYPAD_SELECT ]
#define EEP_AutoOnly				b.eeprom[EEP_AUTO_ONLY ]

#define EEP_MaxSetpoint				b.eeprom[EEP_MAX_SETPOINT ]
#define EEP_MinSetpoint				b.eeprom[EEP_MIN_SETPOINT ]
#define EEP_SpecialMenuLock			b.eeprom[EEP_SPECIAL_MENU_LOCK ]
//#define EEP_Display					b.eeprom[EEP_DISPLAY ]
#define EEP_ValveTravelTime			b.eeprom[EEP_VALVE_TRAVEL_TIME ]
#define EEP_DayCoolingDeadband			b.eeprom[EEP_DAY_COOLING_DEADBAND ]
#define EEP_DayHeatingDeadband			b.eeprom[EEP_DAY_HEATING_DEADBAND ]
#define EEP_DayCoolingSpLo				b.eeprom[EEP_DAY_COOLING_SETPOINT ]
#define EEP_DayCoolingSpHi				b.eeprom[EEP_DAY_COOLING_SETPOINT  + 1]
#define EEP_DayHeatingSpLo				b.eeprom[EEP_DAY_HEATING_SETPOINT ]
#define EEP_DayHeatingSpHi				b.eeprom[EEP_DAY_HEATING_SETPOINT  + 1]
#define EEP_NightSpLo                  b.eeprom[EEP_NIGHT_SETPOINT ]
#define EEP_NightSpHi                  b.eeprom[EEP_NIGHT_SETPOINT  + 1]
#define EEP_NightHeatingDeadband		b.eeprom[EEP_NIGHT_HEATING_DEADBAND ]
#define EEP_NightCoolingDeadband		b.eeprom[EEP_NIGHT_COOLING_DEADBAND ]
#define EEP_NightHeatingSpLo			    b.eeprom[EEP_NIGHT_HEATING_SETPOINT ]
#define EEP_NightHeatingSpHi			    b.eeprom[EEP_NIGHT_HEATING_SETPOINT + 1]
#define EEP_NightCoolingSpLo			    b.eeprom[EEP_NIGHT_COOLING_SETPOINT ]
#define EEP_NightCoolingSpHi			    b.eeprom[EEP_NIGHT_COOLING_SETPOINT + 1]
#define EEP_FanSpeed				  b.eeprom[EEP_FAN_SPEED ]
#define EEP_HEAT_TABLE3				b.eeprom[EEP_HEAT_PID3_TABLE ]
#define EEP_COOL_TABLE3				b.eeprom[EEP_COOL_PID3_TABLE ]
#define EEP_HEAT_TABLE2				b.eeprom[EEP_HEAT_UNIVERSAL_TABLE ]
#define EEP_COOL_TABLE2				b.eeprom[EEP_COOL_UNIVERSAL_TABLE ]
#define EEP_HEAT_TABLE1				b.eeprom[EEP_HEAT_ORIGINAL_TABLE ]
#define EEP_COOL_TABLE1				b.eeprom[EEP_COOL_ORIGINAL_TABLE ]
#define EEP_Input1Select			b.eeprom[EEP_INPUT1_SELECT ]
#define EEP_PTERM2_HI				b.eeprom[EEP_UNIVERSAL_PTERM  + 1]
#define EEP_PTERM2_LO				b.eeprom[EEP_UNIVERSAL_PTERM ]
#define EEP_ITERM2_HI				b.eeprom[EEP_UNIVERSAL_ITERM  + 1]
#define EEP_ITERM2_LO				b.eeprom[EEP_UNIVERSAL_ITERM ]
#define EEP_UniversalSetpointHi		b.eeprom[EEP_UNIVERSAL_SET + 1]
#define EEP_UniversalSetpointLo		b.eeprom[EEP_UNIVERSAL_SET]
#define EEP_UniversalNightSetpointHi		b.eeprom[EEP_UNIVERSAL_NIGHTSET  + 1]
#define EEP_UniversalNightSetpointLo		b.eeprom[EEP_UNIVERSAL_NIGHTSET ]


#define EEP_Interlock1				b.eeprom[EEP_INTERLOCK_OUTPUT1 ] 
#define EEP_Interlock2				b.eeprom[EEP_INTERLOCK_OUTPUT2 ]
#define EEP_Interlock3				b.eeprom[EEP_INTERLOCK_OUTPUT3 ] 
#define EEP_Interlock4				b.eeprom[EEP_INTERLOCK_OUTPUT4 ]
#define EEP_Interlock5				b.eeprom[EEP_INTERLOCK_OUTPUT5 ] 
#define EEP_Interlock6				b.eeprom[EEP_INTERLOCK_OUTPUT6 ]
#define EEP_Interlock7				b.eeprom[EEP_INTERLOCK_OUTPUT7 ] 
 

#define EEP_PidOutput1				b.eeprom[EEP_PID_OUTPUT1 ]
#define EEP_PidOutput2				b.eeprom[EEP_PID_OUTPUT2 ]
#define EEP_PidOutput3				b.eeprom[EEP_PID_OUTPUT3 ]
#define EEP_PidOutput4				b.eeprom[EEP_PID_OUTPUT4 ]
#define EEP_PidOutput5				b.eeprom[EEP_PID_OUTPUT5 ]
#define EEP_PidOutput6				b.eeprom[EEP_PID_OUTPUT6 ]
#define EEP_PidOutput7				b.eeprom[EEP_PID_OUTPUT7 ]

#define EEP_OutputRange1		    b.eeprom[EEP_RANGE_OUTPUT1 ]
#define EEP_OutputRange2		    b.eeprom[EEP_RANGE_OUTPUT2 ]
#define EEP_OutputRange3		    b.eeprom[EEP_RANGE_OUTPUT3 ]
#define EEP_OutputRange4		    b.eeprom[EEP_RANGE_OUTPUT4 ]
#define EEP_OutputRange5			b.eeprom[EEP_RANGE_OUTPUT5 ]
#define OutputRange(x)				b.eeprom[EEP_RANGE_OUTPUT1  + x]
#define EEP_Output1Scale			b.eeprom[EEP_OUTPUT1_SCALE ]
#define EEP_Output2Scale			b.eeprom[EEP_OUTPUT2_SCALE ]
#define EEP_SetpointIncrease		b.eeprom[EEP_SETPOINT_INCREASE ]
#define EEP_FreezeSetpoint			b.eeprom[EEP_FREEZE_TEMP_SETPOINT ]
#define EEP_FreezeDelayOn			b.eeprom[EEP_FREEZE_DELAY_ON ]
#define EEP_FreezeDelayOff			b.eeprom[EEP_FREEZE_DELAY_OFF ]
#define EEP_TimerOnHi				b.eeprom[EEP_TIMER_ON  + 1]
#define EEP_TimerOn					b.eeprom[EEP_TIMER_ON ]
#define EEP_TimerOffHi				b.eeprom[EEP_TIMER_OFF  + 1]
#define EEP_TimerOff				b.eeprom[EEP_TIMER_OFF ]
#define EEP_MinAddress				b.eeprom[EEP_MIN_ADDRESS ]
#define EEP_MaxAddress				b.eeprom[EEP_MAX_ADDRESS ]
#define EEP_TimerUnits				b.eeprom[EEP_TIMER_UNITS ]
#define EEP_InputManuEnable			b.eeprom[EEP_INPUT_MANU_ENABLE ]
#define EEP_OutputManuEnable		b.eeprom[EEP_OUTPUT_MANU_ENABLE ]
#define EEP_BaseAddress				b.eeprom[EEP_BASE_ADDRESS ]
#define EEP_DeadMaster				b.eeprom[EEP_DEAD_MASTER ]
#define EEP_RoundDisplay			b.eeprom[EEP_ROUND_DISPLAY ]
#define EEP_Configuration			b.eeprom[EEP_CONFIGURATION ]
#define EEP_TimerSelect				b.eeprom[EEP_TIMER_SELECT ]
#define EEP_Output1Function			b.eeprom[EEP_OUTPUT1_FUNCTION ]
#define EEP_Output2Function			b.eeprom[EEP_OUTPUT2_FUNCTION ]
#define EEP_Output3Function			b.eeprom[EEP_OUTPUT3_FUNCTION ]
#define EEP_Output4Function			b.eeprom[EEP_OUTPUT4_FUNCTION ]
#define EEP_Output5Function			b.eeprom[EEP_OUTPUT5_FUNCTION ]
#define EEP_DefaultSetpoint			b.eeprom[EEP_DEFAULT_SETPOINT ]
#define EEP_SetpointControl			b.eeprom[EEP_SETPOINT_CONTROL ]
#define EEP_PirStage				b.eeprom[EEP_PIR_STAGE ]
#define EEP_FreeCoolHi				b.eeprom[EEP_FREE_COOL_TRIGGER  + 1]
#define EEP_FreeCoolLo				b.eeprom[EEP_FREE_COOL_TRIGGER ]
#define EEP_MinAir					b.eeprom[EEP_MIN_AIR ]

#define EEP_WorkMode			    b.eeprom[EEP_WORK_MODE ]
#define EEP_SunIconControl			b.eeprom[EEP_SUN_ICON_CONTROL ]
#define EEP_MoonIconControl			b.eeprom[EEP_MOON_ICON_CONTROL ]
#define EEP_InterThermistorManual	b.eeprom[EEP_INTERNAL_SENSOR_MANUAL ]

/*************************************************************************************/
#ifdef TSTAT_ARM

#define Calibration_CO2_LO     		b.eeprom[EEP_CO2_CALIBRATION] 
#define Calibration_CO2_HI     		b.eeprom[EEP_CO2_CALIBRATION + 1]

#define FanTable0(x)							b.eeprom[EEP_FAN0_OPER_TABLE_COAST + x]	
//#define FanTable0[0]							b.eeprom[EEP_FAN0_OPER_TABLE_COAST]						
//#define FanTable0[1]							b.eeprom[EEP_FAN0_OPER_TABLE_COOL1]	
//#define FanTable0[2]							b.eeprom[EEP_FAN0_OPER_TABLE_COOL2]	
//#define FanTable0[3]							b.eeprom[EEP_FAN0_OPER_TABLE_COOL3]	
//#define FanTable0[4]							b.eeprom[EEP_FAN0_OPER_TABLE_HEAT1]	
//#define FanTable0[5]							b.eeprom[EEP_FAN0_OPER_TABLE_HEAT2]	
//#define FanTable0[6]							b.eeprom[EEP_FAN0_OPER_TABLE_HEAT3]

#define FanTable1(x)							b.eeprom[EEP_FAN1_OPER_TABLE_COAST + x]	
//#define FanTable1[0]							b.eeprom[EEP_FAN1_OPER_TABLE_COAST]						
//#define FanTable1[1]							b.eeprom[EEP_FAN1_OPER_TABLE_COOL1]	
//#define FanTable1[2]							b.eeprom[EEP_FAN1_OPER_TABLE_COOL2]	
//#define FanTable1[3]							b.eeprom[EEP_FAN1_OPER_TABLE_COOL3]	
//#define FanTable1[4]							b.eeprom[EEP_FAN1_OPER_TABLE_HEAT1]	
//#define FanTable1[5]							b.eeprom[EEP_FAN1_OPER_TABLE_HEAT2]	
//#define FanTable1[6]							b.eeprom[EEP_FAN1_OPER_TABLE_HEAT3]	

#define FanTable2(x)							b.eeprom[EEP_FAN2_OPER_TABLE_COAST + x]	
//#define FanTable2[0]							b.eeprom[EEP_FAN2_OPER_TABLE_COAST]						
//#define FanTable2[1]							b.eeprom[EEP_FAN2_OPER_TABLE_COOL1]	
//#define FanTable2[2]							b.eeprom[EEP_FAN2_OPER_TABLE_COOL2]	
//#define FanTable2[3]							b.eeprom[EEP_FAN2_OPER_TABLE_COOL3]	
//#define FanTable2[4]							b.eeprom[EEP_FAN2_OPER_TABLE_HEAT1]	
//#define FanTable2[5]							b.eeprom[EEP_FAN2_OPER_TABLE_HEAT2]	
//#define FanTable2[6]							b.eeprom[EEP_FAN2_OPER_TABLE_HEAT3]	

#define FanTable3(x)							b.eeprom[EEP_FAN3_OPER_TABLE_COAST + x]	
//#define FanTable3[0]							b.eeprom[EEP_FAN3_OPER_TABLE_COAST]						
//#define FanTable3[1]							b.eeprom[EEP_FAN3_OPER_TABLE_COOL1]	
//#define FanTable3[2]							b.eeprom[EEP_FAN3_OPER_TABLE_COOL2]	
//#define FanTable3[3]							b.eeprom[EEP_FAN3_OPER_TABLE_COOL3]	
//#define FanTable3[4]							b.eeprom[EEP_FAN3_OPER_TABLE_HEAT1]	
//#define FanTable3[5]							b.eeprom[EEP_FAN3_OPER_TABLE_HEAT2]	
//#define FanTable3[6]							b.eeprom[EEP_FAN3_OPER_TABLE_HEAT3]	

#define FanTableAuto(x)							b.eeprom[EEP_FANAUT_OPER_TABLE_COAST + x]	
//#define FanTableAuto[0]							b.eeprom[EEP_FANAUT_OPER_TABLE_COAST]						
//#define FanTableAuto[1]							b.eeprom[EEP_FANAUT_OPER_TABLE_COOL1]	
//#define FanTableAuto[2]							b.eeprom[EEP_FANAUT_OPER_TABLE_COOL2]	
//#define FanTableAuto[3]							b.eeprom[EEP_FANAUT_OPER_TABLE_COOL3]	
//#define FanTableAuto[4]							b.eeprom[EEP_FANAUT_OPER_TABLE_HEAT1]	
//#define FanTableAuto[5]							b.eeprom[EEP_FANAUT_OPER_TABLE_HEAT2]	
//#define FanTableAuto[6]							b.eeprom[EEP_FANAUT_OPER_TABLE_HEAT3]	

#define PID2_Output_Table(x)							b.eeprom[EEP_UNIVERSAL_OUTPUT_BEGIN + x]
//#define PID2_Output_Table[0]							b.eeprom[EEP_UNIVERSAL_OUTPUT_BEGIN]	
//#define PID2_Output_Table[1]							b.eeprom[EEP_UNIVERSAL_OUTPUT_COOL1]	
//#define PID2_Output_Table[2]							b.eeprom[EEP_UNIVERSAL_OUTPUT_COOL2]	
//#define PID2_Output_Table[3]							b.eeprom[EEP_UNIVERSAL_OUTPUT_COOL3]	
//#define PID2_Output_Table[4]							b.eeprom[EEP_UNIVERSAL_OUTPUT_HEAT1]	
//#define PID2_Output_Table[5]							b.eeprom[EEP_UNIVERSAL_OUTPUT_HEAT2]	
//#define PID2_Output_Table[6]							b.eeprom[EEP_UNIVERSAL_OUTPUT_HEAT3]	

#define PID1_Valve_Table(x)							b.eeprom[EEP_VALVE_OPER_TABLE_BEGIN + x]
//#define PID1_Valve_Table[0]							b.eeprom[EEP_VALVE_OPER_TABLE_BEGIN]
//#define PID1_Valve_Table[1]							b.eeprom[EEP_VALVE_OPER_TABLE_COOL1]
//#define PID1_Valve_Table[2]							b.eeprom[EEP_VALVE_OPER_TABLE_COOL2]
//#define PID1_Valve_Table[3]							b.eeprom[EEP_VALVE_OPER_TABLE_COOL3]
//#define PID1_Valve_Table[4]							b.eeprom[EEP_VALVE_OPER_TABLE_HEAT1]
//#define PID1_Valve_Table[5]							b.eeprom[EEP_VALVE_OPER_TABLE_HEAT2]
//#define PID1_Valve_Table[6]							b.eeprom[EEP_VALVE_OPER_TABLE_HEAT3]

#define PID2_Valve_Table(x)							b.eeprom[EEP_UNIVERSAL_VALVE_BEGIN + x]
//#define PID2_Valve_Table[0]							b.eeprom[EEP_UNIVERSAL_VALVE_BEGIN]
//#define PID2_Valve_Table[1]							b.eeprom[EEP_UNIVERSAL_VALVE_COOL1]
//#define PID2_Valve_Table[2]							b.eeprom[EEP_UNIVERSAL_VALVE_COOL2]
//#define PID2_Valve_Table[3]							b.eeprom[EEP_UNIVERSAL_VALVE_COOL3]
//#define PID2_Valve_Table[4]							b.eeprom[EEP_UNIVERSAL_VALVE_HEAT1]
//#define PID2_Valve_Table[5]							b.eeprom[EEP_UNIVERSAL_VALVE_HEAT2]
//#define PID2_Valve_Table[6]							b.eeprom[EEP_UNIVERSAL_VALVE_HEAT3]

#define PID1_Valve_Off_Table(x)							b.eeprom[EEP_VALVE_OFF_TABLE_BEGIN + x]
//#define PID1_Valve_Off_Table[0]							b.eeprom[EEP_VALVE_OFF_TABLE_BEGIN]
//#define PID1_Valve_Off_Table[1]							b.eeprom[EEP_VALVE_OFF_TABLE_COOL1]
//#define PID1_Valve_Off_Table[2]							b.eeprom[EEP_VALVE_OFF_TABLE_COOL2]
//#define PID1_Valve_Off_Table[3]							b.eeprom[EEP_VALVE_OFF_TABLE_COOL3]
//#define PID1_Valve_Off_Table[4]							b.eeprom[EEP_VALVE_OFF_TABLE_HEAT1]
//#define PID1_Valve_Off_Table[5]							b.eeprom[EEP_VALVE_OFF_TABLE_HEAT2]
//#define PID1_Valve_Off_Table[6]							b.eeprom[EEP_VALVE_OFF_TABLE_HEAT3]

#define Output_Delay_OfftoOn(x)							b.eeprom[EEP_OUTPUT1_DELAY_OFF_TO_ON + x]
//#define Output_Delay_OfftoOn[0]							b.eeprom[EEP_OUTPUT_DELAY_OFF_TO_ON]
//#define Output_Delay_OfftoOn[1]							b.eeprom[EEP_OUTPUT2_DELAY_OFF_TO_ON]
//#define Output_Delay_OfftoOn[2]							b.eeprom[EEP_OUTPUT3_DELAY_OFF_TO_ON]
//#define Output_Delay_OfftoOn[3]							b.eeprom[EEP_OUTPUT4_DELAY_OFF_TO_ON]
//#define Output_Delay_OfftoOn[4]							b.eeprom[EEP_OUTPUT5_DELAY_OFF_TO_ON]
//#define Output_Delay_OfftoOn[5]							b.eeprom[EEP_OUTPUT6_DELAY_OFF_TO_ON]
//#define Output_Delay_OfftoOn[6]							b.eeprom[EEP_OUTPUT7_DELAY_OFF_TO_ON]

#define Output_Delay_OntoOff(x)							b.eeprom[EEP_OUTPUT_DELAY_ON_TO_OFF + x]
//#define Output_Delay_OntoOff[1]							b.eeprom[EEP_OUTPUT2_DELAY_ON_TO_OFF]
//#define Output_Delay_OntoOff[2]							b.eeprom[EEP_OUTPUT3_DELAY_ON_TO_OFF]
//#define Output_Delay_OntoOff[3]							b.eeprom[EEP_OUTPUT4_DELAY_ON_TO_OFF]
//#define Output_Delay_OntoOff[4]							b.eeprom[EEP_OUTPUT5_DELAY_ON_TO_OFF]
//#define Output_Delay_OntoOff[5]							b.eeprom[EEP_OUTPUT6_DELAY_ON_TO_OFF]
//#define Output_Delay_OntoOff[6]							b.eeprom[EEP_OUTPUT7_DELAY_ON_TO_OFF]

#define ScheduleDHomeHour							b.eeprom[EEP_SCHEDULE_DHOME_HOUR]
#define ScheduleDHomeMin							b.eeprom[EEP_SCHEDULE_DHOME_MIN]
#define ScheduleWorkHour							b.eeprom[EEP_SCHEDULE_WORK_HOUR]
#define ScheduleWorkMin								b.eeprom[EEP_SCHEDULE_WORK_MIN]
#define ScheduleNHomeHour							b.eeprom[EEP_SCHEDULE_NHOME_HOUR]
#define ScheduleNHomeMin							b.eeprom[EEP_SCHEDULE_NHOME_MIN]
#define ScheduleSleepHour							b.eeprom[EEP_SCHEDULE_SLEEP_HOUR]
#define ScheduleSleepMin							b.eeprom[EEP_SCHEDULE_SLEEP_MIN]
#define ScheduleHolidayByear						b.eeprom[EEP_SCHEDULE_HOLIDAY_BYEAR]
#define ScheduleHolidayBmon							b.eeprom[EEP_SCHEDULE_HOLIDAY_BMONTH]
#define ScheduleHolidayBday							b.eeprom[EEP_SCHEDULE_HOLIDAY_BDAY]
#define ScheduleHolidayEyear							b.eeprom[EEP_SCHEDULE_HOLIDAY_EYEAR]
#define ScheduleHolidayEmon							b.eeprom[EEP_SCHEDULE_HOLIDAY_EMONTH]
#define ScheduleHolidayEday							b.eeprom[EEP_SCHEDULE_HOLIDAY_EDAY]
#define Day2_Event4_HI							b.eeprom[EEP_DAY2_EVENT4_HI]
#define Day2_Event4_LO							b.eeprom[EEP_DAY2_EVENT4_LO]


#define Output_PWM_Table(x)						b.eeprom[EEP_OUTPUT_PWM_AUTO_BEGIN + x]
#define Output_PWM_Off_Table(x)						b.eeprom[EEP_OUTPUT_PWM_OFF_BEGIN + x]
//#define Output_PWM_Table[0]							b.eeprom[EEP_OUTPUT_PWM_AUTO_BEGIN]
//#define Output_PWM_Table[1]							b.eeprom[EEP_OUTPUT_PWM_AUTO_COOL1]
//#define Output_PWM_Table[2]							b.eeprom[EEP_OUTPUT_PWM_AUTO_COOL2]
//#define Output_PWM_Table[3]							b.eeprom[EEP_OUTPUT_PWM_AUTO_COOL3]
//#define Output_PWM_Table[4]							b.eeprom[EEP_OUTPUT_PWM_AUTO_HEAT1]
//#define Output_PWM_Table[5]							b.eeprom[EEP_OUTPUT_PWM_AUTO_HEAT2]
//#define Output_PWM_Table[6]							b.eeprom[EEP_OUTPUT_PWM_AUTO_HEAT3]

								
//	EEP_ENDS,				   //
//  EXTERNAL_SENSOR_1,		                // MDF 6/7/04 Added EXTERNAL_SENSOR_1
//	ANALOG_INPUT1,
//	ANALOG_INPUT2,
//	ANALOG_INPUT3,	//310
//	ANALOG_INPUT4,
//	ANALOG_INPUT5,
//	ANALOG_INPUT6,
//	ANALOG_INPUT7,
//	ANALOG_INPUT8,
	
//	ADJUSTING_COOLING,		        		//110 not stored in b.eeprom   b is strcut link.
//	ADJUSTING_HEATING,
//	ADJUSTING_PIR,
//	ADJUSTING_FAN_SPEED ,
//	ADJUSTING_GRID_MODE, 
//	ADJUSTING_SYSTEM_MODE, //system mode: heating/cooling/auto
//	ADDRESS_PLUG_N_PLAY ,                   // 0
//	TEMPRATURE_SENSOR ,                       // 1
//	COOLING_VALVE ,                         // 2
//	HEATING_VALVE ,                         // 3
//	PID,									// 4	 120
//	PID_UNIVERSAL ,
//	COOL_HEAT_MODE ,						// 6
//	MODE_OPERATION ,						// 7
//	PID2_MODE_OPERATION,
//	DIGITAL_OUTPUT_STATUS ,		//330			// 8 //out1,2,3 and if on-off type, 4 and 5
//  PIC_VERSION,                                      //EXPLOREADDRESS,	
//	EEP_OVERRIDE_TIMER_LEFT	,
//	EEP_LAST_KEY_TIMER,//130 whenever press a key,the timer start to count,from 0 to 65535
//	EEP_KEYPAD_VALUE,
//	EEP_DISPLAY_HUNDRED,
//	EEP_DISPLAY_TEN,
//	EEP_DISPLAY_DIGITAL,
//	EEP_DISPLAY_STATUS,		  //340
//	EEP_INTERNAL_THERMISTOR,

#define ValvePercent			b.eeprom[EEP_VALVE_PERCENT]

#define ManualRelay(x)			b.eeprom[EEP_MANU_RELAY1 + x]
//#define ManualRelay[0]			b.eeprom[EEP_MANU_RELAY1]
//#define ManualRelay[1]			b.eeprom[EEP_MANU_RELAY2]
//#define ManualRelay[2]			b.eeprom[EEP_MANU_RELAY3]
//#define ManualRelay[3]			b.eeprom[EEP_MANU_RELAY4]
//#define ManualRelay[4]			b.eeprom[EEP_MANU_RELAY5]

#define User_Info(x)			b.eeprom[EEP_LINE1_CHAR1 + x]
//#define User_Info[0]			b.eeprom[EEP_LINE1_CHAR1]
//#define User_Info[1]			b.eeprom[EEP_LINE1_CHAR1 + 1]
//#define User_Info[2]			b.eeprom[EEP_LINE1_CHAR2]
//#define User_Info[3]			b.eeprom[EEP_LINE1_CHAR2 + 1]
//#define User_Info[4]			b.eeprom[EEP_LINE1_CHAR3]
//#define User_Info[5]			b.eeprom[EEP_LINE1_CHAR3 + 1]
//#define User_Info[6]			b.eeprom[EEP_LINE1_CHAR4]
//#define User_Info[7]			b.eeprom[EEP_LINE1_CHAR4 + 1]
//#define User_Info[8]			b.eeprom[EEP_LINE2_CHAR1]
//#define User_Info[9]			b.eeprom[EEP_LINE2_CHAR1 + 1]
//#define User_Info[10]			b.eeprom[EEP_LINE2_CHAR2]
//#define User_Info[11]			b.eeprom[EEP_LINE2_CHAR2 + 1]
//#define User_Info[12]			b.eeprom[EEP_LINE2_CHAR3]
//#define User_Info[13]			b.eeprom[EEP_LINE2_CHAR3 + 1]
//#define User_Info[14]			b.eeprom[EEP_LINE2_CHAR4]
//#define User_Info[15]			b.eeprom[EEP_LINE2_CHAR4 + 1]

#define Disp_internal_sensor(x)			b.eeprom[EEP_INTERNAL_CHAR1 + x]
//#define Disp_internal_sensor[0]			b.eeprom[EEP_INTERNAL_CHAR1]
//#define Disp_internal_sensor[1]			b.eeprom[EEP_INTERNAL_CHAR1 + 1]
//#define Disp_internal_sensor[2]			b.eeprom[EEP_INTERNAL_CHAR2]
//#define Disp_internal_sensor[3]			b.eeprom[EEP_INTERNAL_CHAR2 + 1]
//#define Disp_internal_sensor[4]			b.eeprom[EEP_INTERNAL_CHAR3]
//#define Disp_internal_sensor[5]			b.eeprom[EEP_INTERNAL_CHAR3 + 1]
//#define Disp_internal_sensor[6]			b.eeprom[EEP_INTERNAL_CHAR4]
//#define Disp_internal_sensor[7]			b.eeprom[EEP_INTERNAL_CHAR4 + 1]

#define Disp_AI1(x)			b.eeprom[EEP_AI1_CHAR1 + x]
//#define Disp_AI1[0]			b.eeprom[EEP_AI1_CHAR1]
//#define Disp_AI1[1]			b.eeprom[EEP_AI1_CHAR1 + 1]
//#define Disp_AI1[2]			b.eeprom[EEP_AI1_CHAR2]
//#define Disp_AI1[3]			b.eeprom[EEP_AI1_CHAR2 + 1]
//#define Disp_AI1[4]			b.eeprom[EEP_AI1_CHAR3]
//#define Disp_AI1[5]			b.eeprom[EEP_AI1_CHAR3 + 1]
//#define Disp_AI1[6]			b.eeprom[EEP_AI1_CHAR4]
//#define Disp_AI1[7]			b.eeprom[EEP_AI1_CHAR4 + 1]

#define Disp_AI2(x)			b.eeprom[EEP_AI2_CHAR1 + x]
//#define Disp_AI2[0]			b.eeprom[EEP_AI2_CHAR1]
//#define Disp_AI2[1]			b.eeprom[EEP_AI2_CHAR1 + 1]
//#define Disp_AI2[2]			b.eeprom[EEP_AI2_CHAR2]
//#define Disp_AI2[3]			b.eeprom[EEP_AI2_CHAR2 + 1]
//#define Disp_AI2[4]			b.eeprom[EEP_AI2_CHAR3]
//#define Disp_AI2[5]			b.eeprom[EEP_AI2_CHAR3 + 1]
//#define Disp_AI2[6]			b.eeprom[EEP_AI2_CHAR4]
//#define Disp_AI2[7]			b.eeprom[EEP_AI2_CHAR4 + 1]

#define Disp_AI3(x)			b.eeprom[EEP_AI3_CHAR1 + x]
//#define Disp_AI3[0]			b.eeprom[EEP_AI3_CHAR1]
//#define Disp_AI3[1]			b.eeprom[EEP_AI3_CHAR1 + 1]
//#define Disp_AI3[2]			b.eeprom[EEP_AI3_CHAR2]
//#define Disp_AI3[3]			b.eeprom[EEP_AI3_CHAR2 + 1]
//#define Disp_AI3[4]			b.eeprom[EEP_AI3_CHAR3]
//#define Disp_AI3[5]			b.eeprom[EEP_AI3_CHAR3 + 1]
//#define Disp_AI3[6]			b.eeprom[EEP_AI3_CHAR4]
//#define Disp_AI3[7]			b.eeprom[EEP_AI3_CHAR4 + 1]

#define Disp_AI4(x)			b.eeprom[EEP_AI4_CHAR1 + x]
//#define Disp_AI4[0]			b.eeprom[EEP_AI4_CHAR1]
//#define Disp_AI4[1]			b.eeprom[EEP_AI4_CHAR1 + 1]
//#define Disp_AI4[2]			b.eeprom[EEP_AI4_CHAR2]
//#define Disp_AI4[3]			b.eeprom[EEP_AI4_CHAR2 + 1]
//#define Disp_AI4[4]			b.eeprom[EEP_AI4_CHAR3]
//#define Disp_AI4[5]			b.eeprom[EEP_AI4_CHAR3 + 1]
//#define Disp_AI4[6]			b.eeprom[EEP_AI4_CHAR4]
//#define Disp_AI4[7]			b.eeprom[EEP_AI4_CHAR4 + 1]

#define Disp_AI5(x)			b.eeprom[EEP_AI5_CHAR1 + x]
//#define Disp_AI5[0]			b.eeprom[EEP_AI5_CHAR1]
//#define Disp_AI5[1]			b.eeprom[EEP_AI5_CHAR1 + 1]
//#define Disp_AI5[2]			b.eeprom[EEP_AI5_CHAR2]
//#define Disp_AI5[3]			b.eeprom[EEP_AI5_CHAR2 + 1]
//#define Disp_AI5[4]			b.eeprom[EEP_AI5_CHAR3]
//#define Disp_AI5[5]			b.eeprom[EEP_AI5_CHAR3 + 1]
//#define Disp_AI5[6]			b.eeprom[EEP_AI5_CHAR4]
//#define Disp_AI5[7]			b.eeprom[EEP_AI5_CHAR4 + 1]

#define Disp_AI6(x)			b.eeprom[EEP_AI6_CHAR1 + x]
//#define Disp_AI6[0]			b.eeprom[EEP_AI6_CHAR1]
//#define Disp_AI6[1]			b.eeprom[EEP_AI6_CHAR1 + 1]
//#define Disp_AI6[2]			b.eeprom[EEP_AI6_CHAR2]
//#define Disp_AI6[3]			b.eeprom[EEP_AI6_CHAR2 + 1]
//#define Disp_AI6[4]			b.eeprom[EEP_AI6_CHAR3]
//#define Disp_AI6[5]			b.eeprom[EEP_AI6_CHAR3 + 1]
//#define Disp_AI6[6]			b.eeprom[EEP_AI6_CHAR4]
//#define Disp_AI6[7]			b.eeprom[EEP_AI6_CHAR4 + 1]

#define Disp_AI7(x)			b.eeprom[EEP_AI7_CHAR1 + x]
//#define Disp_AI7[0]			b.eeprom[EEP_AI7_CHAR1]
//#define Disp_AI7[1]			b.eeprom[EEP_AI7_CHAR1 + 1]
//#define Disp_AI7[2]			b.eeprom[EEP_AI7_CHAR2]
//#define Disp_AI7[3]			b.eeprom[EEP_AI7_CHAR2 + 1]
//#define Disp_AI7[4]			b.eeprom[EEP_AI7_CHAR3]
//#define Disp_AI7[5]			b.eeprom[EEP_AI7_CHAR3 + 1]
//#define Disp_AI7[6]			b.eeprom[EEP_AI7_CHAR4]
//#define Disp_AI7[7]			b.eeprom[EEP_AI7_CHAR4 + 1]

#define Disp_AI8(x)			b.eeprom[EEP_AI8_CHAR1 + x]
//#define Disp_AI8[0]			b.eeprom[EEP_AI8_CHAR1]
//#define Disp_AI8[1]			b.eeprom[EEP_AI8_CHAR1 + 1]
//#define Disp_AI8[2]			b.eeprom[EEP_AI8_CHAR2]
//#define Disp_AI8[3]			b.eeprom[EEP_AI8_CHAR2 + 1]
//#define Disp_AI8[4]			b.eeprom[EEP_AI8_CHAR3]
//#define Disp_AI8[5]			b.eeprom[EEP_AI8_CHAR3 + 1]
//#define Disp_AI8[6]			b.eeprom[EEP_AI8_CHAR4]
//#define Disp_AI8[7]			b.eeprom[EEP_AI8_CHAR4 + 1]

#define Disp_OUT1(x)			b.eeprom[EEP_OUT1_CHAR1 + x]
//#define Disp_OUT1[0]			b.eeprom[EEP_OUT1_CHAR1]
//#define Disp_OUT1[1]			b.eeprom[EEP_OUT1_CHAR1 + 1]
//#define Disp_OUT1[2]			b.eeprom[EEP_OUT1_CHAR2]
//#define Disp_OUT1[3]			b.eeprom[EEP_OUT1_CHAR2 + 1]
//#define Disp_OUT1[4]			b.eeprom[EEP_OUT1_CHAR3]
//#define Disp_OUT1[5]			b.eeprom[EEP_OUT1_CHAR3 + 1]
//#define Disp_OUT1[6]			b.eeprom[EEP_OUT1_CHAR4]
//#define Disp_OUT1[7]			b.eeprom[EEP_OUT1_CHAR4 + 1]

#define Disp_OUT2(x)			b.eeprom[EEP_OUT2_CHAR1 + x]
//#define Disp_OUT2[0]			b.eeprom[EEP_OUT2_CHAR1]
//#define Disp_OUT2[1]			b.eeprom[EEP_OUT2_CHAR1 + 1]
//#define Disp_OUT2[2]			b.eeprom[EEP_OUT2_CHAR2]
//#define Disp_OUT2[3]			b.eeprom[EEP_OUT2_CHAR2 + 1]
//#define Disp_OUT2[4]			b.eeprom[EEP_OUT2_CHAR3]
//#define Disp_OUT2[5]			b.eeprom[EEP_OUT2_CHAR3 + 1]
//#define Disp_OUT2[6]			b.eeprom[EEP_OUT2_CHAR4]
//#define Disp_OUT2[7]			b.eeprom[EEP_OUT2_CHAR4 + 1]

#define Disp_OUT3(x)			b.eeprom[EEP_OUT3_CHAR1 + x]
//#define Disp_OUT3[0]			b.eeprom[EEP_OUT3_CHAR1]
//#define Disp_OUT3[1]			b.eeprom[EEP_OUT3_CHAR1 + 1]
//#define Disp_OUT3[2]			b.eeprom[EEP_OUT3_CHAR2]
//#define Disp_OUT3[3]			b.eeprom[EEP_OUT3_CHAR2 + 1]
//#define Disp_OUT3[4]			b.eeprom[EEP_OUT3_CHAR3]
//#define Disp_OUT3[5]			b.eeprom[EEP_OUT3_CHAR3 + 1]
//#define Disp_OUT3[6]			b.eeprom[EEP_OUT3_CHAR4]
//#define Disp_OUT3[7]			b.eeprom[EEP_OUT3_CHAR4 + 1]

#define Disp_OUT4(x)			b.eeprom[EEP_OUT4_CHAR1 + x]
//#define Disp_OUT4[0]			b.eeprom[EEP_OUT4_CHAR1]
//#define Disp_OUT4[1]			b.eeprom[EEP_OUT4_CHAR1 + 1]
//#define Disp_OUT4[2]			b.eeprom[EEP_OUT4_CHAR2]
//#define Disp_OUT4[3]			b.eeprom[EEP_OUT4_CHAR2 + 1]
//#define Disp_OUT4[4]			b.eeprom[EEP_OUT4_CHAR3]
//#define Disp_OUT4[5]			b.eeprom[EEP_OUT4_CHAR3 + 1]
//#define Disp_OUT4[6]			b.eeprom[EEP_OUT4_CHAR4]
//#define Disp_OUT4[7]			b.eeprom[EEP_OUT4_CHAR4 + 1]

#define Disp_OUT5(x)			b.eeprom[EEP_OUT5_CHAR1 + x]
//#define Disp_OUT5[0]			b.eeprom[EEP_OUT5_CHAR1]
//#define Disp_OUT5[1]			b.eeprom[EEP_OUT5_CHAR1 + 1]
//#define Disp_OUT5[2]			b.eeprom[EEP_OUT5_CHAR2]
//#define Disp_OUT5[3]			b.eeprom[EEP_OUT5_CHAR2 + 1]
//#define Disp_OUT5[4]			b.eeprom[EEP_OUT5_CHAR3]
//#define Disp_OUT5[5]			b.eeprom[EEP_OUT5_CHAR3 + 1]
//#define Disp_OUT5[6]			b.eeprom[EEP_OUT5_CHAR4]
//#define Disp_OUT5[7]			b.eeprom[EEP_OUT5_CHAR4 + 1]

#define Disp_OUT6(x)			b.eeprom[EEP_OUT6_CHAR1 + x]
//#define Disp_OUT6[0]			b.eeprom[EEP_OUT6_CHAR1]
//#define Disp_OUT6[1]			b.eeprom[EEP_OUT6_CHAR1 + 1]
//#define Disp_OUT6[2]			b.eeprom[EEP_OUT6_CHAR2]
//#define Disp_OUT6[3]			b.eeprom[EEP_OUT6_CHAR2 + 1]
//#define Disp_OUT6[4]			b.eeprom[EEP_OUT6_CHAR3]
//#define Disp_OUT6[5]			b.eeprom[EEP_OUT6_CHAR3 + 1]
//#define Disp_OUT6[6]			b.eeprom[EEP_OUT6_CHAR4]
//#define Disp_OUT6[7]			b.eeprom[EEP_OUT6_CHAR4 + 1]

#define Disp_OUT7(x)			b.eeprom[EEP_OUT7_CHAR1 + x]
//#define Disp_OUT7[0]			b.eeprom[EEP_OUT7_CHAR1]
//#define Disp_OUT7[1]			b.eeprom[EEP_OUT7_CHAR1 + 1]
//#define Disp_OUT7[2]			b.eeprom[EEP_OUT7_CHAR2]
//#define Disp_OUT7[3]			b.eeprom[EEP_OUT7_CHAR2 + 1]
//#define Disp_OUT7[4]			b.eeprom[EEP_OUT7_CHAR3]
//#define Disp_OUT7[5]			b.eeprom[EEP_OUT7_CHAR3 + 1]
//#define Disp_OUT7[6]			b.eeprom[EEP_OUT7_CHAR4]
//#define Disp_OUT7[7]			b.eeprom[EEP_OUT7_CHAR4 + 1]

#define ScheduleA_Name(x)			b.eeprom[EEP_SCHEDULEA_CHAR1 + x]
//#define ScheduleA_Name[0]			b.eeprom[EEP_SCHEDULEA_CHAR1]
//#define ScheduleA_Name[1]			b.eeprom[EEP_SCHEDULEA_CHAR2]
//#define ScheduleA_Name[2]			b.eeprom[EEP_SCHEDULEA_CHAR3]
//#define ScheduleA_Name[3]			b.eeprom[EEP_SCHEDULEA_CHAR4]
//#define ScheduleB_Name[0]			b.eeprom[EEP_SCHEDULEB_CHAR1]
//#define ScheduleB_Name[1]			b.eeprom[EEP_SCHEDULEB_CHAR2]
//#define ScheduleB_Name[2]			b.eeprom[EEP_SCHEDULEB_CHAR3]
//#define ScheduleB_Name[3]			b.eeprom[EEP_SCHEDULEB_CHAR4]
//#define ScheduleC_Name[0]			b.eeprom[EEP_SCHEDULEC_CHAR1]
//#define ScheduleC_Name[1]			b.eeprom[EEP_SCHEDULEC_CHAR2]
//#define ScheduleC_Name[2]			b.eeprom[EEP_SCHEDULEC_CHAR3]
//#define ScheduleC_Name[3]			b.eeprom[EEP_SCHEDULEC_CHAR4]
//#define ScheduleD_Name[0]			b.eeprom[EEP_SCHEDULED_CHAR1]
//#define ScheduleD_Name[1]			b.eeprom[EEP_SCHEDULED_CHAR2]
//#define ScheduleD_Name[2]			b.eeprom[EEP_SCHEDULED_CHAR3]
//#define ScheduleD_Name[3]			b.eeprom[EEP_SCHEDULED_CHAR4]


//	EEP_WALL_CHAR1,							
//	EEP_WALL_CHAR2,
//	EEP_WALL_CHAR3,
//	EEP_WALL_CHAR4,
//	EEP_CEILING_CHAR1,						
//	EEP_CEILING_CHAR2,
//	EEP_CEILING_CHAR3,
//	EEP_CEILING_CHAR4,
//	EEP_OUTDOOR_CHAR1,					
//	EEP_OUTDOOR_CHAR2,
//	EEP_OUTDOOR_CHAR3,
//	EEP_OUTDOOR_CHAR4,
//	EEP_AVERAGE_CHAR1,						
//	EEP_AVERAGE_CHAR2,
//	EEP_AVERAGE_CHAR3,
//	EEP_AVERAGE_CHAR4,
//	EEP_HEAT_COOL,
//	EEP_PWM_OUT4,
//	EEP_PWM_OUT5,
//  EEP_DELAY1_ON_TO_OFF_TIME_CURRENT,      
//	EEP_DELAY2_ON_TO_OFF_TIME_CURRENT,
//	EEP_DELAY3_ON_TO_OFF_TIME_CURRENT,
//	EEP_DELAY4_ON_TO_OFF_TIME_CURRENT,
//	EEP_DELAY5_ON_TO_OFF_TIME_CURRENT,
//	EEP_DELAY6_ON_TO_OFF_TIME_CURRENT,
//	EEP_DELAY7_ON_TO_OFF_TIME_CURRENT,
//	EEP_DELAY1_OFF_TO_ON_TIME_CURRENT,
//	EEP_DELAY2_OFF_TO_ON_TIME_CURRENT,
//	EEP_DELAY3_OFF_TO_ON_TIME_CURRENT,
//	EEP_DELAY4_OFF_TO_ON_TIME_CURRENT, //380
//	EEP_DELAY5_OFF_TO_ON_TIME_CURRENT,
//	EEP_DELAY6_OFF_TO_ON_TIME_CURRENT,
//	EEP_DELAY7_OFF_TO_ON_TIME_CURRENT,
//	MAXEXTERNADDRESS,	

#define DeadMsater_AutoManual						b.eeprom[EEP_DEADMASTER_AUTO_MANUAL]
#define DeadMsater_OutputState					b.eeprom[EEP_DEADMASTER_OUTPUT_STATE]

#define DeadMsater_CoolOutput_LO				b.eeprom[EEP_DEADMASTER_COOL_OUTPUT]
#define DeadMsater_CoolOutput_HI				b.eeprom[EEP_DEADMASTER_COOL_OUTPUT + 1]
#define DeadMsater_HeatOutput_LO				b.eeprom[EEP_DEADMASTER_HEAT_OUTPUT]
#define DeadMsater_HeatOutput_HI				b.eeprom[EEP_DEADMASTER_HEAT_OUTPUT + 1]

//#define Day_Setpoint_LO									b.eeprom[EEP_DAY_SETPOINT]
//#define Day_Setpoint_HI									b.eeprom[EEP_DAY_SETPOINT + 1]
	
#define Day_WindowInterlock_CSP_LO			b.eeprom[EEP_WINDOW_INTERLOCK_COOLING_SETPOINT]
#define Day_WindowInterlock_CSP_HI			b.eeprom[EEP_WINDOW_INTERLOCK_COOLING_SETPOINT + 1]
#define Day_WindowInterlock_HSP_LO			b.eeprom[EEP_WINDOW_INTERLOCK_HEATING_SETPOINT]
#define Day_WindowInterlock_HSP_HI			b.eeprom[EEP_WINDOW_INTERLOCK_HEATING_SETPOINT + 1]

#define Current_SP_Offset								b.eeprom[CURRENT_SETPOINT_OFFSET] 
#define Hum_PIC_Version									b.eeprom[HUM_PIC_VERSION] 


//    HUMIDITY_RH,	
//	HUMIDITY_FREQUENCY,	 //390
//	HUM_PIC_UPDATE ,
//	HUM_CAL_NUM,
//	HUM_CURRENT_DEFAULT, //decide where the  calibration data will be stored, current table or default	table 
//	HUM_FIRST_CAL_FLAG,
//	HUM_CAL_EREASE,
//  	HUMCOUNT1_H,	//401

//	HUMRH1_H	= HUMCOUNT1_H + 2, //

//	HUMCOUNT2_H = HUMRH1_H + 2,		

//	HUMRH2_H 	= HUMCOUNT2_H + 2,	

//	HUMCOUNT3_H = HUMRH2_H+ 2,		

//	HUMRH3_H	= HUMCOUNT3_H + 2, //411

//	HUMCOUNT4_H = HUMRH3_H + 2,	//

//	HUMRH4_H	= HUMCOUNT4_H + 2,

//	HUMCOUNT5_H = HUMRH4_H + 2,	

//	HUMRH5_H	= HUMCOUNT5_H + 2,

//	HUMCOUNT6_H = HUMRH5_H + 2 , //421				

//	HUMRH6_H	= HUMCOUNT6_H + 2, //

//	HUMCOUNT7_H = HUMRH6_H + 2 ,

//	HUMRH7_H	= HUMCOUNT7_H + 2,

//	HUMCOUNT8_H = HUMRH7_H + 2 ,

//	HUMRH8_H	= HUMCOUNT8_H + 2, //431

//	HUMCOUNT9_H = HUMRH8_H + 2 ,   //

//	HUMRH9_H	= HUMCOUNT9_H + 2,

//	HUMCOUNT10_H = HUMRH9_H + 2 ,			

//	HUMRH10_H	= HUMCOUNT10_H +2 , //439

//    EEP_HARDWARE_INFORMATION = HUMRH10_H + 2, //L TO H

#define Hardware_Info_LO			b.eeprom[EEP_HARDWARE_INFORMATION]
#define Hardware_Info_HI			b.eeprom[EEP_HARDWARE_INFORMATION + 1]

#define RTC_Time_Year					b.eeprom[EEP_YEAR]
#define RTC_Time_Month					b.eeprom[EEP_MONTH]	
#define RTC_Time_Week					b.eeprom[EEP_WEEK]	
#define RTC_Time_Day					b.eeprom[EEP_DAY]	
#define RTC_Time_Hour					b.eeprom[EEP_HOUR]	
#define RTC_Time_Minute					b.eeprom[EEP_MINUTE]	
#define RTC_Time_Second					b.eeprom[EEP_SECOND]
#define RTC_Time_Alarm					b.eeprom[EEP_DIAGNOSTIC_ALARM]

//#define AI1_Function				b.eeprom[EEP_ANALOG1_FUNCTION]
//#define AI2_Function				b.eeprom[EEP_ANALOG2_FUNCTION]
//#define AI3_Function				b.eeprom[EEP_ANALOG3_FUNCTION]
//#define AI4_Function				b.eeprom[EEP_ANALOG4_FUNCTION]
//#define AI5_Function				b.eeprom[EEP_ANALOG5_FUNCTION]
//#define AI6_Function				b.eeprom[EEP_ANALOG6_FUNCTION]
//#define AI7_Function				b.eeprom[EEP_ANALOG7_FUNCTION]
//#define AI8_Function				b.eeprom[EEP_ANALOG8_FUNCTION]
#define AI_Function(x)        b.eeprom[EEP_ANALOG1_FUNCTION+x]
#define AI_Function1        b.eeprom[EEP_ANALOG1_FUNCTION] 
#define AI_Function2        b.eeprom[EEP_ANALOG2_FUNCTION] 
#define AI_Function3        b.eeprom[EEP_ANALOG3_FUNCTION] 
#define AI_Function4        b.eeprom[EEP_ANALOG4_FUNCTION] 
#define AI_Function5        b.eeprom[EEP_ANALOG5_FUNCTION] 
#define AI_Function6        b.eeprom[EEP_ANALOG6_FUNCTION] 
#define AI_Function7        b.eeprom[EEP_ANALOG7_FUNCTION] 
#define AI_Function8        b.eeprom[EEP_ANALOG8_FUNCTION] 

#define Out6_Function				b.eeprom[EEP_OUTPUT6_FUNCTION]
#define Out7_Function				b.eeprom[EEP_OUTPUT7_FUNCTION]

#define Ecomony_CSP_LO					b.eeprom[EEP_ECOMONY_COOLING_SETPOINT]
#define Ecomony_CSP_HI					b.eeprom[EEP_ECOMONY_COOLING_SETPOINT + 1]
#define Ecomony_HSP_LO					b.eeprom[EEP_ECOMONY_HEATING_SETPOINT]
#define Ecomony_HSP_HI					b.eeprom[EEP_ECOMONY_HEATING_SETPOINT + 1]

#define Hum_Offset_flag					b.eeprom[HUM_OFFSETFLAG] 	
//#define AO1_Manual_Value_LO					b.eeprom[EEP_MANUAL_AO1]
//#define AO1_Manual_Value_HI					b.eeprom[EEP_MANUAL_AO1 + 1]
//#define AO2_Manual_Value_LO					b.eeprom[EEP_MANUAL_AO2]
//#define AO2_Manual_Value_HI					b.eeprom[EEP_MANUAL_AO2 + 1]

//	EEP_ICON 
//	EEP_EXTERNAL_SENSOR1 = EEP_MANUAL_AO2 + 2,
//	EEP_EXTERNAL_SENSOR2 = EEP_EXTERNAL_SENSOR1 + 2,
//	HUM_LOCK_A = EEP_EXTERNAL_SENSOR2 + 2,//= EEP_USER_INFO_SCREEN2_LINE2_CHAR_78 + 2,
//	HUM_LOCK_B,

#define lcd_rotate_max						b.eeprom[EEP_LCD_ROTATE_ENABLE] 
#define schedule_on_off						b.eeprom[EEP_SCHEDULE_ON_OFF] 

#define disp_item_queue(x)				b.eeprom[EEP_DISP_ITEM_TEMPERATURE + x]
//#define disp_item_queue[0]				b.eeprom[EEP_DISP_ITEM_TEMPERATURE] 
//#define disp_item_queue[1]				b.eeprom[EEP_DISP_ITEM_SETPOINT] 
//#define disp_item_queue[2]				b.eeprom[EEP_DISP_ITEM_AI1] 
//#define disp_item_queue[3]				b.eeprom[EEP_DISP_ITEM_AI2] 
//#define disp_item_queue[4]				b.eeprom[EEP_DISP_ITEM_AI3] 
//#define disp_item_queue[5]				b.eeprom[EEP_DISP_ITEM_AI4] 
//#define disp_item_queue[6]				b.eeprom[EEP_DISP_ITEM_AI5] 
//#define disp_item_queue[7]				b.eeprom[EEP_DISP_ITEM_AI6] 
//#define disp_item_queue[8]				b.eeprom[EEP_DISP_ITEM_AI7] 
//#define disp_item_queue[9]				b.eeprom[EEP_DISP_ITEM_AI8] 
//#define disp_item_queue[10]				b.eeprom[EEP_DISP_ITEM_MODE] 
//#define disp_item_queue[11]				b.eeprom[EEP_DISP_ITEM_USER_INFO] 
//#define disp_item_queue[12]				b.eeprom[EEP_DISP_ITEM_CLOCK_DATE] 
//#define disp_item_queue[13]				b.eeprom[EEP_DISP_ITEM_CLOCK_TIME] 
//#define disp_item_queue[14]				b.eeprom[EEP_DISP_ITEM_EXTENAL_SENSOR1] 
//#define disp_item_queue[15]				b.eeprom[EEP_DISP_ITEM_EXTENAL_SENSOR2] 
//#define disp_item_queue[16]				b.eeprom[EEP_DISP_ITEM_EXTENAL_SENSOR3] 
//#define disp_item_queue[17]				b.eeprom[EEP_DISP_ITEM_EXTENAL_SENSOR4] 
//#define disp_item_queue[18]				b.eeprom[EEP_DISP_ITEM_OUT1] 
//#define disp_item_queue[19]				b.eeprom[EEP_DISP_ITEM_OUT2] 
//#define disp_item_queue[20]				b.eeprom[EEP_DISP_ITEM_OUT3] 
//#define disp_item_queue[21]				b.eeprom[EEP_DISP_ITEM_OUT4] 
//#define disp_item_queue[22]				b.eeprom[EEP_DISP_ITEM_OUT5] 
//#define disp_item_queue[23]				b.eeprom[EEP_DISP_ITEM_OUT6] 
//#define disp_item_queue[23]				b.eeprom[EEP_DISP_ITEM_OUT7] 	   

#define Calibration_External_Tem_LO					b.eeprom[EEP_EXT_SENSOR_TEMP_CAL]
#define Calibration_External_Tem_HI					b.eeprom[EEP_EXT_SENSOR_TEMP_CAL + 1]

#define LeftButton1					b.eeprom[EEP_BUTTON_LEFT1]
#define LeftButton2					b.eeprom[EEP_BUTTON_LEFT2]
#define LeftButton3					b.eeprom[EEP_BUTTON_LEFT3]
#define LeftButton4					b.eeprom[EEP_BUTTON_LEFT4]

#define hum_manual_enable								b.eeprom[EEP_HUM_INPUT_MANUAL_ENABLE]
#define HumInputManualValue_LO					b.eeprom[EEP_HUM_INPUT_MANUAL_VALUE]
#define HumInputManualValue_HI					b.eeprom[EEP_HUM_INPUT_MANUAL_VALUE + 1] 	 

#define co2_manual_enable								b.eeprom[EEP_CO2_INPUT_MANUAL_ENABLE]
#define CO2InputManualValue_LO					b.eeprom[EEP_CO2_INPUT_MANUAL_VALUE]
#define CO2InputManualValue_HI					b.eeprom[EEP_CO2_INPUT_MANUAL_VALUE + 1] 	

#define PID2_Output_OFF_Table(x)							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_BEGIN + x]	
//#define PID2_Output_OFF_Table[0]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_BEGIN]	
//#define PID2_Output_OFF_Table[1]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_COOL1]	
//#define PID2_Output_OFF_Table[2]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_COOL2]	
//#define PID2_Output_OFF_Table[3]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_COOL3]	
//#define PID2_Output_OFF_Table[4]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_HEAT1]	
//#define PID2_Output_OFF_Table[5]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_HEAT2]	
//#define PID2_Output_OFF_Table[6]							b.eeprom[EEP_UNIVERSAL_OFF_TABLE_HEAT3]	

#define PID2_Valve_OFF_Table(x)							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_BEGIN + x]
//#define PID2_Valve_OFF_Table[0]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_BEGIN]
//#define PID2_Valve_OFF_Table[1]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_COOL1]
//#define PID2_Valve_OFF_Table[2]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_COOL2]
//#define PID2_Valve_OFF_Table[3]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_COOL3]
//#define PID2_Valve_OFF_Table[4]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_HEAT1]
//#define PID2_Valve_OFF_Table[5]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_HEAT2]
//#define PID2_Valve_OFF_Table[6]							b.eeprom[EEP_UNIVERSAL_OFF_VALVE_HEAT3]

#define KeypadReverse												b.eeprom[EEP_KEYPAD_REVERSE]
#define PirSensorSelect											b.eeprom[EEP_PIR_SENSOR_SELECT]
#define PirSensorZero											b.eeprom[EEP_PIR_SENSOR_ZERO]
#define HumHeatControl											b.eeprom[EEP_HUM_HEATING_CONTROL]
#define ID_Lock															b.eeprom[EEP_ID_WRITE_ENABLE]
#define PirTimer														b.eeprom[EEP_PIR_TIMER]


//	EEP_LIGHT_SENSOR,  //select which item will be on AI8
//	EEP_PIR_SENSOR_VALUE,

#define Supply_SP_LO 														b.eeprom[EEP_SUPPLY_SETPOINT]
#define Supply_SP_HI														b.eeprom[EEP_SUPPLY_SETPOINT + 1] 	  

#define Supply_Max_SP_LO 												b.eeprom[EEP_MAX_SUPPLY_SETPOINT]
#define Supply_Max_SP_HI												b.eeprom[EEP_MAX_SUPPLY_SETPOINT + 1] 

#define Supply_Min_SP_LO 												b.eeprom[EEP_MIN_SUPPLY_SETPOINT]
#define Supply_Min_SP_HI												b.eeprom[EEP_MIN_SUPPLY_SETPOINT + 1] 

#define Max_AirflowSP_Cool_LO 									b.eeprom[EEP_MAX_AIRFLOW_COOLING]
#define Max_AirflowSP_Cool_HI										b.eeprom[EEP_MAX_AIRFLOW_COOLING + 1] 

#define Max_AirflowSP_Heat_LO 									b.eeprom[EEP_MAX_AIRFLOW_HEATING]
#define Max_AirflowSP_Heat_HI										b.eeprom[EEP_MAX_AIRFLOW_HEATING + 1] 

#define Occ_Min_Airflow_LO 													b.eeprom[EEP_OCC_MIN_AIRFLOW]
#define Occ_Min_Airflow_HI													b.eeprom[EEP_OCC_MIN_AIRFLOW + 1] 
#define Unocc_Min_Airflow_LO 													b.eeprom[EEP_UNOCC_MIN_AIRFLOW]
#define Unocc_Min_Airflow_HI													b.eeprom[EEP_UNOCC_MIN_AIRFLOW + 1] 

#define Airflow_SP_LO 													b.eeprom[EEP_AIRFLOW_SETPOINT]
#define Airflow_SP_HI														b.eeprom[EEP_AIRFLOW_SETPOINT + 1] 

#define VAV_Control															b.eeprom[EEP_VAV_CONTROL] 

#define PID3_InputSelect												b.eeprom[EEP_PID3_INPUT_SELECT] 

#define PID3_Valve_Table(x)								b.eeprom[EEP_PID3_VALVE_OPER_TABLE_BEGIN + x]
//#define PID3_Valve_Table[0]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_BEGIN]
//#define PID3_Valve_Table[1]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_COOL1]
//#define PID3_Valve_Table[2]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_COOL2]
//#define PID3_Valve_Table[3]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_COOL3]
//#define PID3_Valve_Table[4]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_HEAT1]
//#define PID3_Valve_Table[5]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_HEAT2]
//#define PID3_Valve_Table[6]							b.eeprom[EEP_PID3_VALVE_OPER_TABLE_HEAT3]

#define pid3_cool_db										b.eeprom[EEP_PID3_COOLING_DB]
#define pid3_heat_db										b.eeprom[EEP_PID3_HEATING_DB]
#define pid3_pterm											b.eeprom[EEP_PID3_PTERM]
#define pid3_iterm											b.eeprom[EEP_PID3_ITERM]


//	EEP_PID3_OUTPUT,
#define PID3_Output_Table(x)							b.eeprom[EEP_PID3_OUTPUT_BEGIN + x]
//#define PID3_Output_Table[0]							b.eeprom[EEP_PID3_OUTPUT_BEGIN]	
//#define PID3_Output_Table[1]							b.eeprom[EEP_PID3_OUTPUT_COOL1]	
//#define PID3_Output_Table[2]							b.eeprom[EEP_PID3_OUTPUT_COOL2]	
//#define PID3_Output_Table[3]							b.eeprom[EEP_PID3_OUTPUT_COOL3]	
//#define PID3_Output_Table[4]							b.eeprom[EEP_PID3_OUTPUT_HEAT1]	
//#define PID3_Output_Table[5]							b.eeprom[EEP_PID3_OUTPUT_HEAT2]	
//#define PID3_Output_Table[6]							b.eeprom[EEP_PID3_OUTPUT_HEAT3]	

#define PID3_Valve_Off_Table(x)							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_BEGIN + x]
//#define PID3_Valve_Off_Table[0]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_BEGIN]
//#define PID3_Valve_Off_Table[1]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_COOL1]
//#define PID3_Valve_Off_Table[2]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_COOL2]
//#define PID3_Valve_Off_Table[3]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_COOL3]
//#define PID3_Valve_Off_Table[4]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_HEAT1]
//#define PID3_Valve_Off_Table[5]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_HEAT2]
//#define PID3_Valve_Off_Table[6]							b.eeprom[EEP_PID3_VALVE_OFF_TABLE_HEAT3]

#define PID3_Output_OFF_Table(x)							b.eeprom[EEP_PID3_OFF_OUTPUT_BEGIN + x]
//#define PID3_Output_OFF_Table[0]							b.eeprom[EEP_PID3_OFF_OUTPUT_BEGIN]	
//#define PID3_Output_OFF_Table[1]							b.eeprom[EEP_PID3_OFF_OUTPUT_COOL1]	
//#define PID3_Output_OFF_Table[2]							b.eeprom[EEP_PID3_OFF_OUTPUT_COOL2]	
//#define PID3_Output_OFF_Table[3]							b.eeprom[EEP_PID3_OFF_OUTPUT_COOL3]	
//#define PID3_Output_OFF_Table[4]							b.eeprom[EEP_PID3_OFF_OUTPUT_HEAT1]	
//#define PID3_Output_OFF_Table[5]							b.eeprom[EEP_PID3_OFF_OUTPUT_HEAT2]	
//#define PID3_Output_OFF_Table[6]							b.eeprom[EEP_PID3_OFF_OUTPUT_HEAT3]	


//	EEP_WIRELESS_PIR_RESPONSE1,
//	EEP_WIRELESS_PIR_RESPONSE2,
//	EEP_WIRELESS_PIR_RESPONSE3,
//	EEP_WIRELESS_PIR_RESPONSE4,
//	EEP_WIRELESS_PIR_RESPONSE5,
//	EEP_PRESSURE_VALUE,	

#define PressureManualValue_LO							b.eeprom[EEP_PRESSURE_MANUAL_VALUE]
#define PressureManualValue_HI							b.eeprom[EEP_PRESSURE_MANUAL_VALUE + 1]
#define PressureManualEnable								b.eeprom[EEP_PRESSURE_MANUAL_ENABLE]

//	EEP_AQ_VALUE,
#define AQManualValue_LO										b.eeprom[EEP_AQ_MANUAL_VALUE]
#define AQManualValue_HI										b.eeprom[EEP_AQ_MANUAL_VALUE + 1]
#define AQManualEnable											b.eeprom[EEP_AQ_MANUAL_ENABLE]


//	EEP_TEMPERATURE_OF_PRESSURE_SENSOR,
//	EEP_TEMPERATURE_OF_HUM_SENSOR,

#define SP_DisplaySelect										b.eeprom[EEP_SP_DISPLAY_SELECT]

#define PID3_DaySP_LO												b.eeprom[EEP_PID3_DAY_SP]
#define PID3_DaySP_HI												b.eeprom[EEP_PID3_DAY_SP + 1]
#define PID3_NightSP_LO											b.eeprom[EEP_PID3_NIGHT_SP]
#define PID3_NightSP_HI											b.eeprom[EEP_PID3_NIGHT_SP + 1]


//	NEW_EEP_SERIALNUMBER_LOWORD = EEP_PID3_NIGHT_SP + 2, 
	            
//	NEW_EEP_SERIALNUMBER_HIWORD  = NEW_EEP_SERIALNUMBER_LOWORD + 2,

//	NEW_EEP_ADDRESS = NEW_EEP_SERIALNUMBER_HIWORD + 2, //829
//	NEW_EEP_PRODUCT_MODEL,
//	NEW_EEP_HARDWARE_REV,
//	BK_SN,
//	BK_EEP_SERIALNUMBER_LOWORD,

//	BK_EEP_SERIALNUMBER_HIWORD  = BK_EEP_SERIALNUMBER_LOWORD + 2,
//	
//	BK_EEP_PRODUCT_MODEL = BK_EEP_SERIALNUMBER_HIWORD + 2,
//	BK_EEP_HARDWARE_REV,


//#define UserInfoEnable											b.eeprom[EEP_TSTAT_NAME_ENABLE]

#define UserInfo_Name(x)										b.eeprom[EEP_TSTAT_NAME1 + x]

#define Show_ID_Enable											b.eeprom[EEP_SHOW_ID]

#define MinTransducerRange									b.eeprom[EEP_TRANSDUCER_RANGE_MIN]
#define MaxTransducerRange									b.eeprom[EEP_TRANSDUCER_RANGE_MAX]

#define ICON_ManualMode											b.eeprom[EEP_ICON_MANUAL_MODE]
#define ICON_ManualValue										b.eeprom[EEP_ICON_MANUAL_VALUE]
#define TimeFormat													b.eeprom[EEP_TIME_FORMAT]
#define FreeCoolConfig											b.eeprom[EEP_FREE_COOL_CONFIG]
#define RS485_Mode													b.eeprom[EEP_RS485_MODE]
#define InputFilter(x)											b.eeprom[EEP_INPUT1_FILTER + x]
#define CO2_Filter													b.eeprom[EEP_CO2_FILTER]
#define HUM_Filter													b.eeprom[EEP_HUM_FILTER]
#define CyclingDelay												b.eeprom[EEP_CYCLING_DELAY]
#define ChangeOverDelay											b.eeprom[EEP_CHANGOVER_DELAY]
#define ManualValvePercent									b.eeprom[EEP_VALVE_PERCENT]
#define ManualRelayAll											b.eeprom[MANUAL_RELAY]
#define ManualAO1_HI												b.eeprom[MANUAL_COOL_VALVE_HI]
#define ManualAO1_LO												b.eeprom[MANUAL_COOL_VALVE_LO]
#define ManualAO2_HI 												b.eeprom[MANUAL_HEAT_VALVE_HI]
#define ManualAO2_LO 												b.eeprom[MANUAL_HEAT_VALVE_LO]

#define LightStage 													b.eeprom[LIGHTING_STAGE]
#define SerialNumberWRFlag 									b.eeprom[EEP_SERINALNUMBER_WRITE_FLAG]
#define ManualAI_HI(x) 										  b.eeprom[MANUAL_ANALOG1_HI + (x)*2]
#define ManualAI_LO(x) 										  b.eeprom[MANUAL_ANALOG1_LO + (x)*2]

#define ManualInternalSensor_HI 							b.eeprom[MANUAL_INTERNAL_HI]
#define ManualInternalSensor_LO 							b.eeprom[MANUAL_INTERNAL_LO]

#define RS485WRFlag 													b.eeprom[RS485_WRITEFLAG]
#define LCDTurnOff 														b.eeprom[EEP_LCD_TURN_OFF]
#define DemandResponse 												b.eeprom[EEP_DEMAND_RESPONSE]
#define LockRegister 													b.eeprom[EEP_LOCK_REGISTER]
#define SetpointUNLimit												b.eeprom[EEP_SETPOINT_UNLIMIT]


//#define Hum_C_RH_LO(x) 												b.eeprom[EEP_HUM_C_RH1 + x*2]
//#define Hum_C_RH_HI(x) 												b.eeprom[EEP_HUM_C_RH1 + x*2 + 1]
//#define Hum_C_FRE_LO(x) 											b.eeprom[EEP_HUM_C_FRE1 + x*2]
//#define Hum_C_FRE_HI(x) 											b.eeprom[EEP_HUM_C_FRE1 + x*2 + 1]

//#define HumRefreshTable 											b.eeprom[EEP_HUM_C_REFRESH_TABLE]
//#define HumErase 															b.eeprom[EEP_HUM_C_ERASE]
//#define HumLock					 											b.eeprom[EEP_HUM_C_LOCK]

#define hum_cal_hi														b.eeprom[EEP_HUM_CALIBRATION + 1]
#define hum_cal_lo														b.eeprom[EEP_HUM_CALIBRATION]

#define user_setting_enable										b.eeprom[USER_SETTING]
#define Modename(x) 													b.eeprom[EEP_MODE1_NAME1 + x]
#define heat_cool_user												b.eeprom[EEP_HC_USER]
#define Plugandplay														b.eeprom[ADDRESS_PLUG_N_PLAY]
#define Sleep_sp_lo														b.eeprom[EEP_SLEEP_SP]
#define Sleep_sp_hi														b.eeprom[EEP_SLEEP_SP + 1]

#define SleepCooldb														b.eeprom[EEP_SLEEP_COOLING_DB]
#define SleepHeatdb														b.eeprom[EEP_SLEEP_HEATING_DB]
#define SleepHeatSp_h													b.eeprom[EEP_SLEEP_HEATING_SP + 1]
#define SleepHeatSp_l													b.eeprom[EEP_SLEEP_HEATING_SP]
#define SleepCoolSp_h													b.eeprom[EEP_SLEEP_COOLING_SP + 1]
#define SleepCoolSp_l													b.eeprom[EEP_SLEEP_COOLING_SP]

#define Away_sp_hi														b.eeprom[EEP_AWAY_SP + 1]
#define Away_sp_lo														b.eeprom[EEP_AWAY_SP]


#define Max_work_sp														b.eeprom[EEP_MAX_WORK_SP]
#define Min_work_sp														b.eeprom[EEP_MIN_WORK_SP]
#define Max_sleep_sp													b.eeprom[EEP_MAX_SLEEP_SP]
#define Min_sleep_sp													b.eeprom[EEP_MIN_SLEEP_SP]
#define Max_holiday_sp												b.eeprom[EEP_MAX_HOLIDAY_SP]
#define Min_holiday_sp												b.eeprom[EEP_MIN_HOLIDAY_SP]
#define Pir_Sensetivity												b.eeprom[EEP_PIR_SENSETIVITY]
 
#define FrcEnable       					b.eeprom[EEP_FRC_ENABLE] //free cooling  function enable/disable 0:DIABLE  1:ENABLE
#define FrcBaseSelect   					b.eeprom[EEP_FRC_BASE_SELECT] //FREE COOLING BASE SELECT 0: TEMPERATUERE BASE 1: ENTHALPY BASE
#define FrcSpaceTemUnit   				b.eeprom[EEP_FRC_SPACETEM_UNIT]
#define FrcSupplyTemUnit   				b.eeprom[EEP_FRC_SUPPLYTEM_UNIT]
#define FrcOutdoorTemUnit   			b.eeprom[EEP_FRC_OUTDOORTEM_UNIT]

#define FrcMinFreshAir   					b.eeprom[EEP_FRC_MIN_FRESH_AIR] // MINIMAL FRESH AIR] UNIT PERCENTAGE
#define FrcMinFreshAirTemLow   		b.eeprom[EEP_FRC_MIN_FRESH_AIRTEM_L] // MINIMAL FRESH AIR TEMPERATURE] UNIT DEGREE C/F
#define FrcMinFreshAirTemHi   		b.eeprom[EEP_FRC_MIN_FRESH_AIRTEM_H] // MINIMAL FRESH AIR TEMPERATURE] UNIT DEGREE C/F
#define FrcOutputConfig					  b.eeprom[EEP_FRC_OUTPUT_CONFIG]  // FREE COOLING OUTPUT CHECK] OUTPUT SOURCE] OUTPUT FUNCTION
#define FrcPid2Config					   	b.eeprom[EEP_FRC_PID2_CONFIG]   // FREE COOLING PID2 CONFIG CHECK] CHECK IF PID2 HAS INPUT FROM SUPPLY SENSOR] ALSO CHECK IF IT IS SET TO NEGATIVE COOLING
#define FrcOutputMode							   b.eeprom[EEP_FRC_OUTPUT_MODE]  // SET FAN TO OFF

#define FrcTotalConfig   					b.eeprom[EEP_FRC_TOTAL_CONFIG]//
#define FrcPresentMode  					b.eeprom[EEP_FRC_PRESENT_MODE]//
#define IconOutputControl(x)      b.eeprom[EEP_ICON_DAY_OUTPUT_CONTROL + x]


//#define ScheduleModeNum						b.eeprom[EEP_SCHEDULE_MODE_NUM]
//#define BoardSelect								b.eeprom[EEP_BOARD_SELECT]

#endif


	





//#endif //TSTAT_ARM
/*************************************************************************************/
