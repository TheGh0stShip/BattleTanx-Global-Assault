#include "types.h"

typedef struct { f32 m[4][4]; } MtxF;
typedef struct Item {
    void *dl;
    void *tex;
    u8 pad8;
    u8 flags;
    u16 angle;
    f32 pos[3];
    f32 sx;
    f32 sy;
    f32 lift;
    struct Item *next;
} Item;
typedef struct Node {
    void *dl;
    Item *items[5];
    struct Node *next;
} Node;
typedef struct { u8 pad[0x48]; u8 sub[0x190]; } Cam;
typedef struct { u8 pad[0x78]; Cam cam; } Obj;

extern u8 D_802194A5;
extern Node *D_8017CE60[][32];
extern Obj D_80235F00[];
extern u32 *func_8007AD94(void);
extern u32 *func_8007ADB0(void);
extern void func_8007ACF8(void *);
extern void func_8007AB64(u8 *, u16);
extern void func_8007AC34(void *, s32);
extern void func_8009EEE0(MtxF *);
extern void func_8009F8A0(MtxF *, MtxF *, u16);
extern void func_8009F824(MtxF *, f32, f32, f32);
extern void func_8009F288(void *, f32 *, f32 *);
extern void func_8009EF30(MtxF *, f32 *);
extern void guMtxF2L(MtxF *, void *);

s32 func_8007B8EC(s32 idx, u8 *a1, void **a2, u16 *a3) {
    struct { MtxF m; void *p; } mm;
    MtxF rot;
    f32 vec[3];
    s32 last = -1;
    s32 count;
    s32 j;
    u32 *gm;
    void *tex;
    u32 limit;
    Cam *ctx;
    s32 i;
    Obj *obj;
    Item *it;
    Node *node;
    u32 *dl;
    f32 y;
    f32 s;
    f32 lift;

    gm = func_8007AD94();
    dl = func_8007ADB0();
    count = D_802194A5;
    tex = 0;
    ctx = 0;
    limit = gm[1] + 0xADE0;
    for (j = 0; j < 32; j++) {
        for (node = D_8017CE60[idx][j]; node != 0; node = node->next) {
            if (node->dl != 0) {
                func_8007ACF8(node->dl);
                tex = 0;
            }
            for (i = 0; i < count; i++) {
                for (it = node->items[i]; it != 0; it = it->next) {
                    if (limit < *dl) return -1;
                    if (i != last) {
                        if (i == 0x7F) obj = 0;
                        else obj = &D_80235F00[i];
                        ctx = &obj->cam;
                        func_8007ACF8(a2[i]);
                        last = i;
                        func_8007AB64(a1 + i * 0x44, a3[i]);
                    }
                    if (it->tex != tex) {
                        func_8007ACF8(it->tex);
                        tex = it->tex;
                    }
                    func_8009EEE0(&rot);
                    mm.p = 0;
                    if (it->flags & 1) {
                        if (it->angle != 0) func_8009F8A0(&rot, &mm.m, it->angle);
                        else mm.m = rot;
                        func_8009F824(&mm.m, it->sx, it->sy, 1.0f);
                    } else {
                        rot.m[0][0] = it->sx;
                        rot.m[1][1] = it->sy;
                        if (it->angle != 0) func_8009F8A0(&rot, &mm.m, it->angle);
                        else mm.m = rot;
                    }
                    func_8009F288(&ctx->sub, it->pos, vec);
                    lift = it->lift;
                    if (lift != 0.0f) {
                        if (((y = vec[1]) > 0.0f ? y : -y) > 4.0) {
                            s = (y + lift) / y;
                            vec[0] *= s;
                            vec[2] *= s;
                            vec[1] = y * s;
                        }
                    }
                    func_8009EF30(&mm.m, vec);
                    if (!(dl[2] < gm[3] + 0xC000)) return -1;
                    guMtxF2L(&mm.m, (void *)dl[2]);
                    mm.p = (void *)dl[2];
                    dl[2] += 0x40;
                    func_8007AC34(mm.p, 2);
                    func_8007ACF8(it->dl);
                }
            }
        }
    }
    return 0;
}
