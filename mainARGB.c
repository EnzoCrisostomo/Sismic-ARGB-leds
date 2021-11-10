//Autor: Enzo Crisóstomo Nogueira - Matrícula 190086751
//
//Fita ARGB ws2812b     DATA = P3.0
//
//LCD                    SDA = P4.1
//                       SCL = P4.2
//
//UART              BAUDRATE = 57600
//                   Console = PuTTY
//
//UCS             MCLK = 8.000.000Hz
//               SMCLK = 8.000.000Hz
//                ACLK = 1.000.000Hz


#include <msp430.h> 
#include <inttypes.h>
#include "configMSP/pmm.h"
#include "configMSP/clock.h"
#include "configMSP/LCD.h"
#include "configMSP/spi.h"
#include "configMSP/MSPbuttons.h"
#include "configMSP/delayTimer.h"
#include "configMSP/uart.h"

#include "argb/modos.h"
#include "argb/stripControl.h"


static uint16_t i;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    // Stop watchdog timer
    pmmVCore (1);
    pmmVCore (2);

    clockInit();

    spi_config();
    setupMSPbuttons(3000);
    lcd_gpio_config();
    lcd_i2c_config();

    if (pcf_teste(PCF_ADR)==FALSE){
        led_VM();           //Indicar que não houve ACK
        while(TRUE);        //Travar
    }
    else    led_VD();       //Houve ACK, tudo certo

    lcd_inic();     //Inicializar LCD
    pcf_write(8);   //Acender Back Light

    lcd_cmdo(0xC);

    uart_config();
    delay_timer_us(1000);

    __enable_interrupt();

    uart_str("=====>Controlador de Leds ARGB<=====\r\n"
             "Digite ajuda para ver os comandos disponiveis!\r\n");

    setup();

    enum modos myMode = OFF;
    uint8_t cor = 0;
    char* cmd;
    while(1) {
        cmd = ler_comando_uart();
        if (cmd)
        {
            uint8_t error = 0;
            uint8_t argCount = 0;

            char arg[COMMAND_MAX_SIZE];
            getArg(argCount, cmd, arg);
            argCount++;

            if (strcmpr(arg, "off"))
            {
                myMode = OFF;
                off();
            }
            else if (strcmpr(arg, "singlecolor"))
            {
                myMode = SINGLE_COLOR;
                single_color();
            }
            else if (strcmpr(arg, "colorcycle"))
            {
                if(myMode != COLOR_CYCLE)
                {
                    myMode = COLOR_CYCLE;
                    color_cycle();
                }
                getArg(argCount, cmd, arg);
                argCount++;
                if(strcmpr(arg, "-s"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        changeSpeedColorCycle(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
            }
            else if(strcmpr(arg, "rainbow"))
            {
                if(myMode != RAINBOW)
                {
                    myMode = RAINBOW;
                    rainbow();
                }
                getArg(argCount, cmd, arg);
                argCount++;
                if(strcmpr(arg, "-d"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        changeRainbowDirection(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
                if(strcmpr(arg, "-s"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        changeRainbowSpeed(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
            }
            else if (strcmpr(arg, "breathing"))
            {
                if(myMode != BREATHING)
                {
                    myMode = BREATHING;
                    breathing();
                }
                getArg(argCount, cmd, arg);
                argCount++;
                if(strcmpr(arg, "-s"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        setBreathingSpeed(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
                if(strcmpr(arg, "-tl"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        setBreathingTopLimit(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
                if(strcmpr(arg, "-bl"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        setBreathingBottomLimit(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
            }
            else if (strcmpr(arg, "pulse"))
            {
                if(myMode != PULSE)
                {
                    myMode = PULSE;
                    pulse();
                }
                getArg(argCount, cmd, arg);
                argCount++;
                if(strcmpr(arg, "-p"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                    {
                        setPoints(str_to_uint8_t(arg));
                        pulse();
                    }
                    else
                        error = 1;
                }
                if(strcmpr(arg, "-o"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                    {
                        setOffset(str_to_uint8_t(arg));
                        pulse();
                    }
                    else
                        error = 1;
                }
                if(strcmpr(arg, "-d"))
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        setDecay(str_to_uint8_t(arg));
                    else
                        error = 1;
                }
            }
            else if (strcmpr(arg, "pingpong"))
            {
                if(myMode != PING_PONG)
                {
                    myMode = PING_PONG;
                    ping_pong();
                }
            }
            else if (strcmpr(arg, "speed"))
            {
                getArg(argCount, cmd, arg);
                argCount++;
                if(is_number(arg))
                    setup_strip_update_timer(str_to_uint8_t(arg));
                else
                    error = 1;
            }
            else if (strcmpr(arg, "color"))
            {
                uint8_t values[4];
                for(i = 0; i < 4; i++)
                {
                    getArg(argCount, cmd, arg);
                    argCount++;
                    if(is_number(arg))
                        values[i] = str_to_uint8_t(arg);
                    else
                    {
                        error = 1;
                        break;
                    }
                }
                if(!error)
                {
                    setColor((enum colorClasses)values[0], values[1], values[2], values[3]);
                    refresh();
                }
            }
            else if (strcmpr(arg, "ajuda"))
            {
                getArg(argCount, cmd, arg);
                argCount++;
                uart_char('\n');
                uart_char('\n');
                if(strcmpr(arg, "off"))
                {
                    uart_str("=====>Off<=====\r\n"
                             "off => Desliga os leds.\r\n");
                }
                else if (strcmpr(arg, "singlecolor"))
                {
                    uart_str("=====>Single Color<=====\r\n"
                             "singlecolor => Cor[0] em todos leds.\r\n");
                }
                else if (strcmpr(arg, "colorcycle"))
                {
                    uart_str("=====>Color Cycle<=====\r\n"
                             "colorcycle => Alterna entre as cores do arco-iris.\r\n"
                             "-s [0-50]  => Altera o tamanho do passo entre as cores.\r\n"
                             "-d  [0-1]  => Altera a direcao da troca de cores.\r\n");
                }
                else if (strcmpr(arg, "rainbow")){
                    uart_str("=====>Rainbow<=====\r\n"
                             "rainbow   => Alterna entre as cores do arco-iris led por led.\r\n"
                             "-s [0-50] => Altera o tamanho do passo entre as cores.\r\n"
                             "-d  [0-1] => Altera a direcao da troca de cores.\r\n");
                }
                else if (strcmpr(arg, "breathing"))
                {
                    uart_str("=====>Breathing<=====\r\n"
                             "breathing   => Aumenta e diminui periodicamente o brilho de todos leds.\r\n"
                             "-s  [0-255] => Altera a velocidade do efeito.\r\n"
                             "-tl [0-255] => Altera o brilho maximo.\r\n"
                             "-bl [0-255] => Altera o brilho minimo.\r\n");
                }
                else if (strcmpr(arg, "pulse"))
                {
                    uart_str("=====>Pulse<=====\r\n"
                             "pulse      => Lanca pulsos de luz passando cada led.\r\n"
                             "-p  [0-10] => Altera quantidade de pontos.\r\n"
                             "-o [0-255] => Altera o tamanho percorrido fora dos leds.\r\n"
                             "-d [0-255] => Altera o rastro deixado pelo ponto.\r\n");
                }
                else if (strcmpr(arg, "pingpong"))
                {
                    uart_str("=====>Ping Pong<=====\r\n"
                             "pingpong   => Lanca um pulso de cada lado que ao se encontrarem misturam as cores.\r\n");
                }
                else
                    uart_str(   "=====>Modos<=====\r\n"
                                "off         => Desliga os leds.\r\n"
                                "singlecolor => Cor[0] em todos leds.\r\n"
                                "colorcycle  => Alterna entre as cores do arco-iris.\r\n"
                                "rainbow     => Alterna entre as cores do arco-iris led por led.\r\n"
                                "breathing   => Aumenta e diminui periodicamente o brilho de todos leds.\r\n"
                                "pulse       => Lanca pulsos de luz passando cada led.\r\n"
                                "pingpong    => Lanca um pulso de cada lado que ao se encontraremmistura as cores.\r\n\n"
                                "=====>Configuracoes globais<=====\r\n"
                                "speed [valor] => Muda a velocidade de atualizacao dos leds.\r\n"
                                "color [index] [r] [g] [b] => Muda uma das 5 cores disponiveis<=====.\r\n\n"
                                "Mais informacoes em ajuda [comando]!\r\n"
                                "Exemplo ==>ajuda breathing\r\n");
            }
            else
            {
                error = 1;
            }
            if(error)
            {
                uart_str("Comando errado! Tente \"ajuda\" ou \"ajuda [comando]\".\r\n");
            }
            for(i = 0; i < COMMAND_MAX_SIZE; i++)
            {
                cmd[i] = ' ';
            }
        }

        //Modos
        if(mon_s1() == PRESSIONADO)
        {
            myMode++;
            if(myMode == MODE_COUNT)
            {
                myMode = OFF;
            }
            switch(myMode)
            {
            case OFF:
                off();
                break;
            case SINGLE_COLOR:
                single_color();
                break;
            case COLOR_CYCLE:
                color_cycle();
                break;
            case RAINBOW:
                rainbow();
                break;
            case BREATHING:
                breathing();
                break;
            case PULSE:
                pulse();
                break;
            case PING_PONG:
                ping_pong();
                break;
            default:
                myMode = OFF;
                off();
                break;
            }
        }
        //Cores
        if(mon_s2() == PRESSIONADO)
        {
            switch(cor)
            {
            case 0:
                setColor(PRIMARY, 75, 17, 0);
                setColor(SECONDARY, 30, 0, 75);
                break;
            case 1:
                setColor(PRIMARY, 0, 55, 22);
                setColor(SECONDARY, 43, 0, 75);
                break;
            case 2:
                setColor(PRIMARY, 0, 45, 33);
                setColor(SECONDARY, 25, 50, 0);
                break;
            case 3:
                setColor(PRIMARY, 10, 0, 75);
                setColor(SECONDARY, 75, 10, 7);
                break;
            case 4:
                setColor(PRIMARY, 0, 30, 75);
                setColor(SECONDARY, 40, 35, 0);
                break;
            case 5:
                setColor(PRIMARY, 75, 0, 9);
                setColor(SECONDARY, 12, 25, 90);
                break;
            default:
                cor = 0;
                break;
            }

            secondaryButton();

            cor++;
            if(cor > 5)
                cor = 0;
        }
        update();
    }
}
