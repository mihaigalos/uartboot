#pragma once

#include "config.h"

constexpr uint8_t kStartOfHeading PROGMEM{0x01};
constexpr uint8_t kWaitForDataStartBootloader PROGMEM{5};
constexpr uint8_t kSizeOfFlashPage PROGMEM{128};

using CRC32Type = uint32_t;
using DestinationAddreessType = uint16_t;

constexpr uint8_t kSizeOfCRC32{sizeof(CRC32Type)};
constexpr uint16_t kSizeOfDestinationAddress{sizeof(DestinationAddreessType)};

static constexpr uint8_t kPageSize{SPM_PAGESIZE};
static constexpr uint8_t kCRC32Offset{kPageSize};
static constexpr uint8_t kDestinationAddressOffset{kCRC32Offset + kSizeOfCRC32};

class UartBoot
{
public:
    UartBoot();
    bool isReflashNecessary(uint32_t &application_timestamp) const;
    virtual bool isCrcOk(const uint8_t (&in)[kSizeOfFlashPage + kSizeOfCRC32 + kSizeOfDestinationAddress], const uint8_t length, const CRC32Type &expectedCrc) const;
    void writeOnePageToFlash(const uint8_t (&in)[kSizeOfFlashPage + kSizeOfCRC32 + kSizeOfDestinationAddress]) const;
#ifdef TESTING
    virtual void writePageBufferToFlash(const uint16_t address) const;
    virtual void writeToPageBuffer(const uint16_t address, const uint8_t *data) const;
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