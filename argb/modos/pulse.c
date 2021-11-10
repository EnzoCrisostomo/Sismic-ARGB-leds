#include "pulse.h"
#include "../stripControl.h"

static uint16_t i;
static uint16_t dotPos[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//user modifiable
static uint8_t dimmAmount = 30;
static uint8_t dotQuantity = 2;
static uint16_t offset = 0;

void setPoints(uint8_t value)
{
    if(value > 10)
        value = 10;
    dotQuantity = value;
}

void setOffset(uint8_t value)
{
    offset = value;
}

void setDecay(uint8_t value)
{
    dimmAmount = 255 - value;
}

void prepPulse()
{
    if(dotQuantity > 10)
        dotQuantity = 10;
    setStripBrightness(0);
    setStripColor_L(getColor(PRIMARY));
    uint16_t dist = STRIP_SIZE / dotQuantity;
    for(i = 0; i < dotQuantity; i++)
    {
        dotPos[i] = i * dist;
        setLedBrightness(dotPos[i], 200);
    }
}

void updatePulse()
{
    dimmStrip(dimmAmount);

    for(i = 0; i < dotQuantity; i++)
    {
        dotPos[i]++;
        if(dotPos[i] < STRIP_SIZE + offset)
        {
            if(dotPos[i] < STRIP_SIZE)
            {
                setLedBrightness(dotPos[i], 200);
            }
        }
        else
            dotPos[i] = 0;
    }
}

void addDotPulse()
{
    dotQuantity++;
    if(dotQuantity >= 10)
        dotQuantity = 1;
    prepPulse();
}
