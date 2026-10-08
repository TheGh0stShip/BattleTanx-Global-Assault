/* ---- 0x800DC000/func_800DC634.c ---- */
typedef float f32; typedef unsigned char u8; typedef unsigned short u16;
extern f32 D_8007561C;
typedef struct { f32 m[16]; u8 b; } M;
typedef struct { char p0[11]; u8 sc; f32 pos[3]; char p1[0x1c-0x18]; u16 rz, ry, rx; char p2[4]; u8 c; } O;
extern void func_8009EEE0(f32 *);
extern void func_8009EF30(f32 *, f32 *);
extern void func_8009F8A0(f32 *, M *, u16);
extern void func_8009FB68(M *, f32 *, u16);
extern void func_8009FA04(f32 *, M *, u16);
extern void func_8009F824(M *, f32, f32, f32);
void func_800DC634(O *o, int a1, int a2, M *out) {
    f32 m[16];
    func_8009EEE0(m);
    func_8009EF30(m, o->pos);
    func_8009F8A0(m, out, ~(o->rx * 3));
    func_8009FB68(out, m, o->ry - 0x4000);
    func_8009FA04(m, out, o->rz);
    if (o->sc) func_8009F824(out, D_8007561C, D_8007561C, D_8007561C);
    out->b = o->c;
}

