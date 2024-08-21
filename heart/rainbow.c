#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void tick(uint8_t mode, int t) {
    for (int i = 0; i < NCOLOR; i++) {
        uint16_t hue = i * (HSV_HUE_MAX/NCOLOR);
        uint8_t r, g, b;
        fast_hsv2rgb_8bit(hue, 255, 255, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
    }
    /*
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
        int r = rand() % 500;
        if (r < 5) {
            a->dx += rand()%3 - 1;
        } else if (r == 6) {
            a->dx = -a->dx;
        }
        if (a->dx == 0) {
            a->dx = 1;
        }
    }
    */
}
