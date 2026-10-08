/* SPAN 0x800C8238 */
/* Exact via two label-gated func_800C7C10 normalizer rules (one reorder and
 * one inserted sltu), not a pure C source match. Uses the libultra gbi forms
 * of gDPSetPrimColor and gDPScisFillRectangle. See
 * docs/NORMALIZER_ASSISTED.md. */
/* RODATA_VRAM 0x80073D3C */
#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

#define G_SETPRIMCOLOR 0xFA
#define G_FILLRECT 0xF6
#define _SHIFTL(v, s, w) (((u32)(v) & ((0x01 << (w)) - 1)) << (s))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define gDPSetPrimColor(pkt, m, l, r, g, b, a)                                          \
    {                                                                                    \
        Gfx* _g = (Gfx*)(pkt);                                                           \
        _g->w0 = (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
        _g->w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); \
    }
#define gDPScisFillRectangle(pkt, ulx, uly, lrx, lry)                                   \
    {                                                                                    \
        Gfx* _g = (Gfx*)(pkt);                                                           \
        _g->w0 = (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL(MAX((lrx), 0), 14, 10) |          \
                  _SHIFTL(MAX((lry), 0), 2, 10));                                        \
        _g->w1 = (_SHIFTL(MAX((ulx), 0), 14, 10) | _SHIFTL(MAX((uly), 0), 2, 10));       \
    }

typedef struct {
    u8 pad0[2];
    u16 w;
    u16 h;
} HudImage;

typedef struct {
    s16 x;
    s16 y;
    s16 x4;
    s16 y4;
    u8 pad4[0xC];
    f32 scale;
    HudImage* image;
} MapView;

typedef struct {
    u8 pad0[2];
    s16 x;
    s16 y;
    s16 x4;
    s16 y4;
} HudPos;

typedef struct {
    u8 pad0[0x16];
    u16 x;
    u16 y;
} HudItem;

typedef struct {
    s16 x;
    s16 y;
    u16 angle;
    u16 flags;
} MapMarker;

typedef struct {
    u8 pad0[0xC];
    void* image;
} MarkerSprite;

extern s16 D_803977F0;
extern s16 D_803977F2;
extern f32 D_80397800;
extern HudImage* D_80397804;
extern s8 D_80117EB0;
extern u16 D_8011F1F4;
extern MapMarker D_803A6060[];
extern f32 D_8011F214;
extern u8 D_8011F21C[][3];
extern MarkerSprite D_801168D0;
extern u8 D_8011DD00[];
extern u8 D_8011DCC8[];
extern u8 D_8011DC90[];
extern void func_8007BDFC(Gfx** dl, s32 mode);
extern void func_8007C9B8(Gfx** dl, void* sprite, s16 x, s16 y, f32 sx, f32 sy);
extern void func_800C7650(Gfx** dl, s16 x, s16 y, s16 w, s16 h);

void func_800C7C10(Gfx** pdl, HudItem* item, HudPos* pos) {
    Gfx* dl = *pdl;
    u16 mode;
    u16 i;
    s16 mx;
    s16 my;
    u16 flags;
    s16 x;
    s16 y;
    s16 x4;
    s16 y4;
    u8 r;
    u8 g;
    u8 b;
    u16 angle;
    f32 sx;
    f32 sy;

    func_8007BDFC(&dl, 4);
    gDPSetPrimColor(dl, 0, 0, 0, 0, 0, 0x96);
    dl++;
    func_8007C9B8(&dl, D_80397804, pos->x, pos->y, 1.0f, 1.0f);
    mode = 0;
    if (D_80117EB0 < 3) {
        func_800C7650(&dl, pos->x, pos->y, D_80397804->w, D_80397804->h);
    }
    for (i = 0; i < D_8011F1F4; i++) {
        mx = (D_803A6060[i].x - D_803977F0) * D_80397800;
        my = (D_803A6060[i].y - D_803977F2) * D_80397800;
        flags = D_803A6060[i].flags;
        if ((flags & 0x100) && D_8011F214 > 0.0f) {
            continue;
        }
        if (mx >= 0 && mx < D_80397804->w && my >= 0 && my < D_80397804->h) {
                x = item->x + pos->x + mx;
                y = item->y + pos->y + my;
                r = D_8011F21C[flags & 0xF][0];
                g = D_8011F21C[flags & 0xF][1];
                b = D_8011F21C[flags & 0xF][2];
                if (flags & 0x8000) {
                    gDPSetPrimColor(dl++, 0, 0, r, g, b, 0xFF);
                    x4 = x - 4;
                    y4 = y - 4;
                    angle = D_803A6060[i].angle;
                    if ((u16)(angle - 0x1000) < 0x2000) {
                        sx = 1.0f;
                        sy = -1.0f;
                        D_801168D0.image = D_8011DD00;
                    } else if ((u16)(angle - 0x3000) < 0x2000) {
                        sx = sy = 1.0f;
                        D_801168D0.image = D_8011DCC8;
                    } else if ((u16)(angle - 0x5000) < 0x2000) {
                        sx = sy = 1.0f;
                        D_801168D0.image = D_8011DD00;
                    } else if ((u16)(angle - 0x7000) < 0x2000) {
                        sx = sy = 1.0f;
                        D_801168D0.image = D_8011DC90;
                    } else if ((u16)(angle + 0x7000) < 0x2000) {
                        sx = -1.0f;
                        sy = 1.0f;
                        D_801168D0.image = D_8011DD00;
                    } else if ((u16)(angle + 0x5000) < 0x2000) {
                        sx = -1.0f;
                        sy = 1.0f;
                        D_801168D0.image = D_8011DCC8;
                    } else if ((u16)(angle + 0x3000) < 0x2000) {
                        sx = sy = -1.0f;
                        D_801168D0.image = D_8011DD00;
                    } else {
                        sx = 1.0f;
                        sy = -1.0f;
                        D_801168D0.image = D_8011DC90;
                    }
                    if (mode != 2) {
                        func_8007BDFC(&dl, 2);
                        mode = 2;
                    }
                    func_8007C9B8(&dl, &D_801168D0, x4, y4, sx, sy);
                } else if (flags & 0x4000) {
                    Gfx* gfx;

                    if (mode != 3) {
                        func_8007BDFC(&dl, 3);
                        mode = 3;
                    }
                    gfx = dl;
                    gDPSetPrimColor(gfx++, 0, 0, r, g, b, 0xFF);
                    gDPScisFillRectangle(gfx++, (s16)(x - 1), (s16)(y - 1), (s16)(x - 1) + 3, (s16)(y - 1) + 3);
                    dl = gfx;
                } else {
                    Gfx* gfx;

                    if (mode != 3) {
                        func_8007BDFC(&dl, 3);
                        mode = 3;
                    }
                    gfx = dl;
                    gDPSetPrimColor(gfx++, 0, 0, r, g, b, 0xFF);
                    gDPScisFillRectangle(gfx++, x, y, x + 2, y + 2);
                    dl = gfx;
                }
        }
    }
    D_8011F1F4 = 0;
    *pdl = dl;
}
