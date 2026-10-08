/* ---- 0x800F6800/b/src/f7e14.c ---- */
typedef struct { char pad[36]; unsigned short h24; } Obj;
typedef struct { int a; int type; } Msg;
extern void func_800B22F8(int);
extern void func_800A1BE0(Obj *);
void func_800F7E14(Obj *o, Msg *m) {
    switch (m->type) {
    case 12: case 21:
        func_800B22F8(o->h24);
        func_800A1BE0(o);
    }
}

