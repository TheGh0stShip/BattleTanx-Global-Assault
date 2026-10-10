.set noat
.set noreorder
.set gp=64

.section .text, "ax"

# The ROM also contains libultra's later __osException at 0x80104FB0.
# Keep the boot-time handler unambiguous when both objects are linked.
.equ __bootExceptionHandler, 0x80078E4C

.align 0
.globl __bootExceptionPreamble
.ent __bootExceptionPreamble
__bootExceptionPreamble:
    lui     $k0, %hi(__bootExceptionHandler)
    addiu   $k0, $k0, %lo(__bootExceptionHandler)
    jr      $k0
     nop

.globl D_80078CF4
D_80078CF4:
__bootExceptionPreamble_general:
    nop
    nop
    nop
    nop
    nop
    lui     $k0, 0x8000
    ori     $k0, $k0, 0x0194
    jr      $k0
     nop

__bootExceptionPreamble_tlb:
    mtc0    $k0, $30
    lui     $k0, %hi(__bootExceptionHandler)
    addiu   $k0, $k0, %lo(__bootExceptionHandler)
    jr      $k0
     nop

__bootExceptionPreamble_cache:
    nop
    nop
    nop
    nop
    nop
    lui     $k0, 0x8000
    ori     $k0, $k0, 0x0014
    jr      $k0
     nop
.end __bootExceptionPreamble

.align 0
.globl func_80078D50
.ent func_80078D50
func_80078D50:
    addi    $sp, $sp, -8
    sw      $ra, 0($sp)
    lui     $t0, %hi(D_80078CE0)
    addiu   $t0, $t0, %lo(D_80078CE0)
    lui     $t1, %hi(__bootExceptionPreamble_general)
    addiu   $t1, $t1, %lo(__bootExceptionPreamble_general)
    lui     $t2, 0x8000
    ori     $t2, $t2, 0x0180
    lui     $t3, %hi(__bootExceptionPreamble_general)
    addiu   $t3, $t3, %lo(__bootExceptionPreamble_general)
1:
    lw      $t4, 0($t2)
    lw      $t5, 0($t0)
    addiu   $t0, $t0, 4
    sw      $t4, 0($t1)
    addiu   $t1, $t1, 4
    sw      $t5, 0($t2)
    bne     $t0, $t3, 1b
     addiu  $t2, $t2, 4

    lui     $t0, %hi(__bootExceptionPreamble_tlb)
    addiu   $t0, $t0, %lo(__bootExceptionPreamble_tlb)
    lui     $t1, %hi(__bootExceptionPreamble_cache)
    addiu   $t1, $t1, %lo(__bootExceptionPreamble_cache)
    lui     $t2, 0x8000
    lui     $t3, %hi(__bootExceptionPreamble_cache)
    addiu   $t3, $t3, %lo(__bootExceptionPreamble_cache)
2:
    lw      $t4, 0($t2)
    lw      $t5, 0($t0)
    addiu   $t0, $t0, 4
    sw      $t4, 0($t1)
    addiu   $t1, $t1, 4
    sw      $t5, 0($t2)
    bne     $t0, $t3, 2b
     addiu  $t2, $t2, 4

    jal     func_80078DFC
     nop
    jal     func_80078E18
     nop
    addiu   $t0, $zero, -0x802
    lui     $at, %hi(D_80128178)
    sw      $t0, %lo(D_80128178)($at)
    lw      $ra, 0($sp)
    jr      $ra
     addi   $sp, $sp, 8
.end func_80078D50
