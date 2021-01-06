#include "uartboot.h"

int main()
{
    uint8_t wait_for_data{kWaitForDataStartBootloader};
    bool done{false};
    do
    {
        if (uart_read() == kStartOfHeading)
        {
            done = true;
        }

        _delay_ms(1000);
    } while (!--wait_for_data && !done);
}
