#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void tick(uint8_t mode, int16_t t) {
    for (int i = 0; i < NCOLOR; i++) {
        switch ((t >> 7) & 0x3) {
        case 0:
            colors[i].r = 255;
            colors[i].g = 255;
            colors[i].b = 255;
            break;
        case 1:
            colors[i].r = 255;
            colors[i].g = 0;
            colors[i].b = 0;
            break;
        case 2:
            colors[i].r = 0;
            colors[i].g = 0;
            colors[i].b = 0;
            break;
        }
    }
}
