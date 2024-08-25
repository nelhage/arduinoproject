#include <avr/io.h>
#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void tick(uint8_t mode, int t) {
    for (int i = 0; i < NLED; i++) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
        if ((mode & 1) == 0) {
            leds[i].r = 255;
        }
        if ((mode & 2) == 0) {
            leds[i].g = 255;
        }
    }
}
