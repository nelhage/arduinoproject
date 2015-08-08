#include <stdint.h>

#ifdef AUTOMATON_SELFTEST
#include <stdio.h>
#include <string.h>
#define debug printf
#else
#define debug(...)
#endif

#if 1
#undef debug
#define debug(...)
#endif

void step_automaton(uint8_t automaton,
                    uint8_t *in,
                    uint8_t *out,
                    int len) {
    uint8_t state = in[0] << 1;
    int i;
    for (i = 0; i < len; i++) {
        uint8_t o = 0;
        int j;
        for (j = 0; j < 8; j++) {
            if (j == 1) {
                state |= in[i] & 0xF0;
            }
            if (j == 5 && i != len - 1) {
                state |= in[i+1] << 4;
            }

            uint8_t bits = (state & 7);

            debug("i=%d j=%d bits=%d on=%d\n", i, j, bits, !!(automaton & (1 << bits)));

            if ((automaton & (1 << bits))) {
                o |= 1 << j;
            }
            state >>= 1;
        }
        debug("i=%d in=%x out=%x\n", i, in[i], o);
        out[i] = o;
    }
}

#ifdef AUTOMATON_SELFTEST
#define N 8

int main(void) {
    uint8_t in[N], out[N];
    memset(in, 0, sizeof(in));
    in[0] = 0x55;
    uint8_t *a = in, *b = out;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 8*N; j++) {
            printf("%c", (a[j/8] & (1<<(j%8))) ? 'X' : ' ');
        }
        printf("\n");
        step_automaton(110, a, b, N);
        uint8_t *t = a;
        a = b; b = t;
    }
    return 0;
}
#endif
