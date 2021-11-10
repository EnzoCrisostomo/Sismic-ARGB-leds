#include <msp430.h>
#include "delayTimer.h"
//  USGING TIMER A1 WITH ACLK = 1.000.000Hz

void delay_timer_us(uint16_t microseconds)
{
    //ACLK = 1.000.000Hz
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;
    TA1CCR0 = microseconds - 1;
    while((TA1CTL&TAIFG) == 0);
    TA1CTL = MC__STOP;
}

void delay_timer_ms(uint16_t milliseconds)
{
    //ACLK = 1.000.000Hz
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR;
    TA1CCR0 = 999;
    uint16_t cont = 0;
    while(cont < milliseconds)
    {
        while((TA1CTL&TAIFG) == 0);
        TA1CTL &= ~TAIFG;
        cont++;
    }
    TA1CTL = MC__STOP;
}

void delay_timer_s(uint16_t seconds)
{
    //ACLK = 1.000.000Hz
    TA1CTL = TASSEL__ACLK | MC__UP | ID__4 | TACLR;
    TA1EX0 = TAIDEX_4;
    TA1CCR0 = 49999;
    uint16_t cont = 0;
    while(cont < seconds)
    {
        while((TA1CTL&TAIFG) == 0);
        TA1CTL &= ~TAIFG;
        cont++;
    }
    TA1CTL = MC__STOP;
    TA1EX0 = TAIDEX_0;
}
