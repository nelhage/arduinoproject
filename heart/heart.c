#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

struct light {
    uint8_t g, r, b;
};

extern void lightit(struct light *data, uint16_t cnt);
void step_automaton(uint8_t automaton, uint8_t *in, uint8_t *out, int len);

uint8_t sin_table[256] = {
128, 127, 127, 127, 127, 127, 126, 126, 126, 125, 124, 124, 123, 122, 122, 121, 120, 119, 118, 117, 116, 115, 114, 112, 111, 110, 109, 107, 106, 104, 103, 101, 100, 98, 97, 95, 94, 92, 90, 89, 87, 85, 84, 82, 80, 79, 77, 75, 74, 72, 70, 69, 67, 65, 64, 62, 60, 59, 57, 56, 54, 52, 51, 49, 48, 46, 45, 43, 42, 41, 39, 38, 36, 35, 34, 32, 31, 30, 29, 28, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 14, 13, 12, 11, 11, 10, 9, 8, 8, 7, 7, 6, 6, 5, 4, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 6, 6, 7, 7, 8, 8, 9, 10, 11, 11, 12, 13, 14, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31, 32, 34, 35, 36, 38, 39, 41, 42, 43, 45, 46, 48, 49, 51, 52, 54, 56, 57, 59, 60, 62, 64, 65, 67, 69, 70, 72, 74, 75, 77, 79, 80, 82, 84, 85, 87, 89, 90, 92, 94, 95, 97, 98, 100, 101, 103, 104, 106, 107, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 121, 122, 122, 123, 124, 124, 125, 126, 126, 126, 127, 127, 127, 127, 127
};

struct arrow {
    int x;
    int dx;
    int8_t r,g,b;
};

#define NCOLOR 120
#define NDOT   2

struct light colors[NCOLOR];
struct arrow arrows[NDOT] = {
    {
        0, 2,
        5, 1, 10,
    },
    {
        60, -1,
        -5, 10, 1,
    }
};

void saturating_add(uint8_t *dst, uint8_t add) {
    if (*dst + add < *dst) {
        *dst = 0xff;
    } else {
        *dst += add;
    }
}

int main(void)
{
    DDRB |= _BV(PINB0);

    // pin7 (D7) = input, pullup
    DDRD &= ~_BV(PIND7);
    PORTD |= _BV(PIND7);
    uint8_t t = 0;

    for(;;) {
        t++;
        for (int i = 0; i < NCOLOR; i++) {
            uint8_t scale = sin_table[t];
            colors[i].r = 64 + sin_table[scale] / 2;
            colors[i].g = 0;
            colors[i].b = 16 + sin_table[scale] / 16;
        }
        for (int i = 0; i < NDOT; i++) {
            struct arrow *a = &arrows[i];
            for (int j = 0; j < 10; j++) {
                saturating_add(&colors[(NCOLOR + a->x - j) % NCOLOR].r, a->r*(10 - j));
                saturating_add(&colors[(NCOLOR + a->x - j) % NCOLOR].g, a->g*(10 - j));
                saturating_add(&colors[(NCOLOR + a->x - j) % NCOLOR].b, a->b*(10 - j));
            }
            a->x += a->dx;
            a->x %= NCOLOR;
            if (a->x < 0) {
                a->x += NCOLOR;
            }
        }
        lightit(colors, sizeof(colors));
        _delay_ms(100);
    }

    return 0;
}
