typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef float f32;

typedef struct Obj {
    s32 unk0; s16 unk4; s16 unk6; s16 unk8; s16 unkA; s16 unkC; s16 unkE;
    u8 unk10; u8 unk11; u8 unk12; u8 unk13; u8 unk14; u8 unk15; u8 unk16; u8 unk17;
    f32 unk18; f32 unk1C; f32 unk20; u8 pad24[0x1C]; f32 unk40; f32 unk44; f32 unk48;
    u8 pad4C[2]; u16 unk4E; u16 unk50; u8 unk52; u8 unk53; u8 pad54; u8 unk55; u8 pad56[2];
} Obj;
typedef struct Stream { u8 *ptr; Obj *obj; f32 timer; u16 unkC; u8 padE[2]; } Stream;
typedef struct Snd { u8 pad0[0xC]; s32 unkC; s32 unk10; s32 unk14; u8 pad18[4]; } Snd;

extern f32 D_80219488;
extern u8 D_803A6A00; extern u8 D_803A6A04; extern u16 D_803A6A06;
extern Obj D_803A6A08[16];
extern Stream D_803A66C0[50];
extern s32 D_80116874, D_80116890, D_801168AC, D_801168C8, D_8011686C, D_80116888, D_801168A4, D_801168C0;
extern Snd D_80116860, D_8011687C, D_80116898, D_801168B4;
extern s32 D_80121CE8[];
extern u16 D_803A666E; extern u16 D_803A6670;
extern u8 D_803A6FC8; extern u8 D_803A6672; extern u8 D_803A7025; extern u8 D_803A666C;
extern f32 D_803A6FD4; extern s32 D_803A7018; extern u16 D_803A66BE;
extern f32 D_803A69F8; extern f32 D_803A69E8; extern f32 D_803A69EC;
extern u8 D_80121CE5; extern u8 D_803A701C;

u16 func_800D12B0(u8 *, u16);
void func_800D05E0(Obj *, s32);
void func_800D0A74(Obj *);
void func_8007BCF0(void *, s32);
extern u8 D_B04729C0[], D_B0472BAC[], D_B0472BB0[], D_B0472D9C[], D_B0472DA0[], D_B0472F8C[], D_B0472F90[], D_B047317C[];
void func_80096810(void *, void *, void *, s32);
void func_80097D14(s32, s32);

static inline u8 findStream(Stream *s) {
    u8 j;
    for (j = 0; j < 50; j++) {
        if (s == &D_803A66C0[j]) return j;
    }
    return 255;
}

void func_800D441C(Stream *s) {
    Obj *obj;
    u8 *ptr;
    u8 *p2;
    u16 i;
    f32 zero;

    s->timer -= D_80219488;
    obj = s->obj;
    if (s->timer <= 0.0f) {
        ptr = s->ptr;
        while (s->timer <= 0.0f) {
            ptr += func_800D12B0(ptr, s->unkC);
            switch (D_803A6A04) {
            case 1:
            case 2:
                s->timer += D_803A6A06;
                break;
            case 8:
                s->unkC = D_803A6A00;
                for (i = 0; i < 16; i++) {
                    if (!(D_803A6A08[i].unk50 & 0x8000)) break;
                }
                if (i >= 16) {
                    obj = 0;
                } else {
                    D_803A6A08[i].unk50 = 0x8000;
                    obj = &D_803A6A08[i];
                }
                obj->unk52 = 1;
                obj->unkA = 0; obj->unk8 = 0; obj->unkE = 0; obj->unkC = 0; obj->unk6 = 0; obj->unk4 = 0;
                obj->unk18 = obj->unk1C = obj->unk20 = 0.0f;
                obj->unk13 = 255; obj->unk12 = 255; obj->unk11 = 255; obj->unk10 = 255;
                obj->unk4E = 0xFFFF;
                obj->unk0 = 0;
                obj->unk48 = 1.0f; obj->unk44 = 1.0f;
                obj->unk50 |= 0x4000;
                s->obj = obj;
                obj->unk53 = findStream(s);
                obj->unk55 = 255;
                func_800D05E0(obj, 0);
                p2 = ptr;
                D_803A6A04 = 255;
                do {
                    p2 += func_800D12B0(p2, s->unkC);
                    if (D_803A6A04 == 31) {
                        Snd *b = (Snd *)D_80121CE8[D_803A6A00];
                        if (b->unkC == 0 && b->unk14 != 0) {
                            func_8007BCF0(b, 1);
                        }
                    } else if (D_803A6A04 == 28) {
                        switch (D_803A6A00) {
                        case 0:
                            if (D_80116860.unkC == 0) {
                                func_80096810(D_B04729C0, D_B0472BAC, &D_80116860, 0);
                                if (D_8011686C == 0 && D_80116874 != 0) {
                                    func_8007BCF0(&D_80116860, 1);
                                }
                            }
                            break;
                        case 1:
                            if (D_8011687C.unkC == 0) {
                                if (D_80116888 == 0 && D_80116890 != 0) {
                                    func_8007BCF0(&D_8011687C, 1);
                                }
                                func_80096810(D_B0472BB0, D_B0472D9C, &D_8011687C, 1);
                            }
                            break;
                        case 2:
                            if (D_80116898.unkC == 0) {
                                if (D_801168A4 == 0 && D_801168AC != 0) {
                                    func_8007BCF0(&D_80116898, 1);
                                }
                                func_80096810(D_B0472DA0, D_B0472F8C, &D_80116898, 2);
                            }
                            break;
                        case 3:
                            if (D_801168B4.unkC == 0) {
                                if (D_801168C0 == 0 && D_801168C8 != 0) {
                                    func_8007BCF0(&D_801168B4, 1);
                                }
                                func_80096810(D_B0472F90, D_B047317C, &D_801168B4, 3);
                            }
                            break;
                        }
                    }
                } while (D_803A6A04 != 0);
                break;
            case 21:
                obj->unk0 = D_803A7018;
                break;
            case 31:
                obj->unk0 = D_80121CE8[D_803A6A00];
                break;
            case 28:
                obj->unk52 = D_803A6A00;
                break;
            case 22:
                obj->unk4 = obj->unkC = D_803A666E;
                obj->unk6 = obj->unkE = D_803A6670;
                break;
            case 33:
                obj->unk8 = D_803A666E;
                obj->unkA = D_803A6670;
                break;
            case 25:
                obj->unk10 = obj->unk14 = D_803A6FC8;
                obj->unk11 = obj->unk15 = D_803A6672;
                obj->unk12 = obj->unk16 = D_803A7025;
                obj->unk13 = obj->unk17 = D_803A666C;
                break;
            case 15:
                obj->unk10 = obj->unk14 = D_803A6FC8;
                obj->unk11 = obj->unk15 = D_803A6672;
                obj->unk12 = obj->unk16 = D_803A7025;
                break;
            case 26:
                obj->unk13 = obj->unk17 = D_803A6A00;
                break;
            case 23:
                obj->unkC = D_803A666E;
                obj->unkE = D_803A6670;
                obj->unk18 = D_803A6FD4;
                break;
            case 24:
                obj->unk14 = D_803A6FC8;
                obj->unk15 = D_803A6672;
                obj->unk16 = D_803A7025;
                obj->unk17 = D_803A666C;
                obj->unk1C = D_803A6FD4;
                break;
            case 27:
                obj->unk17 = D_803A666C;
                obj->unk1C = D_803A6FD4;
                break;
            case 29:
                obj->unk50 |= 0x4000;
                break;
            case 30:
                obj->unk50 &= ~0x4000;
                break;
            case 32:
                func_800D05E0(obj, D_803A6A00);
                break;
            case 34:
                obj->unk4E = D_803A66BE;
                break;
            case 35:
                obj->unk40 = D_803A69F8;
                obj->unk20 = 0.0001f;
                break;
            case 36:
                obj->unk20 = 0.0f;
                break;
            case 37:
                obj->unk50 &= ~3;
                obj->unk50 |= D_803A6A00 & 3;
                break;
            case 40:
                obj->unk44 = D_803A69E8;
                obj->unk48 = D_803A69EC;
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
                s->ptr = 0;
                return;
            }
        }
        s->ptr = ptr;
    }
    func_800D0A74(obj);
}
