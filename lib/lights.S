#include <avr/io.h>

/*
 *
 * clock = 16Mhz = 62.5 ns / clock
 * data on PB0 = pin 8
 *
 *
 * send square wave; each bit is high / low
 *
 * T0H = 350ns  ± 150ns  = 5.6 cycles
 * T1H = 700ns           = 11.2 cycles
 * T0L = 800ns           = 12.8 cycles
 * T1L = 600ns           = 9.6 cycles
 * TRESET = >50us
 *
 * GRB; MSB first
 */

.globl lightit
lightit:
        // lightit(ptr, count)
        // ptr   == r24:r25
        // count == r22:r23

        // X = ptr
        mov r26, r24
        mov r27, r25
outer:
        ldi r19, 3
pixel:
        ld  r18, X+
        ldi r20, 8
byte:
        sbi _SFR_IO_ADDR(PORTB), 0    // 2     HIGH

        // zero = 6 ; one = 11
        ldi r19, 0      // 1
        lsl r18         // 1
        brcc high0      // 1/2

        ldi r19, 1      // 1
        nop             // 1
        nop             // 1
        nop             // 1
        nop             // 1
        nop             // 1

high0:
        cbi _SFR_IO_ADDR(PORTB), 0    // 2      LOW

        // zero = 12 ; one = 9
        and r19, r19    // 1
        brne low1       // 1/2

        nop             // 1
        nop             // 1
        nop             // 1
        nop             // 1

low1:
        nop             // 1
        nop             // 1

        dec r20         // 1
        brne byte       // 1/2

        dec r19
        brne pixel

        dec r22
        brne outer

        ret
