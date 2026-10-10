/* SPAN 0x800BDA30 */
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


static inline Node *focused(Tree *t) {
    Node *n;

    n = t->nodes;
    while (n->kind != 0) {
        if (n->flags & 0x80) {
            return n;
        }
        n++;
    }
    return 0;
}

static inline Node *prevSelectable(Tree *t, Node *from) {
    Node *base = t->nodes;
    Node *p;

    for (p = from - 1; p >= base; p--) {
        if (p->flags & 0x10) {
            return p;
        }
    }
    return 0;
}

void func_800BD93C(Tree *t) {
    Node *cur = focused(t);
    Node *n;
    Node *last;

    if (cur != 0) {
        n = prevSelectable(t, cur);
        if (n == 0) {
            for (last = cur + 1; last->kind != 0; last++) {
            }
            n = prevSelectable(t, last);
        }
        cur->flags &= 0x7F;
        n->flags |= 0x80;
    }
}
