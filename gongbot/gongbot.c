#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "serial.h"
#include "fft/ffft.h"

void adc_init()
{
    OCR1AL = 50;
    OCR1BL = 49;
    /* prescaler = 64, CTC mode */
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
    TIMSK1 = _BV(OCIE1B);

    /* internal pull-ups interfere with the ADC. disable the
     * pull-up on the pin if it's being used for ADC. either
     * writing 0 to the port register or setting it to output
     * should be enough to disable pull-ups. */
    PORTC = 0x00;
    DDRC = 0x00;
    /* unless otherwise configured, arduinos use the internal Vcc
     * reference. MUX 0x0f samples the ground (0.0V). */
    ADMUX = _BV(REFS0) | _BV(ADLAR) | 0x00;
    /*
     * Enable the ADC system, use 128 as the clock divider on a 16MHz
     * arduino (ADC needs a 50 - 200kHz clock) and start a sample. the
     * AVR needs to do some set-up the first time the ADC is used; this
     * first, discarded, sample primes the system for later use.
     */
    ADCSRB  = _BV(ADTS2) | _BV(ADTS0);
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADATE);
    ADCSRA |= _BV(ADEN);
    ADCSRA |= _BV(ADIE);
    sei();
    ADCSRA |= _BV(ADSC);
    /* wait for the ADC to return a sample */
    // loop_until_bit_is_clear(ADCSRA, ADSC);
}

static complex_t bfly_buff[FFT_N];
static struct {
    int16_t capture[FFT_N];
    uint8_t next;
    uint16_t spectrum[FFT_N/2];
} fft;

static FILE uart_out, uart_in;


ISR(ADC_vect)
{
    fft.capture[fft.next++ % FFT_N] = ADC - (1 << 15);
}

ISR(TIMER1_COMPB_vect) {}

volatile uint16_t clk;

int main(void) {
    DDRB = 0xff;
    uart_init();
    uart_setup_stdio(&uart_in, &uart_out);
    adc_init();

    int i;

    while (1) {
        fgetc(&uart_in);

        cli();
        fft_input(fft.capture, bfly_buff);
        sei();
        fft_execute(bfly_buff);
        fft_output(bfly_buff, fft.spectrum);

        for (i = 0; i < FFT_N/2; ++i)
            fprintf(&uart_out, "%4x ", fft.spectrum[i]);
        fprintf(&uart_out, "\n");
    }

    return 0;
}
