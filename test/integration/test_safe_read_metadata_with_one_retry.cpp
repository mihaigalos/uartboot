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
    MOCK_METHOD(void, uart_write, (uint8_t), (const, override));
};

class Fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
        data_[kCRC32OffsetInMetadata + 0] = 0xB6;
        data_[kCRC32OffsetInMetadata + 1] = 0xFF;
        data_[kCRC32OffsetInMetadata + 2] = 0x44;
        data_[kCRC32OffsetInMetadata + 3] = 0x00;
    }
    static uint8_t onUartRead()
    {
        uint8_t result{data_[pos_]};
        if (++pos_ == sizeof(Metadata))
        {
            pos_ = 0;
            data_[kCRC32OffsetInMetadata + 3] = 0x1A;
        }
        return result;
    }

    static void onUartWrite(uint8_t in)
    {
        static uint8_t i{0};
        transmit_to_host_buffer_[i++] = static_cast<TECommunicationResult>(in);
    }

protected:
    UartBootMock sut_{};
    static uint8_t data_[kMetadataSize];
    static TECommunicationResult transmit_to_host_buffer_[8];
    static uint8_t pos_;
};

uint8_t Fixture::data_[kMetadataSize]{0xab, 0xd6, 0x9c, 0x3a, 0xd6, 0x22, 0xd1, 0x01,
                                      0x6d, 0x88, 0xcf, 0xc4, 0x15, 0x1d, 0xad, 0x3e,
                                      0x7a, 0xb9, 0x06, 0x4f, 0x21, 0x94, 0x23, 0xae,
                                      0xa9, 0x20, 0xcf, 0x28, 0x87, 0xab};

TECommunicationResult Fixture::transmit_to_host_buffer_[8];
uint8_t Fixture::pos_{0};

TEST_F(Fixture, SafeReadPageFromHostCrcMismatch_WhenTypical)
{
    TECommunicationResult expected{TECommunicationResult::Ok};
    TECommunicationResult actual{TECommunicationResult::Invalid};
    Metadata in{};
    EXPECT_CALL(sut_, uart_read())
        .WillRepeatedly(Invoke(onUartRead));
    EXPECT_CALL(sut_, uart_write(_))
        .WillRepeatedly(Invoke([&](uint8_t param) { Fixture::onUartWrite(param); }));

    sut_.safeReadMetadata(in);

    actual = transmit_to_host_buffer_[0];
    ASSERT_EQ(actual, TECommunicationResult::CRCMismatch);

    actual = transmit_to_host_buffer_[1];
    ASSERT_EQ(actual, expected);
}
