#ifndef ARGB_MODOS_H_
#define ARGB_MODOS_H_

#include "modos/singleColor.h"
#include "modos/colorCycle.h"
#include "modos/rainbow.h"
#include "modos/breathing.h"
#include "modos/pulse.h"
#include "modos/pingPong.h"

enum modos
{
    OFF = 0,
    SINGLE_COLOR,
    COLOR_CYCLE,
    RAINBOW,
    BREATHING,
    PULSE,
    PING_PONG,

    MODE_COUNT
};

void setup();
void update();
void refresh();
void secondaryButton();

void off();
void single_color();
void color_cycle();
void rainbow();
void breathing();
void pulse();
void ping_pong();

#endif /* ARGB_MODOS_H_ */
