#ifndef LCD_H
#define LCD_H
#include <stdint.h>
void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_putc(char c);
void lcd_puts(const char *s);
void lcd_create_char(uint8_t slot, const uint8_t pattern[8]);
#endif
