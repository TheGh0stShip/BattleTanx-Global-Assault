# Normalizer-assisted reconstruction units

These units reconstruct the retail ROM exactly, but their C sources do not
independently compile to the retail instructions. They are tracked separately
from source matches. Each rule is gated by its function label and aborts unless
its instruction pattern fires exactly once.

| Function | Rule and exact pattern | Action | Retail words reproduced |
| --- | --- | --- | --- |
| `func_8007E118` | `addiu sp,-24; sw ra,20(sp); sw s0,16(sp); lbu v1,0(a0); li v0,2; bne v1,v0; move s0,a1` | Reorder the seven independent/dependency-safe prologue instructions: entry load first, save/assign `s0` before the constant, and save `ra` in the branch delay slot. | At `0x8007E118`–`0x8007E130`: `90830000 27BDFFE8 AFB00010 00A08021 24020002 14620005 AFBF0014`. |
| `func_800C74AC` | `li $2,-1; sw $2,D_802195D4; li $2,7` | Rename the first constant/store pair from `$2` (`v0`) to `$3` (`v1`). | At `0x800C74AC`/`0x800C74B4`: `2403FFFF`, `AC2395D4` instead of `2402FFFF`, `AC2295D4`. |
| `func_800CEA50` | `addiu i,i,1; lbu count,0(base); andi i,i,0xffff; sltu ...,i,count` | Reorder the independent count load before the increment. | At `0x800CEC1C`/`0x800CEC20`: `90C30000`, `24A50001`. |
| `func_800CEA50` | `lb $2,D_80117EB0; la $16,D_801216A0; bne $2,$3,label; li $2,-1688731648` | Rename the load to `$3` and reverse the compare operands to preserve the retail CSE equivalence-class choice. | At `0x800CED5C`, `0x800CED60`, `0x800CED6C`: `3C038011`, `80637EB0`, `14620004`. |
| `func_800C7C10` | `lw $5,D_80397804` before the primitive-colour packet stores | Move the sprite-pointer load after `sw $2,4($3)`. | At `0x800C7C68`–`0x800C7C84`: `3C018007 C4203D3C 3C02FA00 AC620000 24020096 AC620004 3C058039 8CA57804`. |
| `func_800C7C10` | `lhu $2,D_8011F1F4; beq $2,$0,label` | Insert the unsigned nonzero result retained by the retail combine pass. | At `0x800C7CE8`: `0002102B` (`sltu $2,$0,$2`). |
| `func_800C1938` | Three exact load/copy patterns documented in `shape_controls_config` | Reorder two loads and copy the lookup index through `$2`/`$10`. | Reproduces the retail sequences at `0x800C1A04`, `0x800C1A28`, and the corresponding branch/load words; disabling the rule leaves 50 C-body word differences. |
| `func_800C1938` | Loop tail immediately before `li $2,0x50000000` | Insert the unexplained dead indexed load `andi; sll; lui/addu/lw` of `D_80117F24[player]`. | `30C2FFFF 00021080 3C018011 00220821 8C237F24`. |

The `func_800C1938` dead load remains an open compiler/source-shape question.
The normalizer restores the bytes; it does not explain why the retail compiler
retained a value that is overwritten before use.
