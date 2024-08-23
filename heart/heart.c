#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "art.h"

struct light colors[NCOLOR];
uint8_t mode;

int main(void)
{
    DDRB |= _BV(PINB0);

    // 0 = PD0, 1 = PD1

    DDRD &= ~(_BV(PIND0) | _BV(PIND1));
    // set pullup
    PORTD |= (_BV(PIND0) | _BV(PIND1));

    int16_t t = 0;

    for(;;) {
        t++;

        tick(PIND & 0x3, t);
        lightit(colors, sizeof(colors));
        _delay_ms(100);
    }

    return 0;
}
