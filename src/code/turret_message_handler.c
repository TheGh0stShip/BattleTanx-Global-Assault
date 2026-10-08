/* ---- 0x800E0000/f26a8.c ---- */
typedef struct { char pad[0x48]; void *obj48; } Def;
typedef struct {
    char pad0[0xC]; Def *def; void *obj10; char pad14[0x24-0x14];
    float x; float y; char pad2C[0x36-0x2C]; unsigned char id; char p37; unsigned char mode;
} Ent;
typedef struct { int pad; int sub; } Msg;
typedef struct { unsigned char b0; char p1[3]; int i4; char p8[4]; unsigned short hC; } Data;
typedef union { int i; struct { unsigned char b; char p[3]; float x; float y; } s; } Out;
extern unsigned short func_8009E9C8(Data *, float *);
extern void func_800E2018(Ent *, unsigned short, int);

void func_800E26A8(Ent *e, Msg *m, unsigned int type, Data *d, Out *o) {
    switch (type) {
    case 0:
        switch (m->sub) {
        case 4:
            if (e->mode != 2) {
                o->i = 11;
                func_800E2018(e, d->hC, 1);
            }
            break;
        case 35:
            if (e->mode == 0) {
                e->mode = 1;
                e->def->obj48 = e->obj10;
            }
            break;
        case 28:
            if (e->mode != 2) {
                func_800E2018(e, d->hC, 1);
            }
            break;
        }
        break;
    case 4:
        if (d->i4 == 0 && e->mode != 2 && e->id == d->b0) {
            o->s.b = 1;
            o->s.x = e->x;
            o->s.y = e->y;
        }
        break;
    case 3:
    case 5:
        if (e->mode != 2) {
            func_800E2018(e, func_8009E9C8(d, &e->x), 0);
        }
        break;
    }
}

