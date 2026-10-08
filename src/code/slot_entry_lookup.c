typedef struct E { char pad[0x1C]; int base; char p2[0x18]; } E;
typedef struct Q { char d[16]; } Q;
typedef struct S { int x; E *e; char pad[0x9BA0]; Q *q; } S;
Q *func_800DF758(S *s, int i, unsigned short k) {
    E *e = &s->e[i];
    if (k == 0xFFFF) return 0;
    return &s->q[e->base + k];
}
