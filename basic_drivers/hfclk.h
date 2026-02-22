#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Initialize TIMER0 as a simple microsecond counter
void timer_init(void);

// Busy-wait delay (use sparingly)
void busy_wait_ms(uint32_t ms);

// Get current timestamp in microseconds
uint32_t timer_get_us(void);

#endif