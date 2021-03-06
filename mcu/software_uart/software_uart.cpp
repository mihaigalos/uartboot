#include "software_uart.h"

#ifndef F_CPU
#error Please #define F_CPU
#endif

// Internal computing macros
#define BIT_TIME_NANOSEC 1000000000UL / FIXED_BAUD_RATE
#define ONE_CLOCK_CYCLE_NANOSEC (1000000000UL / F_CPU)
#define CLOCK_CYCLES_PER_FULL_WAIT_LOOP 3 // using 3cc for each iteration
#define PRESCALE_WAIT_ONE_BIT                       \
    ((BIT_TIME_NANOSEC / ONE_CLOCK_CYCLE_NANOSEC) / \
     (CLOCK_CYCLES_PER_FULL_WAIT_LOOP))

#define PRESCALE_WAIT_ONE_BIT_RX_NO_OFFSET PRESCALE_WAIT_ONE_BIT

#define PRESCALE_WAIT_ONE_BIT_RX PRESCALE_WAIT_ONE_BIT
#define PRESCALE_WAIT_HALF_BIT_RX \
    (static_cast<uint8_t>(PRESCALE_WAIT_ONE_BIT_RX)) / 2

#define INSTRUCTION_OFFSET_TX \
    7 // clock cycles needed  before starting ; this represents the number of bytes in the prologue
#define PRESCALE_WAIT_ONE_BIT_TX (PRESCALE_WAIT_ONE_BIT - INSTRUCTION_OFFSET_TX)

#if PRESCALE_WAIT_ONE_BIT_RX == 0
#ifdef F_CPU
#error PRESCALE_WAIT_ONE_BIT_RX is 0. Try decreasing the Baudrate.
#endif
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#if defined(RX_PIN) || defined(TX_PIN)
void uart_init()
{
#ifdef TX_PIN
    UART_DDR |= (1 << TX_PIN);
    UART_OUT_PORT_MAPPING |= (1 << TX_PIN); // Tx line high when idle
#endif

#ifdef RX_PIN
    UART_DDR &= ~(1 << RX_PIN);
#endif
}

#endif // #if defined (RX_PIN) || defined (TX_PIN)

#ifdef RX_PIN

uint8_t uart_read()
{
    uint8_t temporary = 0;
    uint8_t readValue = 0;
    uint8_t bitPosition = 0x40;

    __asm__ volatile(

        "wait: \n\t"
        "sbic %[uart_in_port_mapping], %[rx_pin]\n\t"
        "rjmp wait \n\t"
        "rcall halfDelay \n\t"

        "sbic %[uart_in_port_mapping], %[rx_pin]\n\t"
        "rjmp wait \n\t"
        "rcall bitDelayReceive \n\t"

        "read8bits: \n\t"

        "sbic %[uart_in_port_mapping], %[rx_pin]\n\t" // 2cc true, 1cc false
        "rjmp setBit\n\t"
        "rjmp skipBitSet\n\t"

        "setBit:   \n\t"
        "ori %[read_value], 0x80\n\t"

        "skipBitSet: \n\t"
        "rcall bitDelayReceive \n\t"

        "lsr %[read_value] \n\t"
        "lsr %[bit_position] \n\t"

        "breq eof_read8bits \n\t"
        "rjmp read8bits \n\t"

        // delay routines

        "halfDelay: \n\t"
        "ldi %[temporary], %[prescale_wait_half_bit] \n\t"
        "rjmp loop_3cc \n\t"
        "bitDelayReceive: \n\t"
        "mov %[temporary], %[prescale_wait_one_bit_rx] \n\t"
        "loop_3cc: \n\t"
        "dec %[temporary] \n\t" // 1cc
        "brne loop_3cc \n\t"    // 2cc (true), 1cc (false)
        "ret \n\t"

        // done
        "eof_read8bits: \n\t"

        : [ read_value ] "=&r"(readValue)
        : [ rx_pin ] "M"(RX_PIN),
          [ temporary ] "r"(temporary),
          [ bit_position ] "r"(bitPosition),
          [ prescale_wait_half_bit ] "M"(PRESCALE_WAIT_HALF_BIT_RX),
          [ prescale_wait_one_bit_rx ] "r"(PRESCALE_WAIT_ONE_BIT_RX),
          [ uart_in_port_mapping ] "M"(_SFR_IO_ADDR(UART_IN_PORT_MAPPING))

    );
    return readValue;
}
#endif // #ifdef RX_PIN

#ifdef TX_PIN
void uart_write(uint8_t value)
{
    uint8_t temporary = 0, bitsRemaining = 8; // 8 bits
    __asm__ volatile(

        "cbi %[uart_out_port_mapping], %[tx_pin] \n\t" // falling edge : start condition
        "rcall bitDelaySend \n\t"

        "write8bits: \n\t"
        "mov %[temporary], %[value] \n\t"
        "andi %[value], %[constant_0x01] \n\t"
        "breq setPinLow \n\t"                          // and == 0, meaning LSB was 0, set bit low
        "sbi %[uart_out_port_mapping], %[tx_pin] \n\t" // and == 1, meaning LSB was 1, set bit high
        "rjmp pinSetFinished \n\t"                     // .. and jump to pinSetFinished

        "setPinLow: \n\t"
        "cbi %[uart_out_port_mapping], %[tx_pin] \n\t"
        "nop \n\t" // additional no operation to balance the clock cycle count
                   // when taking the previous branches
        "pinSetFinished:"
        "mov %[value], %[temporary] \n\t"
        "lsr %[value] \n\t"
        "rcall bitDelaySend \n\t"
        "dec %[bits_remaining] \n\t"
        "brne write8bits \n\t"
        "rjmp eof_write8bits \n\t"

        "bitDelaySend: \n\t"
        "mov %[temporary], %[prescale_wait_one_bit_tx] \n\t"
        "repeat_3cc: \n\t"
        "dec %[temporary] \n\t" // 1cc
        "brne repeat_3cc \n\t"  // 2cc (true), 1cc (false)
        "ret \n\t"

        // done
        "eof_write8bits: \n\t"
        "sbi %[uart_out_port_mapping], %[tx_pin] \n\t" // stop bit
        "rcall bitDelaySend \n\t"
        :
        : [ tx_pin ] "M"(TX_PIN),
          [ value ] "r"(value),
          [ constant_0x01 ] "M"(0x01),
          [ bits_remaining ] "r"(bitsRemaining),
          [ uart_out_port_mapping ] "M"(_SFR_IO_ADDR(UART_OUT_PORT_MAPPING)),
          [ temporary ] "r"(temporary),
          [ prescale_wait_one_bit_tx ] "r"(PRESCALE_WAIT_ONE_BIT_TX)

    );
}
#endif // #ifdef TX_PIN