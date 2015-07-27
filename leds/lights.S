#include <avr/io.h>

.macro docolor col
        // r18 = color data
        // r20 = counter
        // r19 = true if high
        ldi r20, 8
top\col:
        sbi _SFR_IO_ADDR(PORTB), 0    // 2     HIGH

        // zero = 6 ; one = 11
        ldi r19, 0      // 1
        lsr r18         // 1
        breq high0\col  // 1/2

        ldi r19, 1      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1

high0\col:
        cbi _SFR_IO_ADDR(PORTB), 0    // 2      LOW

        // zero = 12 ; one = 9
        and r19, r19    // 1
        brne low1\col   // 1/2

        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1

low1\col:
        mov r0, r0      // 1
        mov r0, r0      // 1

        dec r20         // 1
        brne top\col    // 1/2
.endm


.globl lightit
lightit:
        // lightit(rgb)
        // r22 == b
        // r23 == g
        // r24 == r

        mov r18, r23
        docolor green

        mov r18, r24
        docolor red

        mov r18, r22
        docolor blue

        ret
