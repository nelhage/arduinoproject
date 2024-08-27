#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "art.h"

struct light leds[NLED];
uint8_t mode;

int main(void)
{
    DDRB |= _BV(PINB0);

    DDRB &= ~(_BV(PINB1) | _BV(PINB2));
    // set pullup
    PORTB |= (_BV(PINB1) | _BV(PINB2));

    uint16_t t = 0;

    for(;;) {
        t++;

        tick((PINB & (0x3 << 1)) >> 1, t);
        lightit(leds, NLED);
        _delay_ms(50);
    }

    return 0;
}
