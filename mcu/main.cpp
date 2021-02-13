#include "uartboot.h"

int main()
{
    initCommon();
    LED_ON();
    UartBoot{}.run();
}
