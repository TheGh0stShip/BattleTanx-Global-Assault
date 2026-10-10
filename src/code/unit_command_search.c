typedef unsigned char u8; typedef unsigned short u16;
typedef struct Tank { char p0[0x94]; u8 busy; char p95[0xA4 - 0x95]; int flags; char pa8[0x168 - 0xA8]; unsigned int mode; } Tank;
typedef struct { int op; float (*fn)(Tank *, int); u16 n; } Cmd;
typedef struct { char p0[0x1C]; Cmd q[4]; u16 count; } Unit;
Tank *func_800A98B8(Unit *, Tank *);
int func_80095B68(Tank *);
void func_80086208(Tank *, int);

void func_80086FF4(Unit *o) {
    u16 i;
    int used = 0;

    for (i = 0; i < o->count; i++) {
        Cmd *c = &o->q[i];
        unsigned int n = c->n;
        Tank *best = 0;
        float bestf = 0.0f;
        Tank *u;

        for (u = func_800A98B8(o, 0); u != 0; u = func_800A98B8(o, u)) {
            if (n == 0) break;
            if (u->flags & used) continue;
            if (c->fn == 0) {
                int ok = 0;
                if (func_80095B68(u) != 0 && u->busy == 0) {
                    switch (u->mode) {
                    case 6: case 9: case 17: case 18: case 19: case 20: case 21:
                        ok = 1;
                        break;
                    default:
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    if (u->mode != c->op) func_80086208(u, c->op);
                    n--;
                }
                used |= u->flags;
            } else {
                float f = c->fn(u, c->op);
                if (f <= 0.0f) {
                    used |= u->flags;
                } else {
                    if (bestf < f) {
                        Tank *t = best;
                        best = u;
                        bestf = f;
                        u = t;
                    }
                    if ((n >= 2) & (u != 0)) {
                        if (u->mode != c->op) func_80086208(u, c->op);
                        n--;
                        used |= u->flags;
                    }
                }
            }
        }
        if ((n != 0) & (best != 0)) {
            if (best->mode != c->op) func_80086208(best, c->op);
            used |= best->flags;
        }
    }
}
