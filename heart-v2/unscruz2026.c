#include "art.h"
#include <stdlib.h>

void mode_rainbow_ranges(uint16_t t, uint8_t s1, uint8_t s2) {
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

void mode_ourainbowrous(uint16_t t, uint8_t s1, uint8_t s2) {
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

struct sparkle_state {
    uint8_t prev : 1;
    uint8_t cur : 1;
};

struct sparkle_state is_sparkling[NLED];

void mode_nipunn(uint16_t t, uint8_t s1, uint8_t s2) {
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

    const uint8_t val = 128;
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

void mode_hv(uint16_t t, uint8_t s1, uint8_t s2) {
    uint16_t h = ((uint16_t)s1)*(HSV_HUE_STEPS/HSV_HUE_SEXTANT);
    struct light rgb = hsv2rgb(h, 255, s2);

    for (int i = 0; i < NLED; i++) {
        leds[i] = rgb;
    }
}

const int16_t N_MODES = 4;

void tick(uint16_t t, uint8_t s0, uint8_t s1, uint8_t s2) {
    int16_t mode = (s0 * N_MODES) / 256;
    switch (mode) {
    case 0:
        mode_hv(t, s1, s2);
        break;
    case 1:
        mode_rainbow_ranges(t, s1, s2);
        break;
    case 2:
        mode_ourainbowrous(t, s1, s2);
        break;
    case 3:
        mode_nipunn(t, s1, s2);
        break;
    }
}
