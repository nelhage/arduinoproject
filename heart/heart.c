#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "art.h"

ISR(WDT_vect) {
}

struct light leds[NLED];
uint8_t mode;

void lowpower() {
    PRR |= (_BV(PRTWI) | _BV(PRTIM0) | _BV(PRTIM1)
            | _BV(PRTIM2) | _BV(PRSPI) | _BV(PRUSART0)
            | _BV(PRADC));

    DIDR0 |= _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC1D);
    DIDR1 |= _BV(AIN1D) | _BV(AIN0D);
}

void setup_watchdog(uint8_t prescaler) {
    // Prescaler values: 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1s, 7=2s, 8=4s, 9=8s

    MCUSR &= ~(1 << WDRF);
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    WDTCSR = (1 << WDIE) | (prescaler & 7) | ((prescaler & 8) ? (1 << WDP3) : 0);
}

void sleep_tick(void) {
    setup_watchdog(5);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    sei();
    sleep_mode();
    cli();

    wdt_disable();
}

int main(void)
{
    DDRD  &= 0x3;
    PORTD  = 0xfc;

    DDRC  = 0;
    PORTC = 0xff;

    DDRB = _BV(PINB0);
    // DDRB &= ~(_BV(PINB1) | _BV(PINB2));
    // set pullup
    PORTB = 0xfe;

    lowpower();

    uint16_t t = 0;

    for(;;) {
        t++;

        tick((PINB & (0x3 << 1)) >> 1, t);
        lightit(leds, NLED);
        // sleep_tick();
        _delay_ms(50);
    }

    return 0;
}
