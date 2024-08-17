#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "art.h"


int main(void)
{
    DDRB |= _BV(PINB0);

    int t = 0;

    for(;;) {
        t++;

        tick(t);
        lightit(colors, sizeof(colors));
        _delay_ms(100);
    }

    return 0;
}
