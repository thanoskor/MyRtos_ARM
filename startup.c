#include <stdint.h>
extern int main(void);

#define STACK_TOP 0x20010000  // full 64 KB RAM

void Reset_Handler(void);
void Default_Handler(void);
void HardFault_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void);
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void RTC1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/* nRF52832 has 15 core + 48 peripheral IRQs (63 total) */

/* Symbols provided by linker script */
extern uint32_t _sidata;  // start of initialized data in flash
extern uint32_t _sdata;   // start of .data in RAM
extern uint32_t _edata;   // end of .data in RAM

extern uint32_t _sbss;    // start of .bss in RAM
extern uint32_t _ebss;    // end of .bss in RAM

/* Vector table in FLASH */
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))STACK_TOP,  // initial SP
    Reset_Handler,              // Reset
    NMI_Handler,                // NMI
    HardFault_Handler,          // HardFault
    MemManage_Handler,          // MemManage
    BusFault_Handler,           // BusFault
    UsageFault_Handler,         // UsageFault
    0, 0, 0, 0,                 // Reserved
    SVC_Handler,                // SVCall
    DebugMon_Handler,           // DebugMonitor
    0,                          // Reserved
    PendSV_Handler,             // PendSV
    SysTick_Handler,            // SysTick

    /* Peripheral IRQs (Start at IRQ #0) */
    /* 0..16 are other peripherals, 17 is RTC1 */
    Default_Handler,   // IRQ 0: POWER_CLOCK
    Default_Handler,   // IRQ 1: RADIO
    Default_Handler,   // IRQ 2: UARTE0_UART0
    Default_Handler,   // IRQ 3: SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0
    Default_Handler,   // IRQ 4: SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1
    Default_Handler,   // IRQ 5: NFCT
    Default_Handler,   // IRQ 6: GPIOTE
    Default_Handler,   // IRQ 7: SAADC
    Default_Handler,   // IRQ 8: TIMER0
    Default_Handler,   // IRQ 9: TIMER1
    Default_Handler,   // IRQ 10: TIMER2
    Default_Handler,   // IRQ 11: RTC0
    Default_Handler,   // IRQ 12: TEMP
    Default_Handler,   // IRQ 13: RNG
    Default_Handler,   // IRQ 14: ECB
    Default_Handler,   // IRQ 15: CCM_AAR
    Default_Handler,   // IRQ 16: WDT
    RTC1_IRQHandler,   // IRQ 17: RTC1  <-- MOVE IT HERE
    Default_Handler,   // IRQ 18: QDEC
    Default_Handler,   // IRQ 19: COMP_LPCOMP
    Default_Handler,   // IRQ 20: SWI0_EGU0
    Default_Handler,   // IRQ 21: SWI1_EGU1
    Default_Handler,   // IRQ 22: SWI2_EGU2
    Default_Handler,   // IRQ 23: SWI3_EGU3
    Default_Handler,   // IRQ 24: SWI4_EGU4
    Default_Handler,   // IRQ 25: SWI5_EGU5
    Default_Handler,   // IRQ 26: TIMER3
    Default_Handler,   // IRQ 27: TIMER4
    Default_Handler,   // IRQ 28: PWM0
    Default_Handler,   // IRQ 29: PDM
    Default_Handler,   // IRQ 30: NVMC_ACL
    Default_Handler,   // IRQ 31: PPI
    Default_Handler,   // IRQ 32: MWU
    Default_Handler,   // IRQ 33: PWM1
    Default_Handler,   // IRQ 34: PWM2
    Default_Handler,   // IRQ 35: SPIM2_SPIS2_SPI2
    Default_Handler,   // IRQ 36: RTC2
    Default_Handler,   // IRQ 37: I2S
    Default_Handler,   // IRQ 38: FPU
    Default_Handler,   // IRQ 39: USBD (if applicable, or Reserved)
    Default_Handler,   // IRQ 40: UARTE1
    Default_Handler,   // IRQ 41: QSPI
    Default_Handler,   // IRQ 42: CRYPTOCELL
    Default_Handler,   // IRQ 43: SPIM3
    Default_Handler,   // IRQ 44: PWM3
    Default_Handler,   // IRQ 45: QDEC (duplicate/reserved check datasheet)
    Default_Handler,   // IRQ 46: COMP (duplicate/reserved check datasheet)
    Default_Handler    // IRQ 47: ...
};

#define SCB_CPACR (*(volatile uint32_t*)0xE000ED88)
#define FPU_FPCCR (*(volatile uint32_t *)0xE000EF34)
void FPU_Init(void) {
    //Enable FPU (Cortex-M4F)
    SCB_CPACR |= (0xF << 20);  // enable CP10 & CP11 (full access to FPU)
    FPU_FPCCR |= (1 << 31) | (1 << 30); // ASPEN and LSPEN bits
}


#include "basic_drivers/uart.h"
#include "basic_drivers/lfclk.h"
#include "basic_drivers/rtc_tick.h"

/* Reset handler */
void Reset_Handler(void) {
    

    FPU_Init();

    uart_init();
    
    //Initialize .data section
    uint32_t *src = &_sidata;   // flash location of initialized data
    uint32_t *dst = &_sdata;    // RAM location
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    //Zero .bss section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    clock_lf_init();
    rtc_tick_init(1000); // 10ms tick
    rtc_tick_start();

    //Call main
    main();

    //If main returns, loop forever
    uart_write("Main returned!\n");
    while(1);
}

//Default handler for unimplemented IRQs
void Default_Handler(void) {
    uart_write("Default_Handler Called");
    while(1);
}

void HardFault_Handler(void) {
    uart_write("HardFault_Handler Called");
    while(1);
}

void PendSV_Handler(void) {
    // This will be called by the RTC1_IRQHandler to trigger a context switch.
    // The actual context switching logic is in the RTOS code, not here.
    uart_write("PendSV_Handler Called - Context Switch Triggered");
}