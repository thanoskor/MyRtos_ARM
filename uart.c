#include "uart.h"

#define UARTE0_BASE 0x40002000UL

#define UARTE0_TASKS_STARTTX (*(volatile uint32_t*)(UARTE0_BASE + 0x008))
#define UARTE0_EVENTS_ENDTX  (*(volatile uint32_t*)(UARTE0_BASE + 0x120))
#define UARTE0_TXD_PTR       (*(volatile uint32_t*)(UARTE0_BASE + 0x544))
#define UARTE0_TXD_MAXCNT    (*(volatile uint32_t*)(UARTE0_BASE + 0x548))
#define UARTE0_ENABLE        (*(volatile uint32_t*)(UARTE0_BASE + 0x500))


static void utoa_simple(uint32_t v, char *s)
{
    char *p = s;

    do {
        *p++ = (v % 10) + '0';
        v /= 10;
    } while (v);

    *p = '\0';

    // reverse
    char *a = s, *b = p - 1;
    while (a < b) {
        char t = *a;
        *a++ = *b;
        *b-- = t;
    }
}

static uint32_t my_strlen(const char *s) {
    uint32_t len = 0;
    while (*s++) len++;
    return len;
}


void uart_init(void)
{
    UARTE0_ENABLE = 8;   // enable UARTE
}

void uart_write(const char* s)
{
    uint32_t len = my_strlen(s);

    UARTE0_EVENTS_ENDTX = 0;
    UARTE0_TXD_PTR = (uint32_t)s;
    UARTE0_TXD_MAXCNT = len;
    UARTE0_TASKS_STARTTX = 1;

    while(!UARTE0_EVENTS_ENDTX);
}

void uart_write_uint(uint32_t d) {
    char buf[30];
    utoa_simple(d, buf);
    uart_write(buf);
}

