typedef struct Pad { signed char x; char pad1[3]; int btn; int held; } Pad;
typedef struct Q { char pad[0xC]; int c; } Q;
typedef struct P {
    char pad0[0xB]; unsigned char id; char pad1[4]; Q *q;
    char pad2[0x10C - 0x14]; struct Menu *owner; unsigned char active;
    char pad3[0x1C4 - 0x111]; int f1C4; char pad4[0x1EC - 0x1C8]; int f1EC;
} P;
typedef struct Menu {
    char pad0[0xC]; P *p; int count; int idx; unsigned int state; float t;
    unsigned char shown; char pad21[3]; int out; int time; int arg;
} Menu;
typedef struct Entry { int a; char pad[0x8C]; int f90; char pad2[0x3C]; } Entry;
extern int D_802195CC;
extern int D_8021945C;
extern float D_80219488;
extern float D_80075090, D_800750A0;
extern double D_80075098, D_800750A8;
extern Entry D_80122E38[];
extern Pad *func_80098250(int);
extern int func_800A8F34(P *, int *, unsigned char *);
extern void func_800CAFA8(int);
extern void func_800CAD9C(int, int);
extern int func_8009D144(void);
extern void func_800CAE10(int *, int *, int);
extern int func_800A9080(P *, int, int);
extern void func_800CB0C0(int);
extern void func_800A9054(P *, int);
extern void func_800CAED0(int);
extern void func_800A9660(P *);

void func_800D6EEC(Menu *arg0, int *res) {
    int ids[16];
    unsigned char ok[16];
    Pad *pad;
    int n;
    int cnt = 0;
    int i;
    Menu *m = arg0;

    if (D_802195CC == 7) return;
    if (m->p->owner != m || !m->p->active) {
        m->time = D_8021945C;
        return;
    }
    pad = func_80098250(m->p->id);
    n = func_800A8F34(m->p, ids, ok);
    for (i = 0; i < n; i++) cnt += ok[i] != 0;
    if (D_802195CC == 3 && !m->shown) {
        func_800CAFA8(m->p->id);
        func_800CAD9C(D_80122E38[ids[m->idx]].a, m->p->id);
        if (func_8009D144()) {
            func_800CAE10(&m->p->q->c, &D_80122E38[ids[m->idx]].f90, m->p->id);
        }
        m->shown = 1;
    }
    if (n != m->count || (m->state == 0 && !ok[m->idx])) {
        m->count = n;
        m->idx = 0;
        if (cnt) {
            if (!ok[0]) {
                do { m->idx++; } while (!ok[m->idx]);
            }
            m->time = D_8021945C;
        }
        m->state = 0;
        m->t = 0;
        func_800CAD9C(D_80122E38[ids[m->idx]].a, m->p->id);
        if (func_8009D144()) {
            func_800CAE10(&m->p->q->c, &D_80122E38[ids[m->idx]].f90, m->p->id);
        }
    }
    switch (m->state) {
    case 1:
        m->t += D_80219488 * D_80075090;
        if (m->t > D_80075098) {
            m->state = 0;
            m->t = 0;
            if (--m->idx == -1) m->idx = n - 1;
            if (!ok[m->idx]) { m->state = 1; m->t = 0; }
            goto upd;
        }
        break;
    case 2:
        m->t += D_80219488 * D_800750A0;
        if (m->t > D_800750A8) {
            m->state = 0;
            m->t = 0;
            if (++m->idx >= n) m->idx = 0;
            if (!ok[m->idx]) { m->state = 2; m->t = 0; }
        upd:
            if (m->state == 0) {
                func_800CAD9C(D_80122E38[ids[m->idx]].a, m->p->id);
                if (func_8009D144()) {
                    func_800CAE10(&m->p->q->c, &D_80122E38[ids[m->idx]].f90, m->p->id);
                }
            }
        }
        break;
    case 0:
        if (cnt) {
            if ((pad->held & 0x8000) || D_8021945C - m->time > 300) {
                if (func_800A9080(m->p, ids[m->idx], m->arg) >= 0) {
                    func_800CB0C0(m->p->id);
                    func_800A9054(m->p, m->idx);
                    *res = 1;
                    m->p->f1EC = 0;
                    if (func_8009D144()) {
                        m->p->q->c -= D_80122E38[ids[m->idx]].f90;
                    }
                }
            }
            if ((pad->btn & 0x202) || pad->x < -30) {
                m->state = 2; m->t = 0; m->time = D_8021945C;
                func_800CAED0(m->p->id);
            }
            if ((pad->btn & 0x101) || pad->x > 30) {
                m->state = 1; m->t = 0; m->time = D_8021945C;
                func_800CAED0(m->p->id);
            }
        }
        break;
    }
    m->out = m->state == 0 ? ids[m->idx] : 15;
    if (!cnt && D_8021945C - m->time > 15 && m->p->owner == m && m->p->f1C4) {
        func_800A9660(m->p);
    }
}
