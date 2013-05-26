#include <avr/io.h>
#include <stdio.h>

void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char uart_getchar() {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

int uart_putchar_stdio(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r');
    }
    uart_putchar(c);
    return 0;
}
int uart_getchar_stdio(FILE *stream) {
    return uart_getchar();
}

void uart_setup_stdio(FILE *in, FILE *out)
{
    fdev_setup_stream(out, uart_putchar_stdio, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(in, NULL, uart_getchar_stdio, _FDEV_SETUP_READ);
}
