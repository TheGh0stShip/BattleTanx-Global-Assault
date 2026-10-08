typedef struct { char pad[4]; unsigned short h; } C;
typedef struct { char pad[0x10]; C *c; char pad2[0x214-0x14]; unsigned short cnt; } D;
typedef struct L { char pad[0x20]; struct L *next; } L;
typedef struct { char pad[0x95]; unsigned char lv; char pad2[0xA0-0x96]; L *list; char pad3[0x1D0-0xA4]; D *d; } E;
typedef struct { char pad[4]; int type; char pad2[4]; E *e; char pad3[0x1B-0x10]; unsigned char idx; } M;
typedef struct { char pad[10]; unsigned char a; char pad2[0x20-11]; int cnt; } S;
typedef struct { char pad[0x74]; int f; char pad2[0x250-0x78]; } P;
extern int D_802194A0;
extern unsigned char D_802194A4;
extern int D_802194B0;
extern P D_80235F00[];
static inline P *getP(int i) { if (i == 127) return 0; return &D_80235F00[i]; }
extern void func_8009B3C8(void);
extern void func_8009B35C(void);
void func_800E9CAC(S *s, M *m) {
    E *e;
    P *p;
    switch (D_802194A0) {
    case 14:
        if (m->type != 4) break;
        e = m->e;
        if (e->lv >= D_802194A4) break;
        if (s->a != 0) break;
        while (e->list != 0) {
            e->list = e->list->next;
            s->cnt++;
            e->d->cnt--;
        }
        if (s->cnt < D_802194B0) break;
        func_8009B3C8();
        break;
    case 7: case 9:
        if (m->type != 4) break;
        e = m->e;
        if (e->lv < D_802194A4) {
            if (s->a != 0) break;
            if (e->d->c->h < D_802194B0) break;
            func_8009B3C8();
        } else {
            if (s->a == 0) break;
            func_8009B35C();
        }
        break;
    case 4: case 10:
        if (m->type != 28) break;
        p = getP(m->idx);
        p->f = 1;
        break;
    }
}
