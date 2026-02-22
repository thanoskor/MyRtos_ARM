#ifndef RTC_TICK_H
#define RTC_TICK_H

#include <stdint.h>

/**
 * @brief Configures the RTC1 peripheral to generate periodic interrupts.
 * @param period_ms The desired RTOS tick interval in milliseconds.
 */
void rtc_tick_init(uint32_t period_ms);

/**
 * @brief Starts the RTC1 counter and enables the tick interrupt.
 */
void rtc_tick_start(void);

/**
 * @brief Stops the RTC1 counter.
 */
void rtc_tick_stop(void);

#endif