#include <avr/io.h>
#include <util/delay.h>

/*
 *
 * clock = 16Mhz = 62.5 ns / clock
 * data on PB0 = pin 8
 *
 *
 * send square wave; each bit is high / low
 *
 * T0H = 350ns  ± 150ns  = 5.6 cycles
 * T1H = 700ns           = 11.2 cycles
 * T0L = 800ns           = 12.8 cycles
 * T1L = 600ns           = 9.6 cycles
 * TRESET = >50us
 *
 * GRB; MSB first
 */

extern void lightit(uint8_t *data, uint8_t cnt);
uint8_t colors[8*3];
/*= {
    // G  R     B
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x80, 0x80, 0x80,
    0xc0, 0xc0, 0xc0,
    0xff, 0xff, 0xff,
};
*/

int main(void)
{
    DDRB = 0xFF;
    while(1) {
        uint8_t i;
        for (i = 0; i != 255; ++i) {
            int b;
            for (b = 0; b < 8; b++) {
                if (i & (1<<b)) {
                    colors[3*b + 0] = 0xff;
                    colors[3*b + 1] = 0xff;
                    colors[3*b + 2] = 0xff;
                } else {
                    colors[3*b + 0] = 0x00;
                    colors[3*b + 1] = 0x00;
                    colors[3*b + 2] = 0x00;
                }
            }
            lightit(colors, sizeof(colors));
            _delay_ms(500);
        }
    }
    return 0;
}
