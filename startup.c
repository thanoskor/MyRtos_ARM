#include <stdint.h>
extern int main(void);

#define STACK_TOP 0x20010000  // full 64 KB RAM

void Reset_Handler(void);
void Default_Handler(void);

/* nRF52832 has 15 core + 48 peripheral IRQs (63 total) */
#define WEAK_DEFAULT __attribute__((weak, alias("Default_Handler")))

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
    Default_Handler,            // NMI
    Default_Handler,            // HardFault
    Default_Handler,            // MemManage
    Default_Handler,            // BusFault
    Default_Handler,            // UsageFault
    0,0,0,0,                    // reserved
    Default_Handler,            // SVCall
    Default_Handler,            // DebugMonitor
    0,                          // reserved
    Default_Handler,            // PendSV
    Default_Handler,            // SysTick

    /* Peripheral IRQs (48) */
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler
};

#include "uart.h"

/* Reset handler */
void Reset_Handler(void) {
    uint32_t *src, *dst;

    //Enable FPU (Cortex-M4F)
    volatile uint32_t *SCB_CPACR = (uint32_t*)0xE000ED88;
    *SCB_CPACR |= (0xF << 20);  // enable CP10 & CP11 (full access to FPU)

    //Initialize .data section
    src = &_sidata;   // flash location of initialized data
    dst = &_sdata;    // RAM location
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    //Zero .bss section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

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
