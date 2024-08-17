#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "art.h"


int main(void)
{
    DDRB |= _BV(PINB0);

    uint8_t t = 0;

    for(;;) {
        t++;

        lightit(colors, sizeof(colors));
        _delay_ms(100);
    }

    return 0;
}
