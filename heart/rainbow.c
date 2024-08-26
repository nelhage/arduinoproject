#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void rainbow(uint16_t t) {
    static int phase = 0;
    int hi = t >> 7;
    int v = (hi & 3) - 1;
    if (hi & 4) {
        v = - v;
    }
    phase += 10 * v;

    for (int i = 0; i < NLED; i++) {
        int hue = (i * (HSV_HUE_MAX/NLED) + phase) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
    }
}

void ourainbowrous(uint16_t t) {
    int phase = t; // (t*t*t)/8;
    // phase += 10;

    for (int i = 0; i < NLED; i++) {
        uint16_t clamped_t = t % 3000;
        uint16_t off = (clamped_t * clamped_t)/512;

        uint16_t logical_pos = (i + off) % NLED;

        uint16_t hue = (2 * logical_pos * (HSV_HUE_MAX/(NLED*5)) + phase) % HSV_HUE_MAX;

        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 128, &r, &g, &b);
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
    }
}

void tick(uint8_t mode, uint16_t t) {
    if ((mode & 3) == 3) {
        rainbow(t);
    } else if ((mode & 3) == 2) {
        ourainbowrous(t);
    }
}
