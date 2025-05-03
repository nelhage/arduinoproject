#include "art.h"
#include <stdlib.h>

#include <stdio.h>

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

void rainbow_ranges(uint16_t t) {
    int phase = t / 2;

    int hi = t >> 9;
    int band = hi % 6;

    int COLOR_RANGE = HSV_HUE_STEPS / 6;
    int COLOR_MIN = HSV_HUE_STEPS * band / 6;

    int nslice = 12;
    int col_per_slice = NLED/nslice;

    for (int i = 0; i < NLED/2; i++) {
        int logical_pos = (i + phase) % NLED;

        int my_slice = logical_pos / col_per_slice;
        int my_off = (logical_pos % col_per_slice);
        if (my_slice % 2 == 1) {
            my_off = col_per_slice - 1 - my_off;
        }

        int hue_offset = (my_off * COLOR_RANGE) / col_per_slice;
        hue_offset = (hue_offset) % COLOR_RANGE;

        if (hue_offset < 0) {
            hue_offset += COLOR_RANGE;
        }

        int hue = hue_offset + COLOR_MIN;

        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
        leds[NLED-1-i].r = r;
        leds[NLED-1-i].g = g;
        leds[NLED-1-i].b = b;
    }
}

void symmetric_rainbow(uint16_t t) {
    static int phase = 0;
    int hi = t >> 7;
    int v = (hi & 3) - 1;
    if (v < 0) {
        v = - v;
    }
    phase += 10 * v ;

    for (int i = 0; i < NLED/2; i++) {
        int hue = (i * (HSV_HUE_MAX*2/NLED) - phase) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
        leds[NLED-1-i].r = r;
        leds[NLED-1-i].g = g;
        leds[NLED-1-i].b = b;
    }
}

// void twinkles(uint16_t t) {
//     int phase = t;
//     // phase += 10;

//     for (int i = 0; i < NLED; i++) {
//         int off = (i + 2*t) % NLED;
//         if (off < 0) {
//             off += NLED;
//         }
//         int hue = ((off) * (HSV_HUE_MAX/(NLED*5)) + phase) % HSV_HUE_MAX;
//         if (hue < 0) {
//             hue += HSV_HUE_MAX;
//         }
//         uint8_t r, g, b;
//         fast_hsv2rgb_8bit((uint16_t)hue, 255, 128, &r, &g, &b);
//         leds[i].r = r;
//         leds[i].g = g;
//         leds[i].b = b;
//     }
// }

struct sparkle_state {
    uint8_t prev : 1;
    uint8_t cur : 1;
};

struct sparkle_state is_sparkling[NLED];

struct light hsv2rgb(uint16_t hue, uint8_t sat, uint8_t val) {
    struct light out;
    fast_hsv2rgb_8bit(hue, sat, val, &out.r, &out.g, &out.b);
    return out;
}

void nipunn(uint16_t t) {
    const int fade_nsparkles = 30;
    const int pause_nsparkles = 3;
    const int base_sparkle_nframes = 20;
    const int transition_nsparkles = fade_nsparkles + pause_nsparkles;

    static uint16_t hue_a = 0;
    static uint16_t hue_b = (2 * HSV_HUE_STEPS) / 3;
    static int frame_in_sparkle = 0;
    static int sparkle_no = 0;
    static int transition_no = 0;

    int sparkle_nframes = transition_no == 0 ? base_sparkle_nframes : 2*base_sparkle_nframes;

    int frac_on = sparkle_no * sparkle_no;

    if (frame_in_sparkle == 0) {
        for (int i = 0; i < NLED; i++) {
            is_sparkling[i].prev = sparkle_no == 0 ? 0 : is_sparkling[i].cur;
            is_sparkling[i].cur = (rand() % (fade_nsparkles*fade_nsparkles)) < frac_on;
        }
    }

    const uint8_t val = 255;
    struct light color_a = hsv2rgb(hue_a, 255, val);
    struct light color_b = hsv2rgb(hue_b, 255, val);
    struct light black = {0, 0, 0};

    for (int i = 0; i < NLED; i++) {
        int cur = is_sparkling[i].cur;
        int prev = is_sparkling[i].prev;

        struct light prev_color;
        struct light cur_color;

        if (transition_no == 0) {
            prev_color = prev ? color_a : black;
            cur_color = cur ? color_a : black;
        } else {
            prev_color = prev ? color_b : color_a;
            cur_color = cur ? color_b : color_a;
        }

        leds[i] = lerp3(prev_color, cur_color, sparkle_nframes, frame_in_sparkle);
        //  leds[i] = color_a;
    }

    frame_in_sparkle++;
    if (frame_in_sparkle == sparkle_nframes) {
        frame_in_sparkle = 0;
        sparkle_no++;
        if (sparkle_no == transition_nsparkles) {
            sparkle_no = 0;
            transition_no = transition_no ? 0 : 1;
            if (transition_no == 0) {
                hue_a = (hue_a + HSV_HUE_STEPS / 8) % HSV_HUE_STEPS;
                hue_b = (hue_b + HSV_HUE_STEPS / 8) % HSV_HUE_STEPS;
            }
        }
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
        //purple_but_with_hsv(t);
        //purple(t);
        rainbow_ranges(t);
        //symmetric_rainbow(t);
    } else if ((mode & 3) == 2) {
        ourainbowrous(t);
    }  else if ((mode & 3) == 1) {
        nipunn(t);
    }
}
