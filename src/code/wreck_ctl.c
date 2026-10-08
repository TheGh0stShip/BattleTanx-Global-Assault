typedef struct { float m[4][4]; unsigned char flag; } MtxFl;
typedef struct { int pad; int type; } Ctl;
typedef struct { float x, y, z; } Vec3;
typedef struct Ent2 Ent2;
typedef struct { void (*fn)(Ctl *, Ent2 *, int, int *, MtxFl *); int pad[2]; } CtlType;
struct Ent2 {
    char pad0[0xC]; Vec3 pos; unsigned short ang; char p1A[2]; unsigned char flag;
    char p1D[0x28 - 0x1D]; unsigned short h28; char p2A[0x40 - 0x2A]; Ctl *ctl;
};
extern CtlType D_80224B5C[];
extern void func_8009EEE0(void *);
extern void func_800B1610(unsigned short, unsigned char);
extern void func_8009F288(void *, Vec3 *, Vec3 *);
extern void func_800B129C(unsigned short, short, short);
extern int func_8009D5B4(float);

void func_800E2E0C(Ent2 *e, Vec3 *pos, unsigned short *ang, unsigned char *flag) {
    int a[2];
    MtxFl m;
    unsigned short base;
    int v;

    if (e->ctl != 0) {
        func_8009EEE0(&m);
        a[0] = 0;
        a[1] = 0;
        m.flag = 0;
        if (D_80224B5C[e->ctl->type].fn != 0) {
            D_80224B5C[e->ctl->type].fn(e->ctl, e, 1, a, &m);
        }
        { unsigned char f = m.flag; if (e->flag != f) {
            e->flag = f;
            func_800B1610(e->h28, f);
        } }
        func_8009F288(&m, &e->pos, pos);
        func_800B129C(e->h28, pos->x, pos->y);
        base = e->ang;
        if (m.m[2][0] < 0.0f) {
            v = -func_8009D5B4(m.m[2][2]);
        } else {
            v = func_8009D5B4(m.m[2][2]);
        }
        v = (unsigned short)v;
        *ang = base + v;
        *flag = m.flag;
    } else {
        *pos = e->pos;
        *ang = e->ang;
        *flag = e->flag;
    }
}
