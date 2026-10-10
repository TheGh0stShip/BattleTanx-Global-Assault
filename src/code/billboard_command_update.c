/* SPAN 0x800EBF80 */
typedef struct { char pad[168]; float v[3]; char p2[592 - 180]; } Player;
typedef struct { float m[16]; int z; } L;
typedef struct { int a, b, c; } Trip;
typedef struct { int pad; int a, b, c; } Loc;
typedef struct { unsigned int w0, w1; } Cmd;
typedef struct { char pad[12]; float x, y, z; unsigned char b24; char p[3]; int t28; float v32[3]; } Obj;
extern int D_8021945C;
extern unsigned char D_802194A5;
extern Trip ***D_803A5688;
extern Player D_80235F00[];
extern unsigned char func_800AD14C(float, float, float, int);
extern int func_800EBA98(int, float *);
extern int func_8007B0E4(Cmd *, int, int);
extern int func_8009E9C8(float *, float *);
extern void func_8009EEE0(L *);
extern void func_8009EFD4(L *, float, float, float, int);
extern void func_8007B1F0(int, int, int, int, int, L *, int, int, int, int);
void func_800EBDA0(Obj *a0) {
    Cmd cmds[2];
    L l;
    Loc t;
    unsigned char r;
    int i, c, g, n;
    float *s1;
    unsigned short a;
    Trip *p;
    Player *pl;

    r = func_800AD14C(a0->x, a0->y, 30.0f, a0->b24);
    if (r) {
        c = ~((D_8021945C - a0->t28) * 51) & 0xFF;
        cmds[0].w0 = 0xFA000000;
        cmds[1].w0 = 0xFB000000;
        cmds[0].w1 = c | 0xFF00;
        cmds[1].w1 = (c << 24) | (c << 16) | 0xFF00;
        p = **D_803A5688;
        n = p->c;
        t.a = p->a;
        t.c = p->b;
        g = func_800EBA98(n, a0->v32);
        g = func_8007B0E4(cmds, 2, g);
        for (i = 0; i < D_802194A5; i++) {
            if ((r >> i) & 1) {
                s1 = &a0->x;
                if (i == 127) {
                    pl = 0;
                } else {
                    pl = &D_80235F00[i];
                }
                a = func_8009E9C8(pl->v, s1);
                func_8009EEE0(&l);
                func_8009EFD4(&l, s1[0], s1[2], s1[1], a);
                l.z = 0;
                func_8007B1F0(t.a, g, t.c, 0, 0, &l, 0, i, 1, 1);
            }
        }
    }
}
