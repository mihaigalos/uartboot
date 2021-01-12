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
    for (uint8_t tries = 0; tries < kRetriesOnCommunicationFailure && result != TECommunicationResult::Ok; ++tries)
    {
        Metadata metadata;
        result = readMetadata(metadata);
        uart_write(static_cast<uint8_t>(result));
    }
    return result;
}

const TECommunicationResult UartBoot::readPageWithMetadataFromHost(uint8_t (&in)[kPageWithCrcAndDestinationSize]) const
{
    uint8_t readBytes{0};

    for (; readBytes < kPageWithCrcAndDestinationSize; ++readBytes)
    {
        in[readBytes] = uart_read();
    }

    CRC32Type expectedCrc = static_cast<CRC32Type>(in[kCRC32Offset + 0]) << 24;
    expectedCrc |= static_cast<CRC32Type>(in[kCRC32Offset + 1]) << 16;
    expectedCrc |= static_cast<CRC32Type>(in[kCRC32Offset + 2]) << 8;
    expectedCrc |= static_cast<CRC32Type>(in[kCRC32Offset + 3]);

    if (!isCrcOk(in, kPageSize, expectedCrc))
    {
        return TECommunicationResult::CRCMismatch;
    }

    return TECommunicationResult::Ok;
}

const TECommunicationResult UartBoot::safeReadPageWithMetadataFromHost(uint8_t (&in)[kPageWithCrcAndDestinationSize]) const
{
    TECommunicationResult result{TECommunicationResult::Invalid};

    for (uint8_t tries = 0; tries < kRetriesOnCommunicationFailure && result != TECommunicationResult::Ok; ++tries)
    {
        result = readPageWithMetadataFromHost(in);
        uart_write(static_cast<uint8_t>(result));
    }

    return result;
}

void UartBoot::main()
{
    eraseApplication();
}