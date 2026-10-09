/* SPAN 0x8008A764 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { int serial; int type; char p8[4]; int alive; char p10[0x250 - 0x10]; } Obj;
typedef struct { Obj *node; int serial; } Ent;
typedef struct { u8 active; u8 done; u16 depth; Ent e[5]; } Stack;
extern char *D_80114680;
extern struct { char p[6]; u8 count; } D_802194A0;
extern Obj D_80235F00[];
Ent *func_8008A3F8(Stack *, int);

static inline int valid(Ent *e) {
    Obj *o = e->node;
    if (o == 0 || o->serial != e->serial) return 0;
    switch (o->type) {
    case 0x43:
        return 1;
    case 4:
        return o->alive != 0;
    }
    return 0;
}

static inline Obj *wp_at(int i) { return &D_80235F00[i]; }
static inline Obj *waypoint(int i) {
    if (i == 127) return 0;
    return wp_at(i);
}

Ent *func_8008A62C(Stack *s) {
    char *g = D_80114680;
    Ent *e;

    if (s->done) {
        s->depth++;
        if (s->depth >= D_802194A0.count) {
            if (s->depth == D_802194A0.count) return (Ent *)(g + 0xC064);
            s->depth = 0;
        }
        e = func_8008A3F8(s, s->depth);
    } else {
        s->done = 1;
        e = &s->e[s->depth];
        if (!valid(e)) {
            u16 d = s->depth;
            Obj *w = waypoint(d);
            {
                Ent *t = &s->e[d];
                t->node = w;
                t->serial = w->serial;
                e = t;
            }
        }
    }
    return e;
}
