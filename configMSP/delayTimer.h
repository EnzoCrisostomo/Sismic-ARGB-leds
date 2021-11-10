#ifndef CONFIGMSP_DELAYTIMER_H_
#define CONFIGMSP_DELAYTIMER_H_

#include <inttypes.h>

//USGING TIMER A1 WITH ACLK = 1.000.000Hz
void delay_timer_us(uint16_t microseconds);
void delay_timer_ms(uint16_t milliseconds);
void delay_timer_s(uint16_t seconds);


#endif /* CONFIGMSP_DELAYTIMER_H_ */
