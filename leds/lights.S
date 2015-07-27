#include <avr/io.h>

.globl lightit
lightit:
        // lightit(ptr, count)
        // ptr   == r24:r25
        // count == r22

        // X = ptr
        mov r26, r24
        mov r27, r25
outer:

        ld  r18, X+
        ldi r20, 8
byte:
        sbi _SFR_IO_ADDR(PORTB), 0    // 2     HIGH

        // zero = 6 ; one = 11
        ldi r19, 0      // 1
        lsr r18         // 1
        breq high0      // 1/2

        ldi r19, 1      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1

high0:
        cbi _SFR_IO_ADDR(PORTB), 0    // 2      LOW

        // zero = 12 ; one = 9
        and r19, r19    // 1
        brne low1       // 1/2

        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1
        mov r0, r0      // 1

low1:
        mov r0, r0      // 1
        mov r0, r0      // 1

        dec r20         // 1
        brne byte       // 1/2

        dec r22
        brne outer

        ret
