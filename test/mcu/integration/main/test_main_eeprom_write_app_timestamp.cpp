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
    MOCK_METHOD(uint32_t, readLatestApplicationTimestampFromInternalEeprom, (), (const, override));
    MOCK_METHOD(uint8_t, uart_read, (), (const, override));
    MOCK_METHOD(void, writeLatestApplicationTimestampToInternalEeprom, (const uint32_t latest_timestamp), (const, override));
};

class Fixture : public ::testing::Test
{
public:
    void SetUp() override
    {
    }
    static uint8_t onUartRead()
    {
        static uint8_t i{0};
        return data_[++i - 1];
    }
    static void onWriteLatestApplicationTimestampToInternalEeprom(const uint32_t latest_timestamp)
    {
        application_timestamp_ = latest_timestamp;
    }

protected:
    UartBootMock sut_{};
    static uint8_t data_[kMetadataSize + kPageWithCrcAndDestinationSize];
    static uint32_t application_timestamp_;
};

uint8_t Fixture::data_[kMetadataSize + kPageWithCrcAndDestinationSize]{
    0xab, 0xd6, 0x9c, 0x3a, 0xd6, 0x22, 0xd1, 0x01,
    0x6d, 0x88, 0xcf, 0xc4, 0x15, 0x1d, 0xad, 0x3e,
    0x7a, 0xb9, 0x06, 0x4f, 0x21, 0x94, 0x23, 0xae,
    0xa9, 0x20, 0xcf, 0x28,
    0x01, 0x00,
    0x1B, 0xBA, 0x99, 0x36,

    0x7e, 0x8a, 0x8b, 0xeb, 0x77, 0x7b, 0xa9, 0xd0, 0x80, 0x36, 0x10, 0x8c, 0xd4, 0x79, 0x1b, 0x50,
    0x96, 0x45, 0x11, 0xef, 0x8f, 0x26, 0xdc, 0xc6, 0xa8, 0x88, 0x01, 0x09, 0xd8, 0x71, 0xf1, 0x6f,
    0xfa, 0x70, 0x50, 0x10, 0x60, 0x69, 0xb9, 0xd6, 0xa1, 0xad, 0x12, 0xf9, 0x6e, 0x20, 0x1a, 0x4d,
    0x9e, 0xff, 0xa0, 0x26, 0x58, 0x79, 0xae, 0x7f, 0x5e, 0xf8, 0x20, 0x69, 0xa2, 0x35, 0xb6, 0x38,
    0x9a, 0xb1, 0xc2, 0x12, 0x1f, 0xdd, 0x50, 0x5d, 0x25, 0xd3, 0xd6, 0x16, 0x5c, 0xf8, 0x46, 0x02,
    0x44, 0x81, 0x32, 0x9f, 0xc0, 0x3c, 0x36, 0xb1, 0x5f, 0x76, 0x1a, 0xfd, 0x4f, 0x5d, 0xa7, 0xd8,
    0x45, 0x80, 0x10, 0xf0, 0xc0, 0xe9, 0x31, 0x76, 0x4c, 0xdc, 0x56, 0x6b, 0xdd, 0x43, 0x0b, 0x35,
    0xcb, 0x38, 0xaa, 0x28, 0x62, 0x85, 0x57, 0xbd, 0x7b, 0x82, 0x1e, 0xba, 0xfa, 0x21, 0xb0, 0xd6,
    0x00, 0x00,
    0x36, 0x0B, 0x1A, 0x16};
uint32_t Fixture::application_timestamp_{0};

TEST_F(Fixture, MainWriteEEPROMWithApplicationTimestampOk_WhenTypical)
{
    uint32_t expected{0xAE239421};
    uint32_t actual{0};
    Metadata in;
    EXPECT_CALL(sut_, uart_read())
        .WillRepeatedly(Invoke(onUartRead));
    EXPECT_CALL(sut_, readLatestApplicationTimestampFromInternalEeprom()).WillOnce(Return(0xFFFFFFFF));
    EXPECT_CALL(sut_, writeLatestApplicationTimestampToInternalEeprom(_))
        .WillRepeatedly(Invoke([&](const uint32_t latest_timestamp) { Fixture::onWriteLatestApplicationTimestampToInternalEeprom(latest_timestamp); }));

    sut_.main();
    actual = application_timestamp_;

    ASSERT_EQ(actual, expected);
}