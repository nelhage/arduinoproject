#include "art.h"
#include <stdlib.h>

#include "fast_hsv2rgb.h"


void rainbow(int16_t t) {
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

void rainbow_ranges(int16_t t) {
    int phase = t;

    int hi = t >> 7;
    int band = hi % 6;

    int COLOR_RANGE = HSV_HUE_STEPS / 6;
    int COLOR_MIN = HSV_HUE_STEPS * band / 6;

    int nslice = 12;
    int col_per_slice = NCOLOR/nslice;

    for (int i = 0; i < NCOLOR/2; i++) {
        int logical_pos = (i + phase) % NCOLOR;

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
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
        colors[NCOLOR-1-i].r = r;
        colors[NCOLOR-1-i].g = g;
        colors[NCOLOR-1-i].b = b;
    }
}

void purple(int16_t t) {
    int phase=t;

    for (int i = 0; i < NCOLOR/2; i++) {
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
        colors[NCOLOR-1-i].r = 207; // light violet
        colors[NCOLOR-1-i].g = 159;
        colors[NCOLOR-1-i].b = 255;
    }

}


void purple_but_with_hsv(int16_t t) {
    int phase = t;
    // just bisecting this to try to find where the purple is
    int PURPLE_RANGE = HSV_HUE_STEPS / 24;
    int PURPLE_MIN = HSV_HUE_STEPS * 17 / 24;

    for (int i = 0; i < NCOLOR/2; i++) {

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
        colors[NCOLOR-1-i].r = r;
        colors[NCOLOR-1-i].g = g;
        colors[NCOLOR-1-i].b = b;
    }
}

void symmetric_rainbow(int16_t t) {
    static int phase = 0;
    int hi = t >> 7;
    int v = (hi & 3) - 1;
    if (v < 0) {
        v = - v;
    }
    phase += 10 * v ;

    for (int i = 0; i < NCOLOR/2; i++) {
        int hue = (i * (HSV_HUE_MAX*2/NCOLOR) - phase) % HSV_HUE_MAX;
        if (hue < 0) {
            hue += HSV_HUE_MAX;
        }
        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, 255, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
        colors[NCOLOR-1-i].r = r;
        colors[NCOLOR-1-i].g = g;
        colors[NCOLOR-1-i].b = b;
    }
}

// void twinkles(int16_t t) {
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

struct sparkle_state {
    uint8_t prev : 1;
    uint8_t cur : 1;
};

struct sparkle_state is_sparkling[NCOLOR];

static inline int lerp(int from, int to, int nstep, int step) {
    return from + (((to - from) * step) / nstep);
}

void nipunn(int16_t t) {
    const int hue_a = 0;
    const int hue_b = HSV_HUE_STEPS / 2;
    const int sparkle_nframes = 5;
    const int fade_nsparkles = 50;
    const int pause_nsparkles = 5;

    const int transition_nsparkles = fade_nsparkles + pause_nsparkles;


    int frame_in_sparkle = t % sparkle_nframes;
    int sparkle_no = (t / sparkle_nframes) % transition_nsparkles;
    int transition_no = t / (sparkle_nframes * transition_nsparkles);

    int frac_on = sparkle_no * sparkle_no;

    if (frame_in_sparkle == 0) {
        for (int i = 0; i < NCOLOR; i++) {
            is_sparkling[i].prev = sparkle_no == 0 ? 0 : is_sparkling[i].cur;
            is_sparkling[i].cur = (rand() % (fade_nsparkles*fade_nsparkles)) < frac_on;
        }
    }

    for (int i = 0; i < NCOLOR; i++) {
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

            val = 255;
            hue = lerp(prev_hue, dst_hue, sparkle_nframes, frame_in_sparkle);
            break;
        }
        }

        uint8_t r, g, b;
        fast_hsv2rgb_8bit((uint16_t)hue, 255, val, &r, &g, &b);
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
    }
}

void tick(uint8_t mode, int16_t t) {
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
