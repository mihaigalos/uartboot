# uartboot

![CI](https://github.com/mihaigalos/uartboot/workflows/CI/badge.svg) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE)

An UART bootloader for Arduino/AVR.

** Warning **

Using CuteCom on Linux for operations with the serial port.
Need to cd to bazel-bin/ and run the executable with sudo from there, since `sudo bazel run //host:host demo.hex` would rebuild everything for `root`. 