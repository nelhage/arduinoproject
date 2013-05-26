#define BAUD 19200

#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

static FILE uart_input;
static FILE uart_output;

int main (void)
{
    uart_init();
    uart_setup_stdio(&uart_input, &uart_output);
    stdout = &uart_output;
    stdin  = &uart_input;
    while(1) {
        int c = getc(stdin);
        putc(c, stdout);
    }

    return 1;
}

