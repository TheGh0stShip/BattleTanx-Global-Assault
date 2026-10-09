/* SPAN 0x8008960C */
/* RODATA_VRAM 0x80071700 */
typedef unsigned char u8;
typedef struct { float x, y; } Pt;
typedef struct { char p0[0x1F4]; int w[2]; } Owner;
typedef struct {
    char p0[8]; float x; float y; char p10[0xD0 - 0x10]; Pt tgt; char pd8[0xE8 - 0xD8]; u8 valid;
    char pe9[0x1BC - 0xE9]; float range; char p1c0[0x1CC - 0x1C0]; float skill; Owner *owner;
} Tank;
float func_8009D8A0(float);
int func_8009D914(void);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

inline int func_8008916C(int type, float d) {
    switch (type) {
    case 1:
        return 2;
    case 2:
        return 3;
    case 3:
        return 4;
    case 10:
        return 9;
    case 15:
        return d <= 300.0f ? 12 : 0;
    case 17:
        return d <= 700.0f ? 14 : 0;
    }
    return 0;
}

int func_80089200(Tank *e) {
    int r = 1;
    Pt *t = e->valid ? &e->tgt : 0;
    float d = DIST(e->x - t->x, e->y - t->y);
    int a, b;

    if (d <= e->range) {
        if (func_8009D8A0(1.0f) < e->skill) {
            Owner *o = e->owner;
            int *w = o->w;
            if (func_8009D914() & 1) {
                a = func_8008916C(o->w[0], d);
                b = func_8008916C(w[1], d);
            } else {
                a = func_8008916C(o->w[1], d);
                b = func_8008916C(w[0], d);
            }
            if (a != 0) {
                r = a;
            } else {
                r = b;
                if (r == 0) r = 1;
            }
        }
    } else {
        r = 0;
    }
    return r;
}
