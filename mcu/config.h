#pragma once

#include <stdint.h>

#ifndef TESTING
#include <avr-bootloader-common/all.h>
#include <util/delay.h>
#include "software_uart/software_uart.h"
#define virtual__ virtual //needed for mocking

#else // TESTING
#include "testing_helper.h"
#include <avr-bootloader-common/crc32.h>
#define virtual__ // no need for virtual functions
#endif            // TESTING

#include <stdint.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif //__AVR__

#define LED_PORT_NAME B
#define LED_PIN 0
