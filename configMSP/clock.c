/*
 * UCS - Unified Clock System Library
 * 
 * This library provides a helper for setting system clocks 
 * to a pre-defined value. The function clockInit(), will
 * connect the launchpad crystals to the UCS module and use 
 * them as reference to set MCLK @16Hz, SMCLK @4MHz and
 * ACLK @1MHz.
 */

#include <msp430.h>
#include "clock.h"

void clockInit() {

    // Configure crystal ports
    P5SEL |= BIT2 | BIT3 | BIT4 | BIT5;     // Configure P5 to use Crystals

    // This should make XT1 startup in 500ms and XT2 in less than 1ms
    UCSCTL6 = XT2DRIVE_3      |             // Turn up crystal drive to
              XT1DRIVE_3      |             // speed up oscillator startup
              XCAP_3          |             // Use maximum capacitance (12pF)
//            XT1OFF          |             // Make sure XT1 and XT2
//            XT2OFF          |             // oscillators are active
//            SMCLKOFF        |             // Leave SMCLK ON
//            XT1BYPASS       |             // Not using external clock source
//            XT2BYPASS       |             // Not using external clock source
//            XTS             |             // XT1 low-frequency mode, which
              0;                            // means XCAP bits will be used

    UCSCTL0 = 0x00;                         // Let FLL manage DCO and MOD

    UCSCTL1 = DCORSEL_5       |             // Select DCO range to around 16MHz
//            DISMOD          |             // Enable modulator
              0;

    UCSCTL2 = FLLD__1         |             // Set FLL dividers
              FLLN(4);                      // DCOCLK = 1 * 4 * FLLREF

    UCSCTL3 = SELREF__XT2CLK  |             // Use XT2 Oscillator for FLLREF
              FLLREFDIV__1;                 // divided by 1

    UCSCTL5 = DIVPA__1        |             // Output dividers to 1
               DIVA__4        |             // ACLK  divided by 4
               DIVS__2        |             // SMCLK divided by 2
               DIVM__2;                     // MCLK  divided by 1

    UCSCTL7 = 0;                            // Clear XT2,XT1,DCO fault flags

    UCSCTL8 = SMCLKREQEN      |             // Enable conditional requests for
               MCLKREQEN      |             // SMCLK, MCLK and ACLK. In case one
               ACLKREQEN;                   // fails, another takes over

    do {                                    // Check if all clocks are oscillating
      UCSCTL7 &= ~(   XT2OFFG |             // Try to clear XT2,XT1,DCO fault flags,
                    XT1LFOFFG |             // system fault flags and check if
                       DCOFFG );            // oscillators are still faulty
      SFRIFG1 &= ~OFIFG;                    //
    } while (SFRIFG1 & OFIFG);              // Exit only when everything is ok

    UCSCTL6 &= ~(XT1DRIVE_3   |             // Xtal is now stable,
                 XT2DRIVE_3);               // reduce drive strength (to save power)

    UCSCTL4 = SELA__XT2CLK    |             // ACLK  = XT2/4 =>   1.000.000 Hz
              SELS__DCOCLK    |             // SMCLK = DCO/2 =>   8.000.000 Hz
              SELM__DCOCLK;                 // MCLK  = DCO   =>  16.000.000 Hz
}

