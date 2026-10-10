.set noat
.set noreorder
.set gp=64

.section .text, "ax"

# Install the boot monitor's general and TLB exception-vector stubs, then make
# those self-modifying writes visible to instruction fetch.

.align 0
.globl func_8007919C
.ent func_8007919C
func_8007919C:
    lui     $v1, 0x8000
    ori     $v1, $v1, 0x0180
    lui     $v0, %hi(D_80078CF4)
    addiu   $v0, $v0, %lo(D_80078CF4)
    addi    $a0, $v0, 0x14
1:
    lw      $at, 0($v0)
    addi    $v0, $v0, 4
    sw      $at, 0($v1)
    addi    $v1, $v1, 4
    bne     $v0, $a0, 1b
     nop

    lui     $v1, 0x8000
    lui     $v0, %hi(D_80078D2C)
    addiu   $v0, $v0, %lo(D_80078D2C)
    addi    $a0, $v0, 0x14
2:
    lw      $at, 0($v0)
    addi    $v0, $v0, 4
    sw      $at, 0($v1)
    addi    $v1, $v1, 4
    bne     $v0, $a0, 2b
     nop

    jal     func_80078DFC
     nop
    jal     func_80078E18
     nop
    jr      $ra
     nop

# Out-of-line case tails reached by the exception dispatch table in
# exception_handler.s.  They are internal control-flow blocks rather than
# callable entries, but remain in this source unit so progress classifies them
# as executable code.
.globl D_80079208
D_80079208:
    sw      $a0, 0x338($k0)
    j       D_80079240
     nop

.globl D_80079214
D_80079214:
    mfc0    $at, $14
    nop
    addiu   $at, $at, 4
    mtc0    $at, $14
    addiu   $at, $zero, 0
    j       D_80078EDC
     nop

.globl D_80079230
D_80079230:
    lui     $v0, %hi(func_80078524)
    addiu   $v0, $v0, %lo(func_80078524)
    j       D_80079244
     nop

.globl D_80079240
D_80079240:
    ld      $v0, 0x330($k0)

.globl D_80079244
D_80079244:
    mfc0    $at, $14
    nop
    addiu   $at, $at, 4
    mtc0    $at, $14
    ld      $at, 0x328($k0)
    mfc0    $k0, $30
    eret
.end func_8007919C
