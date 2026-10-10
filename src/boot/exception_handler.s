.set noat
.set noreorder
.set gp=64

.section .text, "ax"

# Boot-time exception monitor dispatcher.  The context record is rooted at
# D_80127E40; func_80079260 saves it and this routine restores it before eret.

.macro RESTORE_GPR reg, offset
    ld      \reg, \offset($k0)
.endm

.macro RESTORE_EVEN_FPR reg, offset
    ldc1    \reg, \offset($k0)
.endm

.macro RESTORE_ODD_FPR reg, offset
    ldc1    \reg, \offset($k0)
.endm

.align 0
.globl __bootException
.ent __bootException
__bootException:
    lui     $k0, %hi(D_80127E40)
    addiu   $k0, $k0, %lo(D_80127E40)
    sd      $at, 0x328($k0)
    sd      $v0, 0x330($k0)
    mfc0    $at, $13
    lw      $v0, 0x338($k0)
    srl     $at, $at, 2
    andi    $at, $at, 0x1F
    beqz    $at, 2f
     srlv   $v0, $v0, $at
    andi    $v0, $v0, 1
    bnez    $v0, 3f
     nop
1:
    ld      $at, 0x328($k0)
    ld      $v0, 0x330($k0)
    bgez    $zero, D_80078CF4
     nop
2:
    mfc0    $at, $13
    nop
    andi    $at, $at, 0x0800
    beqz    $at, 1b
     nop
    bgez    $zero, 5f
     nop
3:
    addiu   $v0, $zero, 9
    bne     $at, $v0, 4f
     nop
    mfc0    $v0, $14
    nop
    lw      $v0, 0($v0)
    nop
    srl     $v0, $v0, 6
    addiu   $at, $v0, -1024
    bgez    $at, 9f
     nop
8:
    addiu   $at, $zero, 9
4:
.globl D_80078EDC
D_80078EDC:
    sb      $at, 0x320($k0)
    sb      $zero, 0x322($k0)
5:
    addiu   $at, $zero, 1
    sw      $at, 0x33C($k0)
    ld      $at, 0x328($k0)
    ld      $v0, 0x330($k0)
    sd      $ra, 0x0F8($k0)
    jal     func_80079260
     nop
    lui     $sp, %hi(D_801287C0)
    addiu   $sp, $sp, %lo(D_801287C0)
    jal     func_800785F0
     nop
    jal     func_80078DFC
     nop
    jal     func_80078E18
     nop

    lui     $k0, %hi(D_80127E40)
    addiu   $k0, $k0, %lo(D_80127E40)
    addiu   $at, $zero, 1
    sb      $at, 0x320($k0)
    ld      $t0, 0x108($k0)
    nop
    mthi    $t0
    ld      $t0, 0x100($k0)
    nop
    mtlo    $t0
    ld      $t0, 0x178($k0)
    nop
    mtc0    $t0, $12
    ld      $t0, 0x110($k0)
    nop
    mtc0    $t0, $14
    ld      $t0, 0x0D0($k0)
    mtc0    $t0, $30

    ld      $t0, 0x178($k0)
    lui     $t1, 0x2000
    and     $t1, $t1, $t0
    beqz    $t1, 7f
     nop
    lw      $t0, 0x318($k0)
    nop
    ctc1    $t0, $0
    lw      $t0, 0x31C($k0)
    nop
    ctc1    $t0, $31
    RESTORE_EVEN_FPR $f0,  0x218
    RESTORE_EVEN_FPR $f2,  0x228
    RESTORE_EVEN_FPR $f4,  0x238
    RESTORE_EVEN_FPR $f6,  0x248
    RESTORE_EVEN_FPR $f8,  0x258
    RESTORE_EVEN_FPR $f10, 0x268
    RESTORE_EVEN_FPR $f12, 0x278
    RESTORE_EVEN_FPR $f14, 0x288
    RESTORE_EVEN_FPR $f16, 0x298
    RESTORE_EVEN_FPR $f18, 0x2A8
    RESTORE_EVEN_FPR $f20, 0x2B8
    RESTORE_EVEN_FPR $f22, 0x2C8
    RESTORE_EVEN_FPR $f24, 0x2D8
    RESTORE_EVEN_FPR $f26, 0x2E8
    RESTORE_EVEN_FPR $f28, 0x2F8
    RESTORE_EVEN_FPR $f30, 0x308
    ld      $t0, 0x178($k0)
    lui     $t1, 0x0400
    and     $t1, $t1, $t0
    beqz    $t1, 7f
     nop
    RESTORE_ODD_FPR $f1,  0x220
    RESTORE_ODD_FPR $f3,  0x230
    RESTORE_ODD_FPR $f5,  0x240
    RESTORE_ODD_FPR $f7,  0x250
    RESTORE_ODD_FPR $f9,  0x260
    RESTORE_ODD_FPR $f11, 0x270
    RESTORE_ODD_FPR $f13, 0x280
    RESTORE_ODD_FPR $f15, 0x290
    RESTORE_ODD_FPR $f17, 0x2A0
    RESTORE_ODD_FPR $f19, 0x2B0
    RESTORE_ODD_FPR $f21, 0x2C0
    RESTORE_ODD_FPR $f23, 0x2D0
    RESTORE_ODD_FPR $f25, 0x2E0
    RESTORE_ODD_FPR $f27, 0x2F0
    RESTORE_ODD_FPR $f29, 0x300
    RESTORE_ODD_FPR $f31, 0x310
7:
    RESTORE_GPR $at, 0x008
    RESTORE_GPR $v0, 0x010
    RESTORE_GPR $v1, 0x018
    RESTORE_GPR $a0, 0x020
    RESTORE_GPR $a1, 0x028
    RESTORE_GPR $a2, 0x030
    RESTORE_GPR $a3, 0x038
    RESTORE_GPR $t0, 0x040
    RESTORE_GPR $t1, 0x048
    RESTORE_GPR $t2, 0x050
    RESTORE_GPR $t3, 0x058
    RESTORE_GPR $t4, 0x060
    RESTORE_GPR $t5, 0x068
    RESTORE_GPR $t6, 0x070
    RESTORE_GPR $t7, 0x078
    RESTORE_GPR $s0, 0x080
    RESTORE_GPR $s1, 0x088
    RESTORE_GPR $s2, 0x090
    RESTORE_GPR $s3, 0x098
    RESTORE_GPR $s4, 0x0A0
    RESTORE_GPR $s5, 0x0A8
    RESTORE_GPR $s6, 0x0B0
    RESTORE_GPR $s7, 0x0B8
    RESTORE_GPR $t8, 0x0C0
    RESTORE_GPR $t9, 0x0C8
    RESTORE_GPR $k1, 0x0D8
    RESTORE_GPR $gp, 0x0E0
    RESTORE_GPR $sp, 0x0E8
    RESTORE_GPR $s8, 0x0F0
    RESTORE_GPR $ra, 0x0F8
    sw      $zero, 0x33C($k0)
    mfc0    $k0, $30
    eret

9:
    addiu   $at, $v0, -1033
    bgez    $at, 8b
     nop
    lui     $at, %hi(10f)
    addiu   $at, $at, %lo(10f)
    addiu   $v0, $v0, -1024
    sll     $v0, $v0, 2
    addu    $v0, $v0, $at
    lw      $v0, 0($v0)
    nop
    jr      $v0
     nop
10:
    .word   11f
    .word   14f
    .word   15f
    .word   8b
    .word   16f
    .word   17f
    .word   D_80079208
    .word   D_80079214
    .word   D_80079230
11:
    lw      $at, 0x33C($k0)
    nop
    bnez    $at, D_80079240
     nop
    addiu   $at, $zero, 100
12:
    bnez    $at, 12b
     addi   $at, $at, -1
    lui     $at, 0xA460
    ori     $at, $at, 0x0010
13:
    lw      $v0, 0($at)
    nop
    andi    $v0, $v0, 3
    bnez    $v0, 13b
     nop
    lui     $at, 0xB1FF
    ori     $at, $at, 0xFFF0
    lw      $at, 0($at)
    nop
    beqz    $at, D_80079240
     nop
    bgez    $zero, 5b
     nop
14:
    addiu   $at, $zero, -1
    j       4b
     nop
15:
    addiu   $at, $zero, -1
    j       4b
     nop
16:
    j       8b
     nop
17:
    sd      $v1, 0x018($k0)
    sd      $a0, 0x020($k0)
    sd      $ra, 0x0F8($k0)
    bgezal  $zero, func_8007919C
     nop
    ld      $ra, 0x0F8($k0)
    ld      $a0, 0x020($k0)
    bgez    $zero, D_80079240
     ld     $v1, 0x018($k0)
.end __bootException
