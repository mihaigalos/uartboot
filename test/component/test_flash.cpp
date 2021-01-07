#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

#include "mcu/uartboot.h"

class Fixture : public ::testing::Test
{
protected:
    UartBoot sut_{};
};

TEST_F(Fixture, EraseApplicationOk_WhenTypical)
{
    uint8_t expected{0xFF};
    uint8_t actual{0x00};

    sut_.eraseApplication();

    for (auto i = 0; i < kEmulatedFlashSize; ++i)
    {
        uint8_t actual = sut_.emulated_flash_.data[i];
        ASSERT_EQ(actual, expected);
    }
}
