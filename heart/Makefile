all: heart.hex
heart.elf: ../lib/lights.o art.o ../lib/fast_hsv2rgb_8bit.o
heart.o: CFLAGS+=-std=c11

include ../lib/Makefile.common
