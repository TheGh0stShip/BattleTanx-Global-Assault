/* SPAN 0x800BEE0C */
/* RODATA_VRAM 0x80073200 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct Tree Tree;
typedef struct Node Node;

typedef struct {
    float t0;           /* 0x00 */
    float t1;           /* 0x04 */
    s16 x;              /* 0x04 (move keys) */
} KeyArgs;

typedef struct {
    float accel;        /* 0x00 */
    float init;         /* 0x04 */
    s16 vx;             /* 0x08 */
    s16 ylim;           /* 0x0A */
} FallArgs;

typedef struct {
    float a, b;         /* 0x00 */
} RangeArgs;

typedef struct {
    s16 pad[2];
    s16 x;              /* 0x04 */
    s16 y;              /* 0x06 */
} MoveArgs;

typedef struct Key {
    u16 type;           /* 0x00 */
    float t;            /* 0x04 */
    void *args;         /* 0x08 */
    struct Key *next;   /* 0x0C */
} Key;

typedef struct {
    Key *first;         /* 0x00 */
    Key *cur;           /* 0x04 */
    s16 (*onA)(Tree *t, Node *n);   /* 0x08 */
    s16 (*onB)(Tree *t, Node *n);   /* 0x0C */
    s16 (*onC)(Tree *t, Node *n);   /* 0x10 */
} Anim;

typedef struct {
    float init;         /* 0x00 */
    float t;            /* 0x04 */
    s16 busy;           /* 0x08 */
} Sprite;

struct Node {
    u8 kind;            /* 0x00 */
    u8 flags;           /* 0x01 */
    s16 x;              /* 0x02 */
    s16 y;              /* 0x04 */
    u16 unk6;
    void *data;         /* 0x08 */
    Anim *anim;         /* 0x0C */
};

typedef struct {
    u32 up;             /* 0x00 */
    u32 down;           /* 0x04 */
    u32 back;           /* 0x08 */
    u32 menu;           /* 0x0C */
    u32 a;              /* 0x10 */
    u32 b;              /* 0x14 */
    u32 c;              /* 0x18 */
} Buttons;

typedef struct {
    s16 (*pre)(Tree *t);    /* 0x00 */
    void (*up)(Tree *t);    /* 0x04 */
    void (*down)(Tree *t);  /* 0x08 */
    void (*menu)(Tree *t);  /* 0x0C */
    s16 (*back)(Tree *t);   /* 0x10 */
} Callbacks;

struct Tree {
    int unk0;
    Node *nodes;        /* 0x04 */
    int unk8;
    Buttons *btn;       /* 0x0C */
    Callbacks *cb;      /* 0x10 */
    u16 pad;            /* 0x14 */
};

typedef struct {
    s8 sx;              /* 0x00 */
    s8 sy;              /* 0x01 */
    char p2[2];
    u32 held;           /* 0x04 */
    u32 pressed;        /* 0x08 */
} PadState;

typedef struct {
    s16 ax, ay;         /* 0x00 */
    s16 rx, ry;         /* 0x04 */
} Repeat;

typedef struct {
    u16 flags;          /* 0x00 */
    u16 prev;           /* 0x02 */
    s8 parent;          /* 0x04 */
    s8 child[4];        /* 0x05 */
    char p9[3];
    Tree *tree;         /* 0x0C */
} Slot;

extern Repeat D_80116820[];
extern float D_803A5948;
extern u16 D_803A5970;
extern u16 D_803A5972;
extern Slot D_803A57F0[20];

PadState *func_8009836C(u16 pad);
u16 func_8007D33C(void *spr, float dt);
float func_8009D8A0(float range);
void func_800979F4(int sfx);

u32 func_800BDAF8(u16 pad, u32 up, u32 down, u32 left, u32 right) {
    PadState *ps = func_8009836C(pad);
    u32 in = ps->pressed;
    u32 held = ps->held;
    Repeat *r = &D_80116820[pad];

    if ((up | down | left | right) & 0x50000000) {
        r->ay += ps->sy;
        if (r->ay > 500) {
            in |= 0x40000000;
            r->ay -= 500;
        } else if (r->ay < -500) {
            in |= 0x10000000;
            r->ay += 500;
        }
    }
    if ((up | down | left | right) & 0xA0000000) {
        r->ax += ps->sx;
        if (r->ax > 500) {
            in |= 0x20000000;
            r->ax -= 500;
        } else if (r->ax < -500) {
            in |= 0x80000000;
            r->ax += 500;
        }
    }
    if (up & in) {
        r->ry = 0;
    } else if (up & held) {
        if (++r->ry > 10) {
            in |= up;
            r->ry -= 10;
        }
    }
    if (down & in) {
        r->ry = 0;
    } else if (down & held) {
        if (--r->ry < -10) {
            in |= down;
            r->ry += 10;
        }
    }
    if (right & in) {
        r->rx = 0;
    } else if (right & held) {
        if (++r->rx > 10) {
            in |= right;
            r->rx -= 10;
        }
    }
    if (left & in) {
        r->rx = 0;
    } else if (left & held) {
        if (--r->rx < -10) {
            in |= left;
            r->rx += 10;
        }
    }
    return in;
}
inline s16 func_800BDD64(Node *n) {
    Key *k;

    if (n->anim == 0) {
        return 1;
    }
    k = n->anim->cur;
    if (k->t == 0.0f) {
        return 0;
    }
    k->t -= D_803A5948;
    if (k->t <= 0.0) {
        return 1;
    }
    return 0;
}


s16 func_800BDDC8(Node *n) {
    Key *k;
    MoveArgs *m;
    float f;

    if (n->anim == 0) {
        return 1;
    }
    k = n->anim->cur;
    k->t -= D_803A5948;
    m = k->args;
    if (k->t <= 0.0) {
        switch (k->type) {
        case 3:
            n->x = m->x;
            break;
        case 4:
            n->y = m->y;
            break;
        case 2:
        default:
            n->x = m->x;
            n->y = m->y;
            break;
        }
        return 1;
    }
    f = D_803A5948 / (k->t + D_803A5948);
    switch (k->type) {
    case 3:
        n->x = n->x + f * (m->x - n->x);
        break;
    case 4:
        n->y = n->y + f * (m->y - n->y);
        break;
    case 2:
    default:
        n->x = n->x + f * (m->x - n->x);
        n->y = n->y + f * (m->y - n->y);
        break;
    }
    return 0;
}

inline s16 func_800BDF34(Node *n) {
    Key *k;
    FallArgs *a;

    if (n->anim == 0) {
        return 1;
    }
    k = n->anim->cur;
    a = k->args;
    n->y += (int)k->t;
    n->x += (int)(a->vx * D_803A5948);
    if (n->y >= a->ylim) {
        n->y = a->ylim;
        return 1;
    }
    k->t += a->accel * D_803A5948;
    return 0;
}


inline s16 func_800BDFD4(Node *n) {
    Key *k;
    Sprite *s;
    u16 *loop;

    if (n->anim == 0) {
        return 1;
    }
    k = n->anim->cur;
    s = n->data;
    loop = k->args;
    if (func_8007D33C(s, D_803A5948)) {
        if (*loop != 0) {
            k->t = k->t - 1.0;
            if (k->t <= 0.0f) {
                return 1;
            }
        }
        s->busy = 0;
        return 0;
    }
    return 0;
}

inline s16 func_800BE080(Tree *t, Node *n) {
    if (n->anim == 0) {
        return 1;
    }
    return ((s16 (*)(Tree *, Node *))n->anim->cur->args)(t, n);
}

inline void func_800BE30C(Tree *t, Node *n);

s16 func_800BE0BC(Tree *t, Node *n) {
    s16 r;

    if (n->anim == 0) {
        return 1;
    }
    switch (n->anim->cur->type) {
    case 8:
        return 1;
    case 0:
    case 1:
        r = func_800BDD64(n);
        break;
    case 2:
    case 3:
    case 4:
        r = func_800BDDC8(n);
        break;
    case 5:
        r = func_800BDF34(n);
        break;
    case 6:
        r = func_800BDFD4(n);
        break;
    case 7:
        r = func_800BE080(t, n);
        break;
    case 9:
        return 3;
    default:
        r = 0;
        break;
    }
    if (r) {
        n->anim->cur = n->anim->cur->next;
        func_800BE30C(t, n);
    }
    return 0;
}

inline void func_800BE30C(Tree *t, Node *n) {
    Key *k;
    RangeArgs *ra;

    if (n->anim != 0) {
        if (n->anim->cur != 0) {
            k = n->anim->cur;
            switch (k->type) {
            case 1:
                ra = k->args;
                k->t = ra->a + func_8009D8A0(ra->b - ra->a);
                break;
            case 0:
            case 2:
            case 3:
            case 4:
                k->t = ((RangeArgs *)k->args)->a;
                break;
            case 5:
                k->t = ((FallArgs *)k->args)->init;
                break;
            case 6:
                k->t = *(u16 *)k->args;
                ((Sprite *)n->data)->t = ((Sprite *)n->data)->init;
                ((Sprite *)n->data)->busy = 0;
                break;
            case 7:
            case 8:
            case 9:
                break;
            }
            func_800BE0BC(t, n);
        }
    }
}

void func_800BE3FC(Tree *t) {
    Node *n;

    for (n = t->nodes; n->kind != 0; n++) {
        if (n->anim != 0) {
            n->anim->cur = n->anim->first;
            func_800BE30C(t, n);
        }
    }
}

inline s16 func_800BE538(Tree *t) {
    Node *n;
    s16 r;

    for (n = t->nodes; n->kind != 0; n++) {
        if (n->anim != 0 && n->anim->cur != 0) {
            r = func_800BE0BC(t, n);
            if (r == 1 || r == 3) {
                return r;
            }
        }
    }
    return 0;
}

static inline Node *focused(Tree *t) {
    Node *n;

    for (n = t->nodes; n->kind != 0; n++) {
        if (n->flags & 0x80) {
            return n;
        }
    }
    return 0;
}

s16 func_800BE5D0(Tree *t) {
    Callbacks *cb = t->cb;
    Buttons *b;
    void (*fn)(Tree *);
    u32 in;
    Node *n;
    s16 r;

    if (cb->pre != 0 && (r = cb->pre(t)) != 0) {
        return r;
    }
    b = t->btn;
    in = 0;
    if (D_803A5970 == 0 && D_803A5972 == 0) {
        in = func_800BDAF8(t->pad, b->up, b->down, b->b, b->c);
    }
    if (((fn = cb->up) != 0 && (in & b->up)) || ((fn = cb->down) != 0 && (in & b->down))) {
        fn(t);
        func_800979F4(0x2D);
    }
    if (D_803A5970 != 0 || D_803A5972 != 0) {
        in = 0;
    }
    if (in & b->a) {
        n = focused(t);
        if (n != 0 && n->anim != 0 && n->anim->onA != 0) {
            func_800979F4(0x2D);
            r = n->anim->onA(t, n);
            if (r == 0) {
                return func_800BE538(t);
            }
            return r;
        }
    }
    if (D_803A5970 != 0 || D_803A5972 != 0) {
        in = 0;
    }
    if (in & b->b) {
        n = focused(t);
        if (n != 0 && n->anim != 0 && n->anim->onB != 0) {
            func_800979F4(0x2D);
            r = n->anim->onB(t, n);
            if (r == 0) {
                return func_800BE538(t);
            }
            return r;
        }
    }
    if (D_803A5970 != 0 || D_803A5972 != 0) {
        in = 0;
    }
    if (in & b->c) {
        n = focused(t);
        if (n != 0 && n->anim != 0 && n->anim->onC != 0) {
            func_800979F4(0x2D);
            r = n->anim->onC(t, n);
            if (r == 0) {
                return func_800BE538(t);
            }
            return r;
        }
    }
    if (D_803A5970 != 0 || D_803A5972 != 0) {
        in = 0;
    }
    if (cb->menu != 0 && (in & b->menu)) {
        cb->menu(t);
        return func_800BE538(t);
    }
    if (D_803A5970 != 0 || D_803A5972 != 0) {
        in = 0;
    }
    if (in & b->back) {
        if (cb->back != 0) {
            r = cb->back(t);
            if (r == 1 || r == 3) {
                return r;
            }
        } else {
            return 1;
        }
    }
    return func_800BE538(t);
}

static inline s16 findSlot(Tree *t) {
    s16 i;

    for (i = 0; i < 20; i++) {
        if (D_803A57F0[i].flags != 0 && D_803A57F0[i].tree == t) {
            break;
        }
    }
    return i;
}

void func_800BEBA8(Tree *t, Tree *parent, u16 mode) {
    Slot *s;
    Slot *p;
    s16 i;
    s16 j;
    s16 idx;

    if (mode == 2) {
        if (parent != 0) {
            j = findSlot(parent);
            if (j != 20) {
                D_803A57F0[j].flags = 0xC1;
                D_803A57F0[j].tree = t;
                func_800BE3FC(t);
            }
        }
        return;
    }
    for (i = 0; i < 20; i++) {
        if (D_803A57F0[i].flags == 0) {
            break;
        }
    }
    s = &D_803A57F0[i];
    idx = i;
    if (mode == 3) {
        s->flags = 0x81;
    } else {
        s->flags = 0xC1;
    }
    s->child[0] = -1;
    s->child[1] = -1;
    s->child[2] = -1;
    s->child[3] = -1;
    s->tree = t;
    func_800BE3FC(t);
    if (parent != 0) {
        for (i = 0; i < 20; i++) {
            if (D_803A57F0[i].flags != 0 && D_803A57F0[i].tree == parent) {
                break;
            }
        }
        if (i == 20) {
            s->parent = -1;
            p = 0;
        } else {
            s->parent = i;
            p = &D_803A57F0[i];
        }
    } else {
        s->parent = -1;
        p = 0;
    }
    if (p != 0) {
        for (i = 0; i < 4; i++) {
            if (p->child[i] == -1) {
                p->child[i] = idx;
                break;
            }
        }
        {
            u16 f = p->flags;

            p->flags = f & ~2;
            if (mode == 1) {
                p->flags = f & ~3;
            }
        }
    }
    s->prev = s->flags;
}
