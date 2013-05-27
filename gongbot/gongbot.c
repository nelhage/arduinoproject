#include <avr/io.h>
#include <avr/pgmspace.h>

#include "serial.h"
#include "fft/ffft.h"

void adc_init()
{
    /* internal pull-ups interfere with the ADC. disable the
     * pull-up on the pin if it's being used for ADC. either
     * writing 0 to the port register or setting it to output
     * should be enough to disable pull-ups. */
    PORTC = 0x00;
    DDRC = 0x00;
    /* unless otherwise configured, arduinos use the internal Vcc
     * reference. MUX 0x0f samples the ground (0.0V). */
    ADMUX = _BV(REFS0)  | _BV(ADLAR) | 0x00;
    /*
     * Enable the ADC system, use 128 as the clock divider on a 16MHz
     * arduino (ADC needs a 50 - 200kHz clock) and start a sample. the
     * AVR needs to do some set-up the first time the ADC is used; this
     * first, discarded, sample primes the system for later use.
     */
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0) | _BV(ADSC);
    /* wait for the ADC to return a sample */
    loop_until_bit_is_clear(ADCSRA, ADSC);
}

unsigned short adc_read(unsigned char pin)
{
    unsigned char l, h, r;

    r = (ADMUX & 0xf0) | (pin & 0x0f);
    ADMUX = r; /* select the input channel */
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);

    /* must read the low ADC byte before the high ADC byte */
    l = ADCL;
    h = ADCH;

    return ((unsigned short)h << 8) | l;
}

static complex_t bfly_buff[FFT_N];
static union {
    int16_t capture[FFT_N];
    uint16_t spectrum[FFT_N/2];
} fft;

static FILE uart_out, uart_in;

int main(void) {
    uart_init();
    uart_setup_stdio(&uart_in, &uart_out);
    adc_init();

    int i, j;

    for(j = 0; j < 2*FFT_N; ++j) {
        fprintf(&uart_out, "> ");
        fflush(&uart_out);
        fgetc(&uart_in);
        for (i = 0; i < FFT_N; ++i)
            fft.capture[i] = adc_read(0) - (1 << 15);

        fft_input(fft.capture, bfly_buff);
        fft_execute(bfly_buff);
        fft_output(bfly_buff, fft.spectrum);

        for (i = 0; i < FFT_N/2; ++i)
            fprintf(&uart_out, "%d ", fft.spectrum[i]);
        fprintf(&uart_out, "\n");
    }


    return 0;
}
