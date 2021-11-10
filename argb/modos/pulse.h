#ifndef ARGB_MODOS_PULSE_H_
#define ARGB_MODOS_PULSE_H_

#include <inttypes.h>

void setPoints(uint8_t value);
void setOffset(uint8_t value);
void setDecay(uint8_t value);

void prepPulse();
void updatePulse();

void addDotPulse();


#endif /* ARGB_MODOS_PULSE_H_ */
