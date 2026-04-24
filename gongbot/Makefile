all: gongbot.hex

include ../lib/Makefile.common

CFLAGS  += -mcall-prologues

gongbot.elf: $(LIB_BUILD)/serial.o fft/ffft.o

EXTRA_CLEAN_FILES=*/ffft.o

gongbot.o: fft/ffft.h
fft/ffft.o: fft/ffft.h
