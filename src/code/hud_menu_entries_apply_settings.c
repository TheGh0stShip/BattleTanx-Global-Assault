typedef struct { unsigned char b0; unsigned char b1; char pad2[6]; void *p8; char padc[4]; } Ent;
extern Ent *D_8011D9D0;
extern char D_80117190[];
extern int D_80117ED4, D_80117ED8, D_80117EDC, D_80117EE0;

static inline void set_ent(Ent *e, int v) {
    switch (v) {
    case 1: case 5: e->b1 = 1; break;
    case 2: case 6: e->b1 = 2; break;
    case 3: e->b1 = 3; break;
    case 4: e->b1 = 4; break;
    default: e->b1 = 16; break;
    }
}

void func_800C48F0(void) {
    Ent *p;
    for (p = D_8011D9D0; p->b0 != 0; p++) {
        if (p->p8 == D_80117190) break;
    }
    set_ent(&p[0], D_80117ED4);
    set_ent(&p[2], D_80117ED8);
    set_ent(&p[3], D_80117EDC);
    set_ent(&p[5], D_80117EE0);
}
