#ifndef LCD_H_
#define LCD_H_
#include <inttypes.h>
//===============================================================//
#define TRUE    1
#define FALSE   0

// Definição do endereço do PCF_8574
#define PCF_ADR1 0x3F
#define PCF_ADR2 0x27
#define PCF_ADR  PCF_ADR2

#define BR_100K    10  //ACLK/100K = 10
#define BR_50K     20  //ACLK/50K  = 20
#define BR_10K    100  //ACLK/10K  = 100

void lcd_char(char x);
void lcd_esp(char x, char *vt);
void lcd_cursor(char x);
void lcd_cmdo(char x);
void lcd_str(char *vt);

void lcd_inic(void);
void lcd_aux(char dado);
 int pcf_read(void);
void pcf_write(char dado);
 int pcf_teste(char adr);
void lcd_i2c_config(void);

void delay(long limite);

void lcd_gpio_config(void);
void led_vd(void);
void led_VD(void);
void led_vm(void);
void led_VM(void);
//===============================================================//

#endif /* LCD_H_ */
