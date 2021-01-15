#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

#include "mcu/uartboot.h"

class UartBootMock : public UartBoot
{
public:
    MOCK_METHOD(uint8_t, uart_read, (), (const, override));
};

class Fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        // [Verified using UART]: AVR has same endianness as amd64: array(byte0 byte1 byte2 byte3) -> uint32(3210)
        data_[kCRC32OffsetInMetadata + 0] = 0xB6;
        data_[kCRC32OffsetInMetadata + 1] = 0xFF;
        data_[kCRC32OffsetInMetadata + 2] = 0x44;
        data_[kCRC32OffsetInMetadata + 3] = 0x1A;
    }
    static uint8_t onUartRead()
    {
        static uint8_t i{0};
        return data_[++i - 1];
    }

protected:
    UartBootMock sut_{};
    static uint8_t data_[kMetadataSize];
};

uint8_t Fixture::data_[kMetadataSize]{0xab, 0xd6, 0x9c, 0x3a, 0xd6, 0x22, 0xd1, 0x01,
                                      0x6d, 0x88, 0xcf, 0xc4, 0x15, 0x1d, 0xad, 0x3e,
                                      0x7a, 0xb9, 0x06, 0x4f, 0x21, 0x94, 0x23, 0xae,
                                      0xa9, 0x20, 0xcf, 0x28, 0x87, 0xab};

TEST_F(Fixture, ReadMetadataFromHostOk_WhenTypical)
{
    TECommunicationResult expected{TECommunicationResult::Ok};
    TECommunicationResult actual{TECommunicationResult::Invalid};
    Metadata in;
    EXPECT_CALL(sut_, uart_read())
        .WillRepeatedly(Invoke(onUartRead));

    actual = sut_.readMetadata(in);

    ASSERT_EQ(actual, expected);
}