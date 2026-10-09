/* SPAN 0x80087EF0 */
/* RODATA_VRAM 0x80071660 */
typedef struct { int flags; char pad[0xCC]; } Slot;
typedef struct { char pad[0xA]; unsigned short ang; int active; } Tgt;
typedef struct { char pad[0x20]; unsigned short ang; char p22[2]; float pos[3]; char p30[0x98 - 0x30]; int slot; } Obj;
extern Slot D_80122EB8[];
extern float D_80219488;
extern unsigned short func_8009D6DC(unsigned short, unsigned short);
extern int func_8009D75C(float *, unsigned short, unsigned short);
extern int func_8009D81C(unsigned short, unsigned short);

int func_80087DF4(Obj *o, Tgt *t) {
    int d;
    int r;
    unsigned short a;

    if (D_80122EB8[o->slot].flags & 1) {
        a = func_8009D6DC(t->ang, o->ang);
        r = func_8009D75C(o->pos, a, (unsigned int)D_80219488 << 10);
        if ((unsigned short)r <= 0x400) {
            d = 0;
        } else {
            d = r - 0x400;
        }
    } else {
        d = func_8009D81C(o->ang, t->ang);
    }
    return (t->active != 0) & ((unsigned short)d < 365);
}
