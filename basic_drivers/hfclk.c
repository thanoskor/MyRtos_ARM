#include "hfclk.h"

/* nRF52832 TIMER0 Register Base */
#define TIMER0_BASE         0x40008000
#define TASKS_START         (*(volatile uint32_t *)(TIMER0_BASE + 0x000))
#define TASKS_STOP          (*(volatile uint32_t *)(TIMER0_BASE + 0x004))
#define TASKS_CLEAR         (*(volatile uint32_t *)(TIMER0_BASE + 0x00C))
#define TASKS_CAPTURE0      (*(volatile uint32_t *)(TIMER0_BASE + 0x040))
#define MODE                (*(volatile uint32_t *)(TIMER0_BASE + 0x504))
#define BITMODE             (*(volatile uint32_t *)(TIMER0_BASE + 0x508))
#define PRESCALER           (*(volatile uint32_t *)(TIMER0_BASE + 0x510))
#define CC0                 (*(volatile uint32_t *)(TIMER0_BASE + 0x540))

void timer_init(void) {
    TASKS_STOP = 1;
    TASKS_CLEAR = 1;
    
    // Set Timer mode (not counter mode)
    MODE = 0; 
    
    // 32-bit width (maximum value 0xFFFFFFFF)
    BITMODE = 3; 
    
    // Prescaler 4: (16MHz / 2^4) = 1MHz = 1us per tick
    PRESCALER = 4; 
    
    TASKS_START = 1;
}

uint32_t timer_get_us(void) {
    // Capture the current timer value into the CC[0] register
    TASKS_CAPTURE0 = 1;
    return CC0;
}

void busy_wait_ms(uint32_t ms) {
    uint32_t start = timer_get_us();
    uint32_t wait = ms * 1000;
    while ((timer_get_us() - start) < wait);
}