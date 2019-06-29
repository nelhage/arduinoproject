#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

struct light {
    uint8_t g, r, b;
};

extern void lightit(struct light *data, uint16_t cnt);
void step_automaton(uint8_t automaton, uint8_t *in, uint8_t *out, int len);

uint8_t sin_table[256] = {
255, 255, 255, 255, 255, 254, 253, 253, 252, 251, 249, 248, 247, 245, 244, 242, 240, 238, 237, 234, 232, 230, 228, 225, 223, 220, 218, 215, 212, 209, 206, 203, 200, 197, 194, 191, 188, 185, 181, 178, 175, 171, 168, 165, 161, 158, 155, 151, 148, 145, 141, 138, 135, 131, 128, 125, 121, 118, 115, 112, 108, 105, 102, 99, 96, 93, 90, 87, 84, 82, 79, 76, 73, 71, 68, 65, 63, 60, 58, 56, 53, 51, 49, 47, 45, 42,
40, 38, 37, 35, 33, 31, 29, 28, 26, 25, 23, 22, 20, 19, 17, 16, 15, 14, 13, 12, 10, 9, 9, 8, 7, 6, 5, 5, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7,
8, 9, 9, 10, 12, 13, 14, 15, 16, 17, 19, 20, 22, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 42, 45, 47, 49, 51, 53, 56, 58, 60, 63, 65, 68, 71, 73, 76, 79, 82, 84, 87, 90, 93, 96, 99, 102, 105, 108, 112, 115, 118, 121, 125, 128, 131, 135, 138, 141, 145, 148, 151, 155, 158, 161, 165, 168, 171, 175, 178, 181, 185, 188, 191, 194, 197, 200, 203, 206, 209, 212, 215, 218, 220, 223, 225, 228, 230, 232, 234, 237, 238, 240, 242, 244, 245, 247, 248, 249, 251, 252, 253, 253, 254, 255, 255, 255, 255
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
