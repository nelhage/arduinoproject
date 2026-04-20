# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Dev environment

Toolchain is pinned via Nix flake + direnv (`.envrc` runs `use flake`). `nix develop` (or entering the directory with direnv enabled) provides `avr-gcc`, `avrdude`, and `emscripten`. No other package manager is used.

## Build / flash workflow

Every project subdirectory (`blink/`, `heart/`, `leds/`, `serial/`, `gongbot/`, `timer/`, `uno-test/`) has its own tiny Makefile that includes `../lib/Makefile.common`. The common Makefile defines the generic `%.o → %.elf → %.hex` and `upload-%` rules; per-project Makefiles only set `all:`, declare extra objects to link (`foo.elf: ../lib/serial.o …`), and add per-file flags.

From any project subdir:

- `make` — builds `<name>.hex`
- `make upload-<name>` — kills any `screen -S arduino` session, then flashes via `avrdude -c arduino`
- `make clean` — project adds to `EXTRA_CLEAN_FILES` for cross-dir artifacts (see `gongbot/Makefile`)

### PROFILE (board selection)

`lib/Makefile.common` takes `PROFILE=diecimila` (default) or `PROFILE=uno`. It controls `MCU_TARGET`, the avrdude `-p` part, the bootloader baud rate, and the macOS device glob (`/dev/tty.usbserial-*` for FTDI Diecimila vs `/dev/tty.usbmodem*` for Uno's 16U2). Override per-invocation (`make PROFILE=uno upload-foo`) or set `PROFILE = uno` at the top of a project's Makefile (see `uno-test/Makefile`).

Device auto-detection: `DEVICE` is set from the profile's glob on macOS and hardcoded to `/dev/ttyUSB0` on Linux. Override with `make DEVICE=… upload-…` if needed.

### Serial console

Connect with `screen -S arduino /dev/tty.usbmodem… <baud>` (exit `Ctrl-A k`). Using the `arduino` session name matters — `upload-%` kills that session before flashing so the port isn't held open.

## Shared library (`lib/`)

- `serial.c` + `inc/serial.h` — UART0 helpers. `inc/serial.h` defines a `uart_init()` that reads `BAUD` (must be `#define`d before including) and uses `<util/setbaud.h>`. `uart_setup_stdio` wires `FILE` streams for `printf`/`getc`.
- `lights.S` + `inc/lights.h` — hand-written AVR assembly for bit-banging addressable LEDs; used by `heart/` and `leds/`.
- `fast_hsv2rgb_8bit.c` + `inc/fast_hsv2rgb.h` — HSV→RGB conversion for LED effects.

## `heart/` — dual-target (AVR + WASM)

`heart/` is unusual: the same art code compiles to both AVR firmware and a WebAssembly module for browser preview.

- The `ART` Make variable (default `art`) selects which `<name>.c` file provides the art routines. A `.which_art` sentinel file forces a rebuild when `ART` changes. Existing art files: `art.c`, `k8r.c`, `rainbow.c`, `unscruz.c`, `toggle.c`, `powertest.c`.
- `make` → AVR `.hex` as usual.
- `make wasm` → uses `emcc` to build `web/wasm/heart.{js,wasm}` from `wasm_iface.c` + the selected art + `fast_hsv2rgb_8bit.c`, exporting `_tick`, `_num_lights`, `_leds`. `web/index.html` + `web/main.js` render the LEDs in a browser.

When adding a new art file, it must implement the same entry points the existing art files expose (check `art.h` and `wasm_iface.c` for the contract).
