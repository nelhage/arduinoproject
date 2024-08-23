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

void rainbow_ranges(int t) {
    int phase = 2 * t;

    int hi = t >> 7;
    int band = hi % 6;

    int COLOR_RANGE = HSV_HUE_STEPS / 6;
    int COLOR_MIN = HSV_HUE_STEPS * band / 6;

    for (int i = 0; i <= NCOLOR/2; i++) {

        int hue_offset = (i * (COLOR_RANGE* 8/NCOLOR) + phase) % COLOR_RANGE;
        if (hue_offset < 0) {
            hue_offset += COLOR_RANGE;
        }

        int hue = hue_offset + COLOR_MIN;

        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
        if (i != 0) {
            colors[NCOLOR-i].r = r;
            colors[NCOLOR-i].g = g;
            colors[NCOLOR-i].b = b;
        }
    }
}

void purple(int t) {
    int phase=t;

    for (int i = 0; i <= NCOLOR/2; i++) {
        int r;
        // r wants to range from 150 to 200, back and forth
        int R_MIN = 150;
        int R_RANGE = 50;
        int r_offset = (i * ((R_RANGE*4)/NCOLOR)) % R_RANGE;

        r = (uint8_t)(R_MIN + r_offset);
        // OK SO CLEARLY these values are definitely not the r, g, and b values the led shows 
        colors[i].r = 112; // byzantium
        colors[i].g = 41;
        colors[i].b = 99;
        colors[NCOLOR-i].r = 207; // light violet
        colors[NCOLOR-i].g = 159;
        colors[NCOLOR-i].b = 255;
    }

}


void purple_but_with_hsv(int t) {
    int phase = t;
    // just bisecting this to try to find where the purple is
    int PURPLE_RANGE = HSV_HUE_STEPS / 24;
    int PURPLE_MIN = HSV_HUE_STEPS * 17 / 24;

    for (int i = 0; i <= NCOLOR/2; i++) {

        int hue_offset = (i * (PURPLE_RANGE* 6/NCOLOR) + phase) % PURPLE_RANGE;
        if (hue_offset < 0) {
            hue_offset += PURPLE_RANGE;
        }

        int hue = hue_offset + PURPLE_MIN;

        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
        if (i != 0) {
            colors[NCOLOR-i].r = r;
            colors[NCOLOR-i].g = g;
            colors[NCOLOR-i].b = b;
        }
    }
}

void symmetric_rainbow(int t) {
    static int phase = 0;
    int hi = t >> 7;
    int v = (hi & 3) - 1;
    if (v < 0) {
        v = - v;
    }
    phase += 10 * v ;

    for (int i = 0; i <= NCOLOR/2; i++) {
        int hue = (i * (HSV_HUE_MAX*2/NCOLOR) - phase) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
        colors[NCOLOR-i].r = r;
        colors[NCOLOR-i].g = g;
        colors[NCOLOR-i].b = b;
    }
}

// void twinkles(int t) {
//     int phase = t;
//     // phase += 10;

//     for (int i = 0; i < NCOLOR; i++) {
//         int off = (i + 2*t) % NCOLOR;
//         if (off < 0) {
//             off += NCOLOR;
//         }
//         int hue = ((off) * (HSV_HUE_MAX/(NCOLOR*5)) + phase) % HSV_HUE_MAX;
//         if (hue < 0) {
//             hue += HSV_HUE_MAX;
//         }
//         uint8_t r, g, b;
//         fast_hsv2rgb_8bit((uint16_t)hue, 255, 128, &r, &g, &b);
//         colors[i].r = r;
//         colors[i].g = g;
//         colors[i].b = b;
//     }
// }

void tick(uint8_t mode, int t) {
    if ((mode & 3) == 3) {
        //purple_but_with_hsv(t);
        //purple(t);
        rainbow_ranges(t);
        //symmetric_rainbow(t);
    } else if ((mode & 3) == 2) {
        purple_but_with_hsv(t);
        //twinkles(t);
    }
}
