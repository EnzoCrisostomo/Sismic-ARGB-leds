#include "modos.h"

#include "argb/stripControl.h"
#include "configMSP/LCD.H"

void (*prepFunction)(void) = turnOffLeds;
void (*updateFunction)(void) = 0;
void (*miscFunction)(void) = turnOffLeds;


void setup()
{
    setup_strip_update_timer(70);
    turnOffLeds();
    updateLeds();
    setStripBrightness(200);
    lcd_cursor(0x00);
    lcd_str("Modo: OFF       ");
    lcd_cursor(0x40);
    lcd_str("Leds Desligados.");

    setColor(PRIMARY, 200, 0, 0);
    setColor(SECONDARY, 0, 0, 200);
}

void refresh()
{
    if(prepFunction)
        prepFunction();

    updateLeds();
}

void update()
{
   if(updateFunction)
   {
       updateFunction();
       updateLeds();
   }
}

void secondaryButton()
{
    if(miscFunction)
        miscFunction();

    updateLeds();
}

void off()
{
    turnOffLeds();
    prepFunction = turnOffLeds;
    updateFunction = 0;
    miscFunction = turnOffLeds;
    lcd_cursor(0x00);
    lcd_str("Modo: OFF       ");
    lcd_cursor(0x40);
    lcd_str("Leds Desligados.");
    updateLeds();
}

void single_color()
{
    prepSingleColor();
    prepFunction = prepSingleColor;
    updateFunction = 0;
    miscFunction = prepSingleColor;
    lcd_cursor(0x00);
    lcd_str("Modo SingleColor");
    lcd_cursor(0x40);
    lcd_str("                ");
    updateLeds();
}

void color_cycle()
{
    prepColorCycle();
    prepFunction = prepColorCycle;
    updateFunction = updateColorCycle;
    miscFunction = incSpeedColorCycle;
    lcd_cursor(0x00);
    lcd_str("Modo ColorCycle ");
    lcd_cursor(0x40);
    lcd_str("                ");
    updateLeds();
}

void rainbow()
{
    prepRainbow();
    prepFunction = prepRainbow;
    updateFunction = updateRainbow;
    miscFunction = toggleRainbowDirection;
    lcd_cursor(0x00);
    lcd_str("Modo Rainbow    ");
    lcd_cursor(0x40);
    lcd_str("                ");
    updateLeds();
}

void breathing()
{
    prepBreathing();
    prepFunction = prepBreathing;
    miscFunction = changeBreathingColor;
    updateFunction = updateBreathing;
    lcd_cursor(0x00);
    lcd_str("Modo Breathing  ");
    lcd_cursor(0x40);
    lcd_str("                ");
    updateLeds();
}

void pulse()
{
    prepPulse();
    prepFunction = prepPulse;
    updateFunction = updatePulse;
    miscFunction = addDotPulse;
    lcd_cursor(0x00);
    lcd_str("Modo Pulse      ");
    lcd_cursor(0x40);
    lcd_str("                ");
    updateLeds();
}

void ping_pong()
{
    prepPingPong();
    prepFunction = prepPingPong;
    updateFunction = updatePingPong;
    miscFunction = prepPingPong;
    lcd_cursor(0x00);
    lcd_str("Modo Ping Pong  ");
    lcd_cursor(0x40);
    lcd_str("                ");
    updateLeds();
}
