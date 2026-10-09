/* Unit 0x800E2520..0x800E26A8 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800E0000/src/code/turret_800E2308.c (func_800E2308/func_800E26A8 dropped: production turret_draw_create.c / turret_message_handler.c own them); re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E26A8 */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[0xC]; short kind; char pE[2]; } Elem;
typedef struct { char pad[0x14]; Elem *elems; char p18[0x38 - 0x18]; } Group;
typedef struct { int pad; Group *groups; } World;
typedef struct { short p0; unsigned short h2; unsigned short h4; unsigned short h6; unsigned short h8; unsigned short hA; unsigned char bC; } Desc;
typedef struct {
    char pad[0xA]; unsigned short hA; void *p0C; void *p10; void *p14; void *p18; void *p1C;
    float f20; Vec3 pos; unsigned short h30; char p32[2]; unsigned short h34; unsigned char b36;
    unsigned char b37; unsigned char b38; unsigned char b39;
} Turret;
extern Turret *func_800A18D0(int, int);
extern void *func_800DF758(World *, int, unsigned short);
extern void *func_800DF558(World *, void *, Vec3 *, unsigned short, unsigned char, int, int, int);
extern unsigned short func_800B1898(Turret *, short, short, int, short, short, short, short, short, short, unsigned short, int, unsigned char);
extern unsigned char func_800B9C68(int);

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

void func_800E2520(Ent *e, Msg *m, Data *d, Out *o) {
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
}

void func_800E25CC(Ent *e, Msg *m, Data *d, Out *o) {
    if (d->i4 == 0 && e->mode != 2 && e->id == d->b0) {
        o->s.b = 1;
        o->s.x = e->x;
        o->s.y = e->y;
    }
}

void func_800E2610(Ent *e, Msg *m, Data *d) {
    if (e->mode != 2) {
        func_800E2018(e, func_8009E9C8(d, &e->x), 0);
    }
}

void func_800E265C(Ent *e, Msg *m, Data *d) {
    if (e->mode != 2) {
        func_800E2018(e, func_8009E9C8(d, &e->x), 0);
    }
}
