#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "bitmap.h"
#include "stm32f10x_adc.h"


typedef union {
  uint8 BYTE;
  struct {
    uint8 b0 :1;                                       /* Port B Data Register Bit 0 */
    uint8 b1 :1;                                       /* Port B Data Register Bit 1 */
    uint8 b2 :1;                                       /* Port B Data Register Bit 2 */
    uint8 b3 :1;                                       /* Port B Data Register Bit 3 */
    uint8 b4 :1;                                       /* Port B Data Register Bit 4 */
    uint8 b5 :1;                                       /* Port B Data Register Bit 5 */
    uint8 b6 :1;                                       /* Port B Data Register Bit 6 */
    uint8 b7 :1;                                       /* Port B Data Register Bit 7 */
  } Bits;
} PTBDSTR;








extern uint16 pwm_duty;
extern uint16 OUT_PINS_1TO5;
extern uint8 dtime[7];
void output_ini(void);
void set_output (TIM_TypeDef* TIMx, u8 channel, u16 duty);
void refresh_output(void);
//extern uint8  output6_function;
//extern uint8  output7_function;

void relay_routine(uint8 relay_number);
void relay_6(uint8 state);
void relay_7(uint8 state);

#endif
