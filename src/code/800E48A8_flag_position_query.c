/* Unit 0x800E48A8..0x800E48E0 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * func_800E48A8 (0x38): message handler that reports the flag position when the ids match and the flag is
 * not held (same shape as func_800E25CC / case 4 of func_800E4DA0). Written in this lane from the ROM listing.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E48E0 */
typedef struct { char pad[0xC]; float x; float y; char pad14[0x1C - 0x14]; unsigned char id; char pad1D[0x40 - 0x1D]; int holder; } FlagObj;
typedef struct { unsigned char id; } FlagQuery;
typedef struct { unsigned char hit; char pad[3]; float x; float y; } FlagPos;

void func_800E48A8(FlagObj *o, void *msg, FlagQuery *q, FlagPos *out) {
    if (o->id == q->id && o->holder == 0) {
        out->hit = 1;
        out->x = o->x;
        out->y = o->y;
    }
}
