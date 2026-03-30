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

    DIDR0 |= _BV(ADC5D) | _BV(ADC4D) | _BV(ADC3D) | _BV(ADC2D) | _BV(ADC1D) | _BV(ADC0D);
    DIDR1 |= _BV(AIN1D) | _BV(AIN0D);
}

void setup_watchdog(uint8_t prescaler) {
    // Prescaler values: 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms, 6=1s, 7=2s, 8=4s, 9=8s
    uint8_t wdtcsr_val = (1 << WDIE) | (prescaler & 7) | ((prescaler & 8) ? (1 << WDP3) : 0);

    MCUSR &= ~(1 << WDRF);

    // Timed sequence: new value must be written within 4 cycles of
    // setting WDCE|WDE. Use inline asm (mirroring avr-libc's
    // wdt_enable) to guarantee the timing.
    __asm__ __volatile__ (
        "in __tmp_reg__,__SREG__" "\n\t"
        "cli" "\n\t"
        "wdr" "\n\t"
        "sts %0, %1" "\n\t"
        "out __SREG__,__tmp_reg__" "\n\t"
        "sts %0, %2" "\n\t"
        : /* no outputs */
        : "M" (_SFR_MEM_ADDR(WDTCSR)),
          "r" ((uint8_t)((1 << WDCE) | (1 << WDE))),
          "r" (wdtcsr_val)
        : "r0"
    );
}

void sleep_tick(void) {
    setup_watchdog(2);

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
        sleep_tick();
    }

    return 0;
}
