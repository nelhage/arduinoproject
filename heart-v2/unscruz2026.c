#include "art.h"
#include "fast_hsv2rgb.h"

void tick(uint16_t t, uint8_t s0, uint8_t s1, uint8_t s2) {
    uint16_t h = ((uint16_t)s1)*(HSV_HUE_STEPS/HSV_HUE_SEXTANT);
    struct light rgb;
    fast_hsv2rgb_8bit(h, 255, s2, &rgb.r, &rgb.g, &rgb.b);


    for (int i = 0; i < NLED; i++) {
        leds[i] = rgb;
    }
}
