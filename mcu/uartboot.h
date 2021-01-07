#pragma once

#include "config.h"

constexpr uint8_t kStartOfHeading PROGMEM{0x01};
constexpr uint8_t kWaitForDataStartBootloader PROGMEM{5};
constexpr uint8_t kSizeOfFlashPage PROGMEM{128};

class UartBoot
{
public:
    UartBoot();
    bool isReflashNecessary(uint32_t &application_timestamp) const;
    virtual bool isCrcOk(const uint8_t (&in)[kSizeOfFlashPage], const uint8_t length, const uint32_t &expectedCrc) const;

#ifdef TESTING
    virtual void eraseApplication() const;
    virtual uint32_t readLatestApplicationTimestampFromInternalEeprom() const;
    virtual uint16_t readWordFromMetadata(uint16_t address) const;
    virtual uint8_t uart_read();
    virtual void _delay_ms(uint16_t);

    mutable FlashEmulator emulated_flash_;
#else // free function implementation from avr-bootloader-common
#endif

private:
    uint32_t crc_table_[crc_table_size];
};