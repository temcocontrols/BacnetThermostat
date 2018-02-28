

#define	DisplaySTACK_SIZE	512

xTaskHandle Handle_Display;

extern xQueueHandle qKey;

void DisplayTask(void)
{
	U8_T temp;
//	static U8_T i = 0;
	Lcd_Initial();
	Lcd_Show_String(0, 0, 0, "Welcome...");

	while(1)
	{
//		if(cQueueReceive(qKey, &temp, 20) == pdTRUE)
//		{
//			if(temp != KEY_NON)
//				Lcd_Full_Screen(0);
//
//			switch(temp)
//			{
//				case KEY_UP_MASK:
//					Lcd_Show_String((i++) % 5, 0, 0, "KEY_UP");
//					break;
//				case KEY_DOWN_MASK:
//					Lcd_Show_String((i++) % 5, 0, 0, "KEY_DOWN");
//					break;
//				case KEY_LEFT_MASK:
//					Lcd_Show_String((i++) % 5, 0, 0, "KEY_LEFT");
//					break;
//				case KEY_RIGHT_MASK:
//					Lcd_Show_String((i++) % 5, 0, 0, "KEY_RIGHT");
//					break;
//			}
//		}
		
//		if(cQueueReceive(qKey, &temp, 20) == pdTRUE)
//		{
//			if(temp != KEY_NON)
//			{
//				
//			}
//		}
//		else
//		taskYIELD();		
	}
}

void vStartDisplayTask(unsigned char uxPriority)
{
	sTaskCreate(DisplayTask, (const signed portCHAR * const)"DisplayTask", DisplaySTACK_SIZE, NULL, uxPriority, (xTaskHandle *)&Handle_Display);
}