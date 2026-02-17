#define UART0_DR (*(volatile unsigned int*)0x4000C000)

void uart_putc(char c) {
    UART0_DR = c;
}

void uart_puts(const char* s) {
    while(*s) uart_putc(*s++);
}

int main(void) {
    while(1) {
        uart_puts("Hello QEMU!\n");
    }
}