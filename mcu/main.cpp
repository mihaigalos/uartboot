#include "uartboot.h"

int main()
{
    _delay_ms(500);
    initCommon();
    LED_ON();
    UartBoot{}.run();
}
