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
protected:
    UartBootMock sut_{};
};

TEST_F(Fixture, ReadUartOk_WhenTypical)
{
    Metadata expected;
    Metadata actual;

    InSequence seq;
    for (uint8_t i = 0; i < kMetadataSize; ++i)
    {
        expected.byte_array[i] = i;
        EXPECT_CALL(sut_, uart_read())
            .WillOnce(Return(i))
            .RetiresOnSaturation();
    }

    sut_.ReadMetadata(actual);

    for (uint8_t i = 0; i < kMetadataSize; ++i)
    {
        ASSERT_EQ(actual.byte_array[i], expected.byte_array[i]);
    }
}