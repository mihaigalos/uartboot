#pragma once

#include <stdint.h>

static constexpr uint32_t eeprom_not_programmed =
    0xFFFFFFFF; // default value in eeprom when not programmed

static constexpr uint16_t metadata_base_offset = 0x0000;

static constexpr uint16_t timestamp_application_byte_offset = 20 + metadata_base_offset;
static constexpr uint16_t application_length_byte_offset = 32 + metadata_base_offset;
static constexpr uint16_t application_byte_offset = 34 + metadata_base_offset;
static constexpr uint16_t application_start_address_byte_offset = 36 + metadata_base_offset;
static constexpr uint16_t application_crc_expected_index = 28 + metadata_base_offset;

static constexpr uint8_t SPM_PAGESIZE{128};
static constexpr uint16_t kEmulatedFlashSize = 32 * 1024;

struct FlashEmulator
{
    uint8_t data[kEmulatedFlashSize];
};