#include <stdint.h>


void uart_init(void);

void uart_write(const char* s);

void uart_write_uint(uint32_t d);