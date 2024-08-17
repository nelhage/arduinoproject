#include <stdlib.h>
#include <stdint.h>

struct light {
    uint8_t g, r, b;
};

extern void lightit(struct light *data, uint16_t cnt);
