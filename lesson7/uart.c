#include <stdarg.h>
#include "address.h"

// the UART control registers.
// some have different meanings for
// read vs write.
// see http://byterunner.com/16550.html
#define RHR 0                 // receive holding register (for input bytes)
#define THR 0                 // transmit holding register (for output bytes)
#define IER 1                 // interrupt enable register
#define IER_RX_ENABLE (1<<0)
#define IER_TX_ENABLE (1<<1)
#define FCR 2                 // FIFO control register
#define FCR_FIFO_ENABLE (1<<0)
#define FCR_FIFO_CLEAR (3<<1) // clear the content of the two FIFOs
#define ISR 2                 // interrupt status register
#define LCR 3                 // line control register
#define LCR_EIGHT_BITS (3<<0)
#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LSR 5                 // line status register
#define LSR_RX_READY (1<<0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1<<5)    // THR can accept another character to send

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.
#define Reg(reg) ((volatile unsigned char *)(UART0_REG_BASE + reg))
#define ReadReg(reg) (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))

// add a character to the output buffer and tell the
// UART to start sending if it isn't already.
void uart_putc(char c)
{
    // wait for Transmit Holding Empty to be set in LSR.
    while((ReadReg(LSR) & LSR_TX_IDLE) == 0);

    WriteReg(THR, c);
}

void uart_puts(char *msg)
{
    char c;

    if (!msg) {
        return;
    }

    while ((c = *msg) != '\0') {
        uart_putc(c);
        msg++;
    }
}

//printf functions
static char digits[] = "0123456789abcdef";

static void print_int(int xx, int base, int sign)
{
    char buf[16];
    int i;
    unsigned int x;

    if(sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do {
        buf[i++] = digits[x % base];
    } while((x /= base) != 0);

    if(sign)
        buf[i++] = '-';

    while(--i >= 0)
        uart_putc(buf[i]);
}

static void print_ptr(unsigned long x)
{
    int i;
    uart_putc('0');
    uart_putc('x');

    for (i = 0; i < (sizeof(unsigned long) * 2); i++, x <<= 4)
        uart_putc(digits[x >> (sizeof(unsigned long) * 8 - 4)]);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(char *fmt, ...)
{
    va_list ap;
    int i, c;
    char *s;

    if (fmt == 0)
        return;

    va_start(ap, fmt);
    for(i = 0; (c = fmt[i] & 0xff) != 0; i++) {
        if(c != '%'){
            uart_putc(c);
            continue;
        }

        c = fmt[++i] & 0xff;
        if(c == 0)
            break;

        switch(c){
        case 'd':
            print_int(va_arg(ap, int), 10, 1);
            break;

        case 'x':
            print_int(va_arg(ap, int), 16, 1);
            break;

        case 'p':
            print_ptr(va_arg(ap, unsigned long));
            break;

        case 's':
            if((s = va_arg(ap, char*)) == 0)
                s = "(null)";

            for(; *s; s++)
                uart_putc(*s);
            break;

        case '%':
            uart_putc('%');
            break;

        default:
            // Print unknown % sequence to draw attention.
            uart_putc('%');
            uart_putc(c);
            break;
        }
    }
}
