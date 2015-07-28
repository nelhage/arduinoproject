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
uint8_t colors[] = {
    // G  R     B
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    /*
    0x80, 0x80, 0x80,
    0xc0, 0xc0, 0xc0,
    0xff, 0xff, 0xff,
    */
};

int main(void)
{
    DDRB = 0xFF;
    DDRD &= ~_BV(PD2);
    while(1) {
        if (PIND & _BV(PD2)) {
            colors[0] = colors[1] = colors[2] = 0x40;
            colors[3] = colors[4] = colors[5] = 0x00;
        } else {
            colors[0] = colors[1] = colors[2] = 0x00;
            colors[3] = colors[4] = colors[5] = 0x40;
        }
        lightit(colors, sizeof(colors));
        _delay_ms(100);
    }
    return 0;
}
