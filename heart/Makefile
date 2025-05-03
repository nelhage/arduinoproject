include ../lib/Makefile.common

ART = art
EXTRA_SRC=../lib/fast_hsv2rgb_8bit.c $(ART).c
LIBS = ../lib/lights.o $(patsubst %.c,%.o,$(EXTRA_SRC))

WHICH_ART := .which_art
dummy := $(shell if test -f $(WHICH_ART) && [ "$$(cat $(WHICH_ART))" == "$(ART)" ]; then :; else echo "$(ART)" > $(WHICH_ART); fi)

all: heart.hex
heart.elf: $(LIBS) .which_art
# heart.elf:: .which_art
heart.o: CFLAGS+=-std=c11

WASM_OUT := web/wasm
$(WASM_OUT)/heart.js $(WASM_OUT)/heart.wasm: wasm_iface.c $(EXTRA_SRC) .which_art
	mkdir -p "${WASM_OUT}"
	emcc -I ../lib/inc/ \
	     -o "${WASM_OUT}/heart.js" \
	     $(filter-out $(WHICH_ART),$^) \
	     -sEXPORTED_FUNCTIONS=_tick,_num_lights,_leds \
	     -O2

wasm: $(WASM_OUT)/heart.wasm
