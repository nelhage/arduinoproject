#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

#include "lights.h"

static FILE uart_output;

#define NLED 120
struct light leds[NLED];

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
  DDRB |= _BV(PB5) | _BV(PB0);

  uart_init();
  uart_setup_stdio(NULL, &uart_output);
  stdout = &uart_output;

  adc_init();

  int i;
  for (i = 0; i < 10; i ++) {
      leds[i].r = 100;
  }
  lightit(leds, NLED);

  while (1) {
    PORTB ^= _BV(PB5);
    printf("ADC0: %u\n", adc_read());
    _delay_ms(1000);
  }

  return 0;
}
