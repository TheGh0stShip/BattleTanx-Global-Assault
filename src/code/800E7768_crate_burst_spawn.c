/* Unit 0x800E7768..0x800E7968 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32).
 * NORMALIZER-ASSISTED: the C below is exact only with the proposed assembler-hazard rules V4_DM (two nops between a 3-operand div macro and a following mult)
 * (see NORMALIZER_ASSISTED.tsv). With the production normalizer it differs (see comparisons/).
 * Origin: claude-work/output/workers/0x800E4800/r2c/func_800E7768.c
 * Not integration-ready until the rule is adopted; the C itself is a complete source match otherwise.
 */
/* SPAN 0x800E7968 */
/* RODATA_VRAM 0x80075F34 */
#include "types.h"
#define NULL 0

typedef struct { f32 x, y, z; } Vec3f;
typedef struct Part { u8 pad0[0x14]; s32 unk14; u8 pad18[0x8]; struct Part* next; } Part;

extern s32 func_800AD14C(f32, f32, f32, u8);
extern void func_800AD6A8(s32, Vec3f*, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32);
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);

void func_800E7768(Part* list, Vec3f* pos, u8 a2, u16 a3, u8 mask) {
    Part* n;
    s32 count;
    s32 r;
    s32 i;
    u16 ang;
    Vec3f tmp;

    count = 0;
    for (n = list; n != NULL; n = n->next) {
        count++;
    }
    if (count == 0) {
        return;
    }
    if (count == 1) {
        s32 r1 = mask & func_800AD14C(pos->x, pos->y, 30.0f, a2);
        if (r1) {
            func_800AD6A8(list->unk14, pos, 1.0f, 1.0f, 0, 0, 0, 0, r1, 0, 0, 0);
        }
    } else {
        r = mask & func_800AD14C(pos->x, pos->y, 90.0f, a2);
        if (r) {
            n = list;
            i = 0;
            if (n != NULL) {
                s32 full = 0xFFFF;
                f32 rad = 60.0f;
                f32 sc = 1.0f;
                s32 rr = r;
                do {
                    ang = a3 + (full / count) * i;
                    tmp.x = pos->x + func_8009D4B0(ang) * rad;
                    tmp.z = pos->z;
                    tmp.y = pos->y + func_8009D510(ang) * rad;
                    func_800AD6A8(n->unk14, &tmp, sc, sc, 0, 0, 0, 0, rr, 0, 0, 0);
                    n = n->next;
                    i++;
                } while (n != NULL);
            }
        }
    }
}
