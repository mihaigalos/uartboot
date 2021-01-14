#pragma once

#include "config.h"

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

static constexpr uint8_t kPageWithCrcAndDestinationSize{kPageSize + kSizeOfCRC32 + kSizeOfDestinationAddress};
static constexpr uint8_t kMaxTriesWithCommunicationFailure{3};

#ifdef TESTING
struct MemoryEmulator
{
    uint8_t data[kFlashSize];
};

using FlashEmulator = MemoryEmulator;
using EEPROMEmulator = MemoryEmulator;
#endif // TESTING

#pragma pack(push, 1)
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
        uint16_t length{0};
        CRC32Type crc32{0};
    } structure;

    uint8_t(byte_array)[sizeof(StructureType)];
};
#pragma pack(pop)

static constexpr uint8_t kMetadataSize{sizeof(Metadata)};
static_assert(kMetadataSize == sizeof(Metadata::byte_array), "kMetadataSize and byte_array size do not match!");
static constexpr uint8_t kCRC32OffsetInMetadata{30};

enum class TECommunicationResult
{
    Invalid,
    Ok,
    CRCMismatch,
};

enum class TEFlashResult
{
    Invalid,
    Ok,
    FlashFailed
};

class UartBoot
{
public:
    UartBoot();
    TEFlashResult main() const;
    bool isReflashNecessary(uint32_t &application_timestamp) const;
    virtual__ bool isCrcOk(const void *in, const uint8_t length, const CRC32Type &expectedCrc) const;
    void writePageToFlash(const uint8_t (&in)[kPageWithCrcAndDestinationSize]) const;
    const TECommunicationResult readMetadata(Metadata &metadata) const;
    const TECommunicationResult safeReadMetadata(Metadata &metadata) const;
    const TECommunicationResult readPageWithMetadataFromHost(uint8_t (&in)[kPageWithCrcAndDestinationSize]) const;
    const TECommunicationResult safeReadPageWithMetadataFromHost(uint8_t (&in)[kPageWithCrcAndDestinationSize]) const;
#ifdef TESTING
    const Metadata decodeMetadata(const uint8_t (&in)[kMetadataSize]) const;
    virtual void writePageBufferToFlash(const uint16_t address) const;
    virtual void writeToPageBuffer(const uint16_t address, const uint8_t *data) const;
    virtual void eraseApplication() const;
    virtual uint32_t readLatestApplicationTimestampFromInternalEeprom() const;
    virtual void writeLatestApplicationTimestampToInternalEeprom(const uint32_t latest_timestamp) const;
    virtual uint16_t readWordFromMetadata(uint16_t address) const;
    virtual uint8_t uart_read() const;
    virtual void uart_write(uint8_t value) const;
    virtual void _delay_ms(uint16_t);

    mutable FlashEmulator emulated_flash_;
#else // free function implementation from avr-bootloader-common
#endif

private:
    uint32_t crc_table_[crc_table_size];
};