/* SPAN 0x800BA1BC */
#include "types.h"

typedef struct { s32 key; s32 val; s32 pad; } Ent;
typedef struct {
    u8 pad0[8];
    s32 n0;              /* 0x8 */
    Ent e0[256];         /* 0xC */
    s32 padC0C;
    s32 n1;              /* 0xC10 */
    Ent e1[256];         /* 0xC14 */
    s32 pad1814;
    s32 n2;              /* 0x1818 */
    Ent e2[0xAF0];       /* 0x181C */
} Tbl;
typedef struct { s32 a, b, c, d, e, f; } Rec;
typedef struct { u8 pad[0x2C]; Rec *recs; } Src;

#define ADDER(name, N, E, MAX)                      \
    static inline s32 name(Tbl *t, s32 key, s32 val) { \
        s32 i;                                      \
        for (i = 0; i < t->N; i++) {                \
            if (key == t->E[i].key) {               \
                return 0;                           \
            }                                       \
        }                                           \
        if (t->N >= MAX) {                          \
            return -1;                              \
        }                                           \
        t->E[t->N].key = key;                       \
        t->E[t->N++].val = val;                     \
        return 0;                                   \
    }
ADDER(add2, n2, e2, 0xAF0)
ADDER(add0, n0, e0, 0x100)
ADDER(add1, n1, e1, 0x100)

s32 func_800B9FD4(Tbl *t, Src *s, s32 idx) {
    Rec *r = &s->recs[idx];

    if (add2(t, r->a, r->b) < 0) {
        return -1;
    }
    if (add0(t, r->c, r->d) < 0) {
        return -1;
    }
    if (r->e != -1) {
        if (add1(t, r->e, r->f) < 0) {
            return -1;
        }
    }
    return 0;
}
