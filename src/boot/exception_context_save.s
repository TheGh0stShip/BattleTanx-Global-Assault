.set noat
.set noreorder
.set gp=64

.section .text, "ax"

# Save the complete VR4300 execution context used by the boot-time exception
# monitor.  k0 points at its 0x340-byte context record at D_80127E40.

.macro SAVE_GPR reg, offset
    sd      \reg, \offset($k0)
.endm

.macro SAVE_CP0 reg, offset
    mfc0    $t0, \reg
    nop
    sd      $t0, \offset($k0)
.endm

.macro SAVE_EVEN_FPR reg, offset
    sdc1    \reg, \offset($k0)
.endm

.macro SAVE_ODD_FPR reg, offset
    sdc1    \reg, \offset($k0)
.endm

.align 0
.globl func_80079260
.ent func_80079260
func_80079260:
    SAVE_GPR $zero, 0x000
    SAVE_GPR $at,   0x008
    SAVE_GPR $v0,   0x010
    SAVE_GPR $v1,   0x018
    SAVE_GPR $a0,   0x020
    SAVE_GPR $a1,   0x028
    SAVE_GPR $a2,   0x030
    SAVE_GPR $a3,   0x038
    SAVE_GPR $t0,   0x040
    SAVE_GPR $t1,   0x048
    SAVE_GPR $t2,   0x050
    SAVE_GPR $t3,   0x058
    SAVE_GPR $t4,   0x060
    SAVE_GPR $t5,   0x068
    SAVE_GPR $t6,   0x070
    SAVE_GPR $t7,   0x078
    SAVE_GPR $s0,   0x080
    SAVE_GPR $s1,   0x088
    SAVE_GPR $s2,   0x090
    SAVE_GPR $s3,   0x098
    SAVE_GPR $s4,   0x0A0
    SAVE_GPR $s5,   0x0A8
    SAVE_GPR $s6,   0x0B0
    SAVE_GPR $s7,   0x0B8
    SAVE_GPR $t8,   0x0C0
    SAVE_GPR $t9,   0x0C8
    SAVE_GPR $k1,   0x0D8
    SAVE_GPR $gp,   0x0E0
    SAVE_GPR $sp,   0x0E8
    SAVE_GPR $s8,   0x0F0

    mfc0    $t0, $30
    sd      $t0, 0x0D0($k0)
    mflo    $t0
    sd      $t0, 0x100($k0)
    mfhi    $t0
    sd      $t0, 0x108($k0)

    SAVE_CP0 $0,  0x118
    SAVE_CP0 $1,  0x120
    SAVE_CP0 $2,  0x128
    SAVE_CP0 $3,  0x130
    SAVE_CP0 $4,  0x138
    SAVE_CP0 $5,  0x140
    SAVE_CP0 $6,  0x148
    SAVE_CP0 $7,  0x150
    SAVE_CP0 $8,  0x158
    SAVE_CP0 $9,  0x160
    SAVE_CP0 $10, 0x168
    SAVE_CP0 $11, 0x170
    SAVE_CP0 $12, 0x178
    SAVE_CP0 $13, 0x180
    SAVE_CP0 $14, 0x188
    SAVE_CP0 $15, 0x190
    SAVE_CP0 $16, 0x198
    SAVE_CP0 $17, 0x1A0
    SAVE_CP0 $18, 0x1A8
    SAVE_CP0 $19, 0x1B0
    SAVE_CP0 $20, 0x1B8
    SAVE_CP0 $21, 0x1C0
    SAVE_CP0 $22, 0x1C8
    SAVE_CP0 $23, 0x1D0
    SAVE_CP0 $24, 0x1D8
    SAVE_CP0 $25, 0x1E0
    SAVE_CP0 $26, 0x1E8
    SAVE_CP0 $27, 0x1F0
    SAVE_CP0 $28, 0x1F8
    SAVE_CP0 $29, 0x200
    SAVE_CP0 $30, 0x208
    SAVE_CP0 $31, 0x210

    ld      $t0, 0x178($k0)
    lui     $t1, 0x2000
    and     $t1, $t1, $t0
    beqz    $t1, 2f
     nop

    cfc1    $t0, $0
    nop
    sw      $t0, 0x318($k0)
    cfc1    $t0, $31
    nop
    sw      $t0, 0x31C($k0)

    SAVE_EVEN_FPR $f0,  0x218
    SAVE_EVEN_FPR $f2,  0x228
    SAVE_EVEN_FPR $f4,  0x238
    SAVE_EVEN_FPR $f6,  0x248
    SAVE_EVEN_FPR $f8,  0x258
    SAVE_EVEN_FPR $f10, 0x268
    SAVE_EVEN_FPR $f12, 0x278
    SAVE_EVEN_FPR $f14, 0x288
    SAVE_EVEN_FPR $f16, 0x298
    SAVE_EVEN_FPR $f18, 0x2A8
    SAVE_EVEN_FPR $f20, 0x2B8
    SAVE_EVEN_FPR $f22, 0x2C8
    SAVE_EVEN_FPR $f24, 0x2D8
    SAVE_EVEN_FPR $f26, 0x2E8
    SAVE_EVEN_FPR $f28, 0x2F8
    SAVE_EVEN_FPR $f30, 0x308

    ld      $t0, 0x178($k0)
    lui     $t1, 0x0400
    and     $t1, $t1, $t0
    beqz    $t1, 2f
     nop

    SAVE_ODD_FPR $f1,  0x220
    SAVE_ODD_FPR $f3,  0x230
    SAVE_ODD_FPR $f5,  0x240
    SAVE_ODD_FPR $f7,  0x250
    SAVE_ODD_FPR $f9,  0x260
    SAVE_ODD_FPR $f11, 0x270
    SAVE_ODD_FPR $f13, 0x280
    SAVE_ODD_FPR $f15, 0x290
    SAVE_ODD_FPR $f17, 0x2A0
    SAVE_ODD_FPR $f19, 0x2B0
    SAVE_ODD_FPR $f21, 0x2C0
    SAVE_ODD_FPR $f23, 0x2D0
    SAVE_ODD_FPR $f25, 0x2E0
    SAVE_ODD_FPR $f27, 0x2F0
    SAVE_ODD_FPR $f29, 0x300
    SAVE_ODD_FPR $f31, 0x310

2:
    ld      $t0, 0x178($k0)
    nop
    andi    $t0, $t0, 4
    bnez    $t0, 3f
     nop
    ld      $t0, 0x188($k0)
    j       4f
     nop
3:
    ld      $t0, 0x208($k0)
    nop
4:
    sd      $t0, 0x110($k0)
    jr      $ra
     nop
.end func_80079260
