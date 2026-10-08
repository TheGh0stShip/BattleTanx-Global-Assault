/* ---- 0x800DC000/b/func_800DC3A8.c ---- */
typedef float f32; typedef unsigned char u8; typedef unsigned short u16;
typedef struct { f32 m[16]; } M;
typedef struct { char p0[10]; u8 big; u8 sc; f32 x, y, z; f32 s; u16 rz, ry, rx, life; u8 b24, fl, id, b27; f32 a[3]; f32 b[3]; int i40; } O;
typedef struct { char p0[11]; u8 b; } P;
extern M D_800755D8;
extern f32 D_80075618;
extern O *func_800A18D0(int, int);
extern void func_8009F8A0(M *, M *, u16);
extern void func_8009FA04(M *, M *, u16);
extern void func_8009F768(M *, M *);
extern void func_8009F1F4(M *, f32 *, f32 *);
O *func_800DC3A8(f32 *pos, u8 id, f32 s, u16 rz, P *p, u8 b24, u8 doM, f32 *A, f32 *B, int i40, u8 big, u8 sc) {
    O *o;
    M m1, m2;
    f32 d1[3], d2[3];
    o = func_800A18D0(11, 68);
    if (o == 0) return 0;
    o->x = pos[0];
    o->z = pos[2];
    o->y = pos[1];
    o->id = id;
    o->s = s;
    o->sc = sc;
    if (big) o->s = o->s / D_80075618;
    o->b24 = b24;
    o->rz = rz;
    o->b27 = p->b;
    o->life = big ? 10000 : 1000;
    o->fl = big ? 9 : 1;
    o->rx = 0;
    o->ry = 0x4000;
    o->i40 = i40;
    o->big = big;
    if (doM) {
        m1 = D_800755D8;
        m2 = D_800755D8;
        o->fl |= 4;
        func_8009F8A0(&m1, &m2, ~(o->rx * 3));
        func_8009FA04(&m2, &m1, o->rz);
        func_8009F768(&m1, &m2);
        d1[0] = A[0] - o->x;
        d1[2] = A[2] - o->z;
        d1[1] = A[1] - o->y;
        d2[0] = B[0] - o->x;
        d2[2] = B[2] - o->z;
        d2[1] = B[1] - o->y;
        func_8009F1F4(&m2, d1, o->a);
        func_8009F1F4(&m2, d2, o->b);
    }
    return o;
}

