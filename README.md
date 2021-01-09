# uartboot

![CI](https://github.com/mihaigalos/uartboot/workflows/CI/badge.svg) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE)

An UART bootloader for Arduino/AVR.

** Warning **

It is the responsibility of the host side to fill the page buffer with unprogrammed values (0xFF) to simplify logic on the mcu side.
The mcu side will always read a fixed-buffer array like this.