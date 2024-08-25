#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void tick(uint8_t mode, uint16_t t) {
    for (int i = 0; i < NLED; i++) {
        switch ((t >> 7) & 0x3) {
        case 0:
            leds[i].r = 255;
            leds[i].g = 255;
            leds[i].b = 255;
            break;
        case 1:
            leds[i].r = 255;
            leds[i].g = 0;
            leds[i].b = 0;
            break;
        case 2:
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
            break;
        }
    }
}
