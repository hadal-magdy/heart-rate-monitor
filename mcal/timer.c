#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "timer.h"
#include "../inc/config.h"

static volatile uint32_t ms_ticks;

ISR(TIMER0_COMPA_vect)
{
    ms_ticks++;
}

void timer_init(void)
{
    TCCR0A = (1 << WGM01);                    /* CTC */
    TCCR0B = (1 << CS01) | (1 << CS00);       /* prescaler 64 */
    OCR0A  = (uint8_t)((F_CPU / 64UL / 1000UL) - 1UL);
    TIMSK0 = (1 << OCIE0A);
}

uint32_t timer_millis(void)
{
    uint32_t t;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = ms_ticks;
    }
    return t;
}
