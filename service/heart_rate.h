#ifndef HEART_RATE_H
#define HEART_RATE_H
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    HR_NO_SIGNAL = 0,
    HR_LOW,
    HR_NORMAL,
    HR_HIGH
} hr_status_t;

void         heart_rate_init(void);
/* Feed one ADC sample. Returns true when a new beat was detected. */
bool         heart_rate_update(uint16_t sample, uint32_t now_ms);
uint16_t     heart_rate_get_bpm(void);
uint16_t     heart_rate_get_ibi(void);
bool         heart_rate_pulse_active(void);
hr_status_t  heart_rate_get_status(void);
const char  *heart_rate_status_str(hr_status_t s);
#endif
