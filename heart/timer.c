#include "art.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "fast_hsv2rgb.h"

void tick(uint8_t mode, uint16_t t) {
    memset(leds, 0, 3*NLED);

    uint16_t secs = t/20;
    int i = secs % NLED;
    leds[i].r = 255;
    leds[i].g = 255;
    leds[i].b = 255;

    i = (i + NLED/2)%NLED;
    leds[i].r = 255;
    leds[i].g = 255;
    leds[i].b = 255;
}
