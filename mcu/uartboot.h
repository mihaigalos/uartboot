#pragma once

#include "config.h"

constexpr uint8_t kStartOfHeading PROGMEM{0x01};
constexpr uint8_t kWaitForDataStartBootloader PROGMEM{5};

using CRC32Type = uint32_t;
using DestinationAddreessType = uint16_t;

constexpr uint8_t kSizeOfCRC32{sizeof(CRC32Type)};
constexpr uint16_t kSizeOfDestinationAddress{sizeof(DestinationAddreessType)};

static constexpr uint8_t kPageSize{SPM_PAGESIZE};
static constexpr uint8_t kCRC32Offset{kPageSize};
static constexpr uint8_t kDestinationAddressOffset{kCRC32Offset + kSizeOfCRC32};

union Metadata {
    Metadata() {}
    struct StructureType
    {
        StructureType() {}
        uint16_t last_free_byte_pointer{0};
        char bootloader_name[8]{'u', 'a', 'r', 't', 'b', 'o', 'o', 't'};
        char application_name[10]{'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'};
        uint32_t application_timestamp{0};
        uint32_t writing_timestamp{0};
        uint32_t crc32{0};
        uint16_t length{0};
    } structure;

    uint8_t(byte_array)[34];
};

static constexpr uint8_t kMetadataSize{sizeof(Metadata)};

class UartBoot
{
public:
    UartBoot();
    bool isReflashNecessary(uint32_t &application_timestamp) const;
    virtual__ bool isCrcOk(const uint8_t (&in)[kPageSize + kSizeOfCRC32 + kSizeOfDestinationAddress], const uint8_t length, const CRC32Type &expectedCrc) const;
    void writeOnePageToFlash(const uint8_t (&in)[kPageSize + kSizeOfCRC32 + kSizeOfDestinationAddress]) const;
    const Metadata decodeMetadata(const uint8_t (&in)[kMetadataSize]) const;
    void readMetadata(uint8_t (&in)[kMetadataSize]) const;
#ifdef TESTING
    virtual void writePageBufferToFlash(const uint16_t address) const;
    virtual void writeToPageBuffer(const uint16_t address, const uint8_t *data) const;
    virtual void eraseApplication() const;
    virtual uint32_t readLatestApplicationTimestampFromInternalEeprom() const;
    virtual uint16_t readWordFromMetadata(uint16_t address) const;
    virtual uint8_t uart_read() const;
    virtual void _delay_ms(uint16_t);

    mutable FlashEmulator emulated_flash_;
#else // free function implementation from avr-bootloader-common
#endif

private:
    uint32_t crc_table_[crc_table_size];
};