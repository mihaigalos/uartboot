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

bool UartBoot::isCrcOk(const uint8_t (&in)[kSizeOfFlashPage], const uint8_t length, const uint32_t &expectedCrc) const
{
    uint32_t crc{0};
    crc32(reinterpret_cast<const void *>(&in[0]), length, &crc_table_[0],
          &crc);
    return crc == expectedCrc;
}

#ifdef TESTING
void UartBoot::eraseApplication() const
{
    for (auto i = 0; i < kEmulatedFlashSize; ++i)
    {
        emulated_flash_.data[i] = 0xFF;
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