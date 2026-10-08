typedef unsigned char u8; typedef signed char s8; typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32;

typedef struct { u8 a; u8 b; u8 pad[6]; void *ptr; u8 pad2[4]; } Sub;
typedef struct { u8 pad[24]; void *p18; u8 pad2[20]; Sub s30; Sub s40; } Obj;
typedef struct { s32 a; Obj *obj; s32 b; void *p0C; void *p10; s16 idx; u8 pad[0x70 - 0x16]; } Slot;

extern u8 D_80117EB0;
extern s32 D_80117EB4;
extern s32 D_80117ED4[];
extern u32 D_80117EF4[];
extern s32 D_80117F04[];
extern u16 D_8011DC80;
extern u16 D_8011DC82;
extern u16 D_8011DC84;
extern Slot D_8011DAE4[];
extern char D_8011D408[], D_8011D424[], D_8011D9E8[], D_8011D9F0[], D_8011D9FC[], D_8011DA04[], D_8011DA10[];
extern char D_8011DA1C[], D_8011DA28[], D_8011DA34[], D_8011DA40[], D_8011DA4C[], D_8011DA58[], D_8011DA64[];
extern char D_80118EDC[], D_80118EE0[];
void func_800BEBA8(Slot *, s32, s32);
void func_800C48F0(void);

static inline void setIcon(Sub *sub, s32 m) {
    switch (m) {
    case 1: sub->ptr = D_8011DA28; break;
    case 2: sub->ptr = D_8011DA34; break;
    case 3: sub->ptr = D_8011DA1C; break;
    case 4: sub->ptr = D_8011DA40; break;
    case 6: sub->ptr = D_8011DA4C; break;
    case 5: sub->ptr = D_8011DA58; break;
    case 0: default: sub->ptr = D_8011DA64; break;
    }
}

static inline void setColor(Sub *sub, u32 m) {
    switch (m) {
    case 1: sub->ptr = D_8011DA10; break;
    case 3: sub->ptr = D_8011DA04; break;
    case 2: default: sub->ptr = D_8011D9FC; break;
    }
}

#define ACTIVE(n) \
    D_80117F04[i] = 2; \
    { Obj *obj = D_8011DAE4[n].obj; \
    D_8011DAE4[n].p0C = D_8011D408; \
    D_8011DAE4[n].p10 = D_8011D424; \
    obj->p18 = D_8011D9E8; \
    obj->s30.b = 144; \
    setColor(&obj->s30, D_80117EF4[i]); \
    obj->s40.b = 16; \
    setIcon(&obj->s40, D_80117ED4[i]); } \
    D_8011DAE4[n].idx = n; \
    func_800BEBA8(&D_8011DAE4[n], arg0, 0);

#define INACTIVE(n, m) \
    obj = D_8011DAE4[n].obj; \
    D_8011DAE4[n].p0C = D_8011D408; \
    D_8011DAE4[n].p10 = D_8011D424; \
    obj->p18 = D_8011D9F0; \
    obj->s30.b = 1; \
    obj->s40.b = 1; \
    setIcon(&obj->s40, m); \
    if (D_80117F04[i] == 0) obj->s30.ptr = D_80118EE0; else obj->s30.ptr = D_80118EDC; \
    D_8011DAE4[n].idx = 0; \
    func_800BEBA8(&D_8011DAE4[n], arg0, 3);

static inline u16 chk(void) {
    if (D_80117EB4 == 8) {
        if (D_8011DC84 != 8) return 1;
        return 0;
    }
    if (D_8011DC84 == 8) return 1;
    return 0;
}

s32 func_800C4F98(s32 arg0) {
    u16 i;
    Obj *obj;

    D_8011DC80 = (s8)D_80117EB0;
    if (D_8011DC80 != D_8011DC82 || chk()) {
        D_8011DC82 = D_8011DC80;
        D_8011DC84 = D_80117EB4;
        for (i = 0; i < 4; i++) {
            D_80117ED4[i] = 0;
            D_80117F04[i] = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            if (D_80117ED4[i] == 0) {
                if (D_80117EB4 == 8) D_80117ED4[i] = 5; else D_80117ED4[i] = 3;
            }
            ACTIVE(0)
            break;
        case 1:
            if (!(i < (s8)D_80117EB0)) {
                if ((s8)D_80117EB0 == i) {
                    D_80117F04[i] = 1;
                    if (D_80117ED4[i] == 0) {
                        if (D_80117EB4 == 8) D_80117ED4[i] = 6; else D_80117ED4[i] = D_80117ED4[0] - 1;
                    }
                }
                INACTIVE(1, D_80117ED4[i])
            } else {
                if (D_80117ED4[i] == 0) {
                    if (D_80117EB4 == 8) D_80117ED4[i] = 6; else D_80117ED4[i] = 2;
                }
                ACTIVE(1)
            }
            break;
        case 2:
            if (!(i < (s8)D_80117EB0)) {
                INACTIVE(2, D_80117ED4[i])
            } else {
                if (D_80117ED4[i] == 0) {
                    if (D_80117EB4 == 8) D_80117ED4[i] = 5; else D_80117ED4[i] = 4;
                }
                ACTIVE(2)
            }
            break;
        case 3:
            if (!(i < (s8)D_80117EB0)) {
                INACTIVE(3, D_80117ED4[i])
            } else {
                if (D_80117ED4[i] == 0) {
                    if (D_80117EB4 == 8) D_80117ED4[i] = 6; else D_80117ED4[i] = 1;
                }
                ACTIVE(3)
            }
            break;
        }
    }
    func_800C48F0();
    return 1;
}
