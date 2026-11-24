#include "led.h"
#include "../inc/config.h"

void led_init(void)
{
    LED_DDR |= (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);
}

void led_set(uint8_t on)
{
    if (on) LED_PORT |= (1 << LED_PIN);
    else    LED_PORT &= ~(1 << LED_PIN);
}
