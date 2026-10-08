#include "types.h"
typedef struct N {
    u8 pad[12]; s32 kind; struct H *head; struct N *next; f32 pos; u16 a; s16 w; u16 id; u16 h;
    s32 e; s32 f; s32 g; s32 dd; s32 c; s32 cc; u16 k;
} N;
typedef struct H { u8 pad[40]; f32 len; f32 base; N *list; } H;
extern void *func_800A18D0(s32, s32);
void func_800EF400(H *hd, s32 kind, u16 a, s16 w, u16 id, s32 e, s32 f, s32 dd, s32 g, u16 k) {
    N *n = func_800A18D0(2, 64);
    N *p;
    s32 v;
    if (n != 0) {
        n->h = 0xFFFF;
        n->pos = hd->base + hd->len;
        hd->len += w;
        n->kind = kind;
        n->id = id;
        n->w = w;
        n->e = e;
        n->dd = dd;
        n->f = f;
        n->g = g;
        n->next = 0;
        n->head = hd;
        n->a = a;
        n->k = k;
        switch (n->kind) {
        case 0: case 2: case 6: v = 200000; goto set;
        case 3: v = 100; goto set;
        case 1: case 4: v = 20;
        set:
            n->cc = v;
            n->c = v;
            break;
        }
        if (hd->list == 0) {
            hd->list = n;
        } else {
            p = hd->list;
            while (p->next != 0) p = p->next;
            p->next = n;
        }
    }
}
