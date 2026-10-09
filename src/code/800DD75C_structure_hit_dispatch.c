/* Unit 0x800DD75C..0x800DD82C (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800DC000/r2/func_800DD75C_wip.c (3 words off) fixed in this lane.
 * Fix: `Hit *h = hits;` kept single-set and the loop indexes `h[i]` (no `h++`), so the address computation is launched right before `move a2,s0` (sched.c adjust_priority; see strategy/CAUSES.md R20).
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DD82C */
typedef unsigned short u16; typedef short s16; typedef int s32;
typedef struct { s32 pad; s32 type; } Obj;
typedef struct { Obj *obj; s32 rest[8]; } Hit;
typedef struct { void (*fn)(Obj *, void *, s32, s32, s32); s32 a, b; } Handler;
extern Handler D_80224B5C[];
extern s16 D_80397650;
extern u16 func_800B3748(u16, s32, Hit *, s32, s32);
typedef struct { char pad[0x40]; u16 id; } Self;
void func_800DD75C(Self *self) {
    Hit hits[32]; Hit *h; s32 i; s32 n;
    h = hits;
    D_80397650 = 0;
    n = func_800B3748(self->id, 0x100000, h, 2, 0);
    for (i = 0; i < n; i++) { if (h[i].obj != 0) { s32 t = h[i].obj->type; if (D_80224B5C[t].fn != 0) { D_80224B5C[t].fn(h[i].obj, self, 0, 0, 0); } } }
}
