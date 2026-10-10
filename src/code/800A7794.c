/* SPAN 0x800A77A8 */
typedef struct {
    char p0[0x1F4];
    int unk1F4;
    int unk1F8;
} UnitStat;

typedef struct {
    char p0[0x20];
    int unk20;
    int unk24;
} StatOut;

void func_800A7794(UnitStat *u, StatOut *o) {
    o->unk20 = u->unk1F4;
    o->unk24 = u->unk1F8;
}
