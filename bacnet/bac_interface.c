#include "main.h"
#include "bacnet.h"
#include "controls.h"
#include "bactimevalue.h"
#include "bacdevobjpropref.h"

#if BAC_COMMON 

// MSTP
void Set_TXEN(uint8_t dir)
{
#if MINI
	if(Modbus.mini_type == MINI_TINY || Modbus.mini_type == MINI_NEW_TINY )
	{			
			UART2_TXEN_TINY = dir;
	}
	else if(Modbus.mini_type == MINI_VAV)
	{			
			UART2_TXEN_VAV = dir;
	}
	else
	{
			UART2_TXEN_BIG = dir;
	}
#endif

#if CM5
	UART2_TXEN = dir;
#endif
}

char* get_label(uint8_t type,uint8_t num)
{
	U8_T out_index;

	if(type == CALENDAR)
	{
		if(num >= MAX_CALENDARS) return NULL;
		return (char*)annual_routines[num].label;
	}
	if(type == SCHEDULE)
	{
		if(num >= MAX_SCHEDULES) return NULL;
		return (char*)weekly_routines[num].label;
	}
	if(type == AV)	
	{		
		if(num >= MAX_AVS) return NULL;

		return (char*)vars[num].label;
	}
	if(type == AI)	
	{		
		if(num >= MAX_AIS) return NULL;
		return (char*)inputs[num].label;
	}
	if(type == AO)
	{		
		Get_index_by_AOx(num,&out_index);
		if(out_index >= MAX_AOS) return NULL;
		return (char*)outputs[out_index].label;
	}
	if(type == BO)
	{
		Get_index_by_BOx(num,&out_index);
		if(out_index >= MAX_BOS) return NULL;
		return (char*)outputs[out_index].label;
	}
	return NULL;
}

char* get_description(uint8_t type,uint8_t num)
{
	U8_T out_index;

	if(type == SCHEDULE)
	{
		if(num >= MAX_SCHEDULES) return NULL;
		return (char*)weekly_routines[num].description;
	}
	if(type == CALENDAR)
	{
		if(num >= MAX_CALENDARS) return NULL;
		return (char*)annual_routines[num].description;
	}
	if(type == AV)	
	{
		if(num >= MAX_AVS) return NULL;
//		return AVS_NAME[num];
		return (char*)vars[num].description;
	}
	if(type == AI)
	{		
		if(num >= MAX_AIS) return NULL;
		return (char*)inputs[num].description;
	}
	if(type == AO)
	{		
		Get_index_by_AOx(num,&out_index);
		if(out_index >= MAX_AOS) return NULL;
		return (char*)outputs[out_index].description;
	}
	if(type == BO)	
	{
		Get_index_by_BOx(num,&out_index);
		if(out_index >= MAX_BOS) return NULL;
		return (char*)outputs[out_index].description;
	}
	
	return NULL;
}

// tbd: add more range
char get_range(uint8_t type,uint8_t num)
{
	uint8_t out_index;

	if(type == AV)	
	{
		if(num >= MAX_AVS) return 0;
// add unit
	
//		if(vars[num].digital_analog == 0)  // digital
//		{
//			return UNITS_NO_UNITS;	
//		}
//		else  // analog
//		{		
//			if((vars[num].range == degC) 
//				return UNITS_DEGREES_CELSIUS;
//			else if((vars[num].range == degF) 
//				return UNITS_DEGREES_FAHRENHEIT;
//			else if(vars[num].range == ma) 
//				return UNITS_MILLIAMPERES;
//			else if((vars[num].range == Volts) 
//				return UNITS_VOLTS;
//			else
//				return UNITS_NO_UNITS;	
//		}
			return UNITS_NO_UNITS;  
	}
	if(type == AI)	
	{
		if(num >= MAX_AIS) return 0;
		if(inputs[num].digital_analog == 0)  // digital
		{
			return UNITS_NO_UNITS;	
		}
		else  // analog
		{
		
			if((inputs[num].range == R10K_40_120DegC) || (inputs[num].range == KM10K_40_120DegC)) 
				return UNITS_DEGREES_CELSIUS;
			else if((inputs[num ].range == R10K_40_250DegF) || (inputs[num].range == KM10K_40_250DegF)) 
				return UNITS_DEGREES_FAHRENHEIT;
			else if(inputs[num].range == I0_20ma) 
				return UNITS_MILLIAMPERES;
			else if((inputs[num].range == V0_10_IN) || (inputs[num].range == V0_5)) 
				return UNITS_VOLTS;
			else
				return UNITS_NO_UNITS;	
		}
	}
	if(type == AO)
	{	
		Get_index_by_AOx(num,&out_index);
		
		if(outputs[out_index].range == 0)
			return UNITS_NO_UNITS;	
		else	
		{		
			if(outputs[out_index].range == V0_10)			
				return UNITS_VOLTS;
			else if((outputs[out_index].range == P0_100_Open) || 
				(outputs[out_index].range == P0_100_Close) ||
				(outputs[out_index].range == P0_100))			
				return UNITS_PERCENT;
			else if(outputs[out_index].range == I_0_20ma)			
				return UNITS_MILLIAMPERES;
			else
				return UNITS_NO_UNITS;	
		}
	}
	if(type == BO)	
	{
		if(num >= MAX_BOS) return 0;
		return UNITS_NO_UNITS;	
	}
	return 0;
}


char get_AM_Status(uint8_t type,uint8_t num)
{	
	
	uint8_t out_index;
	
	if(type == AO)
	{
		Get_index_by_AOx(num,&out_index);
		if((outputs[out_index].auto_manual == 0) && (outputs[out_index].switch_status == SW_AUTO))  // out of sever
			return 0;
		else
			return 1;
		//return outputs[out_index].auto_manual;
	}
	else if(type == BO)	
	{
		Get_index_by_BOx(num,&out_index);
		//return outputs[out_index].auto_manual;
		if((outputs[out_index].auto_manual == 0) && (outputs[out_index].switch_status == SW_AUTO))  // out of sever
			return 0;
		else
			return 1;
	}	
	else if(type == SCHEDULE)	
	{
		if(num >= MAX_SCHEDULES) return 0;
		return weekly_routines[num].auto_manual;
	}
	else if(type == CALENDAR)	
	{
		if(num >= MAX_CALENDARS) return 0;
		return annual_routines[num].auto_manual;
	}
	else if(type == AV)	
	{
		if(num >= MAX_AVS) return 0;
		return vars[num].auto_manual;
	}
	else
		return 0;
}


float Get_bacnet_value_from_buf(uint8_t type,uint8_t priority,uint8_t i)
{	
	uint8_t out_index;
	
	switch(type)
	{
		case AV:
//			Get_AVS();
//			return bacnet_AV.avs[i];
		return (float)swap_double(vars[i].value) / 1000;
			//break;
		case AI:
//			if(inputs[i].range > 0)
//				return (float)swap_double(inputs[i].value) / 1000;
//			else
//				return input_raw[i];
		if(inputs[i].range > 0)
			{
				if(inputs[i].digital_analog == 0)  // digital
				{
					if(inputs[i].range >= ON_OFF  && inputs[i].range <= HIGH_LOW)
					{  // inverse logic
						if(inputs[i].control == 1)
						{
							return 0;
						}
						else
						{
							return 1;
						}
					}	
					else
					{
						if(inputs[i].control == 1)
						{
							return 1;
						}
						else
						{
							return 0;
						}
					}	
				}
				else  // analog
				{
					return (float)swap_double(inputs[i].value) / 1000;
				}	
			}
			else
				return input_raw[i];
		//break;
		case AO:
//				if(outputs[i + max_dos].auto_manual == 0)
//					priority = 15; // AUTO
//				else
//					priority = 7; // manual
		// find output index by AOx
				Get_index_by_AOx(i,&out_index);
				if(outputs[out_index].range > 0)
					return (float)swap_double(outputs[out_index].value)/ 1000;
				else
					return output_raw[out_index];
		//break;
			
		case BO:
				if(outputs[out_index].range >= ON_OFF  && outputs[out_index].range <= HIGH_LOW)
		{  // inverse logic
			if(outputs[out_index].control == 1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}	
		else
		{
			if(outputs[out_index].control == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		case SCHEDULE:			
				return swap_double(weekly_routines[i].value) / 1000;
		//break;
		case CALENDAR:			
				return swap_double(annual_routines[i].value) / 1000;
		//break;		
		default:
			break;
				
	}	
	return 0;
}

char* Get_Object_Name(void)
{
	return panelname;
}

//Wr_one_day					far		wr_times[MAX_WR][MAX_SCHEDULES_PER_WEEK];
// get time & value from weekly roution
BACNET_TIME_VALUE Get_Time_Value(uint8_t object_index,uint8_t day,uint8_t i)
{
	BACNET_TIME_VALUE far array;
	array.Time.hour = wr_times[object_index][day].time[i].hours;
	array.Time.min = wr_times[object_index][day].time[i].minutes;
	array.Time.sec = 0;
	array.Time.hundredths = 0;
	array.Value.type.Enumerated = (i + 1) % 2;
	array.Value.tag = BACNET_APPLICATION_TAG_ENUMERATED;
	return array;

}

uint8_t Get_TV_count(uint8_t object_index,uint8_t day)
{
	char i;
	for(i = 0;i < 8;i++)
	{
		if((wr_times[object_index][day].time[i].hours == 0) && (wr_times[object_index][day].time[i].minutes == 0))
			return i;
	}
	return 8;
}


BACNET_DEVICE_OBJECT_PROPERTY_REFERENCE * Get_Object_Property_References(uint8_t i)
{
	
	return NULL;
}


uint8_t Get_CALENDAR_count(uint8_t object_index)
{
	uint16_t i,j;
	uint16_t count;
	count = 0;
	
	for(i = 0;i < AR_DATES_SIZE;i++)
	{
		for(j = 0;j < 8;j++)
		{
			if(ar_dates[object_index][i] & (0x01 << j)) 
			{
				count++;
			}
		}
	}
	
	return count;
}


void get_bacnet_date_by_dayofyear(uint16_t dayofyear,BACNET_DATE * array)
{
	uint16_t far day;
	uint16_t far i;//,j,k;

	day = dayofyear;

	if( ( Rtc.Clk.year & '\x03' ) == '\x0' )
		month_length[1] = 29;
	else
		month_length[1] = 28;
	
	for(i = 0;i < 12;i++)
	{
		Test[10 + i] = month_length[i];
		if(day > month_length[i])
		{
			day -= month_length[i];
		}
		else
		{
			array->month = i + 1;
			array->day = day + 1;
			i = 12;
		}
	}
	
	if(Rtc.Clk.day_of_year > dayofyear)
	{
		day = Rtc.Clk.week + 7 - (Rtc.Clk.day_of_year - dayofyear) % 7;
	}
	else
		day = Rtc.Clk.week + (dayofyear - Rtc.Clk.day_of_year) % 7;
	
	array->wday = day % 7;
	array->year = 2000 + Rtc.Clk.year;
	

	
}


BACNET_DATE Get_Calendar_Date(uint8_t object_index,uint8_t k)
{
	BACNET_DATE far array;
	
	uint16_t i,j;
	uint16_t dayofyear;
	uint16_t count;
	count = 0;


	for(i = 0;i < AR_DATES_SIZE;i++)
	{	
		for(j = 0;j < 8;j++)
		{
			if(ar_dates[object_index][i] & (0x01 << j)) 
			{
				if(k == count) // find current count
				{
					dayofyear = i * 8 + j;
					// map to BACNET_DATE
					get_bacnet_date_by_dayofyear(dayofyear,&array);
				}
				count++;	
			}
					
		}
	}
	Test[30] = array.year;
	Test[31] = array.month;
	Test[32] = array.day;
	Test[33] = array.wday;

	return array;
}




void write_bacnet_unit_to_buf(uint8_t type,uint8_t priority,uint8_t i,uint8_t unit)
{
	U8_T temp;
	uint8_t out_index;
	//if((AV_Present_Value[0] == Modbus.address) ||(AV_Present_Value[0] == 0) ) // internal AV, DI ,AV ...

		switch(type)
		{
			case AI:
				if(i >=  MAX_AIS) break;
//				if(priority <= 8)	// manual
//					inputs[i].auto_manual = 1;	
//				else // auto
//					inputs[i].auto_manual = 0;
//						
//				if(inputs[i].auto_manual == 1)
				if(unit == UNITS_NO_UNITS)
				{
					inputs[i].range = not_used_input;			
				}
				if(unit == UNITS_DEGREES_CELSIUS)
				{
					inputs[i].range = R10K_40_120DegC;
				}
				if(unit == UNITS_DEGREES_FAHRENHEIT)
				{
					inputs[i].range = R10K_40_250DegF;	
				}				
				if(unit == UNITS_AMPERES)
				{
						inputs[i].range = I0_20ma;
					// software jumper 
					temp = inputs[i].decom;
					temp &= 0x0f;
					temp |= (INPUT_I0_20ma << 4);
					inputs[i].decom = temp;
				}
				if(unit == UNITS_VOLTS)
				{
						inputs[i].range = V0_10_IN;
					// software jumper 
					temp = inputs[i].decom;
					temp &= 0x0f;
					temp |= (INPUT_0_10V << 4);
					inputs[i].decom = temp;
				}
				
				if( (unit != UNITS_VOLTS) && (unit != UNITS_AMPERES) )
				{
						// software jumper 
					temp = inputs[i].decom;
					temp &= 0x0f;
					temp |= (INPUT_THERM << 4);
					inputs[i].decom = temp;
				}
#if  T3_MAP
				push_expansion_in_stack(&inputs[i]);
#endif
				break;
			case BO:
				Get_index_by_BOx(i,&out_index);

				if(out_index >= MAX_BOS) break;
				outputs[out_index].digital_analog = 0;
				if(unit == UNITS_NO_UNITS)
					outputs[out_index].range = 0;
				else
					outputs[out_index].range = OFF_ON;
#if  T3_MAP
				push_expansion_out_stack(&outputs[out_index]);
#endif
				break;
			case AO:
				Get_index_by_AOx(i,&out_index);

				if(out_index >= MAX_AOS) break;
				outputs[out_index].digital_analog = 1;
				if(unit == UNITS_NO_UNITS)
					outputs[out_index].range = 0;
				else
					outputs[out_index].range = V0_10;
#if  T3_MAP				
				push_expansion_out_stack(&outputs[out_index]);	
#endif				
				break;
	
			default:
			break;
		}
}

void write_bacnet_name_to_buf(uint8_t type,uint8_t priority,uint8_t i,char* str)
{
	uint8_t out_index;

	//if((AV_Present_Value[0] == Modbus.address) ||(AV_Present_Value[0] == 0) ) // internal AV, DI ,AV ...

		switch(type)
		{
			case AI:
				if(i >= MAX_AVS) break;
				memcpy(inputs[i].label,str,8);
#if  T3_MAP
				push_expansion_in_stack(&inputs[i]);
#endif
				break;
			case BO:
				Get_index_by_BOx(i,&out_index);
				if(out_index >= MAX_BOS) break;
				memcpy(outputs[out_index].label,str,8);
#if  T3_MAP
				push_expansion_out_stack(&outputs[out_index]);
#endif
				break;
			case AO:
				Get_index_by_AOx(i,&out_index);
				if(out_index >= MAX_AOS) break;
				memcpy(outputs[out_index].label,str,8);
#if  T3_MAP
				push_expansion_out_stack(&outputs[out_index]);
#endif
				break;
			case AV:
				if(i >= MAX_AVS) break;
				memcpy(vars[i].label,str,8);
				break;
			case SCHEDULE:
				if(i >= MAX_SCHEDULES) break;
				memcpy(weekly_routines[i].label,str,8);
				break;
			case CALENDAR:
				if(i >= MAX_CALENDARS) break;
				memcpy(annual_routines[i].label,str,8);
				break;
			default:
			break;
		}
			
}

void wirte_bacnet_value_to_buf(uint8_t type,uint8_t priority,uint8_t i,float value)
{
	uint8_t out_index;
		switch(type)
		{
			case AV:
//				if(priority <= 8)	// manual
//					vars[i].auto_manual = 1;	
//				else 	// auto
//					vars[i].auto_manual = 0;
//												
//				if(vars[i].auto_manual == 1)
			if(i >= MAX_AVS) break;
//				bacnet_AV.avs[i] = value;
////   store AVS			
//				Store_AVS(i);			
			vars[i].value = swap_double(value * 1000);
				break;
			case AI:
				if(i >= MAX_AIS) break;
//				if(priority <= 8)	// manual
//					inputs[i - BASE_BAC].auto_manual = 1;	
//				else // auto
//					inputs[i - BASE_BAC].auto_manual = 0;
				
				
				if(inputs[i].digital_analog == 0)  // digital
				{
					if(inputs[i].range == 0)
						inputs[i].range = OFF_ON;
					inputs[i].value = swap_double(value * 1000);	
//					inputs[i].control = value ? 1 : 0;	
					if( inputs[i].range >= OFF_ON && inputs[i].range <= LOW_HIGH )
					{
						inputs[i].control = value ? 1 : 0;							
					}
					if( inputs[i].range >= ON_OFF && inputs[i].range <= HIGH_LOW )
					{
						inputs[i].control = value ? 0 : 1;		
					}	
				}
				else
				{	
					if(inputs[i].range == 0)
						inputs[i].range = R10K_40_120DegC;
					inputs[i].value = swap_double(value * 1000) ;
				}
#if  T3_MAP
				push_expansion_in_stack(&inputs[i]);
#endif
				break;
			case BO:
				Get_index_by_BOx(i,&out_index);
				if(out_index >= MAX_BOS) break;
//				if(priority <= 8)	// manual
//					outputs[i].auto_manual = 1;	
//				else 	// auto
//					outputs[i].auto_manual = 0;
					
				//if(outputs[i].auto_manual == 1)
				{

					//outputs[i].auto_manual = 1;
					outputs[out_index].digital_analog = 0;
					outputs[out_index].value = swap_double(value * 1000);					
//					outputs[out_index].control = value ? 1 : 0;		
					if( outputs[out_index].range >= OFF_ON && outputs[out_index].range <= LOW_HIGH )
					{
						outputs[out_index].control = value ? 1 : 0;							
					}
					if( outputs[out_index].range >= ON_OFF && outputs[out_index].range <= HIGH_LOW )
					{
						outputs[out_index].control = value ? 0 : 1;		
					}					
					if(outputs[out_index].range == 0)
						outputs[out_index].range = OFF_ON;
				}
#if  T3_MAP
				push_expansion_out_stack(&outputs[out_index]);
#endif
				break;
			case AO:	
				Get_index_by_AOx(i,&out_index);
					if(out_index >= MAX_AOS) break;
//				if(priority <= 8)	// manual
//					outputs[i + max_dos].auto_manual = 1;	
//				else 	// auto
//					outputs[i + max_dos].auto_manual = 0;
				if(outputs[out_index].range == 0)
					outputs[out_index].range = V0_10;
				//outputs[out_index].auto_manual = 1;			
				outputs[out_index].digital_analog = 1;
				//if(outputs[out_index].auto_manual == 1)
				{
					outputs[out_index].value = swap_double(value * 1000);
				}
#if  T3_MAP
				push_expansion_out_stack(&outputs[out_index]);
#endif
				break;
		 case SCHEDULE:
				//if(weekly_routines[i].auto_manual == 1)
				{
					weekly_routines[i].value = swap_double(value * 1000);
				}
				break;
			case CALENDAR:
				//if(annual_routines[i].auto_manual == 1)
				{
					annual_routines[i].value = swap_double(value * 1000);
				}
				break;
			default:
			break;
		}			
}

void write_bacent_AM_to_buf(uint8_t type,uint8_t i,uint8_t am)
{
	uint8_t out_index;
	if(type == BO)
	{	
		Get_index_by_BOx(i,&out_index);
		outputs[out_index].auto_manual = am;
#if  T3_MAP		
		push_expansion_out_stack(&outputs[out_index]);
#endif
	}
	if(type == AO)
	{
		Get_index_by_AOx(i,&out_index);
		outputs[out_index].auto_manual = am;
#if  T3_MAP
		push_expansion_out_stack(&outputs[out_index]);
#endif 
	}
}

void write_bacnet_description_to_buf(uint8_t type,uint8_t priority,uint8_t i,char* str)
{
	uint8_t out_index;
	switch(type)
	{
		case AI:
			if(i >= MAX_AVS) break;
			memcpy(inputs[i].description,str,21);
#if  T3_MAP
			push_expansion_in_stack(&inputs[i]);
#endif
			break;
		case BO:
			Get_index_by_BOx(i,&out_index);
			if(out_index >= MAX_BOS) break;
			memcpy(outputs[out_index].description,str,19);
#if  T3_MAP
			push_expansion_out_stack(&outputs[out_index]);
#endif
			break;
		case AO:
			Get_index_by_AOx(i,&out_index);
			if(out_index >= MAX_AOS) break;
			memcpy(outputs[out_index].description,str,19);
#if  T3_MAP
			push_expansion_out_stack(&outputs[out_index]);
#endif 
			break;
		case AV:
			if(i >= MAX_AVS) break;
			memcpy(vars[i].description,str,21);
			break;
		case SCHEDULE:
			if(i >= MAX_SCHEDULES) break;
			memcpy(weekly_routines[i].description,str,21);
			break;
		case CALENDAR:
			if(i >= MAX_CALENDARS) break;
			memcpy(annual_routines[i].description,str,21);
			break;
		default:
		break;
	}

}


void Set_Object_Name(char * name)
{	
	memcpy(panelname,name,20);	
}


void write_Time_Value(uint8_t index,uint8_t day,uint8_t i,uint8_t hour,uint8_t min/*BACNET_TIME_VALUE time_value*/)
{	
	wr_times[index][day].time[i].hours = hour;//time_value.Time.hour;
	wr_times[index][day].time[i].minutes = min;//time_value.Time.min;
	
}


void write_annual_date(uint8_t index,BACNET_DATE date)
{
	uint16_t day;
	char j;

	if(date.year != 2000 + Rtc.Clk.year) return;
	
	if( ( Rtc.Clk.year & '\x03' ) == '\x0' )
		month_length[1] = 29;
	else
		month_length[1] = 28;
	
	day = 0;
	if(date.month >= 1 && date.month <= 12)
	{
		for(j = 0;j < date.month - 1;j++)
			day += month_length[j];
	}
	day += date.day;
	
	ar_dates[index][(day - 1) / 8] |= (0x01 << ((day - 1) % 8));

}


#endif


//void loopmove(char *str,int step)
//{
//	char *tempstr;
//	int count;
//	int len = strlen(str);
//	if(str == NULL) return;
//	
//	count = step % len;
//	
//	tempstr = malloc(len+1);
//	memcpy(tempstr,str,len);
//	
//	strcat(&tempstr[len-count],str);
//	memcpy(str,tempstr,len);
//	
//	free(tempstr);
//	
//	
//}