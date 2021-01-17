#include "uartboot.h"

UartBoot::UartBoot()
{
    init_table(&crc_table_[0]);
}

bool UartBoot::isReflashNecessary(const uint32_t &application_timestamp) const
{
    const uint32_t current_application_timestamp =
        readLatestApplicationTimestampFromInternalEeprom();

    if (eeprom_not_programmed == current_application_timestamp)
        return true;
    if (application_timestamp > current_application_timestamp)
        return true;
    return false;
}

bool UartBoot::isCrcOk(const void *in, const uint8_t length, const CRC32Type &expectedCrc) const
{
    uint32_t crc{0};
    crc32(in, length, &crc_table_[0], &crc);
    return crc == expectedCrc;
}

void UartBoot::writePageToFlash(const uint8_t (&in)[kPageWithCrcAndDestinationSize]) const
{
    DestinationAddreessType destinationAddress{0};
    destinationAddress |= static_cast<DestinationAddreessType>(in[kDestinationAddressOffset + 0]) << 8;
    destinationAddress |= static_cast<DestinationAddreessType>(in[kDestinationAddressOffset + 1]);

    writeToPageBuffer(destinationAddress, in);
    writePageBufferToFlash(destinationAddress);
}

const TECommunicationResult UartBoot::readMetadata(Metadata &metadata) const
{
    for (uint8_t i = 0; i < kMetadataSize; ++i)
    {
        metadata.byte_array[i] = uart_read();
    }
    if (!isCrcOk(metadata.byte_array, sizeof(Metadata) - kSizeOfCRC32, metadata.structure.crc32))
    {
        return TECommunicationResult::CRCMismatch;
    }

    return TECommunicationResult::Ok;
}
const TECommunicationResult UartBoot::safeReadMetadata(Metadata &metadata) const
{

    TECommunicationResult result{TECommunicationResult::Invalid};
    for (uint8_t tries = 0; tries < kMaxTriesWithCommunicationFailure && result != TECommunicationResult::Ok; ++tries)
    {
        result = readMetadata(metadata);
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

    if (!isCrcOk(page.byte_array, kPageSize + kSizeOfDestinationAddress, page.structure.crc32))
    {
        return TECommunicationResult::CRCMismatch;
    }

    return TECommunicationResult::Ok;
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

TEFlashResult UartBoot::main() const
{
    TEFlashResult result{TEFlashResult::Invalid};
    eraseApplication();

    Metadata metadata{};

    if (TECommunicationResult::Ok == safeReadMetadata(metadata) && isReflashNecessary(metadata.structure.application_timestamp))
    {
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
        }
        if (TEFlashResult::Ok == result)
        {
            writeLatestApplicationTimestampToInternalEeprom(metadata.structure.application_timestamp);
        }
    }
    return result;
}