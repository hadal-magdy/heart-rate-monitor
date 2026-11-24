#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "../inc/config.h"

static void lcd_strobe(void)
{
    LCD_PORT |= (1 << LCD_E);
    _delay_us(2);
    LCD_PORT &= ~(1 << LCD_E);
    _delay_us(60);
}

static void lcd_nibble(uint8_t n)
{
    LCD_PORT = (LCD_PORT & ~LCD_DATA_MASK) | ((n & 0x0F) << LCD_D4_SHIFT);
    lcd_strobe();
}

static void lcd_send(uint8_t value, uint8_t rs)
{
    if (rs) LCD_PORT |= (1 << LCD_RS);
    else    LCD_PORT &= ~(1 << LCD_RS);
    lcd_nibble(value >> 4);
    lcd_nibble(value & 0x0F);
}

static void lcd_cmd(uint8_t c) { lcd_send(c, 0); }

void lcd_init(void)
{
    LCD_DDR |= LCD_DATA_MASK | (1 << LCD_RS) | (1 << LCD_E);
    LCD_PORT &= ~(LCD_DATA_MASK | (1 << LCD_RS) | (1 << LCD_E));

    _delay_ms(50);
    LCD_PORT &= ~(1 << LCD_RS);
    lcd_nibble(0x03); _delay_ms(5);
    lcd_nibble(0x03); _delay_us(150);
    lcd_nibble(0x03); _delay_us(150);
    lcd_nibble(0x02); _delay_us(150);      /* switch to 4-bit */

    lcd_cmd(0x28);                          /* 4-bit, 2 lines, 5x8 */
    lcd_cmd(0x0C);                          /* display on, cursor off */
    lcd_cmd(0x06);                          /* entry mode: increment */
    lcd_clear();
}

void lcd_clear(void)
{
    lcd_cmd(0x01);
    _delay_ms(3);
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
    lcd_cmd(0x80 | ((row ? 0x40 : 0x00) + col));
}

void lcd_putc(char c) { lcd_send((uint8_t)c, 1); }

void lcd_puts(const char *s)
{
    while (*s) lcd_putc(*s++);
}

void lcd_create_char(uint8_t slot, const uint8_t pattern[8])
{
    lcd_cmd(0x40 | ((slot & 0x07) << 3));
    for (uint8_t i = 0; i < 8; i++) lcd_send(pattern[i], 1);
    lcd_cmd(0x80);                          /* back to DDRAM */
}
