#define BAUD 9600

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

static FILE uart_output;

int main(void)
{
  DDRB |= _BV(PB5);

  uart_init();
  uart_setup_stdio(NULL, &uart_output);
  stdout = &uart_output;

  unsigned long i = 0;
  while (1) {
    PORTB ^= _BV(PB5);
    printf("hello, world! %lu\n", i++);
    _delay_ms(500);
  }

  return 0;
}
