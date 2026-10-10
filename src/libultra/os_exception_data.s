.section .data
.balign 16

/*
 * libultra 2.0I os/exceptasm.s hardware-interrupt callback table. The five
 * entries are initialized null pointers; the retail object pads its data
 * section to the next 16-byte boundary.
 */
.globl __osHwIntTable
__osHwIntTable:
.word 0, 0, 0, 0, 0
.space 12
