#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "serial.h"

volatile uint16_t clk;
ISR(TIMER0_COMPA_vect)
{
    clk++;
}

void init_clk() {
    TCCR0A = _BV(WGM01);
    OCR0A  = 50;
    /* prescaler = 64 */
    TCCR0B = _BV(CS01) | _BV(CS00);

    TIMSK0 |= _BV(OCIE0A);
}

static FILE uart_out, uart_in;

int main(void) {
    DDRB = 0xff;
    uart_init();
    uart_setup_stdio(&uart_in, &uart_out);
    init_clk();
    sei();

    while (1) {
        if (clk >= 5000) {
            clk = 0;
            fprintf(&uart_out, ".");
        }
    }

    return 0;
}
