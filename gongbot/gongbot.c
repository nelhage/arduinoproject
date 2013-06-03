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

volatile uint8_t adc_ready = 0;

unsigned short adc_read(unsigned char pin)
{
    uint16_t adc;
    while(!adc_ready);

    /* must read the low ADC byte before the high ADC byte */
    adc = ADC;
    adc_ready = 0;

    return adc;
}

static complex_t bfly_buff[FFT_N];
static union {
    int16_t capture[FFT_N];
    uint16_t spectrum[FFT_N/2];
} fft;

static FILE uart_out, uart_in;

ISR(ADC_vect)
{
    adc_ready = 1;
}

ISR(TIMER1_COMPB_vect)
{
    // adc_ready = 1;
}

volatile uint16_t clk;

int main(void) {
    DDRB = 0xff;
    uart_init();
    uart_setup_stdio(&uart_in, &uart_out);
    adc_init();

    int i;

    while (1) {
        for (i = 0; i < FFT_N; ++i) {
            fft.capture[i] = adc_read(0) - (1 << 15);
        }

        /*
        fft_input(fft.capture, bfly_buff);
        fft_execute(bfly_buff);
        fft_output(bfly_buff, fft.spectrum);
        */

        fgetc(&uart_in);
        for (i = 0; i < FFT_N; ++i)
            fprintf(&uart_out, "%4x ", fft.capture[i]);
        fprintf(&uart_out, "\n");
    }

    return 0;
}
