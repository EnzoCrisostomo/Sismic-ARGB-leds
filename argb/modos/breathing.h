#ifndef ARGB_MODOS_BREATHING_H_
#define ARGB_MODOS_BREATHING_H_

#include <inttypes.h>

void setBreathingTopLimit(uint8_t value);
void setBreathingBottomLimit(uint8_t value);
void setBreathingSpeed(uint8_t value);

void prepBreathing();
void updateBreathing();
void changeBreathingColor();

#endif /* ARGB_MODOS_BREATHING_H_ */
