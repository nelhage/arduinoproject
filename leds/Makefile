all: leds.hex
leds.elf: ../lib/lights.o automaton.o
leds.o: CFLAGS+=-std=c11

include ../lib/Makefile.common
