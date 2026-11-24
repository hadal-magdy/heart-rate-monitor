#include <avr/io.h>
#include "uart.h"
#include "../inc/config.h"

void uart_init(void)
{
    uint16_t ubrr = (uint16_t)((F_CPU / (16UL * UART_BAUD)) - 1UL);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);                         /* TX only */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);        /* 8N1 */
}

void uart_putc(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) { }
    UDR0 = c;
}

void uart_puts(const char *s)
{
    while (*s) {
        uart_putc(*s++);
    }
}
