#include "art.h"
#include <stdlib.h>
#include <stdio.h>

struct light leds[NLED];

int num_lights() {
    return NLED;
}
