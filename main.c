#include <stdint.h>
#include "uart.h"

int x = 6;
int y;
int main(void)
{
    uart_init();
    uart_write_uint(x);
    uart_write_uint(y);
    float z = 0.5f;
    float g = 2.5f;
    float f = g/z;
    
    uart_write("Hello Renode!\n");
    while(1);
}
