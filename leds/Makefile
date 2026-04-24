all: leds.hex

include ../lib/Makefile.common

leds.elf: $(LIB_BUILD)/lights.o automaton.o
leds.o: CFLAGS+=-std=c11
