#ifndef CONFIGMSP_UART_H_
#define CONFIGMSP_UART_H_

#include <inttypes.h>
#define COMMAND_MAX_SIZE 128

uint8_t strcmpr(const char* str1, const char* str2);
uint8_t is_number(char* str);
uint8_t str_to_uint8_t(char* str);
uint16_t str_to_uint16_t(char* str);
void getArg(uint16_t index, char* str, char* newStr);

void uart_config();
char* ler_comando_uart();

void uart_char(char lt);
void uart_str(char* str);
void uart_uint8_t(uint8_t num);
void uart_uint16_t(uint16_t num);
void uart_int8_t(int8_t num);
void uart_int16_t(int16_t num);

#endif /* CONFIGMSP_UART_H_ */
