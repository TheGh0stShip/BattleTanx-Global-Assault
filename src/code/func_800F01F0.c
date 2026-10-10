/* SPAN 0x800F0618 */
/* RODATA_VRAM 0x80076D98 */
#include "types.h"

typedef struct P { u8 pad[0x3A]; u8 b3A; } P;
typedef struct L {
    struct L *next; s32 pad4; P *p; f32 pos[3]; u8 pad18[9]; u8 type; u8 pad22[2]; f32 scale;
} L;
typedef struct A { u8 pad[0x14]; L *list; } A;
typedef struct Pl { u8 pad[168]; u8 f168[592 - 168]; } Pl;
typedef struct DL { void *dl; s32 pad; } DL;
typedef struct M { f32 m[16]; s32 x; } M;

extern u8 D_802194A5;
extern Pl D_80235F00[];
extern s32 D_80125630;
extern DL D_801257D8[];
extern void *D_80125968;
extern void *D_8012596C;
extern void *D_80125970;
extern u8 D_80125978[];
extern u8 func_800AD14C(f32, f32, f32, u8);
extern f32 func_8009D8A0(f32);
extern s32 func_8009FFB8(void *, f32 *, f32 *, u16, f32, f32, M *);
extern s32 func_8007B0E4(u32 *, s32, void *);
extern void func_8007B1F0(void *, s32, void *, s32, s32, M *, s32, s32, s32, s32);

#define DRAW(c1, c2, display_data)                                               \
    prim = c1;                                                                   \
    env = c2;                                                                    \
    g[0] = 0xFA000000;                                                           \
    g[1] = prim;                                                                 \
    g[2] = 0xFB000000;                                                           \
    g[3] = env;                                                                  \
    func_8007B1F0(display_data, func_8007B0E4(g, 2, D_801257D8[D_80125630 & 1].dl), D_80125978, 0, 0, &mt, 0, i, 1, 1)

void func_800F01F0(A *a) {
    L *n;
    s32 i;
    u8 mask;
    u8 cnt;
    Pl *pl;
    u8 *pp;
    f32 r;
    M mt;
    u32 g[4];
    u32 prim;
    u32 env;

    for (n = a->list; n != 0; n = n->next) {
        mask = func_800AD14C(n->pos[0], n->pos[1], n->scale * 64.0f, n->p->b3A);
        cnt = D_802194A5;
        for (i = 0; i < cnt; i++) {
            if (!(n->scale > 0.0f)) {
                continue;
            }
            if (!(u8)((1 << i) & mask)) {
                continue;
            }
            pl = (i == 127) ? 0 : &D_80235F00[i];
            pp = pl->f168;
            if (n->next == 0) {
                break;
            }
            mt.x = 0;
            r = func_8009D8A0(32.0f);
            if (!func_8009FFB8(pp, n->pos, n->next->pos, (u32)((r - 16.0f) * 182.04444444), n->scale, 64.0f, &mt)) {
                continue;
            }
            switch (n->type) {
            case 26:
                DRAW(0x0666FF01, 0xC5DEFF04, D_80125968);
                break;
            case 24:
                DRAW(0x8F428F01, 0xE1D48004, D_80125968);
                break;
            case 8: case 9: case 10: case 11:
                DRAW(0x2D001E01, 0x5A1A1A03, D_8012596C);
                break;
            case 5: case 6: case 7:
                DRAW(0x0C000001, 0x12081C01, D_80125970);
            case 1: case 2: case 3: case 4:
                DRAW(0x0C000001, 0x12081C01, D_80125970);
                break;
            default:
                DRAW(0xE11E1E01, 0xFACA0003, D_8012596C);
                break;
            }
        }
    }
    D_80125630++;
}
