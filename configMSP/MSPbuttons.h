#ifndef MSPBUTTONS_H
#define MSPBUTTONS_H

#include <inttypes.h>

#define SOLTO 0         //Bot�o solto
#define PRESSIONADO 1   //Bot�o acaba de ser pressionado
#define SEGURANDO 2     //Bot�o est� sendo segurado

int16_t mon_s1(void);
int16_t mon_s2(void);

void setupMSPbuttons(uint16_t debounceAmt);

#endif //MSPBUTTONS_H
