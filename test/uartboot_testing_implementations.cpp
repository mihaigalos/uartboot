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

uint8_t UartBoot::uart_read() const { return 0xFF; }
void UartBoot::_delay_ms(uint16_t) {}
#endif