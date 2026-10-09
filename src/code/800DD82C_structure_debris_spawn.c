/* Unit 0x800DD82C..0x800DDF04 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800DC000/r2b/func_800DD82C.c (rodata table made static so it cannot clash with the asm pool label); re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DDF04 */
/* RODATA_VRAM 0x80075668 */
typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32;
typedef float f32;

typedef struct { f32 x, y; } Vec2;
typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    char pad0[0x48]; s32 unk48; u8 unk4C; u8 unk4D; u8 unk4E;
} Obj;

typedef struct {
    char pad0[0xA]; u8 flags; char padB;
    f32 x; f32 y; char pad14[4];
    u16 unk18; u8 unk1A; s8 timer;
    char pad1C[0x10];
    s32 unk2C; s32 unk30; s32 unk34; Obj *obj;
    u8 unk3C; u8 state; u8 unk3E; u8 unk3F;
    u16 idx; s16 unk42;
} Self;

typedef struct {
    u32 flags; s32 unk4; char pad8[0xC]; u16 unk14; char pad16[0xA]; s16 unk20; char pad22[6];
} Slot;

typedef struct {
    char pad0[0xC]; s32 unkC; f32 unk10; f32 unk14; char pad18[4]; u8 unk1C; u8 unk1D; char pad1E[0x26];
} Ent;

extern Slot D_803978E0[];
extern s32 D_8011551C;
extern f32 D_80219488;
extern Ent D_80224EF0[];
typedef struct { Vec2 p[4]; } Quad;
static const Quad D_80075668 = { { { -144.0f, -144.0f }, { -144.0f, 144.0f }, { 144.0f, 144.0f }, { 144.0f, -144.0f } } };

extern void func_80097FB4(s32, f32, f32, f32, s32);
extern void func_800DA7D0(s32, f32 *, s32, s32, s32, s32, s32, s32, s32);
extern void func_800DD75C(Self *);
extern f32 func_8009D8A0(f32);
extern s32 func_8009D914(void);
extern void func_800B2BE4(Slot *, Vec2 *);
extern void func_800A5BD8(Vec3 *, s32, s32, f32, s32 *, s32);
extern void func_800C8238(s16, s16, s32, s32, s32, u16);

void func_800DD82C(Self *self, s32 *done) {
    Vec2 pts[4];
    Vec2 base;
    Vec3 v;
    Quad pts2;
    Vec3 v2;
    s32 pad[2];
    s32 i;
    s32 j;
    f32 h;
    s32 t;
    Vec2 *p;

    switch (self->state) {
    case 0:
        if (self->timer <= 0) {
            self->state = 1;
        }
        break;
    case 1:
        self->state = 2;
        break;
    case 2:
        self->state = 3;
        break;
    case 3:
        func_80097FB4(2, self->x, self->y, 1.0f, self->unk1A);
        self->obj->unk4E &= 0x7F;
        self->state = 4;
        self->unk3C = 0;
        { s32 k = self->unk3E; D_803978E0[self->idx].unk20 = k; }
        func_800DA7D0(self->unk30, &self->x, self->unk18, self->unk1A, 0, 0, 0, 0, 0);
        func_800DD75C(self);
        if ((self->flags & 6) && func_8009D8A0(1.0f) < 0.25f) {
            self->flags |= 1;
        }
        base.x = self->x;
        base.y = self->y;
        p = pts;
        { Slot *arr = D_803978E0;
        func_800B2BE4(&arr[self->idx], p);
        h = (s16)(arr[self->idx].unk20 + arr[self->idx].unk14); }
        for (i = 0; i < 4;) {
do {do {do {
            v.x = (p->x - base.x) * 0.8f + base.x;
            v.y = (p->y - base.y) * 0.8f + base.y;
            v.z = func_8009D8A0(h * 0.2f) + h * 0.6f;
            func_800A5BD8(&v, 0, self->unk1A, 1.0f, &D_8011551C, 0);
            i++;
            p++;
} while (0);} while (0);} while (0);
        }
        break;
    case 4:
        t = self->unk3C + D_80219488 * 255.0f / 30.0f;
        if (t < 256) {
            self->unk3C = t;
        } else {
            self->state = 5;
            self->timer = 15;
        }
        break;
    case 5:
        if (self->timer <= 0) {
            pts2 = D_80075668;
            self->state = 6;
            self->unk3C = 0;
            self->flags &= ~1;
            func_800DA7D0(self->unk34, &self->x, self->unk18, self->unk1A, 0, 0, 0, 0, 0);
            for (j = 0; j < 4; j++) {
                pts2.p[j].x += self->x;
                pts2.p[j].y += self->y;
            }
            base.x = self->x;
            base.y = self->y;
            t = func_8009D914();
            {
            f32 hh = 105.0f;
            if (!(t & 1)) {
                func_80097FB4(9, self->x, self->y, 1.0f, self->unk1A);
            } else {
                func_80097FB4(11, self->x, self->y, 1.0f, self->unk1A);
            }
            for (j = 0; j < 4; j++) {
                v2.x = (pts2.p[j].x - base.x) * 0.8f + base.x;
                v2.y = (pts2.p[j].y - base.y) * 0.8f + base.y;
                v2.z = func_8009D8A0(hh * 0.2f) + hh * 0.6f;
                func_800A5BD8(&v2, 0, self->unk1A, 1.0f, &D_8011551C, 0);
            }
            }
        }
        break;
    case 6:
        t = self->unk3C + D_80219488 * 255.0f / 30.0f;
        if (t >= 256) {
          if (self->unk42 != -1) {
            self->state = 7;
            self->obj->unk4C = 1;
            self->obj->unk4E = 1;
            self->obj->unk48 = self->unk2C;
            { Slot *arr = D_803978E0;
            arr[self->idx].flags |= 0x02000040;
            arr[self->idx].flags &= ~2;
            { s32 k = self->unk3F; arr[self->idx].unk20 = k; } }
        } else {
            self->obj->unk4C = 1;
            self->obj->unk4E = 1;
            self->obj->unk48 = self->unk2C;
            D_803978E0[self->idx].unk4 = 0;
            { Slot *arr = D_803978E0;
            arr[self->idx].flags |= 0x40;
            arr[self->idx].flags &= ~2;
            { s32 k = self->unk3F; arr[self->idx].unk20 = k; } }
            *done = 1;
          }
        } else {
            self->unk3C = t;
        }
        break;
    }
    self->obj->unk4D |= 0xF0;
    if (self->unk42 != -1) {
        Ent *e = &D_80224EF0[self->unk42];
        u16 fl = (-(e->unkC != 0) & (s16)0x8002) | 2;
        func_800C8238(e->unk10, e->unk14, 0, e->unk1C, e->unk1D, fl);
    }
}
