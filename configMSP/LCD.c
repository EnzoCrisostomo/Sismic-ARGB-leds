#include <msp430.h>
#include "LCD.h"

//P4SEL |=  BIT1 | BIT2;  // Use dedicated module

// Imprimir uma letra no LCD (x = abcd efgh)
void lcd_char(char x){
    char lsn,msn;   //nibbles
    lsn=(x<<4)&0xF0;        //lsn efgh 0000
    msn=x&0xF0;             //msn abcd 0000
    pcf_write(msn | 0x9);
    pcf_write(msn | 0xD);
    pcf_write(msn | 0x9);
    ;
    pcf_write(lsn | 0x9);
    pcf_write(lsn | 0xD);
    pcf_write(lsn | 0x9);
}

// Posicionar cursor
void lcd_cursor(char x){
    lcd_cmdo(0x80 | x);
}

// Enviar um comando (RS=0) para o LCD (x = abcd efgh)
void lcd_cmdo(char x){
    char lsn,msn;   //nibbles
    lsn=(x<<4)&0xF0;        //lsn efgh 0000
    msn=x&0xF0;             //msn abcd 0000
    pcf_write(msn | 0x8);
    pcf_write(msn | 0xC);
    pcf_write(msn | 0x8);
    ;
    pcf_write(lsn | 0x8);
    pcf_write(lsn | 0xC);
    pcf_write(lsn | 0x8);
}

// Mapeia no caracter "x" o vetor vt[]
// Reposiciona o cursor em lin=0 e col=0
void lcd_esp(char x, char *vt){
    unsigned int adr,i;
    adr = x<<3;
    lcd_cmdo(BIT6 | adr);
    for (i=0; i<8; i++)
        lcd_char(vt[i]);
    lcd_cursor(0);
}

// Incializar LCD modo 4 bits
void lcd_inic(void){

    // Preparar I2C para operar
    UCB1I2CSA = PCF_ADR;    //Endereço Escravo
    UCB1CTL1 |= UCTR    |   //Mestre TX
                UCTXSTT;    //Gerar START
    while ( (UCB1IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB1TXBUF = 0;                              //Saída PCF = 0;
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //Esperar STT=0
    if ( (UCB1IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
                while(1);

    // Começar inicialização
    lcd_aux(0);     //RS=RW=0, BL=1
    delay(20000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(2);     //2

    // Entrou em modo 4 bits
    lcd_aux(2);     lcd_aux(8);     //0x28
    lcd_aux(0);     lcd_aux(8);     //0x08
    lcd_aux(0);     lcd_aux(1);     //0x01
    lcd_aux(0);     lcd_aux(6);     //0x06
    lcd_aux(0);     lcd_aux(0xF);   //0x0F

    while ( (UCB1IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB1CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    delay(50);
}

// Auxiliar inicialização do LCD (RS=RW=0)
// *** Só serve para a inicialização ***
void lcd_aux(char dado){
    while ( (UCB1IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB1TXBUF = ((dado<<4)&0XF0) | BIT3;            //PCF7:4 = dado;
    delay(50);
    while ( (UCB1IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB1TXBUF = ((dado<<4)&0XF0) | BIT3 | BIT2;     //E=1
    delay(50);
    while ( (UCB1IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB1TXBUF = ((dado<<4)&0XF0) | BIT3;            //E=0;
}

// Ler a porta do PCF
int pcf_read(void){
    int dado;
    UCB1I2CSA = PCF_ADR;                //Endereço Escravo
    UCB1CTL1 &= ~UCTR;                  //Mestre RX
    UCB1CTL1 |= UCTXSTT;                //Gerar START
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);
    UCB1CTL1 |= UCTXSTP;                //Gerar STOP + NACK
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    while ( (UCB1IFG & UCRXIFG) == 0);  //Esperar RX
    dado=UCB1RXBUF;
    return dado;
}

// Escrever dado na porta
void pcf_write(char dado){
    UCB1I2CSA = PCF_ADR;        //Endereço Escravo
    UCB1CTL1 |= UCTR    |       //Mestre TX
                UCTXSTT;        //Gerar START
    while ( (UCB1IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB1TXBUF = dado;                              //Escrever dado
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT)   ;   //Esperar STT=0
    if ( (UCB1IFG & UCNACKIFG) == UCNACKIFG)       //NACK?
                while(1);                          //Escravo gerou NACK
    UCB1CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
}

// Testar endereço I2C
// TRUE se recebeu ACK
int pcf_teste(char adr){
    UCB1I2CSA = adr;                            //Endereço do PCF
    UCB1CTL1 |= UCTR | UCTXSTT;                 //Gerar START, Mestre transmissor
    while ( (UCB1IFG & UCTXIFG) == 0);          //Esperar pelo START
    UCB1CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP);   //Esperar pelo STOP
    if ((UCB1IFG & UCNACKIFG) == 0)     return TRUE;
    else                                return FALSE;
}

// Configurar UCSB0 e Pinos I2C
// P3.0 = SDA e P3.1=SCL
void lcd_i2c_config(void){
    UCB1CTL1 |= UCSWRST;    // UCSI B0 em ressete
    UCB1CTL0 = UCSYNC |     //Síncrono
               UCMODE_3 |   //Modo I2C
               UCMST;       //Mestre
    UCB1BRW = BR_50K;      //50 kbps
    P4SEL |=  BIT1 | BIT2;  // Use dedicated module
    UCB1CTL1 = UCSSEL__ACLK;    //ACLK e remove ressete
}

void led_vd(void)   {P4OUT &= ~BIT7;}   //Apagar verde
void led_VD(void)   {P4OUT |=  BIT7;}   //Acender verde
void led_vm(void)   {P1OUT &= ~BIT0;}   //Apagar vermelho
void led_VM(void)   {P1OUT |=  BIT0;}   //Acender vermelho

// Configurar leds
void lcd_gpio_config(void){
    P1DIR |=  BIT0;      //Led vermelho
    P1OUT &= ~BIT0;      //Vermelho Apagado
    P4DIR |=  BIT7;      //Led verde
    P4OUT &= ~BIT7;      //Verde Apagado
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}

//Escrever uma string
void lcd_str(char *vt){
    char i=0;
    while ( vt[i] != '\0'){
        lcd_char(vt[i]);
        i++;
    }
}
