extern int main(void);

#define STACK_TOP 0x20008000

void Reset_Handler(void);
void Default_Handler(void);

__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))STACK_TOP, // initial SP
    Reset_Handler,             // Reset
    Default_Handler,           // NMI
    Default_Handler,           // HardFault
    Default_Handler,           // MemManage
    Default_Handler,           // BusFault
    Default_Handler,           // UsageFault
    0,0,0,0,                   // reserved
    Default_Handler,           // SVCall
    Default_Handler,           // DebugMonitor
    0,                         // reserved
    Default_Handler,           // PendSV
    Default_Handler,           // SysTick
};

void Reset_Handler(void) {
    main();
    while(1);
}

void Default_Handler(void) {
    while(1);
}
