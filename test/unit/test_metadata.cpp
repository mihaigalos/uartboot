#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

#include "mcu/uartboot.h"

class UartBootMock : public UartBoot
{
public:
};

class Fixture : public ::testing::Test
{
protected:
    UartBootMock sut_{};
};

bool operator==(const GlobalMetadata &lhs, const GlobalMetadata &rhs)
{
    bool result{true};

    result &= lhs.structure.last_free_byte_pointer == rhs.structure.last_free_byte_pointer;

    for (uint8_t i = 0; i < sizeof(lhs.structure.bootloader_name); ++i)
    {
        result &= lhs.structure.bootloader_name[i] == rhs.structure.bootloader_name[i];
    }

    for (uint8_t i = 0; i < sizeof(lhs.structure.application_name); ++i)
    {
        result &= lhs.structure.application_name[i] == rhs.structure.application_name[i];
    }

    result &= lhs.structure.application_timestamp == rhs.structure.application_timestamp;
    result &= lhs.structure.writing_timestamp == rhs.structure.writing_timestamp;
    result &= lhs.structure.crc32 == rhs.structure.crc32;
    result &= lhs.structure.length == rhs.structure.length;

    return result;
}

TEST_F(Fixture, IsDecodedGlobalMetadataOk_WhenTypical)
{
    GlobalMetadata expected;
    expected.structure.last_free_byte_pointer = 0x7890;
    expected.structure.bootloader_name[0] = 'u';
    expected.structure.bootloader_name[1] = 'a';
    expected.structure.bootloader_name[2] = 'r';
    expected.structure.bootloader_name[3] = 't';
    expected.structure.bootloader_name[4] = 'b';
    expected.structure.bootloader_name[5] = 'o';
    expected.structure.bootloader_name[6] = 'o';
    expected.structure.bootloader_name[7] = 't';

    expected.structure.application_name[0] = 'm';
    expected.structure.application_name[1] = 'y';
    expected.structure.application_name[2] = 'a';
    expected.structure.application_name[3] = 'p';
    expected.structure.application_name[4] = 'p';
    expected.structure.application_name[5] = '-';
    expected.structure.application_name[6] = '-';
    expected.structure.application_name[7] = '-';
    expected.structure.application_name[8] = '-';
    expected.structure.application_name[9] = '-';

    expected.structure.application_timestamp = 0x12345678;
    expected.structure.writing_timestamp = 0xBBCCDDEE;
    expected.structure.crc32 = 0xAABBCCDD;
    expected.structure.length = 0x5678;

    uint8_t data_in_big_endian_format[kGlobalGlobalMetadataSize]{
        0x90, 0x78,
        'u', 'a', 'r', 't', 'b', 'o', 'o', 't',
        'm', 'y', 'a', 'p', 'p', '-', '-', '-', '-', '-',
        0x78, 0x56, 0x34, 0x12,
        0xEE, 0xDD, 0xCC, 0xBB,
        0xDD, 0xCC, 0xBB, 0xAA,
        0x78, 0x56};

    auto actual = sut_.decodeGlobalMetadata(data_in_big_endian_format);

    ASSERT_EQ(actual, expected);
}
