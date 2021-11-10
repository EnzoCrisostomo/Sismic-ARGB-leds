#ifndef MSPBUTTONS_H
#define MSPBUTTONS_H

#include <inttypes.h>

#define SOLTO 0         //Botão solto
#define PRESSIONADO 1   //Botão acaba de ser pressionado
#define SEGURANDO 2     //Botão está sendo segurado

int16_t mon_s1(void);
int16_t mon_s2(void);

void setupMSPbuttons(uint16_t debounceAmt);

#endif //MSPBUTTONS_H
