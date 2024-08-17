#!/usr/bin/env bash
set -eu
OUT=web/wasm

mkdir -p "${OUT}"
emcc art.c wasm_iface.c \
     -I ../lib/inc/ \
     -o "${OUT}/heart.js" \
     -sEXPORTED_FUNCTIONS=_tick,_num_lights,_colors \
     -O2
