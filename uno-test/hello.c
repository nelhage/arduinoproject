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

static uint8_t adc_read(uint8_t channel)
{
  ADMUX = (ADMUX & ~0x0f) | (channel & 0x0f);
  ADCSRA |= _BV(ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return ADCH;
}

int main(void)
{
  DDRB |= _BV(PB5) | _BV(PB1) | _BV(PB0);

  uart_init();
  uart_setup_stdio(NULL, &uart_output);
  stdout = &uart_output;

  adc_init();

  _delay_ms(2000);

  while (1) {
    PORTB |= _BV(PB1);
    _delay_us(100);
    uint8_t r = adc_read(0);
    uint8_t g = adc_read(1);
    uint8_t b = adc_read(2);
    PORTB &= ~_BV(PB1);

    for (int i = 0; i < NLED; i++) {
      leds[i].r = r;
      leds[i].g = g;
      leds[i].b = b;
    }
    lightit(leds, NLED);

    _delay_ms(50);
  }

  return 0;
}
