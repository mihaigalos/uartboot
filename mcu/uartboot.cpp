#include "uartboot.h"

const TECommunicationResult UartBoot::isCrcOk(const uint8_t *in, const uint8_t length, const CRCType &expectedCrc) const
{
    CRCType crc{};
    for (uint8_t i = 0; i < length; ++i)
    {
        crc += in[i];
    }
    return crc == expectedCrc ? TECommunicationResult::Ok : TECommunicationResult::CRCMismatch;
}

void UartBoot::writePageToFlash(const uint8_t (&in)[kPageWithCrcAndDestinationSize]) const
{
    DestinationAddreessType destinationAddress{};
    destinationAddress |= static_cast<DestinationAddreessType>(in[kDestinationAddressOffset + 0]) << 8;
    destinationAddress |= static_cast<DestinationAddreessType>(in[kDestinationAddressOffset + 1]);

    writeToPageBuffer(destinationAddress, in);
    writePageBufferToFlash(destinationAddress);
}

const TECommunicationResult UartBoot::ReadMetadata(Metadata &metadata) const
{
    for (uint8_t i = 0; i < kMetadataSize; ++i)
    {
        metadata.byte_array[i] = uart_read();
    }

    return isCrcOk(metadata.byte_array, kMetadataSize - kSizeOfCRC32, metadata.structure.crc);
}
const TECommunicationResult UartBoot::safeReadMetadata(Metadata &metadata) const
{
    TECommunicationResult result{TECommunicationResult::Invalid};
    for (uint8_t tries = 0; tries < kMaxTriesWithCommunicationFailure && result != TECommunicationResult::Ok; ++tries)
    {
        result = ReadMetadata(metadata);
        uart_write(static_cast<uint8_t>(result));
    }
    return result;
}

const TECommunicationResult UartBoot::readPageWithMetadataFromHost(Page &page) const
{
    uint8_t readBytes{0};

    for (; readBytes < kPageWithCrcAndDestinationSize; ++readBytes)
    {
        page.byte_array[readBytes] = uart_read();
    }

    return isCrcOk(page.byte_array, kPageSize + kSizeOfDestinationAddress, page.structure.crc);
}

const TECommunicationResult UartBoot::safeReadPageWithMetadataFromHost(Page &page) const
{
    TECommunicationResult result{TECommunicationResult::Invalid};

    for (uint8_t tries = 0; tries < kMaxTriesWithCommunicationFailure && result != TECommunicationResult::Ok; ++tries)
    {
        result = readPageWithMetadataFromHost(page);
        uart_write(static_cast<uint8_t>(result));
    }

    return result;
}

const TEFlashResult UartBoot::run()
{
    TEFlashResult result{TEFlashResult::Invalid};

    Metadata metadata{};

    if (TECommunicationResult::Ok == safeReadMetadata(metadata))
    {
        LED_TOGGLE();
        eraseApplication();
        for (uint16_t i = 0; i < metadata.structure.length; ++i)
        {
            result = TEFlashResult::Ok;
            Page page{};
            if (TECommunicationResult::Ok == safeReadPageWithMetadataFromHost(page))
            {
                writePageToFlash(page.byte_array);
            }
            else
            {
                result = TEFlashResult::FlashFailed;
                break;
            }
            LED_TOGGLE();
        }
    }
    return result;
}
