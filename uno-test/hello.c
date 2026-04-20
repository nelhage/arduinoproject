#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

static FILE uart_output;

static void adc_init(void)
{
  ADMUX = _BV(REFS0) | _BV(ADLAR);
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

static uint8_t adc_read(void)
{
  ADCSRA |= _BV(ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return ADCH;
}

int main(void)
{
  DDRB |= _BV(PB5);

  uart_init();
  uart_setup_stdio(NULL, &uart_output);
  stdout = &uart_output;

  adc_init();

  while (1) {
    PORTB ^= _BV(PB5);
    printf("ADC0: %u\n", adc_read());
    _delay_ms(1000);
  }

  return 0;
}
