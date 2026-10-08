typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct Sub { char pad[0x1CC]; f32 unk1CC; } Sub;
typedef struct Obj {
    char pad0[8];
    f32 pos[3];          /* 0x08 */
    f32 unk14, unk18;    /* 0x14 */
    char pad1C[4];
    u16 unk20, unk22, unk24; /* 0x20 */
    char pad26[0x94 - 0x26];
    u8 unk94; char pad95[3];
    s32 unk98;
    char pad9C[0x1D0 - 0x9C];
    Sub *unk1D0;
    char pad1D4[4];
    s32 unk1D8;
    char pad1DC[4];
    s32 unk1E0;
    char pad1E4[0x1F4 - 0x1E4];
    u16 unk1F4; char pad1F6[2];
    s16 unk1F8, unk1FA, unk1FC, unk1FE;
    char pad200[8];
    s16 unk208; char pad20A[4]; s16 unk20E;
    char pad210[0x10];
    s32 unk220, unk224, unk228;
    char pad22C[0x4A4 - 0x22C];
    u16 unk4A4;
} Obj;

typedef struct Spawn {
    f32 x, y, z;   /* 0x20 */
    u16 rot;       /* 0x2C */
    s8 b2E, b2F;
    s32 i30, i34, i38, i3C, i40, i44, i48;
} Spawn;

typedef struct Ctx { s32 pc; Obj *obj; f32 timer; u16 unkC; u16 flags; } Ctx;
typedef struct Fx { char pad[0x30]; s8 unk30; } Fx;

extern f32 D_8007448C;
extern char D_80114EB0[], D_80114EC8[], D_80114EE0[], D_80114EF8[], D_801150A4[];
extern char D_80115444[], D_8011551C[], D_80115834[], D_80115868[], D_801159F4[];
extern u8 D_80121CE5;
extern f32 D_80219488;
extern u8 D_803A66BC;
extern u16 D_803A66BE;
extern u16 D_803A69E0;
extern u8 D_803A69E2;
extern f32 D_803A69E8, D_803A69EC, D_803A69F4;
extern u8 D_803A6A00;
extern u16 D_803A6A02;
extern u8 D_803A6A04;
extern u16 D_803A6A06;
extern u8 D_803A701C;

Obj *func_8008AE04(Spawn *);
void func_8008B6D0(Obj *);
void func_8008B82C(Obj *, s32, s32);
void func_8008C5D8(Obj *, s32, s32);
void func_8008E780(Obj *, s32, s32, s32);
void func_80097D14(s32, s32);
void func_800A2C6C(s32, f32 *, s32, s32, s32, s32);
void func_800A5BD8(f32 *, s32, s32, f32, void *, s32);
u16 func_800D12B0(s32, u16);
Fx *func_800D7DE0(s32, f32 *, u8, u16, Sub *, s32, s32);
s32 func_800EC1F8(f32 *, s32, s32, s32, s32, s32);

void func_800D1C90(Ctx *ctx) {
    Spawn sp;
    u16 sp56;
    u8 sp5F;
    Obj *obj;
    s32 pc;
    u8 spawning;
    u16 flags;
    f32 *pos;
    s32 h;
    void *eff;

    obj = ctx->obj;
    flags = 0;
    spawning = 0;
    sp56 = 0;
    sp5F = 0;
    if (obj != 0) {
        if (!(obj->unk1E0 & 1)) {
            ctx->pc = 0;
            ctx->obj = 0;
            return;
        }
        obj->unk14 = obj->pos[0];
        obj->unk18 = obj->pos[1];
        obj->unk22 = obj->unk20;
    }
    ctx->timer -= D_80219488;
    if (ctx->timer <= 0.0f) {
        pc = ctx->pc;
        while (ctx->timer <= 0.0f) {
            pc += func_800D12B0(pc, ctx->unkC);
            switch (D_803A6A04) {
            case 1:
            case 2:
                ctx->timer += D_803A6A06;
                break;
            case 8:
                ctx->flags = 0;
                ctx->unkC = D_803A6A00;
                spawning = 1;
                switch (D_803A6A00) {
                case 10: sp.i30 = 0; break;
                case 11: sp.i30 = 1; break;
                case 12: sp.i30 = 2; break;
                case 13: sp.i30 = 3; break;
                case 14: sp.i30 = 4; break;
                case 15: sp.i30 = 5; break;
                case 16: sp.i30 = 6; break;
                case 17: sp.i30 = 7; break;
                case 18: sp.i30 = 8; break;
                case 19: sp.i30 = 9; break;
                case 20: sp.i30 = 10; break;
                case 21: sp.i30 = 11; break;
                case 22: sp.i30 = 12; break;
                case 23: sp.i30 = 13; break;
                case 24: sp.i30 = 14; break;
                }
                break;
            case 3:
                if (spawning) {
                    sp.x = D_803A69E8;
                    sp.y = D_803A69F4;
                    sp.z = D_803A69EC;
                    sp.rot = D_803A6A02;
                } else {
                    obj->pos[0] += D_803A69E8;
                    obj->pos[1] += D_803A69F4;
                    obj->pos[2] += D_803A69EC;
                    if (D_803A69E2 != 0) {
                        obj->unk20 = D_803A6A02;
                    }
                    if (D_803A66BC != 0) {
                        obj->unk24 = D_803A69E0;
                    }
                }
                break;
            case 9:
                if (!spawning) {
                    switch (D_803A6A00) {
                    case 0: func_8008C5D8(obj, 1, 0); break;
                    case 1: func_8008C5D8(obj, 2, 0); break;
                    case 2: func_8008C5D8(obj, 3, 0); break;
                    case 3: func_8008C5D8(obj, 4, 0); break;
                    case 4: func_8008C5D8(obj, 9, 0); break;
                    case 5: func_8008C5D8(obj, 12, 0); break;
                    }
                }
                break;
            case 10:
                switch (D_803A6A00) {
                case 0:
                    if (!spawning) obj->unk1F8 = 10000;
                    ctx->flags |= 0x8000;
                    break;
                case 1:
                    if (!spawning) { obj->unk220 = 2; obj->unk1FA = 10000; }
                    ctx->flags |= 0x4000;
                    break;
                case 2:
                    if (!spawning) { obj->unk220 = 3; obj->unk1FC = 10000; }
                    ctx->flags |= 0x4000;
                    break;
                case 3:
                    if (!spawning) { obj->unk220 = 4; obj->unk1FE = 10000; }
                    ctx->flags |= 0x4000;
                    break;
                case 4:
                    if (!spawning) { obj->unk220 = 9; obj->unk208 = 10000; }
                    ctx->flags |= 0x4000;
                    break;
                case 5:
                    if (!spawning) { obj->unk220 = 12; obj->unk20E = 10000; }
                    ctx->flags |= 0x4000;
                    break;
                }
                break;
            case 11:
                if (D_803A6A00 == 0) {
                    ctx->flags &= 0x7FFF;
                } else {
                    ctx->flags &= 0xBFFF;
                }
                break;
            case 12:
                if (spawning) {
                    flags |= D_803A6A00;
                } else {
                    obj->unk4A4 &= 0xFFC0;
                    obj->unk4A4 |= D_803A6A00;
                }
                break;
            case 41:
                if (spawning) {
                    flags |= D_803A6A00 << 6;
                } else {
                    obj->unk4A4 &= 0xF83F;
                    obj->unk4A4 |= D_803A6A00 << 6;
                }
                break;
            case 46:
                if (spawning) {
                    if (flags & 0x2000) {
                        flags &= 0xDFFF;
                    } else {
                        flags |= 0x2000;
                    }
                } else {
                    if (obj->unk4A4 & 0x2000) {
                        obj->unk4A4 &= 0xDFFF;
                    } else {
                        obj->unk4A4 |= 0x2000;
                    }
                }
                break;
            case 38:
                if (spawning) {
                    sp56 = D_803A66BE;
                } else {
                    obj->unk1D8 = D_803A66BE;
                }
                break;
            case 39:
                if (spawning) {
                    sp5F = 1;
                } else {
                    func_800D7DE0(obj->unk98, obj->pos, obj->unk94, obj->unk20, obj->unk1D0, obj->unk1F4, 0)->unk30 = obj->unk4A4;
                    func_8008B6D0(obj);
                }
                break;
            case 16:
                h = 0;
                if (spawning) {
                    pos = &sp.x;
                switch (D_803A6A00) {
                case 0:
                    h = func_800EC1F8(pos, 0, 0, 100, 2, 0);
                    eff = D_80115444;
                    break;
                case 1:
                    func_800A2C6C(0, pos, 172, 50, 0, 0xE49D0A);
                    eff = D_801150A4;
                    break;
                case 2: eff = D_8011551C; break;
                case 3: eff = D_80115868; break;
                case 4: eff = D_801159F4; break;
                case 5: eff = D_80115834; break;
                case 7: eff = D_80114EB0; break;
                case 8: eff = D_80114EC8; break;
                case 9: eff = D_80114EE0; break;
                case 10: eff = D_80114EF8; break;
                default: eff = 0; break;
                }
                } else {
                    pos = obj->pos;
                switch (D_803A6A00) {
                case 0:
                    h = func_800EC1F8(pos, 0, 0, 100, 2, 0);
                    eff = D_80115444;
                    break;
                case 1:
                    func_800A2C6C(0, pos, 172, 50, 0, 0xE49D0A);
                    eff = D_801150A4;
                    break;
                case 2: eff = D_8011551C; break;
                case 3: eff = D_80115868; break;
                case 4: eff = D_801159F4; break;
                case 5: eff = D_80115834; break;
                case 7: eff = D_80114EB0; break;
                case 8: eff = D_80114EC8; break;
                case 9: eff = D_80114EE0; break;
                case 10: eff = D_80114EF8; break;
                default: eff = 0; break;
                }
                }
                if (eff != 0) {
                    func_800A5BD8(pos, 0, 0, 1.0f, eff, h);
                }
                break;
            case 47:
                if (D_80121CE5 == 0) {
                    D_803A701C = 1;
                    D_80121CE5 = 1;
                } else {
                    D_803A701C = 20;
                }
                func_80097D14(D_803A6A00, D_803A701C);
                break;
            case 0:
                func_8008B82C(obj, 0, 0xFF);
                ctx->pc = 0;
                return;
            }
        }
        ctx->pc = pc;
    }
    if (spawning) {
        sp.i34 = 4;
        sp.i3C = 0;
        sp.i38 = 0;
        sp.i48 = 0;
        sp.b2F = 1;
        sp.b2E = 0;
        sp.i44 = 0;
        sp.i40 = 0;
        obj = func_8008AE04(&sp);
        obj->unk224 = 0;
        obj->unk228 = 0;
        obj->unk1D0->unk1CC = D_8007448C;
        obj->unk4A4 = flags;
        if (sp56 != 0) {
            obj->unk1D8 = sp56;
        }
        ctx->obj = obj;
        if (sp5F != 0) {
            func_800D7DE0(obj->unk98, obj->pos, obj->unk94, obj->unk20, obj->unk1D0, obj->unk1F4, 0)->unk30 = obj->unk4A4;
            func_8008B6D0(obj);
        }
    }
    if (ctx->flags & 0x8000) {
        func_8008E780(obj, 0, -1, 0);
    }
    if (ctx->flags & 0x4000) {
        func_8008E780(obj, 1, -1, 0);
    }
}
