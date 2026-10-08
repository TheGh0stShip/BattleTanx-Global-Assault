#include "types.h"
#define NULL 0

typedef struct { f32 x, y, z; } Vec3f;
typedef struct Part {
    u8 pad0[0x10]; s32 unk10; u8 pad14[0x8]; s32 unk1C; struct Part* next;
    Vec3f pos; s32 unk30; u8 unk34; u8 pad35[3]; s16 unk38;
} Part;
typedef struct { u8 pad0[0x214]; s16 unk214; } Sub1D0;
typedef struct Obj75F4 {
    u8 pad0[0x8]; Vec3f pos; u8 pad14[0x80]; u8 unk94; u8 pad95[0xB]; Part* list;
    u8 padA4[0x12C]; Sub1D0* unk1D0;
} Obj75F4;

extern s16 func_800B1898(Part*, s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_8021945C;
extern s32 D_802194A0;

void func_800E75F4(Obj75F4* o) {
    Vec3f pos;
    Part* p;
    u8 c;

    pos = o->pos;
    while (o->list != NULL) {
        p = o->list;
        c = o->unk94;
        o->list = p->next;
        p->pos = pos;
        p->unk38 = func_800B1898(p, p->pos.x, p->pos.y, p->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, c);
        p->unk34 = c;
        p->unk10 = 1;
        p->unk1C = 0;
        p->next = NULL;
        p->unk30 = D_8021945C;
        pos.x += 15.0f;
    }
    if (D_802194A0 == 14) {
        o->unk1D0->unk214 = 0;
    }
}
