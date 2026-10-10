/* ---- 0x800E9000/r2b/eaf6c.c ---- */
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { float x, y, z; } Vec3;
typedef struct {
    u8 pad0[64]; short h64; short h66; short h68; short h70; int w72; u8 pad76; u8 b77;
} Model;
typedef struct Obj {
    int pad0; int w4; int pad8; Model *model; int w16; int w20; int pad24;
    int w28; int w32; int pad36; u16 h40; u16 h42; u8 b44; u8 pad45; u8 b46; u8 b47;
} Obj;
typedef struct { Obj *obj; u8 pad[32]; } Hit;
extern short D_80397650;
extern int D_80115834[];
extern float func_8009D8A0(float);
extern int func_8009D914(void);
extern void func_80097FB4(int, int, int, float, int);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);
extern void func_800A60E0(Vec3 *, int, int, int, int);
extern void func_800A1BE0(Obj *);
extern void func_800DA4F0(int, int *, int, int, int, int);
extern u16 func_800B3748(int, int, Hit *, int, int);
extern void func_800B22F8(int);
extern void func_800A2E30(void);

void func_800EAF6C(Obj *o, u16 a1)
{
    int i;
    float base;
    union {
        Hit hits[32];
        struct { Vec3 v; int pad; Vec3 w; } s;
    } u;

    if (o->w20 != 0) {
        o->model->w72 = o->w20;
    } else {
        o->model->b77 = 0;
    }
    if (o->b47 == 5) {
        int n;
        int j;
        D_80397650 = 0;
        n = func_800B3748(o->h40, 0x8000, u.hits, 2, 0);
        for (j = 0; j < n; j++) {
            Obj *e = u.hits[j].obj;
            if (e->w4 == 22 && e->b47 != 5) {
                func_800EAF6C(e, a1);
            }
        }
    }
    if (o->b47 == 7) {
        func_800A2E30();
    }
    func_800B22F8(o->h40);
    func_800DA4F0(o->w16, &o->w28, o->h42, o->b44, a1, 0);
    if (o->b46 != 0) {
        u.s.v.x = func_8009D8A0(o->model->h68 - o->model->h64) + o->model->h64;
        u.s.v.z = func_8009D8A0(25.0f);
        u.s.v.y = func_8009D8A0(o->model->h70 - o->model->h66) + o->model->h66;
        func_800A60E0(&u.s.v, o->h42, o->b44, o->b46, 0);
        switch (o->b46) {
        case 1:
            func_80097FB4(8, o->w28, o->w32, 1.0f, o->b44);
            break;
        case 2:
            func_80097FB4(10, o->w28, o->w32, 1.0f, o->b44);
            break;
        case 3:
            func_80097FB4(9, o->w28, o->w32, 1.0f, o->b44);
            break;
        }
    } else {
        if (!(func_8009D914() & 1)) {
            func_80097FB4(4, o->w28, o->w32, 1.0f, o->b44);
        } else {
            func_80097FB4(40, o->w28, o->w32, 1.0f, o->b44);
        }
        i = 0;
        base = 25.0f;
    loop:
        {
            u.s.w.x = func_8009D8A0(o->model->h68 - o->model->h64) + o->model->h64;
            u.s.w.z = func_8009D8A0(5e+01f) + base;
            u.s.w.y = func_8009D8A0(o->model->h70 - o->model->h66) + o->model->h66;
            func_800A5BD8(&u.s.w, 0, o->b44, 1.0f, D_80115834, 0);
        }
        if (++i < 2) goto loop;
    }
    func_800A1BE0(o);
}

