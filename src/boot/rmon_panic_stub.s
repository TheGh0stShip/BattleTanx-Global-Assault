.set noreorder
.set gp=64

.section .text, "ax"

.globl __rmonPanic
.ent __rmonPanic
__rmonPanic:
    j       0x80003000
     nop
.end __rmonPanic
