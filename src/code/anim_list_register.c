/* ---- 0x800F6800/src/f757c.c ---- */
typedef struct { char pad[10]; unsigned char a, n; void *list[10]; } Mgr;
typedef struct { int x; void *cb; } Obj;
extern Mgr *func_800A18D0(int, int);
extern Mgr *D_80125AC0;
extern char D_80125AC8[];
static __inline__ Mgr *make(void) {
    Mgr *o = func_800A18D0(29, 52);
    if (o == 0) return 0;
    o->a = 0; o->n = 0;
    return o;
}
void func_800F757C(Obj *obj) {
    int i;
    if (D_80125AC0 == 0) D_80125AC0 = make();
    for (i = 0; i < D_80125AC0->n; i++) {
        if (D_80125AC0->list[i] == obj) return;
    }
    if (D_80125AC0->n != 10) {
        D_80125AC0->list[D_80125AC0->n++] = obj;
        obj->cb = D_80125AC8;
    }
}

