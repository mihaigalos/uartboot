#pragma once

#include <stdint.h>

#define LED_PORT_NAME B
#define LED_PIN 0

#ifndef TESTING
#include <avr-bootloader-common/all.h>
#include <util/delay.h>
#include "software_uart/software_uart.h"
#define virtual__ virtual //needed for mocking
#define inline__ inline
#else // TESTING
#include "testing_helper.h"
#include <avr-bootloader-common/crc32.h>
#define virtual__ // no need for virtual functions
#define inline__
#define LED_TOGGLE()
#endif // TESTING

#include <stdint.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif //__AVR__
