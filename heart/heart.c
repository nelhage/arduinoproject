#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

struct light {
    uint8_t g, r, b;
};

extern void lightit(struct light *data, uint16_t cnt);
void step_automaton(uint8_t automaton, uint8_t *in, uint8_t *out, int len);

struct light colors[120];

int main(void)
{
    DDRB |= _BV(PINB0);

    // pin7 (D7) = input, pullup
    DDRD &= ~_BV(PIND7);
    PORTD |= _BV(PIND7);

    for(;;) {
        for (int i = 0; i < sizeof(colors)/sizeof(colors[0]); i++) {
            colors[i].r = rand() & 0xff;
            colors[i].g = 0;
            colors[i].b = rand() & 0x3f;
        }
        lightit(colors, sizeof(colors));
        _delay_ms(200);
    }

    return 0;
}
