.section .data
.balign 4

.globl rspbootTextStart
rspbootTextStart:
.incbin "build/us/rsp/rspboot.bin"
.space 4

.globl gspF3DEX_fifoTextStart
gspF3DEX_fifoTextStart:
.incbin "build/us/rsp/f3dex2/F3DEX2_2.07/F3DEX2_2.07.code"

.globl n_aspMainTextStart
n_aspMainTextStart:
.incbin "build/us/rsp/n_aspMain.text.bin"
