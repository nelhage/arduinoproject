#include "art.h"
#include <stdlib.h>

void mode_rainbow_ranges(uint16_t t, uint8_t s1, uint8_t s2) {
    int phase = t / 2;

    int COLOR_RANGE = HSV_HUE_STEPS / 6;
    int COLOR_MIN = (s1 * HSV_HUE_STEPS)/256;

    uint8_t v = s2;

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
        struct light color = hsv2rgb(hue, 255, v);

        leds[i] = color;
        leds[NLED-1-i] = color;
    }
}

void mode_ourainbowrous(uint16_t t, uint8_t s1, uint8_t s2) {
    int phase = t;
    static int state = 0; // (t*t*t)/8;
    int speed = 100 + s1 * 20;

    state += speed;
    // phase += 10;

    uint16_t off = state / 512;

    for (int i = 0; i < NLED; i++) {
        /*
        uint16_t clamped_t = t % 3000;
        uint16_t off = (clamped_t * clamped_t)/512;
        */

        uint16_t logical_pos = (i + off) % NLED;

        uint16_t hue = (2 * logical_pos * (HSV_HUE_MAX/(NLED*5)) + phase) % HSV_HUE_MAX;

        leds[i] = hsv2rgb((uint16_t)hue, 255, s2);
    }
}

struct sparkle_state {
    uint8_t prev : 1;
    uint8_t cur : 1;
};

struct sparkle_state is_sparkling[NLED];

enum transition {
    BLACK_TO_A,
    A_TO_B,
    B_TO_BLACK,
    N_TRANSITIONS,
};

void mode_nipunn(uint16_t t, uint8_t s1, uint8_t s2) {
    const int fade_nsparkles = 20;
    const int pause_nsparkles = 2;
    const int base_sparkle_nframes = 20;
    const int transition_nsparkles = fade_nsparkles + pause_nsparkles;

    uint16_t hue_adjust = ((uint16_t)s1)*6;

    static uint16_t hue_a = 0;
    static uint16_t hue_b = (2 * HSV_HUE_STEPS) / 3;
    static int frame_in_sparkle = 0;
    static int sparkle_no = 0;
    static enum transition cur_transition = 0;

    int sparkle_nframes;
    switch (cur_transition) {
    case BLACK_TO_A:
        sparkle_nframes = base_sparkle_nframes;
        break;
    case A_TO_B:
        sparkle_nframes = 2*base_sparkle_nframes;
        break;
    case B_TO_BLACK:
        sparkle_nframes = base_sparkle_nframes/4;
        break;
    default:
        __builtin_unreachable();
    }

    int frac_on = sparkle_no * sparkle_no;

    if (frame_in_sparkle == 0) {
        for (int i = 0; i < NLED; i++) {
            is_sparkling[i].prev = sparkle_no == 0 ? 0 : is_sparkling[i].cur;
            is_sparkling[i].cur = (rand() % (fade_nsparkles*fade_nsparkles)) < frac_on;
        }
    }

    const uint8_t val = s2 >> 1;
    struct light color_a =
        hsv2rgb((hue_a+hue_adjust)%HSV_HUE_MAX, 255, val);
    struct light color_b =
        hsv2rgb((hue_b+hue_adjust)%HSV_HUE_MAX, 255, val);
    struct light black = {0, 0, 0};

    struct light from, to;

    switch (cur_transition) {
    case BLACK_TO_A:
        from = black;
        to = color_a;
        break;
    case A_TO_B:
        from = color_a;
        to = color_b;
        break;
    case B_TO_BLACK:
        from = color_b;
        to = black;
        break;
    default:
        __builtin_unreachable();
    }


    for (int i = 0; i < NLED; i++) {
        int cur = is_sparkling[i].cur;
        int prev = is_sparkling[i].prev;
        struct light prev_color = prev ? to : from;
        struct light cur_color = cur ? to : from;


        leds[i] = lerp3(prev_color, cur_color, sparkle_nframes, frame_in_sparkle);
        //  leds[i] = color_a;
    }

    frame_in_sparkle++;
    if (frame_in_sparkle == sparkle_nframes) {
        frame_in_sparkle = 0;
        sparkle_no++;
        if (sparkle_no == transition_nsparkles) {
            sparkle_no = 0;
            cur_transition += 1;
            if (cur_transition == N_TRANSITIONS) {
                cur_transition = BLACK_TO_A;
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
