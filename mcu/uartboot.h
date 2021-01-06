#pragma once

#include "config.h"

#ifndef TESTING
#include <avr-bootloader-common/all.h>
#include <util/delay.h>

#include "software_uart/software_uart.h"

#else // TESTING
#include "testing_helper.h"
#endif // TESTING

#include <stdint.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif //__AVR__

constexpr uint8_t kStartOfHeading PROGMEM{0x01};
constexpr uint8_t kWaitForDataStartBootloader PROGMEM{5};

class UartBoot
{
public:
    bool isReflashNecessary(uint32_t &application_timestamp);

#ifdef TESTING
    virtual uint32_t readLatestApplicationTimestampFromInternalEeprom()
    {
        return 0xFFFFFFFF;
    }
    virtual uint16_t readWordFromMetadata(uint16_t address)
    {
        return 0xFFFF;
    }

    virtual uint8_t uart_read() { return 0xFF; }
    virtual void _delay_ms(uint16_t) {}
#else // implementation from avr-bootloader-common
#endif
};