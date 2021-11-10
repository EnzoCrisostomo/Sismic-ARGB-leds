#include <msp430.h>
#include "stripControl.h"
#include "../configMSP/delayTimer.h"

static struct led ledArray[STRIP_SIZE];
static uint8_t brightnessArray[STRIP_SIZE];
static struct led colors[COLOR_COUNT];
static uint16_t i;

static uint8_t readyToUpdate = 0;

void sendColor(const uint8_t color);

void turnOffLeds()
{
    setStripColor(0, 0, 0);
}

void setup_strip_update_timer(uint16_t speed)
{
    if(speed > 100)
        speed = 100;

    speed *= 290;

    TA0CTL = TASSEL__ACLK | MC__UP | ID__4 | TACLR;
    TA0CCR0 = 30000 - speed - 1;
    TA0CCTL0 = CCIE;
}

void updateLeds()
{
    while(readyToUpdate == 0);
    readyToUpdate = 0;
    __disable_interrupt();
    for(i = 0; i < STRIP_SIZE; i++) {
        struct led* myLed = &ledArray[i];
        uint16_t brightness = brightnessArray[i];
        sendColor(((myLed->green) * (brightness + 1)) >> 8);
        sendColor(((myLed->red) * (brightness + 1)) >> 8);
        sendColor(((myLed->blue) * (brightness + 1)) >> 8);

    }
    __enable_interrupt();
    __delay_cycles(1000);
}

void sendColor(const uint8_t color)
{
    uint8_t curBit = 0x80;;
    uint8_t out;
    while(curBit)
    {
        if(curBit & color)
            out = high;
        else
            out = low;

        while(!(UCB0IFG & UCTXIFG));

        UCB0TXBUF = out;
        curBit = curBit >> 1;
    }
}

void setLedColor(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
    struct led* myLed = &ledArray[index];
    myLed->green = green;
    myLed->red = red;
    myLed->blue = blue;
}

void setLedColor_L(uint16_t index, struct led color)
{
    ledArray[index] = color;
}

void setStripColor(uint8_t red, uint8_t green, uint8_t blue)
{
    for(i = 0; i < STRIP_SIZE; i++)
    {
        ledArray[i].green = green;
        ledArray[i].red = red;
        ledArray[i].blue = blue;
    }
}

void setStripColor_L(struct led color)
{
    for(i = 0; i < STRIP_SIZE; i++)
    {
        ledArray[i] = color;
    }
}

void setLedBrightness(uint16_t index, uint8_t value)
{
    brightnessArray[index] = value;
}

void setStripBrightness(uint8_t value)
{
    for(i = 0; i < STRIP_SIZE; i++)
        brightnessArray[i] = value;
}

void dimmStrip(uint8_t value)
{
    for(i = 0; i < STRIP_SIZE; i++)
    {
        int16_t temp = brightnessArray[i];
        temp -= value;
        if(temp < 0)
            temp = 0;
        brightnessArray[i] = temp;
    }
}

struct led getLed(uint16_t index)
{
    return ledArray[index];
}

struct led getColor(uint16_t index)
{
    return colors[index];
}

struct led* getAllColors()
{
    return colors;
}

void setColor(enum colorClasses index, uint8_t red, uint8_t green, uint8_t blue)
{
    index = index % COLOR_COUNT;
    colors[index].green = green;
    colors[index].red = red;
    colors[index].blue = blue;
}

void setColor_L(enum colorClasses index, struct led color)
{
    index = index % COLOR_COUNT;
    colors[index] = color;
}

void stepStripUp()
{
    i = STRIP_SIZE;
    while(1)
    {
        i--;
        ledArray[i] = ledArray[i - 1];
        if(i <= 1)
            break;
    }
}

void stepStripDown()
{
    for(i = 0; i < STRIP_SIZE - 1; i++)
    {
        ledArray[i] = ledArray[i + 1];
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void ta0(void){
    readyToUpdate = 1;
}
