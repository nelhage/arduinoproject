#define BAUD 19200

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
int uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

static FILE uart_input;
static FILE uart_output;

void uart_setup_stdio() {
    uart_init();
    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&uart_input, NULL, uart_getchar, _FDEV_SETUP_READ);
    stdout = &uart_output;
    stdin  = &uart_input;
}

int main (void)
{
    uart_setup_stdio();
    while(1) {
        putc('A', &uart_output);
        fflush(&uart_output);
    }

    return 1;
}

