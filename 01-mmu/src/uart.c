#include <stdint.h>

#include "uart.h"

#define UART0  NS16550A_UART0_CTRL_ADDR 
#define RBR (UART0 + 0) // Reciever Buffer Register (read only)
#define THR (UART0 + 0) // Transmitter Holding Register (write only)
#define IER (UART0 + 1) // Interrupt Enable Register
#define IIR (UART0 + 2) // Intrrupt Ident Register (read only)
#define FCR (UART0 + 2) // FIFO control register (write only)
#define LCR (UART0 + 3) // Line control register
#define MCR (UART0 + 4) // MODEM control register
#define LSR (UART0 + 5) // Line status register
#define MSR (UART0 + 6) // MODEM Status Register

#define DDL (UART0 + 0)
#define DLM (UART0 + 1)

#define DR_MASK     (1 << 0)  // Data Ready bit (in LSR) mask
#define THR_MASK    (1 << 5)  // Transmitter holding register bit (in LSR) mask


/*
 * Procedure:   mmio_write
 * -----------------------
 * This inline procedure writes [c] to the register with address [reg]
 *
 * @uint64_t reg:   The 64-bit address of the register.
 * @char c:      The character (or byte) that is to be written to the register.
 *
 */
static inline void mmio_write(uint64_t reg, char c) {
    *(volatile char*)reg = c;
}

/*
 * Function:   mmio_read
 * ---------------------
 * This inline function returns the contents of the register with address [reg]
 *
 * @uint64_t reg:   The 64-bit address of the register.
 *
 * @returns:        The value / contents of the register with address [reg]
 *
 */
static inline char mmio_read(uint64_t reg) {
    return *(volatile char*)reg;
}

/*
 * Procedure:   uart_init
 * ----------------------
 * This procedure performs the initialization for QEMU's UART controller.
 * From the inspection of the riscv64-virt.dts, we note that the UART controller's MMIO section
 * has a base address of 0x10000000 and a length/limit of 0x100.
 *
 * We also note that it's clock frequency is 0x384000 and is compatible with ns16550a.
 * To initialize this type of controller, we first set the baud rate using the
 * divisor mechanism.
 *
 * We then set the word length and enable FIFO (first in, first out).
 *
 */
void uart_init() {
    // Disable UART interrupts.
    mmio_write(IER, 0x00);

    // Set the DLAB (divisor latch access bit) to 1 (bit 7)
    // This allows us to set the baud rate.
    mmio_write(LCR, 0x80);

    // According to the specification, the divisor is given by
    //      divisor = ceil( (clock_hz) / (2^4 * baud_rate) )
    // From our dts file, we note that clock_hz = 3686400 Hz.
    // We define our baud_rate as baud_rate = 2^15 = 32768.
    // Hence divisor = ceil ( 3686400  / 2^19 ) = ceil ( 7.03125 ) = 8
    // The divisor is 16-bits, with the least significant in DDL
    // and the most significant in DLM.

    mmio_write(DDL, 0x08);
    mmio_write(DLM, 0x00);

    // We now set word length to 8 bit with no parity
    // and unset the DLAB bit.
    mmio_write(LCR, 0x03);

    // and enable the FIFO, which is bit 0 of FCR.
    mmio_write(FCR, 0x01);

    // Enable reciever buffer interrupt, bit 0 of the IER.
    mmio_write(IER, 0x01);
}
//void uart_init() {
//    uart = (uint8_t *)(void *)NS16550A_UART0_CTRL_ADDR;
//    uint32_t uart_freq = UART0_CLOCK_FREQ;
//    uint32_t baud_rate = UART0_BAUD_RATE;
//    uint32_t divisor = uart_freq / (16 * baud_rate);
//    uart[UART_LCR] = UART_LCR_DLAB;
//    uart[UART_DLL] = divisor & 0xff;
//    uart[UART_DLM] = (divisor >> 8) & 0xff;
//    uart[UART_LCR] = UART_LCR_PODD | UART_LCR_8BIT;
//}
//
//static int ns16550a_putchar(int ch) {
//    while ((uart[UART_LSR] & UART_LSR_RI) == 0)
//        ;
//    return uart[UART_THR] = ch & 0xff;
//}

//void uart_send(char c) { ns16550a_putchar(c); }
void uart_send(char c) {
    while ((mmio_read(LSR) & THR_MASK) == 0) ;
    mmio_write(THR, c);
}

void print_s(const char *s) {
    while (*s != '\0') {
        /* convert newline to carrige return + newline */
        if (*s == '\n') uart_send('\r');
        uart_send(*s++);
    }
}

void print_c(char c) { uart_send(c); }

void print_i(unsigned long int x) {
    if (x < 0) {
        print_c('-');
        x = -x;
    }
    if (x >= 10) print_i(x / 10);
    print_c(x % 10 + '0');
}

void print_h(unsigned long int d) {
    unsigned int n;
    int c;
    for (c = 28; c >= 0; c -= 4) {
        // get highest tetrad
        n = (d >> c) & 0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n += n > 9 ? 0x37 : 0x30;
        uart_send(n);
    }
}
