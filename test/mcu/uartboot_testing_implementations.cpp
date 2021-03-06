#include "mcu/uartboot.h"

#ifdef TESTING

void UartBoot::writePageBufferToFlash(const uint16_t address) const
{
    static_cast<void>(address);
}
void UartBoot::writeToPageBuffer(const uint16_t address, const uint8_t *data) const
{
    for (uint8_t i = 0; i < kPageSize; ++i)
    {
        emulated_flash_.data[address + i] = *data++;
    }
}

void UartBoot::eraseApplication() const
{
    for (auto i = 0; i < kFlashSize; ++i)
    {
        emulated_flash_.data[i] = kFlashUnprogrammedValue;
    }
}

const Metadata UartBoot::decodeMetadata(const uint8_t (&in)[kMetadataSize]) const
{
    return *reinterpret_cast<const Metadata *>(in);
}

uint16_t UartBoot::readWordFromMetadata(const uint16_t address) const
{
    return 0xFFFF;
}

uint8_t UartBoot::uart_read() const { return 0xFF; }
void UartBoot::uart_write(uint8_t value) const
{
    static_cast<void>(value);
}
void UartBoot::_delay_ms(uint16_t) {}
#endif