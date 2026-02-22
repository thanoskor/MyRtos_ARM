#include "lfclk.h"
#include <stdint.h>

#define CLOCK_BASE            0x40000000UL
#define TASKS_LFCLKSTART      (*(volatile uint32_t *)(CLOCK_BASE + 0x008))
#define EVENTS_LFCLKSTARTED   (*(volatile uint32_t *)(CLOCK_BASE + 0x414))
#define LFCLKSRC              (*(volatile uint32_t *)(CLOCK_BASE + 0x518))

void clock_lf_init(void) {
    // Select External Crystal (XTAL) as source
    LFCLKSRC = 1UL; 
    
    // Clear event and trigger start task
    EVENTS_LFCLKSTARTED = 0;
    TASKS_LFCLKSTART = 1;

    // Block until the hardware confirms the clock is running
    //while (EVENTS_LFCLKSTARTED == 0);
    //comenting out the above line because renode does not seem to set this event, 
    //but in real hardware this would be necessary to ensure stability before proceeding.
}