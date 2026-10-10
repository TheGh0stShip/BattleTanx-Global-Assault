/* SPAN 0x800BF000 */
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

extern Slot D_803A57F0[20];

void func_800BE3FC(Tree *t);

inline void func_800BEE0C(u16 idx) {
    u16 i;
    s8 *c;

    for (i = 0; i < 4; i++) {
        c = &D_803A57F0[idx].child[i];
        if (*c > 0) {
            func_800BEE0C(*c);
            *c = -1;
        }
    }
    D_803A57F0[idx].flags = 0;
}

void func_800BEEB4(u16 idx) {
    Slot *p;
    s8 parent;
    u16 any;
    u16 i;

    func_800BEE0C(idx);
    parent = D_803A57F0[idx].parent;
    if (parent >= 0 && D_803A57F0[parent].flags != 0) {
        p = &D_803A57F0[parent];
        any = 0;
        for (i = 0; i < 4; i++) {
            if (p->child[i] == idx) {
                p->child[i] = -1;
                break;
            }
            if (p->child[i] > 0) {
                any = 1;
            }
        }
        if (!any) {
            if (p->flags & 1) {
                p->flags |= 0x40;
                return;
            }
            p->flags |= 0x41;
            func_800BE3FC(p->tree);
        }
    }
}
