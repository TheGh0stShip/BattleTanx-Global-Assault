typedef unsigned short u16;
typedef struct { char pad[0x50]; u16 flags; char pad2[6]; } Ent58;
extern Ent58 D_803A6A08[];

Ent58 *func_800D09E0(void) {
    u16 i;
    for (i = 0; i < 16; i++) {
        if (!(D_803A6A08[i].flags & 0x8000)) break;
    }
    if (i >= 16) return 0;
    D_803A6A08[i].flags = 0x8000;
    return &D_803A6A08[i];
}
