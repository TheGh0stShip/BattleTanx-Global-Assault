/* ---- 0x800F2000/f64b8.c ---- */
typedef struct { char p[0xc]; float x, y; char p2[6]; unsigned char t; char p3; short active; } O;
typedef struct { unsigned char t; char p[3]; int v; } M;
typedef struct { unsigned char f; char p[3]; float x, y; } R;
extern void func_800F5F10(O *, int, M *, R *);
extern void func_800F6344(O *, int, M *);
extern void func_800F6144(O *, int, M *, R *);
void func_800F64B8(O *o, int a1, unsigned int ev, M *m, R *r) {
    switch (ev) {
    case 0: func_800F5F10(o, a1, m, r); break;
    case 4:
        if (m->v == 0 && o->active != 0 && m->t == o->t) { r->f = 2; r->x = o->x; r->y = o->y; }
        break;
    case 5: func_800F6144(o, a1, m, r); break;
    case 3: func_800F6344(o, a1, m); break;
    }
}

/* ---- 0x800F2000/b/f6578.c ---- */
typedef struct S { char p0[8]; short next; char p1[17]; unsigned char idx; char p2[40]; } S;
typedef struct { char p[0x74]; int s74; char p2[0x250 - 0x78]; } P;
extern short D_80224EA0;
extern S D_80224EF0[];
extern P D_80235F00[];
extern void func_800A9A98(P *, int);
static inline P *getp(int i) {
    P *r;
    if (i == 127) r = 0; else r = &D_80235F00[i];
    return r;
}
void func_800F6578(void) {
    S *s;
    P *p;
    int i = D_80224EA0;
    if (i != -1) {
        do {
            s = &D_80224EF0[i];
            p = getp(s->idx);
            if (p->s74 != 2) func_800A9A98(p, 2500);
            i = s->next;
        } while (i != -1);
    }
}

