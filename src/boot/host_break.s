.set noat
.set noreorder
.set gp=64

.section .text, "ax"

.globl func_80078830
.ent func_80078830
func_80078830:
    addiu   $sp, $sp, -0x20
    sw      $s0, 0x10($sp)
    addu    $s0, $a0, $zero
    li      $a0, 0xB1FFFFE4
    sw      $s1, 0x14($sp)
    addu    $s1, $a2, $zero
    sw      $s2, 0x18($sp)
    sw      $ra, 0x1C($sp)
    jal     func_80077CA8
     addu   $s2, $a3, $zero
    li      $a0, 0xB1FFFFE8
    jal     func_80077CA8
     addu   $a1, $s1, $zero
    li      $a0, 0xB1FFFFEC
    jal     func_80077CA8
     addu   $a1, $s2, $zero
    li      $a0, 0xB1FFFFE0
    jal     func_80077CA8
     addu   $a1, $s0, $zero
1:
    jal     func_80077D1C
     li     $a0, 1000
    lui     $a0, 0xB1FF
    jal     func_80077CE0
     ori    $a0, $a0, 0xFFF0
    beql    $v0, $zero, 2f
     lui    $a0, 0xB1FF
    break   1
    lui     $a0, 0xB1FF
2:
    jal     func_80077CE0
     ori    $a0, $a0, 0xFFE0
    bnez    $v0, 1b
     nop
    lui     $a0, 0xB1FF
    jal     func_80077CE0
     ori    $a0, $a0, 0xFFDC
    lw      $ra, 0x1C($sp)
    lw      $s2, 0x18($sp)
    lw      $s1, 0x14($sp)
    lw      $s0, 0x10($sp)
    addiu   $sp, $sp, 0x20
    jr      $ra
     nop
.end func_80078830
