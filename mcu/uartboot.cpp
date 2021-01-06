#include "config.h"

#include <avr-bootloader-common/all.h>
#include <util/delay.h>

#include "uartboot.h"
#include "software_uart/software_uart.h"

constexpr uint8_t kStartOfHeading PROGMEM{0x01};
constexpr uint8_t kWaitForDataStartBootloader PROGMEM{5};

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
