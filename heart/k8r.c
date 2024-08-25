#include "art.h"
#include <stdlib.h>

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

    int hi = t >> 7;
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

void purple(uint16_t t) {
    int phase=t;

    for (int i = 0; i < NLED/2; i++) {
        int r;
        // r wants to range from 150 to 200, back and forth
        int R_MIN = 150;
        int R_RANGE = 50;
        int r_offset = (i * ((R_RANGE*4)/NLED)) % R_RANGE;

        r = (uint8_t)(R_MIN + r_offset);
        // OK SO CLEARLY these values are definitely not the r, g, and b values the led shows
        leds[i].r = 112; // byzantium
        leds[i].g = 41;
        leds[i].b = 99;
        leds[NLED-1-i].r = 207; // light violet
        leds[NLED-1-i].g = 159;
        leds[NLED-1-i].b = 255;
    }

}


void purple_but_with_hsv(uint16_t t) {
    int phase = t;
    // just bisecting this to try to find where the purple is
    int PURPLE_RANGE = HSV_HUE_STEPS / 24;
    int PURPLE_MIN = HSV_HUE_STEPS * 17 / 24;

    for (int i = 0; i < NLED/2; i++) {

        int hue_offset = (i * (PURPLE_RANGE* 6/NLED) + phase) % PURPLE_RANGE;
        if (hue_offset < 0) {
            hue_offset += PURPLE_RANGE;
        }

        int hue = hue_offset + PURPLE_MIN;

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

static inline int lerp(int from, int to, int nstep, int step) {
    return from + (((to - from) * step) / (nstep-1));
}

void nipunn(uint16_t t) {
    const int fade_nsparkles = 30;
    const int pause_nsparkles = 3;
    const int base_sparkle_nframes = 20;
    const int transition_nsparkles = fade_nsparkles + pause_nsparkles;

    static int hue_a = 0;
    static int hue_b = (2 * HSV_HUE_STEPS) / 3;
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

    for (int i = 0; i < NLED; i++) {
        int cur = is_sparkling[i].cur;
        int prev = is_sparkling[i].prev;

        int hue;
        int val;
        switch (transition_no % 2) {
        case 0: {
            int prev_val = prev ? 255 : 0;
            int dst_val = cur ? 255 : 0;

            hue = hue_a;
            val = lerp(prev_val, dst_val, sparkle_nframes, frame_in_sparkle);
            break;
        }
        case 1: {
            int prev_hue = prev ? hue_b : hue_a;
            int dst_hue = cur ? hue_b : hue_a;
            if (frame_in_sparkle < sparkle_nframes / 2) {
                hue = prev_hue;
                val = lerp(255, 0, sparkle_nframes/2, frame_in_sparkle);
            } else {
                hue = dst_hue;
                val = lerp(0, 255, sparkle_nframes/2, frame_in_sparkle - sparkle_nframes/2);
            }
            if (prev == cur) {
                val = 255;
            }

            /*
            val = 255;
            hue = lerp(prev_hue, dst_hue, sparkle_nframes, frame_in_sparkle);
            */
            break;
        }
        }

        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, val, &r, &g, &b);
        leds[i].r = r;
        leds[i].g = g;
        leds[i].b = b;
    }

    frame_in_sparkle++;
    if (frame_in_sparkle == sparkle_nframes) {
        frame_in_sparkle = 0;
        sparkle_no++;
        if (sparkle_no == transition_nsparkles) {
            sparkle_no = 0;
            transition_no = transition_no ? 0 : 1;
            if (transition_no == 0) {
                hue_a = (hue_a + HSV_HUE_STEPS / 6) % HSV_HUE_STEPS;
                hue_b = (hue_b + HSV_HUE_STEPS / 6) % HSV_HUE_STEPS;
            }
        }
    }
}

void tick(uint8_t mode, uint16_t t) {
    if ((mode & 3) == 3) {
        //purple_but_with_hsv(t);
        //purple(t);
        rainbow_ranges(t);
        //symmetric_rainbow(t);
    } else if ((mode & 3) == 2) {
        purple_but_with_hsv(t);
    }  else if ((mode & 3) == 1) {
        nipunn(t);
    }
}
