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
    MOCK_METHOD(uint32_t, readLatestApplicationTimestampFromInternalEeprom, (), (const, override));
    MOCK_METHOD(uint16_t, readWordFromMetadata, (uint16_t), (const, override));
};

class Fixture : public ::testing::Test
{
protected:
    UartBootMock sut_{};
};

TEST_F(Fixture, IsReflashNecessaryWorks_WhenTypical)
{
    bool expected{true};
    bool actual{false};
    uint32_t current_timestamp{0};
    EXPECT_CALL(sut_, readLatestApplicationTimestampFromInternalEeprom()).WillOnce(Return(0xFFFFFFFF));

    actual = sut_.isReflashNecessary(current_timestamp);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, IsReflashNecessaryNotPerformed_WhenTypical)
{
    bool expected{false};
    bool actual{false};
    uint32_t current_timestamp{0x12345678};
    EXPECT_CALL(sut_, readLatestApplicationTimestampFromInternalEeprom()).WillOnce(Return(0x12345678));

    actual = sut_.isReflashNecessary(current_timestamp);

    ASSERT_EQ(actual, expected);
}