#include "uartboot.h"

UartBoot::UartBoot()
{
    init_table(&crc_table_[0]);
}

bool UartBoot::isReflashNecessary(uint32_t &application_timestamp) const
{
    const uint32_t current_application_timestamp =
        readLatestApplicationTimestampFromInternalEeprom();

    application_timestamp =
        static_cast<uint32_t>(readWordFromMetadata(timestamp_application_byte_offset))
        << 16;
    application_timestamp |= static_cast<uint32_t>(readWordFromMetadata(timestamp_application_byte_offset + 2));

    if (eeprom_not_programmed == current_application_timestamp)
        return true;
    if (application_timestamp != current_application_timestamp)
        return true;
    return false;
}

bool UartBoot::isCrcOk(const uint8_t (&in)[kSizeOfFlashPage + kSizeOfCRC32 + kSizeOfDestinationAddress], const uint8_t length, const CRC32Type &expectedCrc) const
{
    uint32_t crc{0};
    crc32(reinterpret_cast<const void *>(&in[0]), length, &crc_table_[0],
          &crc);
    return crc == expectedCrc;
}

void UartBoot::writeOnePageToFlash(const uint8_t (&in)[kSizeOfFlashPage + kSizeOfCRC32 + kSizeOfDestinationAddress]) const
{
    eraseApplication();

    CRC32Type expectedCRC{0};
    expectedCRC |= static_cast<CRC32Type>(in[kCRC32Offset + 0]) << 24;
    expectedCRC |= static_cast<CRC32Type>(in[kCRC32Offset + 1]) << 16;
    expectedCRC |= static_cast<CRC32Type>(in[kCRC32Offset + 2]) << 8;
    expectedCRC |= static_cast<CRC32Type>(in[kCRC32Offset + 3]);

    DestinationAddreessType destinationAddress{0};
    destinationAddress |= static_cast<DestinationAddreessType>(in[kDestinationAddressOffset + 0]) << 8;
    destinationAddress |= static_cast<DestinationAddreessType>(in[kDestinationAddressOffset + 1]);

    if (isCrcOk(in, kSizeOfFlashPage, expectedCRC))
    {
        writeToPageBuffer(destinationAddress, in);
        writePageBufferToFlash(destinationAddress);
    }
}

const Metadata UartBoot::decodeMetadata(const uint8_t (&in)[kMetadataSize]) const
{
    return *reinterpret_cast<const Metadata *>(in);
}

#ifdef TESTING

void UartBoot::writePageBufferToFlash(const uint16_t address) const
{
    static_cast<void>(address);
}
void UartBoot::writeToPageBuffer(const uint16_t address, const uint8_t *data) const
{
    for (uint8_t i = 0; i < kSizeOfFlashPage; ++i)
    {
        emulated_flash_.data[address + i] = *data++;
    }
}

void UartBoot::eraseApplication() const
{
    for (auto i = 0; i < kEmulatedFlashSize; ++i)
    {
        emulated_flash_.data[i] = kFlashUnprogrammedValue;
    }
}

uint32_t UartBoot::readLatestApplicationTimestampFromInternalEeprom() const
{
    return 0xFFFFFFFF;
}
uint16_t UartBoot::readWordFromMetadata(uint16_t address) const
{
    return 0xFFFF;
}

uint8_t UartBoot::uart_read() { return 0xFF; }
void UartBoot::_delay_ms(uint16_t) {}
#endif