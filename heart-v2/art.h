#include "lights.h"
#include "fast_hsv2rgb.h"

#define NLED 120

extern struct light leds[NLED];

void tick(uint16_t t, uint8_t s0, uint8_t s1, uint8_t s2);


static inline int16_t lerp(int16_t from, int16_t to, int16_t nstep, int16_t step) {
    return from + (((to - from) * step) / (nstep-1));
}

static inline struct light lerp3(struct light from, struct light to, int16_t nstep, int16_t step) {
    return (struct light){
        .r = lerp(from.r, to.r, nstep, step),
        .g = lerp(from.g, to.g, nstep, step),
        .b = lerp(from.b, to.b, nstep, step),
    };
}

static inline struct light hsv2rgb(uint16_t hue, uint8_t sat, uint8_t val) {
    struct light out;
    fast_hsv2rgb_8bit(hue, sat, val, &out.r, &out.g, &out.b);
    return out;
}
