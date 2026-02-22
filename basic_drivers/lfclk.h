#ifndef LFCLK_H
#define LFCLK_H

/**
 * @brief Initialize and start the Low Frequency Crystal Oscillator (LFXO).
 * This provides the accurate 32.768 kHz signal required for RTOS timing.
 * This function blocks until the clock is stable.
 */
void clock_lf_init(void);

#endif