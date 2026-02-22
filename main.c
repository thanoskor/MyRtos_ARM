#include <stdint.h>
#include "basic_drivers/uart.h"
 

int main(void)
{
    uart_write("Hello Renode!\n");
    while(1);
}
