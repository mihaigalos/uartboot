#pragma once

#include "config.h"

static constexpr uint8_t kStartOfHeading PROGMEM{0x01};
static constexpr uint8_t kWaitForDataStartBootloader PROGMEM{5};

using CRC32Type = uint32_t;
using DestinationAddreessType = uint16_t;

static constexpr uint8_t kSizeOfCRC32{sizeof(CRC32Type)};
static constexpr uint16_t kSizeOfDestinationAddress{sizeof(DestinationAddreessType)};

static constexpr uint8_t kPageSize{SPM_PAGESIZE};
static constexpr uint8_t kCRC32Offset{kPageSize};
static constexpr uint8_t kDestinationAddressOffset{kCRC32Offset + kSizeOfCRC32};

static constexpr uint16_t kFlashSize{32 * 1024};
static constexpr uint16_t kNumberOfFlashPages{kFlashSize / kPageSize};
static constexpr uint8_t kInvalidValue{0xFF};

static constexpr uint8_t kPageWithMetadataSize{kPageSize + kSizeOfCRC32 + kSizeOfDestinationAddress};

#ifdef TESTING
struct MemoryEmulator
{
    uint8_t data[kFlashSize];
};

using FlashEmulator = MemoryEmulator;
using EEPROMEmulator = MemoryEmulator;
#endif // TESTING

#pragma pack(push, 1)
union GlobalMetadata {
    GlobalMetadata() {}
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
#pragma pack(pop)

static constexpr uint8_t kGlobalMetadataSize{sizeof(GlobalMetadata)};
static_assert(kGlobalMetadataSize == sizeof(GlobalMetadata::byte_array), "kGlobalMetadataSize and byte_array size do not match!");

class UartBoot
{
public:
    UartBoot();
    void main();
    bool isReflashNecessary(uint32_t &application_timestamp) const;
    virtual__ bool isCrcOk(const uint8_t (&in)[kPageWithMetadataSize], const uint8_t length, const CRC32Type &expectedCrc) const;
    void writeOnePageToFlash(const uint8_t (&in)[kPageWithMetadataSize]) const;
    const GlobalMetadata decodeGlobalMetadata(const uint8_t (&in)[kGlobalMetadataSize]) const;
    void readGlobalMetadata(uint8_t (&in)[kGlobalMetadataSize]) const;
    const uint8_t readPageWithMetadataFromHost(uint8_t (&in)[kPageWithMetadataSize]) const;
#ifdef TESTING
    virtual void writePageBufferToFlash(const uint16_t address) const;
    virtual void writeToPageBuffer(const uint16_t address, const uint8_t *data) const;
    virtual void eraseApplication() const;
    virtual uint32_t readLatestApplicationTimestampFromInternalEeprom() const;
    virtual uint16_t readWordFromGlobalMetadata(uint16_t address) const;
    virtual uint8_t uart_read() const;
    virtual void _delay_ms(uint16_t);

    mutable FlashEmulator emulated_flash_;
#else // free function implementation from avr-bootloader-common
#endif

private:
    uint32_t crc_table_[crc_table_size];
};