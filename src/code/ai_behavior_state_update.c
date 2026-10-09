/* SPAN 0x8008615C */
/* RODATA_VRAM 0x80071388 */
typedef struct { int state; int timer; } AiState;
typedef struct { char pad[0x170]; AiState ai; char p178[0x1E0 - 0x178]; int flags; } Obj;
extern int D_8021945C;
extern void func_80088720(Obj *);
extern void func_80088FD4(Obj *);
extern void func_80088854(Obj *);
extern void func_80089008(Obj *);
extern int func_8008C5D8(Obj *, int, int);

void func_80086034(Obj *o) {
    AiState *st = &o->ai;

    switch (st->state) {
    case 1:
        break;
    case 2:
        if (D_8021945C >= st->timer) {
            func_80088720(o);
            func_80088FD4(o);
            st->state = 3;
            func_8008C5D8(o, 8, 0);
        }
        break;
    case 3:
        if (!(o->flags & 8)) {
            st->state = 2;
            st->timer = D_8021945C + 300;
            func_80088854(o);
            func_80089008(o);
        }
        break;
    case 4:
        if (D_8021945C >= st->timer) {
            st->state = 5;
            func_8008C5D8(o, 13, 0);
        }
        break;
    case 5:
        if (!(o->flags & 0x80)) {
            st->state = 4;
            st->timer = D_8021945C + 150;
        }
        break;
    }
}
