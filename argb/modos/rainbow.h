#ifndef ARGB_MODOS_RAINBOW_H_
#define ARGB_MODOS_RAINBOW_H_

#include <inttypes.h>
void prepRainbow();
void updateRainbow();

void toggleRainbowDirection();
void changeRainbowDirection(uint8_t dir);
void changeRainbowSpeed(uint8_t value);

#endif /* ARGB_MODOS_RAINBOW_H_ */
