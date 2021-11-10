#ifndef ARGB_STRIPCONTROL_H_
#define ARGB_STRIPCONTROL_H_

#include <inttypes.h>

#define STRIP_SIZE 60
#define STRIP_BIT_SIZE STRIP_SIZE*24
#define high 0xFC                    //11111000
#define low 0xE0                     //11100000

enum colorClasses
{
    PRIMARY = 0,
    SECONDARY,
    TERTIARY,
    QUATERNARY,
    QUINARY,

    COLOR_COUNT
};

struct led
{
    uint8_t green;
    uint8_t red;
    uint8_t blue;
};

void setup_strip_update_timer(uint16_t speed);

void turnOffLeds();
void updateLeds();
void setLedColor(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
void setLedColor_L(uint16_t index, struct led color);
void setStripColor(uint8_t red, uint8_t green, uint8_t blue);
void setStripColor_L(struct led color);
void setLedBrightness(uint16_t index, uint8_t value);
void setStripBrightness(uint8_t value);
void dimmStrip(uint8_t value);
void stepStripUp();
void stepStripDown();
struct led getLed(uint16_t index);

struct led getColor(enum colorClasses index);
struct led* getAllColors();
void setColor(enum colorClasses index, uint8_t red, uint8_t green, uint8_t blue);
void setColor_L(enum colorClasses index, struct led color);


#endif /* ARGB_STRIPCONTROL_H_ */
