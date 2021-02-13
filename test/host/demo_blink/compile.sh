#! /bin/sh

avr-gcc -std=c++14 -fstack-usage -save-temps -I. -mmcu=atmega644 -Os *.cpp  -o  demo_blink.elf
avr-objcopy -O ihex -R .eeprom *.elf demo_blink.hex
avr-size --target=ihex --format=SysV demo_blink.hex
