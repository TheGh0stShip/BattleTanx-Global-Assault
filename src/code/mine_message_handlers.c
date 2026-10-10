/* ---- 0x800F2000/b/f265c.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 pos;
    unsigned char b24;
    char pad25[3];
    int i28;
    int state;
    float f36;
    int time;
    char pad44[4];
    unsigned short u48;
} Obj26;
typedef struct { unsigned char id; char pad[3]; int flag; } Q26;
typedef struct { unsigned char hit; char pad[3]; float a; float b; } R26;
extern int D_8021945C;
extern char D_8011551C[];
extern void func_800F2288(Obj26 *, int, Q26 *, R26 *);
extern void func_800B22F8(int);
extern float func_8009D8A0(float);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);
extern void func_80097FB4(int, float, float, float, int);
void func_800F265C(Obj26 *o, int a1, unsigned int msg, Q26 *q, R26 *r) {
    switch (msg) {
    case 0:
        func_800F2288(o, a1, q, r);
        break;
    case 4:
        if (q->flag == 0 && o->state == 1 && o->b24 == q->id) {
            r->hit = 1;
            r->a = o->pos.x;
            r->b = o->pos.y;
        }
        break;
    case 5:
        if (o->state == 1) {
            o->state = msg;
        }
        break;
    case 3:
        if (o->state == 1) {
            switch (o->i28) {
            case 0:
                func_800B22F8(o->u48);
                o->u48 = 0xFFFF;
                o->state = 2;
                o->time = D_8021945C;
                func_800A5BD8(&o->pos, 0, o->b24, 1.0f, D_8011551C, 0);
                break;
            case 1:
                func_800B22F8(o->u48);
                o->u48 = 0xFFFF;
                o->state = msg;
                o->f36 = (func_8009D8A0(0.2f) + 0.9f) * 12.0f;
                func_80097FB4(52, o->pos.x, o->pos.y, 1.0f, o->b24);
                break;
            }
        }
        break;
    }
}

/* ---- 0x800F2000/f2810.c ---- */
typedef struct S { char p0[8]; short next; char p1[0x16]; int s20; char p2[0xc]; unsigned short id; char p3[0x12]; } S;
extern short D_80224EAC; extern S D_80224EF0[]; extern void func_800B22F8(int);
int func_800F2810(void) {
    S *s;
    if (D_80224EAC != -1) {
        for (s = &D_80224EF0[D_80224EAC]; ; s = &D_80224EF0[s->next]) {
            if (s->id != 0xFFFF) { func_800B22F8(s->id); s->s20 = 5; return 1; }
            if (s->next == -1) break;
        }
    }
    return 0;
}

/* ---- 0x800F2000/f28ac.c ---- */
typedef struct { int x, y, z; } V3;
typedef struct { int x, y; } V2;
typedef struct { char p[0xc]; int s0c; int s10; V3 a; V2 b; V2 c; int d; unsigned short e; unsigned char g; unsigned char f; } N;
extern void *func_800A18D0(int, int);
void func_800F28AC(V3 *a, V2 *b, V2 *c, int d, unsigned short e, unsigned char f, unsigned char g) {
    N *n = func_800A18D0(40, 56);
    if (n != 0) {
        n->a = *a; n->b = *b; n->c = *c; n->d = d; n->s10 = 0; n->s0c = 0;
        n->e = e; n->f = f; n->g = g;
    }
}

