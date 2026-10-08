/* SPAN 0x800CC2F8 */
/* Seven functions. The catalogue only names func_800CBE40 and func_800CC0B0;
 * func_800CC03C, func_800CC058, func_800CC06C, func_800CC1D8 and
 * func_800CC270 are real entries it misses. func_800CC06C is the out-of-line
 * copy of an `inline` queue push that func_800CC0B0 also expands. All other
 * func_800CBxxx/func_800CCxxx labels inside the span are interior labels. */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
    u16 c;
    u8 padA[0x16];
} SaveSlot;

typedef struct {
    u8* base;
} RaceHudRef;

extern u8 D_803A61A4[];
extern s32 D_80125548;
extern s32 D_80117EB8;
extern s32 D_80125550;
extern s32 D_80125554;
extern s32 D_8012554C;
extern s32 D_80117EC0;
extern s32 D_80117EC8;
extern s8 D_80117ED0;
extern s8 D_80117ED1;
extern RaceHudRef D_80119320;
extern u8 D_80118EDC[];
extern u8 D_80118EE0[];
extern s16 D_8011F230;
extern s16 D_8011F232;
extern u16 D_8011F234;
extern u16 D_8011F236;
extern s16 D_8011F828;
extern s32 D_803A62A8[];
extern SaveSlot D_803A6360[];
extern u8 D_803A6564[];
extern u16 D_8011481C;
extern s32 D_80114820;
extern u8* func_800CB934(void);
extern void func_800C0A6C(void* arg0);
extern void func_800C0C18(void* arg0, u16 arg1);
extern void MusSetMasterVolume(s32 type, s32 volume);
extern void func_80097C1C(s32 volume);
extern s16 func_800991CC(u16 arg0, SaveSlot* slots, void* arg2);
extern s16 func_80099784(u16 arg0, s32* out);

void func_800CBE40(void) {
    u8* p = D_803A61A4;

    D_80125548 = *p++;
    switch (*p++) {
    case 1:
        D_80117EB8 = 2;
        break;
    case 2:
        D_80117EB8 = 3;
        break;
    case 0:
    default:
        D_80117EB8 = 1;
        break;
    }
    p += 2;
    D_80125550 = *(s32*)p;
    p += 4;
    D_80125554 = *(s32*)p;
    D_8012554C = *(s32*)(p + 4);
    p = func_800CB934();
    D_80117EC0 = *p++;
    D_80117EC8 = *p++;
    D_80117ED0 = p[0];
    D_80117ED1 = p[1];
    func_800C0A6C(D_80119320.base + 0x230);
    func_800C0C18(D_80119320.base + 0x1B0, D_80117ED0);
    func_800C0C18(D_80119320.base + 0x1F0, D_80117ED1);
    MusSetMasterVolume(1, D_80117ED0 * 360);
    func_80097C1C(D_80117ED1 * 360);
    if (D_80117EC0 == 1) {
        *(void**)(D_80119320.base + 0x258) = D_80118EDC;
    } else {
        *(void**)(D_80119320.base + 0x258) = D_80118EE0;
    }
    if (D_80117EC8 == 1) {
        *(void**)(D_80119320.base + 0x278) = D_80118EDC;
    } else {
        *(void**)(D_80119320.base + 0x278) = D_80118EE0;
    }
}

void func_800CC03C(void) {
    D_8011F230--;
}

void func_800CC058(void) {
    D_8011F230 = -1;
}

inline void func_800CC06C(s32 cmd) {
    if (D_8011F230 < 10) {
        D_803A62A8[++D_8011F230] = cmd;
    }
}

u16 func_800CC0B0(void) {
    s32 info;
    u16 i;

    for (i = 0; i < 16; i++) {
        D_803A6360[i].b = 0;
        D_803A6360[i].c = 0;
        D_803A6360[i].a = 0;
    }
    D_8011F232 = func_800991CC(D_8011F234, D_803A6360, D_803A6564);
    if (D_8011F232 != 0) {
        func_800CC06C(6);
        return D_8011F232;
    }
    D_8011F232 = func_80099784(D_8011F234, &info);
    if (D_8011F232 != 0) {
        func_800CC06C(6);
        return D_8011F232;
    }
    D_8011F236 = 1;
    D_8011F828 = info >> 8;
    return 0;
}

s32 func_800CC1D8(void) {
    u16 i;

    if (D_8011F236 == 0 && func_800CC0B0() != 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        if (D_803A6360[i].c == D_8011481C && D_803A6360[i].b == D_80114820) {
            return i;
        }
    }
    return -1;
}

s32 func_800CC270(s32 index) {
    s32 r;
    s32 v;
    if (D_8011F236 == 0 && func_800CC0B0() != 0) {
        r = 0;
    } else {
        r = 1;
        v = D_803A6360[index].b;
        if (v == 0 && (v = D_803A6360[index].c) == 0) {
            r = D_803A6360[index].a != 0;
        }
    }
    return r;
}
