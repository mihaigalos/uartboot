load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("@avr_tools//tools/avr:hex.bzl", "eeprom", "hex", "listing")
load("@bazel_gazelle//:def.bzl", "gazelle")
load("@com_github_bazelbuild_buildtools//buildifier:def.bzl", "buildifier")
load("@io_bazel_rules_go//go:def.bzl", "go_test")

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
    "-DADDRESS_SANITIZER",
    "-DTESTING",
    "-fno-omit-frame-pointer",
    "-fsanitize=address",
    "-g",
    "-O0",
    "-pedantic",
    "-std=gnu++14",
    "-Wall",
    "-Werror",
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
    linkstatic = 1,
)

cc_binary(
    name = "uartboot.elf",
    srcs = glob(
        [
            "mcu/**/*.cpp",
            "mcu/**/*.c",
            "mcu/**/*.h",
        ],
        exclude = ["mcu/testing_helper.h"],
    ),
    copts = select({
        ":avr": [
            "-DF_CPU=" + F_CPU,
        ] + DEFAULT_COMPILER_OPTIONS,
        "//conditions:default": [],
    }),
    includes = [
        "avr-bootloader-common",
        "mcu",
    ],
    linkopts = select({
        ":avr": DEFAULT_COMPILER_OPTIONS,
        "//conditions:default": [],
    }),
    tags = ["mcu_binary_file"],
    deps = [
        ":bootloader_h",
        "@avr-bootloader-common",
    ],
)

hex(
    name = "uartboot_hex",
    src = ":uartboot.elf",
    tags = ["mcu_binary_file"],
)

eeprom(
    name = "uartboot_eeprom",
    src = ":uartboot.elf",
    tags = ["mcu_binary_file"],
)

listing(
    name = "uartboot_listing",
    src = ":uartboot.elf",
    tags = ["mcu_binary_file"],
)

[
    cc_test(
        name = "unit/" + unit_name,
        srcs = glob([
            "mcu/uartboot.cpp",
            "mcu/uartboot.h",
            "mcu/config.h",
            "mcu/testing_helper.h",
            "test/mcu/uartboot_testing_implementations.cpp",
            "test/mcu/unit/" + unit_name + ".cpp",
        ]),
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        includes = [
            "mcu",
        ],
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["unit"],
        deps = DEFAULT_TEST_DEPS + [
            "@avr-bootloader-common",
        ],
    )
    for unit_name in [
        file_name.replace("test/mcu/unit/", "").replace(".cpp", "")
        for file_name in glob(["test/mcu/unit/**/*.cpp"])
    ]
]

[
    cc_test(
        name = "component/" + component_name,
        srcs = glob([
            "mcu/uartboot.cpp",
            "mcu/uartboot.h",
            "mcu/config.h",
            "mcu/testing_helper.h",
            "test/mcu/uartboot_testing_implementations.cpp",
            "test/mcu/component/" + component_name + ".cpp",
        ]),
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        includes = [
            "mcu",
        ],
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["component"],
        deps = DEFAULT_TEST_DEPS + [
            "@avr-bootloader-common",
        ],
    )
    for component_name in [
        file_name.replace("test/mcu/component/", "").replace(".cpp", "")
        for file_name in glob(["test/mcu/component/**/*.cpp"])
    ]
]

[
    cc_test(
        name = "integration/" + integration_name,
        srcs = glob([
            "mcu/uartboot.cpp",
            "mcu/uartboot.h",
            "mcu/config.h",
            "mcu/testing_helper.h",
            "test/mcu/uartboot_testing_implementations.cpp",
            "test/mcu/integration/" + integration_name + ".cpp",
        ]),
        copts = DEFAULT_TEST_COMPILE_OPTIONS,
        includes = [
            "mcu",
        ],
        linkopts = DEFAULT_TEST_LINK_OPTIONS,
        tags = ["integration"],
        deps = DEFAULT_TEST_DEPS + [
            "@avr-bootloader-common",
        ],
    )
    for integration_name in [
        file_name.replace("test/mcu/integration/", "").replace(".cpp", "")
        for file_name in glob(["test/mcu/integration/**/*.cpp"])
    ]
]

buildifier(name = "buildifier")

gazelle(
    name = "gazelle",
    command = "fix",
    prefix = "github.com/mihaigalos/uartboot",
)

go_test(
    name = "unit_tests",
    srcs = [
        "test/host/unit_typical_test.go",
    ],
    data = ["test/host/demo.hex"],
    embed = ["//host:host_lib"],
)

go_test(
    name = "integration_tests",
    srcs = [
        "test/host/integration_typical_test.go",
    ],
    data = ["test/host/demo.hex"],
    embed = ["//host:host_lib"],
)
