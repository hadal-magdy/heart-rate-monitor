#ifndef CONFIG_H
#define CONFIG_H

#include <avr/io.h>

/* ---------- System ---------- */
#ifndef F_CPU
#define F_CPU 8000000UL          /* Internal RC 8 MHz (CKDIV8 unprogrammed) */
#endif
#define UART_BAUD            9600UL
#define SAMPLE_PERIOD_MS     2           /* 500 Hz sampling */
#define LCD_REFRESH_MS       250

/* ---------- Pulse sensor ---------- */
#define PULSE_ADC_CHANNEL    0           /* PC0 / ADC0 */

/* ---------- Heart-rate classification ---------- */
#define BPM_LOW_LIMIT        60
#define BPM_HIGH_LIMIT       100

/* ---------- LCD 16x2, 4-bit mode on PORTB (RW tied to GND) ---------- */
#define LCD_PORT             PORTB
#define LCD_DDR              DDRB
#define LCD_RS               PB0
#define LCD_E                PB1
#define LCD_D4_SHIFT         2           /* D4..D7 = PB2..PB5 */
#define LCD_DATA_MASK        (0x0F << LCD_D4_SHIFT)

/* ---------- Beat LED ---------- */
#define LED_PORT             PORTD
#define LED_DDR              DDRD
#define LED_PIN              PD5

#endif
