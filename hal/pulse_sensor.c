#include "pulse_sensor.h"
#include "../mcal/adc.h"
#include "../inc/config.h"

void pulse_sensor_init(void)
{
    adc_init();
}

uint16_t pulse_sensor_read(void)
{
    return adc_read(PULSE_ADC_CHANNEL);
}
