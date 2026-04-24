PROFILE = uno

all: hello.hex

include ../lib/Makefile.common

LIBS := $(LIB_BUILD)/lights.o $(LIB_BUILD)/fast_hsv2rgb_8bit.o

hello.elf: $(LIB_BUILD)/serial.o $(LIBS)
.PRECIOUS: hello.o
