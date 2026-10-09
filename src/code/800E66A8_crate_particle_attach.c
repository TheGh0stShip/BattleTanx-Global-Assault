/* Normalizer-assisted: one three-instruction scheduler ordering. */
#include "types.h"
#define NULL 0

typedef struct { f32 x, y, z; } Vec3f;
typedef struct Part {
    u8 pad0[0x10]; s32 unk10; u8 pad14[0x4]; struct Src* unk18; struct Src* unk1C; struct Part* next;
    Vec3f pos; s32 unk30; u8 unk34; u8 unk35; u8 pad36[2]; s16 unk38;
} Part;
typedef struct Counter { u8 pad0[4]; u16 unk4; } Counter;
typedef struct Item { u8 pad0[0x10]; Counter* unk10; u8 pad14[0x23C]; } Item;
typedef struct Src { u8 pad0[0xC]; Part* list; u8 pad10[0xD]; u8 unk1D; } Src;

extern void func_800E7F60(Src*, Vec3f*, u8*);
extern void func_800B129C(u16, s16, s16);
extern void func_800B1610(u16, u8);
extern s16 func_800B1898(Part*, s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_8009D144(void);
extern void func_800A9B64(void*, s32);
extern Item D_80235F00[];

static inline Item* getItem(s32 slot) {
    if (slot == 0x7F) return NULL;
    return &D_80235F00[slot];
}

void func_800E66A8(Part* p, Src* s) {
    if (p->unk10 == 1) {
        func_800E7F60(s, &p->pos, &p->unk34);
        func_800B129C(p->unk38, p->pos.x, p->pos.y);
        func_800B1610(p->unk38, p->unk34);
    } else {
        func_800E7F60(s, &p->pos, &p->unk34);
        p->unk38 = func_800B1898(p, p->pos.x, p->pos.y, p->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, p->unk34);
    }
    p->unk35 = s->unk1D;
    p->unk10 = 0;
    p->unk1C = s;
    p->unk18 = s;
    p->next = s->list;
    s->list = p;
    getItem(s->unk1D)->unk10->unk4++;
    if (func_8009D144()) {
        func_800A9B64(getItem(s->unk1D), 12);
    } else {
        func_800A9B64(getItem(s->unk1D), 4);
    }
}
