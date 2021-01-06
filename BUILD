load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@avr_tools//tools/avr:hex.bzl", "eeprom", "hex", "listing")

BOOTLOADER_START_ADDRESS = "0x7800"

EEPROM_CONFIGURATION_START_BYTE = "0x03F6"

F_CPU = "8000000UL"

DEFAULT_COMPILER_OPTIONS = [
    "-fdiagnostics-color",
    "-fpack-struct",
    "-fshort-enums",
    "-funsigned-bitfields",
    "-funsigned-char",
    "-g",
    "-lm -Wl,--relax,--gc-sections,-Map=uartboot.map -Wl,--section-start=.text=" + BOOTLOADER_START_ADDRESS,
    "-mmcu=$(MCU)",
    "-Os",
    "-pedantic",
    "-std=gnu++14",
    "-Wall",
    "-Werror",
    "-Wl,-Map=uartboot.map,--cref",
]

config_setting(
    name = "avr",
    values = {
        "cpu": "avr",
    },
)

genrule(
    name = "gen_bootloader_h",
    srcs = ["mcu/uartboot.cpp"],
    outs = ["bootloader.h"],
    cmd = "printf \"%s\n%s %s\n%s %s\" " +
          "\"#pragma once\" " +
          "\"#define BOOTLOADER_START_ADDRESS \" \"" + BOOTLOADER_START_ADDRESS + "\" " +
          "\"#define EEPROM_CONFIGURATION_START_BYTE \" \"" + EEPROM_CONFIGURATION_START_BYTE +
          "\" >$@",
)

cc_library(
    name = "bootloader_h",
    srcs = [":gen_bootloader_h"],
    linkstatic=1,
)


cc_binary(
    name = "uartboot.elf",
    srcs = glob([
        "mcu/**/*.cpp",
        "mcu/**/*.c",
        "mcu/**/*.h",
    ]),
    copts = select({
        ":avr": [
            "-DF_CPU=" + F_CPU,
        ] + DEFAULT_COMPILER_OPTIONS,
        "//conditions:default": [],
    }),
    includes = [
        "mcu",
    ],
    linkopts = select({
        ":avr": DEFAULT_COMPILER_OPTIONS,
        "//conditions:default": [],
    }),
    deps = [
        ":bootloader_h",
        "@avr-bootloader-common",
    ],
)

hex(
    name = "uartboot_hex",
    src = ":uartboot.elf",
)

eeprom(
    name = "uartboot_eeprom",
    src = ":uartboot.elf",
)

listing(
    name = "uartboot_listing",
    src = ":uartboot.elf",
)

