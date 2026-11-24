#ifndef PULSE_SENSOR_H
#define PULSE_SENSOR_H
#include <stdint.h>
void     pulse_sensor_init(void);
uint16_t pulse_sensor_read(void);   /* raw 0..1023 */
#endif
