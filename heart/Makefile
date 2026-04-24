all: heart.hex

include ../lib/Makefile.common

ART = art
EXTRA_SRC = ../lib/fast_hsv2rgb_8bit.c $(ART).c
LIBS = $(LIB_BUILD)/lights.o $(LIB_BUILD)/fast_hsv2rgb_8bit.o $(ART).o

heart.elf: $(LIBS) $(STAMPS)/ART
heart.o: CFLAGS+=-std=c11

WASM_OUT := web/wasm
$(WASM_OUT)/heart.js $(WASM_OUT)/heart.wasm: wasm_iface.c $(EXTRA_SRC) $(STAMPS)/ART
	mkdir -p "${WASM_OUT}"
	emcc -I ../lib/inc/ \
	     -o "${WASM_OUT}/heart.js" \
	     $(filter-out $($(STAMPS)/%),$^) \
	     -sEXPORTED_FUNCTIONS=_tick,_num_lights,_leds \
	     -O2

wasm: $(WASM_OUT)/heart.wasm
