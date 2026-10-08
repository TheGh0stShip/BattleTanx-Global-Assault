typedef struct { char pad[0xC]; float x; float y; } Obj;
extern float D_800741FC;
extern Obj *func_800A1A28(Obj *, int);
extern Obj *func_800A1A80(Obj *, int);

Obj *func_800D0F58(float x, float y) {
    Obj *best = 0;
    float bestd = D_800741FC;
    Obj *p;
    float d;

    for (p = func_800A1A28(0, 1); p != 0; p = func_800A1A28(p, 1)) {
        d = (p->x - x) * (p->x - x) + (p->y - y) * (p->y - y);
        if (d < bestd) { bestd = d; best = p; }
    }
    for (p = func_800A1A80(0, 1); p != 0; p = func_800A1A80(p, 1)) {
        d = (p->x - x) * (p->x - x) + (p->y - y) * (p->y - y);
        if (d < bestd) { bestd = d; best = p; }
    }
    return best;
}
