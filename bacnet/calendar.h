/**************************************************************************
*
* Copyright (C) 2005 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#ifndef CALENDAR_H
#define CALENDAR_H

#include <stdbool.h>
#include <stdint.h>
#include "bacdef.h"
#include "rp.h"
#include "wp.h"
#include "bacdevobjpropref.h"
#include "bactimevalue.h"


//BACnetDateRange Structure
typedef struct BACnetDateRange
{
    BACNET_DATE startDate;
    BACNET_DATE endDate;
}BACnetDateRange;

//BACnetWeekNDay BACnetWeekNDay
typedef struct BACnetWeekNDay
{
    uint8_t month;
    uint8_t weekOfMonth;
    uint8_t dayOfWeek;
}BACnetWeekNDay;


typedef enum BACnetCalendarEntryType
{
	BACnetCalendarEntryTypeDate,
	BACnetCalendarEntryTypeDateRange,
	BACnetCalendarEntryTypeWeekNDay
}BACnetCalendarEntryType;


//BACnetCalendarEntry Structure
typedef struct BACnetCalendarEntry
{
	BACnetCalendarEntryType choice;
	BACNET_DATE date;
	BACnetDateRange dateRange;
	BACnetWeekNDay weekNDay;
}BACnetCalendarEntry;


bool Calendar_Valid_Instance(uint32_t object_instance);
unsigned Calendar_Count(void);
uint32_t Calendar_Index_To_Instance(unsigned index);
unsigned Calendar_Instance_To_Index(uint32_t object_instance);

int Calendar_Encode_Property_APDU(
    uint8_t * apdu,
    uint32_t object_instance,
    BACNET_PROPERTY_ID property,
    uint32_t array_index,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code);

bool Calendar_Write_Property(BACNET_WRITE_PROPERTY_DATA * wp_data);

#endif
