#include "breathing.h"
#include "../stripControl.h"

static enum breathing
{
    DIMINUINDO = 0,
    AUMENTANDO,
    DIMINUINDO_BACKGROUND,
    AUMENTANDO_BACKGROUND,

    BREATHING_STATE_COUNT
} estado;

static int16_t brilho;

//user controllable
static uint8_t topLimit = 160;
static uint8_t bottomLimit = 5;
static uint8_t stepAmount = 1;

void setBreathingTopLimit(uint8_t value)
{
    topLimit = value;
}

void setBreathingBottomLimit(uint8_t value)
{
    bottomLimit = value;
}

void setBreathingSpeed(uint8_t value)
{
    stepAmount = value;
}

void prepBreathing()
{
    brilho = topLimit;
    setStripColor_L(getColor(PRIMARY));
    setStripBrightness(brilho);
    estado = DIMINUINDO;
}

void updateBreathing()
{
    switch(estado)
    {
    case DIMINUINDO:
    {
        brilho -= stepAmount;
        if(brilho < bottomLimit)
        {
           brilho = bottomLimit + 1;
           estado = AUMENTANDO;
        }
        setStripBrightness(brilho);
        break;
    }
    case AUMENTANDO:
    {
        brilho += stepAmount;
        if(brilho > topLimit)
        {
           brilho = topLimit - 1;
           estado = DIMINUINDO;
        }
        setStripBrightness(brilho);
        break;
    }
    case DIMINUINDO_BACKGROUND:
    {
        break;
    }
    case AUMENTANDO_BACKGROUND:
    {
        break;
    }
    default:
        estado = DIMINUINDO;
        break;
    }
}

void changeBreathingColor()
{
    setStripColor_L(getColor(PRIMARY));
}
