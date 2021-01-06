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

DEFAULT_TEST_COMPILE_OPTIONS = [
    "-fsanitize=address",
    "-DADDRESS_SANITIZER",
    "-DTESTING",
    "-O0",
    "-g",
    "-fno-omit-frame-pointer",
]

DEFAULT_TEST_LINK_OPTIONS = [
    "-fsanitize=address",
    "-static-libasan",
]


DEFAULT_TEST_DEPS = [
    "@gtest",
    "@gtest//:gtest_main",
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
    ],exclude = ["mcu/testing_helper.h"]),
    copts = select({
        ":avr": [
            "-DF_CPU=" + F_CPU,
        ] + DEFAULT_COMPILER_OPTIONS,
        "//conditions:default": [],
    }),
    includes = [
        "mcu",
        "avr-bootloader-common",
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

[
    cc_test(
        name = "component/" + component_name,
        srcs = glob([
            "mcu/uartboot.cpp",
            "mcu/uartboot.h",
            "mcu/config.h",
            "mcu/testing_helper.h",
            "test/component/" + component_name + ".cpp",
        ]),
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        includes = [
            "mcu",
        ],
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["component"],
        deps = DEFAULT_TEST_DEPS,
    )
    for component_name in [
        file_name.replace("test/component/", "").replace(".cpp", "")
        for file_name in glob(["test/component/**/*.cpp"])
    ]
]

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

