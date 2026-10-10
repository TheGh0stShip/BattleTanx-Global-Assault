/* ---- 0x800F2000/b/f36d0.c ---- */
typedef struct {
    char pad0[12];
    float f12;
    float f16;
    float y;
    unsigned char id;
    char pad19[7];
    unsigned int state;
    int time;
    float vel;
} Obj36;
typedef struct { unsigned char id; char pad[3]; int flag; } Q36;
typedef struct { unsigned char hit; char pad[3]; float a; float b; } R36;
extern int D_8021945C;
extern unsigned int func_8009D914(void);

void func_800F36D0(Obj36 *o) {
    switch (o->state) {
    case 0:
        break;
    case 1:
        if (D_8021945C - o->time >= 121) {
            o->state = 2;
        }
        break;
    case 2:
        o->vel += -0.6f;
        o->y += o->vel;
        if (o->y < 0.0f) {
            o->y = 0.0f;
            o->state = 3;
        }
        break;
    }
}

void func_800F375C(Obj36 *o, int a1, Q36 *q, R36 *r) {
    if (q->flag == 0 && o->id == q->id) {
        r->hit = 1;
        r->a = o->f12;
        r->b = o->f16;
    }
}

void func_800F3794(Obj36 *o) {
    if (o->state == 0) {
        o->state = 1;
        o->time = D_8021945C - func_8009D914() % 60;
    }
}

/* ---- 0x800F2000/f3800.c ---- */
typedef struct { char p[0xc]; float x, y; char p2[4]; unsigned char t; char p3[7]; int s20; int s24; } O;
typedef struct { unsigned char t; char p[3]; int v; } M;
typedef struct { unsigned char f; char p[3]; float x, y; } R;
extern int D_8021945C;
extern unsigned int func_8009D914(void);
void func_800F3800(O *o, int a1, int ev, M *m, R *r) {
    unsigned int v;
    switch (ev) {
    case 4:
        if (m->v == 0 && o->t == m->t) { r->f = 1; r->x = o->x; r->y = o->y; }
        break;
    case 5:
        if (o->s20 == 0) { o->s20 = 1; v = func_8009D914(); o->s24 = D_8021945C - (v % 60); }
        break;
    }
}

