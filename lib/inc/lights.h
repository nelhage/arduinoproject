#include <stdint.h>

struct light {
    uint8_t g, r, b;
};

extern void _lightit(struct light *data, uint16_t cnt);


inline void lightit(struct light *data, uint16_t n_rgb) {
    _lightit(data, 3*n_rgb);
}
