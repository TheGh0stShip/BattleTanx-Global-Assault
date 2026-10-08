typedef struct { char pad[0xC]; float x; float y; } Obj;
extern float D_80074200;
extern Obj *func_800A1A28(Obj *, int);
extern Obj *func_800A1A80(Obj *, int);

Obj *func_800D104C(float x, float y) {
    Obj *best = 0;
    float bestd = D_80074200;
    Obj *p;
    float d;

    for (p = func_800A1A28(0, 28); p != 0; p = func_800A1A28(p, 28)) {
        d = (p->x - x) * (p->x - x) + (p->y - y) * (p->y - y);
        if (d < bestd) { bestd = d; best = p; }
    }
    for (p = func_800A1A80(0, 28); p != 0; p = func_800A1A80(p, 28)) {
        d = (p->x - x) * (p->x - x) + (p->y - y) * (p->y - y);
        if (d < bestd) { bestd = d; best = p; }
    }
    return best;
}
