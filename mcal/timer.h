#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
void     timer_init(void);       /* Timer0 CTC, 1 ms tick */
uint32_t timer_millis(void);
#endif
