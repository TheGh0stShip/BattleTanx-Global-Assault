/* SPAN 0x8008486C */
typedef struct { char pad[0x1E8]; int id; } Ctl;
typedef struct {
    char pad[0xA8]; int maskA; int maskB; char pB0[0x16C - 0xB0]; unsigned char flags; char p16D[3]; unsigned int state;
    char p174[0x1C8 - 0x174]; int timer; char p1CC[0x1D0 - 0x1CC]; Ctl *ctl; int v1D4; int v1D8;
} Obj;
extern unsigned int D_801ACEF0;
extern unsigned int D_802194A0;
extern void func_80088B6C(Obj *, int, float, int);
extern void func_80086208(Obj *, int);

void func_8008474C(Obj *o) {
    unsigned int *st = &o->state;

    switch (*st) {
    case 1:
        func_80088B6C(o, o->ctl->id, 100.0f, 1);
        *st = 2;
        break;
    case 2:
        if (!(D_801ACEF0 & (o->maskB | o->maskA))) {
            *st = 3;
        } else if (o->flags & 1) {
            *st = 4;
        }
        break;
    case 3:
        if (D_802194A0 < 7) {
            o->timer /= 2;
        } else if (D_802194A0 < 15) {
            o->v1D8 = o->v1D4;
        }
        *st = 4;
        break;
    case 4:
        func_80086208(o, 6);
        break;
    }
}
