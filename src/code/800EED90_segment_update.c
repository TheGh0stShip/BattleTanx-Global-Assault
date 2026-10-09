/* func_800EED90 (0x800EED90-0x800EF400, 0x670).
 * Origin: claude-work/output/workers/r3/ed800/f_800EED90.c; this lane added the rodata placement (tools/autoplace.py).
 * SPAN 0x800EF400
 */
/* RODATA_VRAM 0x80076AE8 */
#include "types.h"
typedef struct N {
    u8 pad[12]; u32 kind; struct H *head; struct N *next; f32 pos; u16 a; s16 w; u16 id; u16 h;
    s32 e; s32 f; s32 g; s32 dd; s32 c;
} N;
typedef struct H {
    u8 pad[12]; f32 x; f32 y; f32 f14; f32 z; u8 pad1C[8]; u16 ang; u8 b26; u8 pad27;
    f32 len; f32 base; N *list; u16 spd; u16 wait; s32 time; u8 mode; u8 st; u8 pad3E[2]; s32 snd;
} H;
extern s32 D_80117EB4;
extern s32 D_8021945C;
extern u32 func_8009D914(void);
extern void func_800EEADC(N *, s32);
extern void func_800EEC2C(N *);
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern void func_800F6650(f32 *, u16, u8, s32, s32, s32);
extern void func_800B22F8(u16);
extern void func_800A1BE0(void *);
extern f32 func_800EE490(f32, f32, f32, s32, s32);
extern void func_800EE8C0(H *, f32);
extern f32 func_80097EE4(f32, f32, u8);
extern s32 func_800981E0(s32);
extern s32 func_80097A6C(s32, s16, s32, s32, s32);
extern void func_800FB8E0(s32, s16);
extern void func_80097BA4(s32, s32);

void func_800EED90(H *arg0, s32 *done) {
    H *h = arg0;
    f32 pos[3];
    N *n;
    N *nx;
    s32 alive;
    f32 v;
    f32 amt;
    f32 sc;
    f32 d;
    f32 t;

    v = 0.0f;
    alive = 0;
    for (n = h->list; n != 0; n = n->next) {
        if (n->c == -100) {
            if (n->next != 0 && n->next->c != -100 && func_8009D914() % 6 == 0) {
                func_800EEADC(n->next, 1);
            }
        }
        if (n->c != -100) {
            if (n->next != 0 && n->next->c == -100 && func_8009D914() % 6 == 0) {
                func_800EEADC(n, 1);
            }
        }
    }
    for (n = h->list; n != 0; n = n->next) {
        if (n->c == 0) {
            n->c = -1;
            switch (n->kind) {
            case 3:
                pos[0] = h->x + (n->pos + n->w / 2) * func_8009D4B0(h->ang);
                pos[1] = h->y + (n->pos + n->w / 2) * func_8009D510(h->ang);
                pos[2] = h->z;
                func_800F6650(pos, h->ang, h->b26, n->g, n->f, n->w / 2);
                if (n->h != 0xFFFF) {
                    func_800B22F8(n->h);
                }
                func_800A1BE0(n);
                *done = 1;
                break;
            case 1:
                func_800EEADC(n, 1);
                break;
            case 4:
                func_800EEC2C(n);
                break;
            }
        } else {
            alive += n->c > 0;
        }
    }
    if (alive == 0) {
        n = h->list;
        while (n != 0) {
            nx = n->next;
            func_800A1BE0(n);
            n = nx;
        }
        *done = 1;
        return;
    }
    if (D_80117EB4 == 10) {
        return;
    }
    switch (h->mode) {
    case 0:
        switch (h->st) {
        case 0:
            v = func_800EE490(h->f14 - h->len, h->base, h->spd, 0, 0);
            d = h->base + v;
            t = h->f14 - h->len;
            if (t - 1.0f < d) {
                h->st = 3;
                d = t;
                h->time = D_8021945C;
            }
            func_800EE8C0(h, d);
            break;
        case 1:
            v = func_800EE490(h->f14 - h->len, h->base, h->spd, 0, 1);
            d = h->base + v;
            if (d < 1.0f) {
                h->st = 2;
                d = 0.0f;
                h->time = D_8021945C;
            }
            func_800EE8C0(h, d);
            break;
        case 3:
            if (h->wait < D_8021945C - h->time) {
                h->st = 1;
            }
            break;
        case 2:
            if (h->wait < D_8021945C - h->time) {
                h->st = 0;
            }
            break;
        }
        break;
    case 1:
        switch (h->st) {
        case 0:
            v = func_800EE490(h->f14 - h->len, h->base, h->spd, 1, 0);
            d = h->base + v;
            if (h->f14 - h->len - 1.0f < d) {
                h->st = 2;
                d = 0.0f;
                h->time = D_8021945C;
            }
            func_800EE8C0(h, d);
            break;
        case 2:
            if (h->wait < D_8021945C - h->time) {
                h->st = 0;
            }
            break;
        }
        break;
    }
    if (v != 0.0f) {
        pos[0] = h->x + func_8009D4B0(h->ang) * (h->base + h->len / 2.0f);
        pos[1] = h->y + func_8009D510(h->ang) * (h->base + h->len / 2.0f);
        sc = func_80097EE4(pos[0], pos[1], h->b26);
        if (v > 0.0f) {
            amt = v * sc;
        } else {
            amt = -v * sc;
        }
        if (amt <= 0.2) {
            amt = 0.0f;
        }
    } else {
        amt = 0.0f;
    }
    if (amt != 0.0f) {
        if (h->snd == 0 || func_800981E0(h->snd) == 0) {
            h->snd = func_80097A6C(0x2C, amt * 255.0f, 0x80, 0, -1);
        } else {
            func_800FB8E0(h->snd, amt * 255.0f);
        }
    } else {
        func_80097BA4(h->snd, 0);
        h->snd = 0;
    }
}
