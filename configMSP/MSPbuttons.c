#include "MSPbuttons.h"
#include <msp430.h>

#define ABERTA  1       //Chave aberta
#define FECHADA 0       //Chave fechada

static int8_t chave = 1;
static int16_t timerConfig = 0;

// Monitorar S1, retorna TRUE se foi acionada
int16_t mon_s1(void){
    static int16_t ultimoEstado = ABERTA;      //Guardar passado de S1
    //Qual o estado atual de S1?
    if ( (P2IN&BIT1) == 0 && chave == 1){    //A chave está fechada?
        if (ultimoEstado == ABERTA){     // Qual o passado de S1?
            chave = 0;
            TA2CTL = timerConfig;
            ultimoEstado = FECHADA;
            return PRESSIONADO;
        }
        else
            return SEGURANDO;
    }
    else if(chave == 1){
        if (ultimoEstado == FECHADA){    //Qual o passado de S1?
            chave = 0;
            TA2CTL = timerConfig;
            ultimoEstado = ABERTA;
            return SOLTO;
        }
    }
    return SOLTO;
}

// Monitorar S2, retorna TRUE se foi acionada
int16_t mon_s2(void){
    static int16_t ultimoEstado = ABERTA;      //Guardar passado de S2
    //Qual o estado atual de S2?
    if ( (P1IN&BIT1) == 0 && chave == 1){    //A chave está fechada?
        if (ultimoEstado == ABERTA){     // Qual o passado de S2?
            chave = 0;
            TA2CTL = timerConfig;
            ultimoEstado = FECHADA;
            return PRESSIONADO;
        }
        else
            return SEGURANDO;
    }
    else if(chave == 1){
        if (ultimoEstado == FECHADA){    //Qual o passado de S2?
            chave = 0;
            TA2CTL = timerConfig;
            ultimoEstado = ABERTA;
            return SOLTO;
        }
    }
    return SOLTO;
}

void _MSPbuttons_timer2Config(uint16_t debounceAmt)
{
    //ACLK = 1MHz
    timerConfig = TASSEL__ACLK | MC__UP | ID__4 | TACLR;
    TA2CTL = MC__STOP;
    TA2CCR0 = debounceAmt;
    TA2CCTL0 = CCIE;
}

// Configurar GPIO
void _MSPbuttons_gpio_config(void){
    //Entradas
    P2DIR &= ~BIT1;     //S1 = P2.1 = entrada
    P2REN |= BIT1;      //Habilitar resistor
    P2OUT |= BIT1;      //Habilitar pullup

    P1DIR &= ~BIT1;     //S2 = P1.1 = entrada
    P1REN |= BIT1;      //Habilitar resistor
    P1OUT |= BIT1;      //Habilitar pullup
}

void setupMSPbuttons(uint16_t debounceAmt)
{
    _MSPbuttons_gpio_config();
    _MSPbuttons_timer2Config(debounceAmt);
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void ta2(void)
{
    TA2CTL = MC__STOP;
    chave = 1;
}
