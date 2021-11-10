#include "../stripControl.h"

#define amt 8

static uint16_t i;
static int8_t redInc = amt;
static int8_t greenInc = 0;
static int8_t blueInc = 0;
static int8_t lastDir = 0;

//userModifiable
static int8_t speed = amt;
static uint8_t direction = 0;

void stepRainbowUp();
void stepRainbowDown();
struct led stepColorUpRainbow(int16_t red, int16_t green, int16_t blue);

void changeRainbowDirection(uint8_t dir)
{
    if(dir)
        direction = 1;
    else
        direction = 0;
}

void changeRainbowSpeed(uint8_t value)
{
    speed = value;
    if(speed > 50)
        speed = 50;
}

void toggleRainbowDirection()
{
    if(direction)
        direction = 0;
    else
        direction = 1;
}

void prepRainbow()
{
    setStripBrightness(200);
    redInc = speed;
    greenInc = 0;
    blueInc = 0;

    struct led color;
    color.red = 0;
    color.green = 255;
    color.blue = 0;

    lastDir = 1;
    direction = 0;

    for(i = 0; i < STRIP_SIZE; i++)
    {
        color = stepColorUpRainbow(color.red, color.green, color.blue);
        setLedColor_L(i, color);
    }
}

void updateRainbow()
{
    if(direction)
        stepRainbowUp();
    else
        stepRainbowDown();
}

void stepRainbowUp()
{
    struct led newColor = getLed(0);
    stepStripUp();

    int16_t red = newColor.red;
    int16_t green = newColor.green;
    int16_t blue = newColor.blue;

    if(lastDir == 0)
    {
        redInc = 0;
        greenInc = 0;
        blueInc = 0;
        if(red != 255 && red != 0)
            if(green == 0)
                redInc = -speed;
            else
                redInc = speed;

        else if(green != 255 && green != 0)
            if(red == 0)
                greenInc = speed;
            else
                greenInc = -speed;

        else if(blue != 255 && blue != 0)
            if(green == 0)
                blueInc = speed;
            else
                blueInc = -speed;

        lastDir = 1;
    }
    red += redInc;
    green += greenInc;
    blue += blueInc;

    if(red >= 255 && green <= 0 && blue <= 0)
    {
        blue -= green;
        green = 0;

        redInc = 0;
        greenInc = 0;
        blueInc = speed;
    }
    else if(red >= 255 && green >= 255 && blue <= 0)
    {
        green -= red - 255;
        red = 255;

        redInc = 0;
        greenInc = -speed;
        blueInc = 0;
    }
    else if(red <= 0 && green >= 255 && blue <= 0)
    {
        red -= blue;
        blue = 0;

        redInc = speed;
        greenInc = 0;
        blueInc = 0;
    }
    else if(red <= 0 && green >= 255 && blue >= 255)
    {
        blue -= green - 255;
        green = 255;

        redInc = 0;
        greenInc = 0;
        blueInc = -speed;
    }
    else if(red <= 0 && green <= 0 && blue >= 255)
    {
        green -= red;
        red = 0;

        redInc = 0;
        greenInc = speed;
        blueInc = 0;
    }
    else if(red >= 255 && green <= 0 && blue >= 255)
    {
        red -= blue - 255;
        blue = 255;

        redInc = -speed;
        greenInc = 0;
        blueInc = 0;
    }

    setLedColor(0, red, green, blue);
}

void stepRainbowDown()
{
    struct led newColor = getLed(STRIP_SIZE - 1);
    stepStripDown();

    int16_t red = newColor.red;
    int16_t green = newColor.green;
    int16_t blue = newColor.blue;

    if(lastDir == 1)
    {
        redInc = 0;
        greenInc = 0;
        blueInc = 0;
        if(red != 255 && red != 0)
            if(green == 0)
                redInc = speed;
            else
                redInc = -speed;

        else if(green != 255 && green != 0)
            if(red == 0)
                greenInc = -speed;
            else
                greenInc = speed;

        else if(blue != 255 && blue != 0)
            if(green == 0)
                blueInc = -speed;
            else
                blueInc = speed;

        lastDir = 0;
    }

    red += redInc;
    green += greenInc;
    blue += blueInc;

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

    setLedColor(STRIP_SIZE - 1, red, green, blue);
}

struct led stepColorUpRainbow(int16_t red, int16_t green, int16_t blue)
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
