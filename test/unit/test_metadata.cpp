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

TEST_F(Fixture, IsDecodedMetadataOk_WhenTypical)
{
    Metadata expected{
        0x7890,
        'u', 'a', 'r', 't', 'b', 'o', 'o', 't',
        'm', 'y', 'a', 'p', 'p', '-', '-', '-', '-', '-',
        0x12345678,
        0x09876543,
        0xAABBCCDD,
        0x5678};
    uint8_t data[sizeof(Metadata)]{
        0x78, 0x90,
        'u', 'a', 'r', 't', 'b', 'o', 'o', 't',
        'm', 'y', 'a', 'p', 'p', '-', '-', '-', '-', '-',
        0x12, 0x34, 0x56, 0x78,
        0x09, 0x87, 0x65, 0x43,
        0xAA, 0xBB, 0xCC, 0xDD,
        0x56, 0x78};

    auto actual = sut_.decodeMetadata(data);

    ASSERT_EQ(actual, expected);
}
