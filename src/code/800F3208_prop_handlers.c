/* 0x800F3208-0x800F32EC: model setup + damage message handler (written in this lane from the ROM listing).
 */
/* SPAN 0x800F32EC */
typedef struct { char pad[0x40]; unsigned char b40; } Mdl;
typedef struct { char pad[0x2C]; int hp; char p30[4]; int x34; int x38; int x3C; unsigned short a40; unsigned char b42; } Obj;
typedef struct { int a; int type; } Msg;
typedef struct { char pad[0x10]; int dmg; int src; } Arg;
extern void func_8009EEE0(Mdl *);
extern void func_8009EFD4(Mdl *, int, int, int, unsigned short);
extern void func_800A9A98(int, int);

void func_800F3208(Obj *o, Msg *m, Arg *arg, Mdl *mdl) {
    func_8009EEE0(mdl);
    func_8009EFD4(mdl, o->x34, o->x3C, o->x38, o->a40);
    mdl->b40 = o->b42;
}
void func_800F3264(Obj *o, Msg *m, Arg *arg, int *out) {
    switch (m->type) {
    case 11: case 37: case 38: case 50:
        if (o->hp > 0) {
            o->hp -= arg->dmg;
            if (o->hp <= 0) {
                func_800A9A98(arg->src, 2000);
            }
        }
        *out = 1;
        break;
    }
}
