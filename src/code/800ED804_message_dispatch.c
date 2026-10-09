/* func_800ED804 (0x800ED804-0x800ED98C, 0x188): message dispatch.
 * Origin: claude-work/output/workers/r3/ed800b/f_800ED804.c; only the rodata placement was missing.
 * SPAN 0x800ED98C
 * RODATA_VRAM 0x800768C0
 */
typedef struct { char pad[24]; float x; float y; char p2[4]; int hp; char p3[4]; unsigned char id; unsigned char dead; } Obj;
typedef struct { unsigned char id; char pad[3]; int w4; char p2[8]; int dmg; } Arg;
typedef struct { int pad; int kind; } Msg;
extern int func_8009E9C8(Arg *, float *);
extern void func_800ED4F4(Obj *, int, int);

void func_800ED804(Obj *obj, Msg *m, unsigned int type, Arg *p, int *out) {
    Obj *o = obj;
    switch (type) {
    case 0:
        switch (m->kind) {
        case 11: case 37: case 38: case 50:
            if (o->dead == 0) {
                *out = 1;
                o->hp -= p->dmg;
                if (o->hp < 0) func_800ED4F4(o, 0, 0);
            }
            break;
        case 4:
            if (o->dead != 0) *out = 9;
            else *out = 1;
            break;
        case 28: case 66:
            *out = 1;
            break;
        case 12: case 21:
            func_800ED4F4(o, 0, 0);
            break;
        }
        break;
    case 4:
        if (p->w4 == 0 && o->dead == 0 && o->id == p->id) {
            *(unsigned char *)out = 1;
            ((float *)out)[1] = o->x;
            ((float *)out)[2] = o->y;
        }
        break;
    case 5:
        if (o->dead == 0) func_800ED4F4(o, (unsigned short)func_8009E9C8(p, &o->x), 1);
    case 3:
        if (o->dead == 0) func_800ED4F4(o, (unsigned short)func_8009E9C8(p, &o->x), 1);
        break;
    }
}
