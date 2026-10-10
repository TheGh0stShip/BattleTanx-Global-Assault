/* SPAN 0x800EA714 */
/* RODATA_VRAM 0x80076260 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y; } Vec2;
typedef struct { u8 pad[72]; int w72; u8 b76; u8 b77; u8 b78; } Model;
typedef struct {
    u8 pad0[12]; Vec3 pos; u16 h24; u8 b26; s8 b27; int w28; int w32; int w36; int w40; Model *model; int w48;
    u8 b52; u8 b53; u8 b54; u8 b55; u16 h56; u8 b58;
} Obj;
typedef struct { int w0; int w4; u8 pad8[12]; u16 h20; u8 pad22[10]; u16 h32; u8 pad34[6]; } Ent40;
typedef struct { int pad0; int w4; u8 pad8[60]; } Ent68;
typedef int (*CbFn)(Ent68 *, Obj *, int, int, int);
typedef struct { CbFn fn; int pad[2]; } CbEnt;
extern int D_8021945C;
extern int D_802194B0[];
extern float D_80219488;
extern Ent40 D_803978E0[];
extern short D_80224EAA;
extern Ent68 D_80224EF0[];
extern CbEnt D_80224B5C[];
extern int D_8011551C[];
extern int D_80115444[];
extern float func_8009D8A0(float);
extern int func_8009D914(void);
extern void func_80097FB4(int, float, float, float, int);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);
extern void func_800DEEDC(int);
extern void func_800B2BE4(Ent40 *, Vec2 *);
extern void func_800DA7D0(int, Vec3 *, int, int, int, int, int, int, int);
extern void func_800EA14C(Obj *);
extern void func_800979F4(int);
extern int func_800EC1F8(Vec3 *, int, int, int, int, int);

void func_800EA224(Obj *o, int *done)
{
    Vec2 pts[4];
    Vec2 *pp;
    Vec2 a;
    Vec3 p;
    Vec3 q;
    int i;
    float h;

    switch (o->b53) {
    case 0:
        if (o->b58 != 0) {
            int v = D_8021945C % o->b58;
            o->model->b77 &= 0xF0;
            o->model->b77 |= v;
        }
        if (o->b27 <= 0) {
            o->b53 = 1;
            if (o->w48 != 0) {
                func_800DEEDC(o->w48);
                o->w48 = 0;
            }
        }
        break;
    case 1:
        o->b53 = 2;
        break;
    case 2:
        o->b53 = 3;
        break;
    case 3:
        if (o->h56 == 0xFFFF) {
            break;
        }
        if (!(func_8009D914() & 1)) {
            func_80097FB4(9, o->pos.x, o->pos.y, 1.0f, o->b26);
        } else {
            func_80097FB4(11, o->pos.x, o->pos.y, 1.0f, o->b26);
        }
        o->model->b78 &= 0x7F;
        o->b53 = 4;
        o->b52 = 0;
        D_803978E0[o->h56].h32 = o->b54;
        a.x = o->pos.x;
        a.y = o->pos.y;
        pp = pts;
        func_800B2BE4(&D_803978E0[o->h56], pp);
        { Ent40 *e = &D_803978E0[o->h56]; h = (short)(e->h32 + e->h20); }
        func_800DA7D0(o->w40, &o->pos, o->h24, o->b26, 0, 0, 0, 0, 0);
        func_800EA14C(o);
        for (i = 0; i < 4; i++) {
            p.x = (pp[i].x - a.x) * 0.8f + a.x;
            p.y = (pp[i].y - a.y) * 0.8f + a.y;
            p.z = func_8009D8A0(h * 0.2f) + h * 0.6f;
            func_800A5BD8(&p, 0, o->b26, 1.0f, D_8011551C, 0);
        }
        switch (o->b55) {
        case 1:
            func_800979F4(21);
            func_800A5BD8(&o->pos, 0, o->b26, 1.0f, D_80115444, func_800EC1F8(&o->pos, o->b26, 127, 100, 99, 0));
            if (D_80224EAA != -1) {
                Ent68 *e = &D_80224EF0[D_80224EAA];
                if (D_80224B5C[e->w4].fn != 0) {
                    D_80224B5C[e->w4].fn(e, o, 2, 0, 0);
                }
            }
            break;
        case 2:
            D_802194B0[0]--;
            break;
        }
        break;
    case 4: {
        int v = o->b52 + D_80219488 * 255.0f / 45.0f;
        if (v >= 256) {
            o->model->b76 = 1;
            o->model->b78 = 1;
            o->model->w72 = o->w36;
            if (o->h56 == 0xFFFF) {
                break;
            }
        D_803978E0[o->h56].w4 = 0;
        (&D_803978E0[o->h56])->w0 |= 0x40;
        (&D_803978E0[o->h56])->w0 &= ~2;
        (&D_803978E0[o->h56])->h32 = o->b54;
            *done = 1;
        } else {
            o->b52 = v;
        }
        break;
    }
    }
    o->model->b77 |= 0xF0;
}
