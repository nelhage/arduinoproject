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
 * T1H = 700ns           = 11.2 cycles
 * T0L = 800ns           = 12.8 cycles
 * T1L = 600ns           = 9.6 cycles
 * TRESET = >50us
 *
 * GRB; MSB first
 */

extern void lightit(uint8_t *data, uint8_t cnt);
uint8_t colors[3 * 60];

int main(void)
{
    DDRB |= _BV(PINB0);

    // pin7 (D7) = input, pullup
    DDRD &= ~_BV(PIND7);
    PORTD |= _BV(PIND7);

    int i;
    int offs = 0;
    while(1) {
        if (!(PIND & _BV(PIND7))) {
            for (i=0; i<60; i++) {
                colors[3*i+0] = colors[3*i+1] = colors[3*i+2] = 0;
            }
        } else {
            for (i=0; i<60; i++) {
                colors[3*i+0] = (i+offs);
                colors[3*i+1] = (60-i-offs);
                colors[3*i+2] = 0;
            }
        }
        lightit(colors, sizeof(colors));
        _delay_ms(10);
        offs++;
    }
    return 0;
}
