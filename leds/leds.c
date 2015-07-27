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

extern void lightit(uint32_t rgb);

int main(void)
{
    DDRB = 0xFF;
    uint8_t i = 0;
    while(1) {
        lightit((((uint32_t)i) << 16) |
                (((uint32_t)i) << 8) |
                (((uint32_t)i) << 0));
        i++;
        _delay_ms(100);
    }
    return 0;
}
