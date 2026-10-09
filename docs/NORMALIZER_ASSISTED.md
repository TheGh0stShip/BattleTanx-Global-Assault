# Normalizer-assisted reconstruction units

These units reconstruct the retail ROM exactly, but their C sources do not
independently compile to the retail instructions. They are tracked separately
from source matches. Each rule is gated by its function label and aborts unless
its instruction pattern fires exactly once.

| Function | Rule and exact pattern | Action | Retail words reproduced |
| --- | --- | --- | --- |
| `func_8007E118` | `addiu sp,-24; sw ra,20(sp); sw s0,16(sp); lbu v1,0(a0); li v0,2; bne v1,v0; move s0,a1` | Reorder the seven independent/dependency-safe prologue instructions: entry load first, save/assign `s0` before the constant, and save `ra` in the branch delay slot. | At `0x8007E118`–`0x8007E130`: `90830000 27BDFFE8 AFB00010 00A08021 24020002 14620005 AFBF0014`. |
| `func_8007A818` | The entry wait and first inlined free-slot search; exact patterns are enforced by `normalize_display_slot_wait` | Keep the `-1` sentinel in `$7`, keep the initial slot in `$8`, use the retail jump-to-success branch shape, and reuse the sentinel for the retry branch. The second search and tail are unchanged source output. | Reproduces the retail words at `0x8007A820`–`0x8007A828`, `0x8007A854`–`0x8007A860`, and `0x8007A87C`–`0x8007A88C`; disabling the rule leaves twelve differing instructions. |
| `func_8007A8F0` | The exact 21-instruction prefix ending before `jal osWritebackDCacheAll` | Reorder existing frame saves, argument moves, display-command stores, and record-index arithmetic; select `$4` rather than `$3` for the index. No instruction is inserted or removed. | Reproduces `0x8007A8F0`–`0x8007A944`; the call at `0x8007A948` through the return is direct source output. |
| `func_800A8E84` | The exact `link`/object/index/phase temporary chain followed by the two `D_80121D90` expression arms | Rotate `$6/$7/$5` to the retail `$5/$6/$4` allocation and reassociate the two address expressions without changing control flow. | Reproduces the retail words at `0x800A8EB0`–`0x800A8EC0`, `0x800A8ED4`, `0x800A8EF0`–`0x800A8F18`, and `0x800A8F20`; disabling the rule leaves eight differing instructions. |
| `func_800A1B44` | The exact entry branch/frame sequence and the `D_80224EF4` reload adjacent to the `D_80235EF0` store | Move the frame allocation before the sentinel test, fill its branch slot with the existing `$6 = $3` copy, remove the later duplicate copy, and hoist the independent bucket-index reload before the free-list-head store. Each of the three label-gated patterns must fire exactly once. | At `0x800A1B4C`–`0x800A1B58`: `27BDFFF8 2402FFFF 1062001F 00603021`; at `0x800A1BA0`–`0x800A1BB0`: `3C018022 00230821 8C224EF4 3C018023 A4255EF0`. |
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
