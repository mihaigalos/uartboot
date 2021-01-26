// This file exists to provide coverage for functions in uartboot_testing_implementations.cpp
// UartBoot uses function primities for interacting with the EEPROM and Flash.
// These primitives are part of https://github.com/mihaigalos/avr-bootloader-common, however,
// in order to test the logic of UartBoot, they are mocked by using virtual methods with the
// same name in class UartBoot.

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <string>

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

TEST_F(Fixture, ReadWordFromMetadata_IncreasesCoverage)
{
    auto expected{0xFFFF};
    auto actual{0};

    actual = sut_.readWordFromMetadata(0xFFFF);

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, UartRead_IncreasesCoverage)
{
    auto expected{0xFF};
    auto actual{0};

    actual = sut_.uart_read();

    ASSERT_EQ(actual, expected);
}

TEST_F(Fixture, _Delay_my_IncreasesCoverage)
{
    auto expected{0};
    auto actual{0};

    sut_._delay_ms(0xFFFF);

    ASSERT_EQ(actual, expected);
}