#include "art.h"
#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#include "fast_hsv2rgb.h"

int8_t led_x[NLED] = {
    0,   -6,  -13,  -19,  -25,  -32,  -38,  -44,  -50,  -56,  -62,
    -68,  -73,  -78,  -83,  -88,  -93,  -97, -101, -105, -109, -112,
    -115, -118, -120, -122, -124, -125, -126, -126, -127, -126, -126,
    -125, -123, -122, -120, -117, -114, -111, -107, -103,  -99,  -95,
    -90,  -84,  -79,  -73,  -67,  -61,  -55,  -48,  -42,  -35,  -29,
    -22,  -16,  -11,   -6,   -2,    0,    2,    6,   11,   16,   22,
    29,   35,   42,   48,   55,   61,   67,   73,   79,   84,   90,
    95,   99,  103,  107,  111,  114,  117,  120,  122,  123,  125,
    126,  126,  127,  126,  126,  125,  124,  122,  120,  118,  115,
    112,  109,  105,  101,   97,   93,   88,   83,   78,   73,   68,
    62,   56,   50,   44,   38,   32,   25,   19,   13,    6
};

const int16_t BLACKOUT_HUES = 120;
const int16_t BLACKOUT_BUFFER = 40;

void sidewipe(uint16_t t) {
    const uint16_t PERIOD = 256;

    int16_t local_t = (int16_t)((t>>2) % PERIOD);
    int16_t tphase;
    if (local_t < PERIOD/2) {
        tphase = lerp(0, 255, PERIOD/2, local_t);
    } else {
        tphase = lerp(255, 0, PERIOD/2, local_t - PERIOD/2);
    }

    /*
    static uint16_t hues[NLED];
    static uint8_t vals[NLED];
    */

    for (int i = 0; i < NLED; i++) {
        int16_t x = led_x[i];

        int16_t phase = ((8*x)/10 + tphase + 256/3);
        int16_t hue = (phase * (HSV_HUE_MAX/255)) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        uint8_t v = 255;
        uint8_t in_sextant = (((uint16_t)hue) & 0xff);
        if (in_sextant < BLACKOUT_HUES) {
            v = 0;
        }
        // hues[i] = hue;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, v, &r, &g, &b);
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
    }

    /*
    const int BLACK_LEDS = 2;
    const int FADE_LEDS = 5;

    memset(vals, 255, NLED);

    int prev_segment = 0;
    for (int i = 0; i < NLED; i++) {
        int segment = hues[i] >> 8;
        if (i != 0 && segment != prev_segment) {
            for (int j = 0; j < (FADE_LEDS + BLACK_LEDS); j++) {
                int v;
                if (j < BLACK_LEDS) {
                    v = 0;
                } else {
                    v = lerp(0, 255, FADE_LEDS, j-BLACK_LEDS);
                }
                if (i - j >= 0) {
                    vals[i-j] = v;
                }
                if (i + j < NLED) {
                    vals[i+j] = v;
                }
            }
            i += FADE_LEDS;
        }
        prev_segment = segment;
    }

    for (int i = 0; i < NLED; i++) {
        fast_hsv2rgb_8bit(hues[i], 255, vals[i],
                          &leds[i].r, &leds[i].g, &leds[i].b);
    }
    */
}

void tick(uint8_t mode, uint16_t t) {

    sidewipe(t);

    /*
    if ((mode & 3) == 3) {
        //purple_but_with_hsv(t);
        //purple(t);
        rainbow_ranges(t);
        //symmetric_rainbow(t);
    } else if ((mode & 3) == 2) {
        ourainbowrous(t);
    }  else if ((mode & 3) == 1) {
        nipunn(t);
    }
    */
}
