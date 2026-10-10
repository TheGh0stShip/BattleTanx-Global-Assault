typedef short s16;
typedef unsigned short u16;

typedef struct {
    char p0[0x3E];
    s16 dir;
} Body;

typedef struct {
    char p0[0x10];
    Body *body;
} Obj;

typedef struct {
    Obj *data;
    u16 id;
    char p6[0x20 - 6];
    u16 side;
    char p22[0x24 - 0x22];
} Contact;

typedef struct {
    int kind;
    char p4[0x28 - 4];
} GridNode;

extern GridNode D_803978E0[];

Obj *func_800EFC28(Body *b);

int func_800B739C(Contact *list, u16 n) {
    Contact *c = list;
    Body *b;
    u16 i;
    int r = 0;

    for (i = 0; i < n; c++, i++) {
        GridNode *g = &D_803978E0[c->id];

        if (g->kind == 0x200000) {
            b = c->data->body;
            if (func_800EFC28(b) == c->data) {
                if (b->dir < 0) {
                    if (c->side == 2) {
                        r = 1;
                    }
                } else if (c->side == 1) {
                    r = 1;
                }
            }
        }
    }
    return r;
}
