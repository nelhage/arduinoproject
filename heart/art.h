#include "lights.h"

#define NLED 120

extern struct light leds[NLED];

void tick(uint8_t mode, uint16_t t);


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
