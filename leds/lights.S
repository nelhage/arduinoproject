#include <avr/io.h>

.globl lightit
lightit:
        ldi r20, 24
top:
        sbi _SFR_IO_ADDR(PORTB), 0    // 2 10     HIGH

        mov r0, r0      // 1 1
        mov r0, r0      // 1 2
        mov r0, r0      // 1 3
        mov r0, r0      // 1 4
        mov r0, r0      // 1 5
        mov r0, r0      // 1 6
        mov r0, r0      // 1 7
        mov r0, r0      // 1 8
        mov r0, r0      // 1 9

        cbi _SFR_IO_ADDR(PORTB), 0    // 2 11     LOW

        mov r0, r0      // 1 1
        mov r0, r0      // 1 2
        mov r0, r0      // 1 3
        mov r0, r0      // 1 4
        mov r0, r0      // 1 5
        mov r0, r0      // 1 6

        dec r20         // 1 7
        brne top        // 1 8
        ret
