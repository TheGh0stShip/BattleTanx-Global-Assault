#include "types.h"

typedef struct {
    u8 pad0[0x68];
    void* label;
    u8 pad6C[4];
    u8 colorA;
    u8 pad71[0xF];
    u8 colorB;
    u8 pad81;
    s16 x;
    u8 pad84[0xC];
    u8 color;
    u8 pad91[7];
    void* icon;
    u8 pad9C[6];
    s16 iconY;
    u8 padA4[0xE];
    s16 frameY;
    u8 padB4[4];
    void* frame;
} ResultsPanel;

typedef struct {
    u8 pad0[4];
    ResultsPanel* panel;
} ResultsPanelRef;

typedef struct {
    u8 pad0[4];
    ResultsPanel* box;
} ResultsBoxRef;

typedef struct {
    s32 value;
} GameMode;

extern ResultsPanelRef D_80120660;
extern ResultsBoxRef D_80121030;
extern u8 D_803A665C;
extern GameMode D_8021949C;
extern s32 D_803A8310;
extern u8 D_801201A8[];
extern u8 D_8012019C[];
extern u8 D_8012012C[];
extern u8 D_801201BC[];
extern u8 D_801201CC[];
extern u8 D_80120134[];
extern void func_800CD85C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_800CDD70(s32 arg0, s32 arg1, u16 arg2, u16 arg3, s32 arg4, s32 arg5,
                          s16 arg6, s16 arg7, s16 arg8);

void func_800CE610(s32 arg0, s32 arg1, u16 arg2, u16 arg3, s32 arg4, s32 arg5, u16 arg6,
                   u16 arg7, u16 arg8) {
    ResultsPanel* p = D_80120660.panel;

    p->icon = D_801201A8;
    p->frame = D_8012019C;
    p->iconY = 0xB9;
    p->frameY = 0xC8;
    p->label = D_8012012C;
    D_803A665C = 1;
    if (D_8021949C.value == 16) {
        p->color = 1;
        p->x = 0xAF;
    } else {
        p->color = 4;
        p->x = 0x23;
    }
    p = D_80121030.box;
    p->colorA = 4;
    p->colorB = 5;
    func_800CD85C(D_803A8310 + 1, arg1, 0, (s16)arg8);
    func_800CDD70(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

void func_800CE77C(s32 arg0, s32 arg1, u16 arg2, u16 arg3, s32 arg4, s32 arg5) {
    ResultsPanel* p = D_80120660.panel;

    p->icon = D_801201BC;
    p->frame = D_801201CC;
    p->iconY = 0xB9;
    p->frameY = 0xC8;
    p->label = D_80120134;
    p = D_80121030.box;
    D_803A665C = 0;
    p->colorA = 1;
    p->colorB = 1;
    func_800CDD70(arg0, arg1, arg2, arg3, arg4, arg5, 0, 0, 0);
}
