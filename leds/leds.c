#include <avr/io.h>
#include <util/delay.h>

/*
 *
 * clock = 16Mhz = 62.5 ns / clock
 * data on PB0 = pin 8
 *
 *
 * send square wave; each bit is high / low
 *
 * T0H = 350ns  ± 150ns  = 5.6 cycles
 * T0L = 800ns           = 12.8 cycles
 * T1H = 700ns           = 11.2 cycles
 * T1L = 600ns           = 9.6 cycles
 * TRESET = >50us
 */

extern void lightit();

int main(void)
{
    DDRB = 0xFF;
    while(1) {
        lightit();
        PORTB |= _BV(PB2);
        _delay_us(500);
        PORTB &= ~_BV(PB2);
        _delay_us(500);
    }
    return 0;
}
