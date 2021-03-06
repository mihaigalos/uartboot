#ifndef UART_READ_H_
#define UART_READ_H_

#include <avr/io.h>

#define FIXED_BAUD_RATE (38400UL) // Desired Baudrate

// Pins used in writing and reading to/from the UART. Comment out if not needed
// to save up Flash space.
#define RX_PIN 4
#define TX_PIN 3

#define UART_DDR DDRB             // Data direction for port
#define UART_IN_PORT_MAPPING PINB // Mapping of UART to physical input port
#define UART_OUT_PORT_MAPPING PORTB

void uart_init();
uint8_t uart_read();
void uart_write(uint8_t value);

#endif /* UART_READ_H_ */