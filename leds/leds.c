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

extern void lightit(uint8_t *data, uint16_t cnt);
void step_automaton(uint8_t automaton, uint8_t *in, uint8_t *out, int len);

#define N 120

uint8_t colors[3 * N];
uint8_t a[(N+7)/8], b[(N+7)/8];
struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color = {
    50, 0, 0
};

int main(void)
{
    DDRB |= _BV(PINB0);

    // pin7 (D7) = input, pullup
    DDRD &= ~_BV(PIND7);
    PORTD |= _BV(PIND7);

    a[0] = 0x55;

    uint8_t *in = a, *out = b;
    uint8_t automaton = 110;

    while(1) {
        step_automaton(automaton, in, out, sizeof(a));

        for (int j = 0; j < 10; j++) {
            for (int i=0; i<N; i++) {
                int old = in[i/8] & (1 << (i%8));
                int new = out[i/8] & (1 << (i%8));

                colors[3*i+0] = 0;
                colors[3*i+1] = 0;
                colors[3*i+2] = 0;

                if (new) {
                    colors[3*i+0] += (color.g * j) / 9;
                    colors[3*i+1] += (color.r * j) / 9;
                    colors[3*i+2] += (color.b * j) / 9;
                }
                if (old) {
                    colors[3*i+0] += (color.g * (9-j)) / 9;
                    colors[3*i+1] += (color.r * (9-j)) / 9;
                    colors[3*i+2] += (color.b * (9-j)) / 9;
                }
            }

            lightit(colors, sizeof(colors)/3);
        }

        _delay_ms(100);

        uint8_t *t = in;
        in = out; out = t;
    }
    return 0;
}
