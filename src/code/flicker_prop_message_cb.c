/* ---- 0x800F6800/src/f7e64.c ---- */
typedef struct { char pad[36]; unsigned short h24; } Obj;
typedef struct { int a; int type; } Msg;
extern void func_800B22F8(int);
extern void func_800A1BE0(Obj *);
void func_800F7E64(Obj *o, Msg *m, int c) {
    if (c == 0) {
        switch (m->type) {
        case 12: case 21:
            func_800B22F8(o->h24);
            func_800A1BE0(o);
        }
    }
}

