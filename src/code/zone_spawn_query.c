/* SPAN 0x800859A8 */
/* RODATA_VRAM 0x80071364 */
typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { char p0[0xC]; short x0, y0, x1, y1; } Rect;
typedef struct { char p0[0xB8]; u16 radius; char pba[0xD0 - 0xBA]; } Zone;
typedef struct { char p0[0x98]; int zone; } Obj;
extern Rect *D_80219498;
extern Zone D_80122E28[];
extern short D_80397650;
float func_8009D8A0(float);
u16 func_800B205C(int, short, short, int, short, short, short, short, int, int, int, int, int);
u16 func_800B3748(int, int, void *, int, int);
void func_80088B6C(Obj *, Pt *, float, int);

int func_800857F0(Obj *o) {
    Pt p;
    char buf[1152];
    Rect *r = D_80219498;
    u16 rad = D_80122E28[o->zone].radius;
    int ok = 0;
    u16 n;

    p.x = (r->x1 - r->x0);
    p.x *= func_8009D8A0(1.0f);
    p.x += r->x0;
    p.y = (r->y1 - r->y0);
    p.y *= func_8009D8A0(1.0f);
    p.y += r->y0;
    n = func_800B205C(0, p.x, p.y, 0, -rad, rad, -rad, rad, 0, 50, 0, 0, 0);
    D_80397650 = 0;
    if (func_800B3748(n, 0x104700F, buf, 0, 1) == 0) {
        func_80088B6C(o, &p, 400.0f, 0);
        ok = 1;
    }
    return ok;
}
