#pragma once

#include <stdint.h>

#ifndef TESTING
#include <avr-bootloader-common/all.h>
#include <util/delay.h>

#include "software_uart/software_uart.h"

#else // TESTING
#include "testing_helper.h"
#include <avr-bootloader-common/crc32.h>
#endif // TESTING

#include <stdint.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif //__AVR__

#define LED_PORT_NAME B
#define LED_PIN 0

static constexpr uint8_t kDataSize{SPM_PAGESIZE};
