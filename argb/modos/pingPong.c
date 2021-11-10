#include "pingPong.h"
#include "../stripControl.h"

static uint16_t i;
static uint16_t dot1;
static uint16_t dot2;


void prepPingPong()
{
    dot1 = 0;
    dot2 = STRIP_SIZE - 1;
    setStripBrightness(0);
    for(i = 0; i < STRIP_SIZE; i++)
    {
        if( i < (STRIP_SIZE >> 1))
        {
            setLedColor_L(i, getColor(PRIMARY));
        }
        else
        {
            setLedColor_L(i, getColor(SECONDARY));
        }
    }

    setLedBrightness(dot1, 200);
    setLedBrightness(dot2, 200);

}

void updatePingPong()
{
    dot1++;
    dot2--;
    if(dot1 > dot2)
    {
        struct led color1 = getColor(PRIMARY);
        struct led color2 = getColor(SECONDARY);
        struct led colorMix;

        colorMix.red = ((uint16_t)color1.red + color2.red) >> 1;
        colorMix.green = ((uint16_t)color1.green + color2.green) >> 1;
        colorMix.blue = ((uint16_t)color1.blue + color2.blue) >> 1;

        setStripColor_L(colorMix);
    }
    setLedBrightness(dot1, 200);
    setLedBrightness(dot2, 200);

    if(dot1 >= STRIP_SIZE || dot2 <= 0)
    {
        dot1 = 0;
        dot2 = STRIP_SIZE - 1;
        for(i = 0; i < STRIP_SIZE; i++)
        {
            if( i < (STRIP_SIZE >> 1))
            {
                setLedColor_L(i, getColor(PRIMARY));
            }
            else
            {
                setLedColor_L(i, getColor(SECONDARY));
            }
        }
    }
    dimmStrip(30);
}

void miscPingPong()
{

}
