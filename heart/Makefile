ART = art
EXTRA_SRC=../lib/fast_hsv2rgb_8bit.c $(ART).c
LIBS = ../lib/lights.o $(patsubst %.c,%.o,$(EXTRA_SRC))

all: heart.hex
heart.o: CFLAGS+=-std=c11

WASM_OUT := web/wasm
$(WASM_OUT)/heart.js $(WASM_OUT)/heart.wasm: wasm_iface.c $(EXTRA_SRC)
	mkdir -p "${WASM_OUT}"
	emcc -I ../lib/inc/ \
	     -o "${WASM_OUT}/heart.js" \
	     $^ \
	     -sEXPORTED_FUNCTIONS=_tick,_num_lights,_colors \
	     -O2

wasm: $(WASM_OUT)/heart.wasm
.PHONY: wasm

include ../lib/Makefile.common
