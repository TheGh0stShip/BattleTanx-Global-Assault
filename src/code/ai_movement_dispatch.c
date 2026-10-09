/* SPAN 0x80087DD4 */
/* RODATA_VRAM 0x80071618 */
typedef struct { int a; int b; short c; unsigned short ang; int active; int e; } Tgt;
typedef struct { char pad[0x168]; int mode; char p16C[0x1D8 - 0x16C]; int hp; char p1DC[0x1E0 - 0x1DC]; int flags; } Obj;
extern void func_80080C04(Obj *);
extern void func_8008543C(Obj *, Tgt *);
extern void func_80085CC4(Obj *, Tgt *);
extern void func_8008380C(Obj *, Tgt *);
extern void func_80085E4C(Obj *, Tgt *);
extern void func_80087570(Obj *, Tgt *);
extern void func_80087EF0(Obj *, Tgt *);
extern void func_80087A80(Obj *, Tgt *);
extern void func_80082198(Obj *);
extern int func_80087DF4(Obj *, Tgt *);
extern void func_800873FC(Obj *);

void func_80087C8C(Obj *o) {
    Tgt t;

    t.a = 0;
    t.b = 0;
    t.c = 0;
    t.ang = 0;
    t.active = 0;
    t.e = 0;
    if ((o->flags & 1) && o->hp > 0) {
        func_80080C04(o);
        if (o->flags & 0x100) {
            switch (o->mode) {
            case 22:
                func_8008543C(o, &t);
                break;
            case 25:
                func_80085CC4(o, &t);
                break;
            case 10:
                func_8008380C(o, &t);
                break;
            case 26:
            case 27:
                func_80085E4C(o, &t);
                break;
            }
        } else {
            func_80087570(o, &t);
            func_80087EF0(o, &t);
            func_80087A80(o, &t);
        }
        if ((o->flags & 1) && o->hp > 0) {
            func_80082198(o);
            if (func_80087DF4(o, &t)) {
                func_800873FC(o);
            }
        }
    }
}
