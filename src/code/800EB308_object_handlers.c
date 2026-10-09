/* 0x800EB308-0x800EB4FC: four message handlers of one object type (written in this lane from the ROM
 * listing; catalogue listed only func_800EB308, size 0x418). */
/* SPAN 0x800EB4FC */
/* RODATA_VRAM 0x80076500 */
typedef struct { char pad[0x48]; void *p48; } Sub;
typedef struct {
    char pad[0xC]; Sub *sub; char p10[8]; void *p18; float x; float y; char p24[8];
    unsigned char id; unsigned char b2D; char p2E; unsigned char state; int timer;
} Obj;
typedef struct { int a; int type; } Msg;
typedef struct { unsigned char id; char p1[3]; int busy; char p8[4]; unsigned short hC; char pE[2]; int dmg; } Arg;
typedef struct { unsigned char hit; char p[3]; float x; float y; } Out;
typedef struct { char pad[0xC]; int dmg; } HitArg;   /* payload of the per-frame hit message */
extern int func_8009E9C8(Arg *, float *);
extern void func_800EAF6C(Obj *, unsigned short);

void func_800EB308(Obj *o, Msg *m, Arg *arg, int *out) {
    switch (m->type) {
    case 11: case 35: case 37: case 38: case 50:
        *out = 1;
        if (o->state == 3) {
            o->timer -= arg->dmg;
            if (o->timer <= 0) goto hit;
            if (o->timer < 75 && o->p18 != 0) o->sub->p48 = o->p18;
            break;
        }
        goto check;
    case 4:
        if (o->state == 2 || o->state == 3 || o->state == 6) { *out = 1; break; }
        *out = 10;
    check:
        if (o->b2D == 0) {
    hit:
            func_800EAF6C(o, arg->hC);
        }
        break;
    case 28: case 66:
        *out = 1;
        break;
    }
}

void func_800EB3E8(Obj *o, Msg *m, Arg *arg, Out *out) {
    if (arg->busy == 0 && o->id == arg->id) {
        out->hit = 1;
        out->x = o->x;
        out->y = o->y;
    }
}

void func_800EB420(Obj *o, Msg *m, Arg *arg) {
    func_800EAF6C(o, func_8009E9C8(arg, &o->x));
}

void func_800EB45C(Obj *o, Msg *m, HitArg *arg) {
    unsigned short h = func_8009E9C8((Arg *)arg, &o->x);
    if (o->state == 3) {
        o->timer -= arg->dmg;
        if (o->timer <= 0) goto hit;
        if (o->timer < 75 && o->p18 != 0) o->sub->p48 = o->p18;
        return;
    }
    if (o->b2D == 0) {
    hit:
        func_800EAF6C(o, h);
    }
}
