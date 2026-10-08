/* ---- 0x800DC000/c/func_800DC6EC.c ---- */
typedef float f32; typedef unsigned char u8; typedef unsigned short u16;
typedef struct { f32 m[16]; u8 b; } M;
typedef struct { char p0[11]; u8 sc; f32 pos[3]; char p1[0x1c-0x18]; u16 rz, ry, rx; char p2[4]; u8 c; } O;
extern void func_8009EEE0(f32 *);
extern void func_8009EF30(f32 *, f32 *);
extern void func_8009F8A0(f32 *, M *, u16);
extern void func_8009FB68(M *, f32 *, u16);
extern void func_8009FA04(f32 *, M *, u16);
extern void func_8009F824(M *, f32, f32, f32);
void func_800DC6EC(O *o, int a1, int a2, int a3, M *out) {
    f32 m[16];
    if (a2 != 1) return;
    func_8009EEE0(m);
    func_8009EF30(m, o->pos);
    func_8009F8A0(m, out, ~(o->rx * 3));
    func_8009FB68(out, m, o->ry - 0x4000);
    func_8009FA04(m, out, o->rz);
    if (o->sc) { union{f32 f; int i;} u; u.i=0x3E800000; func_8009F824(out, u.f,u.f,u.f);}
    out->b = o->c;
}

