#include "rtc_tick.h"

/* RTC1 Hardware Addresses */
#define RTC1_BASE             0x40011000UL
#define TASKS_START           (*(volatile uint32_t *)(RTC1_BASE + 0x000))
#define TASKS_STOP            (*(volatile uint32_t *)(RTC1_BASE + 0x004))
#define TASKS_CLEAR           (*(volatile uint32_t *)(RTC1_BASE + 0x008))
#define EVENTS_COMP0          (*(volatile uint32_t *)(RTC1_BASE + 0x140))
#define INTENSET              (*(volatile uint32_t *)(RTC1_BASE + 0x304))
#define PRESCALER             (*(volatile uint32_t *)(RTC1_BASE + 0x508))
#define CC0                   (*(volatile uint32_t *)(RTC1_BASE + 0x540))

/* System Registers (Cortex-M4 Core) */
#define NVIC_ISER0            (*(volatile uint32_t *)0xE000E100)
#define SCB_ICSR              (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET_BIT         (1UL << 28)
#define RTC1_IRQ_BIT          (1UL << 17)

void rtc_tick_init(uint32_t period_ms) {
    // Set frequency to 1024Hz (32768 / (31 + 1))
    // 1 tick = ~0.976ms. period_ms = counts needed.
    PRESCALER = 31;
    CC0 = period_ms;

    // Enable Compare 0 Interrupt within RTC
    INTENSET = (1UL << 16);

    // Enable RTC1 (IRQ 17) in the NVIC
    NVIC_ISER0 = RTC1_IRQ_BIT;
}

void rtc_tick_start(void) {
    TASKS_CLEAR = 1;
    TASKS_START = 1;
}

void rtc_tick_stop(void) {
    TASKS_STOP = 1;
}

/**
 * @brief Hardware Interrupt Vector for RTC1.
 * This is hidden in the .c file because it is the "glue" 
 * between hardware and the RTOS context switcher.
 */
void RTC1_IRQHandler(void) {
    if (EVENTS_COMP0 != 0) {
        EVENTS_COMP0 = 0;    // Clear Event
        TASKS_CLEAR = 1;     // Reset Counter
        
        // Trigger the RTOS Context Switcher
        SCB_ICSR = PENDSVSET_BIT;
    }
}