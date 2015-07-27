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
    0x80, 0x00, 0x00,
    0x70, 0x00, 0x00,
    0x60, 0x00, 0x00,
    0x50, 0x00, 0x00,
    0x40, 0x00, 0x00,
    0x30, 0x00, 0x00,
    0x20, 0x00, 0x00,
    0x10, 0x00, 0x00,
};

int main(void)
{
    DDRB = 0xFF;
    while(1) {
        lightit(colors, sizeof(colors));
        _delay_ms(100);
    }
    return 0;
}
