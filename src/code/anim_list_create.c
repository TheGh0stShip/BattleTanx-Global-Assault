/* ---- 0x800F6800/src/f71fc.c ---- */
typedef struct { char pad[10]; unsigned char a, b; } Obj;
extern Obj *func_800A18D0(int, int);
Obj *func_800F71FC(void) {
    Obj *o = func_800A18D0(29, 52);
    if (o == 0) return 0;
    o->a = 0; o->b = 0;
    return o;
}

