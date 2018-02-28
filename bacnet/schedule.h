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
#ifndef SCHEDUEL_H
#define SCHEDUEL_H

#include <stdbool.h>
#include <stdint.h>
#include "bacdef.h"
#include "rp.h"
#include "wp.h"
#include "bacdevobjpropref.h"
#include "bactimevalue.h"



#ifndef BACNET_WEEKLY_SCHEDULE_SIZE
#define BACNET_WEEKLY_SCHEDULE_SIZE 8   /* maximum number of data points for each day */
#endif

#ifndef BACNET_SCHEDULE_OBJ_PROP_REF_SIZE
#define BACNET_SCHEDULE_OBJ_PROP_REF_SIZE 4     /* maximum number of obj prop references */
#endif



//    typedef struct bacnet_daily_schedule {
//        BACNET_TIME_VALUE Time_Values[BACNET_WEEKLY_SCHEDULE_SIZE];
//        uint16_t TV_Count;      /* the number of time values actually used */
//    } BACNET_DAILY_SCHEDULE;

//    typedef struct schedule {
//        /* Effective Period: Start and End Date */
//        BACNET_DATE Start_Date;
//        BACNET_DATE End_Date;
//        /* Properties concerning Present Value */
//        BACNET_DAILY_SCHEDULE Weekly_Schedule[7];
//			  bool Schedule_Default;
//			  bool Present_Value;
////        BACNET_APPLICATION_DATA_VALUE Schedule_Default;
////        BACNET_APPLICATION_DATA_VALUE *Present_Value;   /* must be set to a valid value
////                                                         * default is Schedule_Default */
//        BACNET_DEVICE_OBJECT_PROPERTY_REFERENCE
//        Object_Property_References[BACNET_SCHEDULE_OBJ_PROP_REF_SIZE];
//        uint8_t obj_prop_ref_cnt;       /* actual number of obj_prop references */
//        uint8_t Priority_For_Writing;   /* (1..16) */
//        bool Out_Of_Service;
//    } SCHEDULE_DESCR;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    void Schedule_Property_Lists(
        const int **pRequired,
        const int **pOptional,
        const int **pProprietary);

    bool Schedule_Valid_Instance(
        uint32_t object_instance);
    unsigned Schedule_Count(
        void);
    uint32_t Schedule_Index_To_Instance(
        unsigned index);
    unsigned Schedule_Instance_To_Index(
        uint32_t instance);
    bool Schedule_Object_Instance_Add(
        uint32_t instance);

    char *Schedule_Name(
        uint32_t object_instance);
    bool Schedule_Name_Set(
        uint32_t object_instance,
        char *new_name);

    char *Schedule_Description(
        uint32_t instance);
    bool Schedule_Description_Set(
        uint32_t instance,
        char *new_name);

    bool Schedule_Units_Set(
        uint32_t instance,
        uint16_t units);
    uint16_t Schedule_Units(
        uint32_t instance);

    int Schedule_Read_Property(
        BACNET_READ_PROPERTY_DATA * rpdata);
    bool Schedule_Write_Property(
        BACNET_WRITE_PROPERTY_DATA * wp_data);


    void Schedule_Init(
        void);

	int Schedule_Encode_Property_APDU(
    uint8_t * apdu,
    uint32_t object_instance,
    BACNET_PROPERTY_ID property,
    uint32_t array_index,
    BACNET_ERROR_CLASS * error_class,
    BACNET_ERROR_CODE * error_code);



#ifdef __cplusplus
}
#endif /* __cplusplus */
#define ANALOG_INPUT_OBJ_FUNCTIONS \
    OBJECT_ANALOG_INPUT, Analog_Input_Init, Analog_Input_Count, \
    Analog_Input_Index_To_Instance, Analog_Input_Valid_Instance, \
    Analog_Input_Name, Analog_Input_Read_Property, NULL, \
    Analog_Input_Property_Lists, NULL, NULL
#endif
