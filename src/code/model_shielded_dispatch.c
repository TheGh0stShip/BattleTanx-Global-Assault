typedef struct { char pad[72]; int w72; } Sub;
typedef struct { char pad[12]; Sub *p12; char p16[8]; int w24; float x, y; char p36[8]; unsigned char b44, b45, b46, b47; int w48; } Obj;
typedef struct { unsigned char b0; char p[3]; int w4; int w8; union { unsigned short h; int w; } u12; int w16; } Arg;
typedef struct { unsigned char b0; char p[3]; float x, y; } Out;
typedef struct { int w0; int type; } Ent;
unsigned short func_8009E9C8(Arg *, float *);
void func_800EAF6C(Obj *, unsigned short);

static inline void h0(Obj *o, Ent *e, Arg *a, int *out) {
    switch (e->type) {
    case 11: case 35: case 37: case 38: case 50:
        *out = 1;
        if (o->b47 == 3) {
            o->w48 -= a->w16;
            if (o->w48 <= 0) {
                func_800EAF6C(o, a->u12.h);
            } else if (o->w48 < 75 && o->w24 != 0) {
                o->p12->w72 = o->w24;
            }
        } else if (o->b45 == 0) {
            func_800EAF6C(o, a->u12.h);
        }
        break;
    case 4:
        if (o->b47 == 2 || o->b47 == 3) {
            *out = 1;
        } else if (o->b47 == 6) {
            *out = 1;
        } else {
            *out = 10;
            if (o->b45 == 0) func_800EAF6C(o, a->u12.h);
        }
        break;
    case 28: case 66:
        *out = 1;
        break;
    }
}
static inline void h4(Obj *o, Ent *e, Arg *a, Out *out) {
    if (a->w4 == 0 && o->b44 == a->b0) {
        out->b0 = 1;
        out->x = o->x;
        out->y = o->y;
    }
}
static inline void h5(Obj *o, Ent *e, Arg *a) {
    func_800EAF6C(o, func_8009E9C8(a, &o->x));
}
static inline void h3(Obj *o, Ent *e, Arg *a) {
    unsigned short r = func_8009E9C8(a, &o->x);
    if (o->b47 == 3) {
        o->w48 -= a->u12.w;
        if (o->w48 > 0) {
            if (o->w48 < 75 && o->w24 != 0) {
                o->p12->w72 = o->w24;
            }
        } else {
            func_800EAF6C(o, r);
        }
    } else if (o->b45 == 0) {
        func_800EAF6C(o, r);
    }
}
void func_800EB4FC(Obj *arg0, Ent *e, unsigned int cmd, Arg *a, void *out) {
    Obj *o = arg0;
    switch (cmd) {
    case 0: h0(o, e, a, out); break;
    case 4: h4(arg0, e, a, out); break;
    case 5: h5(arg0, e, a); break;
    case 3: h3(arg0, e, a); break;
    }
}
