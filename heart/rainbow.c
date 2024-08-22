#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void rainbow(int t) {
    static int phase = 0;
    int hi = t >> 7;
    int v = (hi & 3) - 1;
    if (hi & 4) {
        v = - v;
    }
    phase += 10 * v;

    for (int i = 0; i < NCOLOR; i++) {
        int hue = (i * (HSV_HUE_MAX/NCOLOR) + phase) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
    }
}

void twinkles(int t) {
    int phase = t;
    // phase += 10;

    for (int i = 0; i < NCOLOR; i++) {
        int off = (i + 2*t) % NCOLOR;
        if (off < 0) {
            off += NCOLOR;
        }
        int hue = ((off) * (HSV_HUE_MAX/(NCOLOR*5)) + phase) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 128, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
    }
}

void tick(uint8_t mode, int t) {
    if ((mode & 3) == 3) {
        rainbow(t);
    } else if ((mode & 3) == 2) {
        twinkles(t);
    }
}
