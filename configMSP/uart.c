#include "uart.h"
#include <msp430.h>

static uint16_t i;
static uint16_t posComando = 0;
static uint8_t finalComando = 0;
static char comando[COMMAND_MAX_SIZE];

#pragma vector = USCI_A1_VECTOR
__interrupt void usci_A1_int(void)
{
    UCA1IV;
    char charRecebido = UCA1RXBUF;
    UCA1TXBUF = charRecebido;
    if(charRecebido == '\x7F')
    {
        comando[posComando] = 0;
        if(posComando)
            posComando--;
    }
    else
    {
        comando[posComando] = charRecebido;
        posComando++;
    }

    if(posComando > COMMAND_MAX_SIZE - 2 || charRecebido == '\r')
    {
        posComando = 0;
        finalComando = 1;
        comando[COMMAND_MAX_SIZE-1] = '\0';
        while( (UCA1IFG & UCTXIFG) == 0);
        UCA1TXBUF = '\n';
    }
}

uint8_t strcmpr(const char* str1, const char* str2)
{
    uint16_t i = 0;
    while (1)
    {
        if (str1[i] != str2[i])
            return 0;
        else if (str1[i] == '\0' && str2[i] == '\0')
            return 1;
        i++;
    }
}

uint8_t is_number(char* str)
{
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

void getArg(uint16_t index, char* str, char* newStr)
{
    static const char lowerLimit = ' ';
    static const char upperLimit = 127;
    uint8_t cont = 0;
    uint16_t pos = 0;


    while (str[pos] <= lowerLimit || str[pos] >= upperLimit)
        pos++;

    while (cont < index && pos < COMMAND_MAX_SIZE)
    {
        if (str[pos] <= lowerLimit || str[pos] >= upperLimit)
        {
            cont++;
            while (str[pos] <= lowerLimit || str[pos] >= upperLimit)
                pos++;
        }
        else
            pos++;
    }
    while (str[pos] <= lowerLimit || str[pos] >= upperLimit)
        pos++;

    uint16_t temp = 0;
    while (!(str[pos] <= lowerLimit || str[pos] >= upperLimit) && pos < COMMAND_MAX_SIZE)
    {
        newStr[temp] = str[pos];
        temp++;
        pos++;
    }
    newStr[temp] = '\0';
}

uint8_t str_to_uint8_t(char* str)
{
    uint8_t value = 0;
    i = 0;
    while(str[i] != '\0')
    {
        value *= 10;
        value += str[i] - '0';
        i++;
    }
    return value;
}

uint16_t str_to_uint16_t(char* str)
{
    uint16_t value = 0;
    i = 0;
    while(str[i] != '\0')
    {
        value *= 10;
        value += str[i] - '0';
        i++;
    }
    return value;
}

char* ler_comando_uart()
{
    if(finalComando)
    {
        finalComando = 0;
        return comando;
    }
    else
        return 0;
}

void uart_char(char lt)
{
    while( (UCA1IFG & UCTXIFG) == 0);
    UCA1TXBUF = lt;
}

void uart_str(char* str)
{
    uint16_t i = 0;
    while(str[i] != '\0')
    {
        uart_char(str[i]);
        i++;
    }
}

void uart_uint8_t(uint8_t num)
{
    uart_char('0' + (num % (1000)) / 100);
    uart_char('0' + (num % (100)) / 10);
    uart_char('0' + (num % (10)) / 1);
}

void uart_uint16_t(uint16_t num)
{
    uart_char('0' + (num % (100000)) / 10000);
    uart_char('0' + (num % (10000)) / 1000);
    uart_char('0' + (num % (1000)) / 100);
    uart_char('0' + (num % (100)) / 10);
    uart_char('0' + (num % (10)) / 1);
}

void uart_int8_t(int8_t num)
{
    if(num < 0)
    {
        uart_char('-');
        num = -num;
    }
    else
        uart_char(' ');

    uart_char('0' + (num % (1000)) / 100);
    uart_char('0' + (num % (100)) / 10);
    uart_char('0' + (num % (10)) / 1);
}

void uart_int16_t(int16_t num)
{
    if(num < 0)
    {
        uart_char('-');
        num = -num;
    }
    else
        uart_char(' ');

    uart_char('0' + (num % (100000)) / 10000);
    uart_char('0' + (num % (10000)) / 1000);
    uart_char('0' + (num % (1000)) / 100);
    uart_char('0' + (num % (100)) / 10);
    uart_char('0' + (num % (10)) / 1);
}

void uart_config()
{
    P4SEL |= BIT4 | BIT5;
    UCA1CTL1 = UCSWRST; //ativar reset
    UCA1CTL0 = 0;
    //ACLK = 1.000.000Hz
    //57600 BRCLK=1000000 BRW=17, BRS=3 , BRF=0 e UCOS16=0
    UCA1BRW = 17;
    UCA1MCTL= UCBRS_3 | UCBRF_0;

    //desativar reset
    UCA1CTL1 = UCSSEL__ACLK;
    //Habilitar interrupções
    UCA1IE = UCRXIE;
}
