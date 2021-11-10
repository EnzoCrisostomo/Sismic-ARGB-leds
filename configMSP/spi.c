#include <msp430.h>
#include "spi.h"

void spi_config()
{
    __bis_SR_register(GIE);                                                 //Set special assembly register that enables maskable interrupts
    UCB0CTL1 = UCSWRST;                                                     //Put UCSI in reset state for configuration
    UCB0CTL0 |= (UCSYNC + UCMST + UCMSB + UCCKPH);                          //Enable settings: SPI mode (insetad of UART) + Master + Most sig byte + UCCKPH
    P3SEL |= BIT0;                                                          //Port select
    P3DIR |= BIT0;
    UCB0CTL1 |= UCSSEL1;                                                    //Disable UCSI reset state and select SMCLK src
    UCB0CTL1 &= ~UCSWRST;
}
