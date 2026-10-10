/* ---- 0x800DC000/b/func_800DC804.c ---- */
typedef float f32; typedef unsigned char u8; typedef unsigned int u32;
typedef struct { char p0[12]; f32 x, y; char p1[4]; f32 dx, dy; char p2[4]; f32 s; char p3[1]; u8 id; char p4[2]; int n; } O;
extern void *D_803A5428[];
extern int func_800ACFE0(f32 *, u8);
extern void func_800AECF0(void *, u8, f32 *, f32 *, int, f32, f32, int, u32 *, int, u8);
void func_800DC804(O *o) {
    f32 v[8];
    u32 g[6];
    int k;
    int r;
    f32 x, y, x2, y2;
    v[0] = x = o->x;
    v[1] = y = o->y;
    v[4] = x2 = o->x + o->dx * o->s;
    y2 = o->y + o->dy * o->s;
    v[2] = x;
    v[6] = x2;
    v[7] = y;
    v[5] = y2;
    v[3] = y2;
    r = func_800ACFE0(v, o->id);
    if ((u8)r) {
        if (o->n == 8) k = 0; else k = 7 - o->n;
        g[0] = 0xFA000000;
        if (k >= 4) g[1] = (u8)((8 - k) * 51); else g[1] = 255;
        g[2] = 0xFCFF97FF;
        g[3] = 0xFFFCFE38;
        g[4] = 0xE200001C;
        g[5] = 0xC81049D8;
        func_800AECF0(D_803A5428[1], o->id, &o->x, &o->dx, 1, 0.25f, o->s, k, g, 3, (u8)r);
    }
}

