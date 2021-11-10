#include "colorCycle.h"
#include "../stripControl.h"

#define SPEED_COLOR_CYCLE 5

static struct led color;
static int8_t redInc = SPEED_COLOR_CYCLE;
static int8_t greenInc = 0;
static int8_t blueInc = 0;

//userModifiable
static uint8_t speed = SPEED_COLOR_CYCLE;
static uint8_t direction;

struct led stepColorUpColorCycle(int16_t red, int16_t green, int16_t blue);

void prepColorCycle()
{
    color = getColor(PRIMARY);
    setStripColor_L(color);
    setStripBrightness(200);
}

void updateColorCycle()
{
    color = stepColorUpColorCycle(color.red, color.green, color.blue);
    setStripColor_L(color);
}

void changeSpeedColorCycle(uint8_t value)
{
    if(value > 50)
        value = 50;
    speed = value;
}

void incSpeedColorCycle()
{
    speed++;
    if(speed > 10)
        speed = 1;
}

struct led stepColorUpColorCycle(int16_t red, int16_t green, int16_t blue)
{
    red   += redInc;
    green += greenInc;
    blue  += blueInc;

    if(red >= 255 && green <= 0 && blue <= 0)
    {
        green -= blue;
        blue = 0;

        redInc = 0;
        greenInc = speed;
        blueInc = 0;
    }
    else if(red >= 255 && green >= 255 && blue <= 0)
    {
        red -= green - 255;
        green = 255;

        redInc = -speed;
        greenInc = 0;
        blueInc = 0;
    }
    else if(red <= 0 && green >= 255 && blue <= 0)
    {
        blue -= red;
        red = 0;

        redInc = 0;
        greenInc = 0;
        blueInc = speed;
    }
    else if(red <= 0 && green >= 255 && blue >= 255)
    {
        green -= blue - 255;
        blue = 255;

        redInc = 0;
        greenInc = -speed;
        blueInc = 0;
    }
    else if(red <= 0 && green <= 0 && blue >= 255)
    {
        red -= green;
        green = 0;

        redInc = speed;
        greenInc = 0;
        blueInc = 0;
    }
    else if(red >= 255 && green <= 0 && blue >= 255)
    {
        blue -= red - 255;
        red = 255;

        redInc = 0;
        greenInc = 0;
        blueInc = -speed;
    }

    //Consertar erro
    if(red > 0 && green > 0 && blue > 0)
    {
        redInc = speed;
        greenInc = 0;
        blueInc = 0;

        red = 0;
        green = 255;
        blue = 0;
    }

    struct led temp;
    temp.red = red;
    temp.green = green;
    temp.blue = blue;
    return temp;
}
