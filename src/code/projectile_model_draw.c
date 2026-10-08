/* ---- 0x800DC000/b/func_800DC214.c ---- */
typedef float f32; typedef unsigned char u8; typedef unsigned short u16;
typedef struct { f32 m[16]; u8 b; } M;
typedef struct { char p0[11]; u8 sc; f32 pos[3]; char p1[0x1c-0x18]; u16 rz, ry, rx; char p2[3]; u8 fl; u8 id; } O;
extern M D_8007558C;
extern f32 D_800755D0, D_800755D4;
extern void *D_803A5440[];
extern u8 func_800AD14C(f32, f32, f32, u8);
extern void func_8009EEE0(f32 *);
extern void func_8009EF30(f32 *, f32 *);
extern void func_8009F8A0(f32 *, M *, u16);
extern void func_8009FB68(M *, f32 *, u16);
extern void func_8009FA04(f32 *, M *, u16);
extern void func_8009F824(M *, f32, f32, f32);
extern void *func_800AA058(u8, f32 *);
extern void func_800AE4D0(void *, void *, M *, int, int, int, u8);
void func_800DC214(O *o) {
    M out = D_8007558C;
    f32 m[16];
    u8 r;
    if (!(o->fl & 1)) return;
    r = func_800AD14C(o->pos[0], o->pos[1], 10.0f, o->id);
    if (!r) return;
    func_8009EEE0(m);
    func_8009EF30(m, o->pos);
    func_8009F8A0(m, &out, ~(o->rx * 6));
    func_8009FB68(&out, m, o->ry - 0x4000);
    func_8009FA04(m, &out, o->rz);
    if (o->fl & 8) func_8009F824(&out, D_800755D0, D_800755D0, D_800755D0);
    if (o->sc) func_8009F824(&out, D_800755D4, D_800755D4, D_800755D4);
    func_800AE4D0(D_803A5440[1], func_800AA058(o->id, o->pos), &out, 0, 0, 0, r);
}

