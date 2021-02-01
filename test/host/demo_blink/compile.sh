#! /bin/sh

avr-gcc -std=c++14 -fstack-usage -save-temps -I. -mmcu=atmega328p -Os *.cpp  -o  demo_blink.elf
avr-objcopy -O ihex -R .eeprom *.elf demo_blink.hex
