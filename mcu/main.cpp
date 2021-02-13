#include "uartboot.h"

int main()
{
    _delay_ms(1000);
    initCommon();
    uart_init();
    uart_write('U');
    LED_ON();
    UartBoot{}.run();
}
