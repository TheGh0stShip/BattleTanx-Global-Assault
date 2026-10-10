.set noat
.set noreorder
.set gp=64

.section .text, "ax"

.globl func_80071000
.ent func_80071000
func_80071000:
    lui     $sp, %hi(D_8021E0B8)
    addiu   $sp, $sp, %lo(D_8021E0B8)
    lui     $t0, %hi(D_80127E30)
    addiu   $t0, $t0, %lo(D_80127E30)
    lui     $t1, %hi(D_803B17B0)
    addiu   $t1, $t1, %lo(D_803B17B0)
    beq     $t0, $t1, 2f
     nop
1:
    addiu   $t0, $t0, 4
    sltu    $at, $t0, $t1
    bnez    $at, 1b
     sw     $zero, -4($t0)
2:
    jal     func_8009ED9C
     nop
.end func_80071000
