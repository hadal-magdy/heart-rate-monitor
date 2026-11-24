#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "app.h"
#include "../inc/config.h"
#include "../mcal/timer.h"
#include "../mcal/uart.h"
#include "../hal/lcd.h"
#include "../hal/led.h"
#include "../hal/pulse_sensor.h"
#include "../service/heart_rate.h"

static const uint8_t heart_icon[8] = {
    0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00
};

static void uart_report(void)
{
    char buf[48];
    hr_status_t st = heart_rate_get_status();
    snprintf(buf, sizeof buf, "BPM=%u,IBI=%u,STATUS=%s\r\n",
             heart_rate_get_bpm(), heart_rate_get_ibi(),
             heart_rate_status_str(st));
    uart_puts(buf);
}

static void lcd_refresh(void)
{
    char line[17];
    hr_status_t st = heart_rate_get_status();

    lcd_set_cursor(0, 0);
    snprintf(line, sizeof line, "BPM:%3u IBI:%4u",
             heart_rate_get_bpm(), heart_rate_get_ibi());
    lcd_puts(line);

    lcd_set_cursor(1, 0);
    snprintf(line, sizeof line, "%-15s", heart_rate_status_str(st));
    lcd_puts(line);

    lcd_set_cursor(1, 15);
    if (heart_rate_pulse_active()) lcd_putc(0);   /* custom heart */
    else                           lcd_putc(' ');
}

void app_init(void)
{
    timer_init();
    uart_init();
    pulse_sensor_init();
    led_init();
    lcd_init();
    heart_rate_init();
    lcd_create_char(0, heart_icon);
    sei();

    lcd_set_cursor(0, 0);
    lcd_puts("Heart Rate Mon.");
    lcd_set_cursor(1, 0);
    lcd_puts("AVR + UART + LCD");
    uart_puts("\r\n== Pulse Heart Rate Monitor ==\r\n");
    _delay_ms(1500);
    lcd_clear();
}

void app_run(void)
{
    uint32_t last_sample = 0, last_lcd = 0;

    for (;;) {
        uint32_t now = timer_millis();

        if (now - last_sample >= SAMPLE_PERIOD_MS) {
            last_sample = now;
            uint16_t s = pulse_sensor_read();
            bool beat = heart_rate_update(s, now);
            led_set(heart_rate_pulse_active());
            if (beat) uart_report();
        }

        if (now - last_lcd >= LCD_REFRESH_MS) {
            last_lcd = now;
            lcd_refresh();
        }
    }
}
