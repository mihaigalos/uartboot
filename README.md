# uartboot

![CI](https://github.com/mihaigalos/uartboot/workflows/CI/badge.svg) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE)

An UART bootloader for Arduino/AVR.

** Warning **

Using CuteCom on Linux for operations with the serial port.

Example flashing of `demo.hex`:
```
sudo bazel-bin/host/host_/host /home/mihai/git/uartboot/host/demo.hex
```