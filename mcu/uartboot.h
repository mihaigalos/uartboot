#pragma once

#include "config.h"

using CRCType = uint16_t;
using DestinationAddreessType = uint16_t;

static constexpr uint8_t kSizeOfCRC32{sizeof(CRCType)};
static constexpr uint16_t kSizeOfDestinationAddress{sizeof(DestinationAddreessType)};

static constexpr uint8_t kPageSize{SPM_PAGESIZE};
static constexpr uint8_t kDestinationAddressOffset{kPageSize};
static constexpr uint8_t kCRCOffset{kDestinationAddressOffset + kSizeOfDestinationAddress};

static constexpr uint16_t kFlashSize{static_cast<uint16_t>(32) * static_cast<uint16_t>(1024)};
static constexpr uint16_t kNumberOfFlashPages{kFlashSize / kPageSize};
static constexpr uint8_t kInvalidValue{0xFF};

static constexpr uint8_t kPageWithCrcAndDestinationSize{kPageSize + kSizeOfDestinationAddress + kSizeOfCRC32};
static constexpr uint8_t kMaxTriesWithCommunicationFailure{3};

#ifdef TESTING
struct MemoryEmulator
{
    uint8_t data[kFlashSize];
};

using FlashEmulator = MemoryEmulator;
using EEPROMEmulator = MemoryEmulator;
#endif // TESTING

#ifdef TESTING
#pragma pack(push, 1)
#endif

union Metadata {
    Metadata() {}
    struct StructureType
    {
        StructureType() {}
        uint16_t last_free_byte_pointer{};
        char bootloader_name[8]{'u', 'a', 'r', 't', 'b', 'o', 'o', 't'};
        char application_name[10]{'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'};
        uint32_t application_timestamp{};
        uint32_t writing_timestamp{};
        uint16_t length{};
        CRCType crc{};
    } structure;

    uint8_t(byte_array)[sizeof(StructureType)];
};

#ifdef TESTING
#pragma pack(pop)
#endif

static constexpr uint8_t kMetadataSize{sizeof(Metadata)};
static_assert(kMetadataSize == sizeof(Metadata::byte_array), "kMetadataSize and byte_array size do not match!");

static constexpr uint8_t kCRCOffsetInMetadata{ kMetadataSize - kSizeOfCRC32 };
static_assert(kCRCOffsetInMetadata == 30, "kCRCOffsetInMetadata  and byte_array size do not match!");

#ifdef TESTING
#pragma pack(push, 1)
#endif

union Page {
    Page() = default;
    struct StructureType
    {
        StructureType() = default;
        uint8_t page[kPageSize];
        DestinationAddreessType destination;
        CRCType crc;
    } structure;
    uint8_t(byte_array)[sizeof(StructureType)];
};

#ifdef TESTING
#pragma pack(pop)
#endif
static_assert(kPageWithCrcAndDestinationSize == sizeof(Page::byte_array), "kPageWithCrcAndDestinationSize and byte_array size do not match!");

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
    const TEFlashResult run();
    virtual__ const TECommunicationResult isCrcOk(const uint8_t *in, const uint8_t length, const CRCType &expectedCrc) const;
    inline__ void writePageToFlash(const uint8_t (&in)[kPageWithCrcAndDestinationSize]) const;
    inline__ const TECommunicationResult ReadMetadata(Metadata &metadata) const;
    inline__ const TECommunicationResult safeReadMetadata(Metadata &metadata) const;
    inline__ const TECommunicationResult readPageWithMetadataFromHost(Page &page) const;
    inline__ const TECommunicationResult safeReadPageWithMetadataFromHost(Page &page) const;
#ifdef TESTING
    const Metadata decodeMetadata(const uint8_t (&in)[kMetadataSize]) const;
    virtual void writePageBufferToFlash(const uint16_t address) const;
    virtual void writeToPageBuffer(const uint16_t address, const uint8_t *data) const;
    virtual void eraseApplication() const;
    virtual uint16_t readWordFromMetadata(uint16_t address) const;
    virtual uint8_t uart_read() const;
    virtual void uart_write(uint8_t value) const;
    virtual void _delay_ms(uint16_t);

    mutable FlashEmulator emulated_flash_;
#else // free function implementation from avr-bootloader-common
#endif

private:
};
