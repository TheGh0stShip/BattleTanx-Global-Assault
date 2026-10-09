/* SPAN 0x800EDF14 */
/* RODATA_VRAM 0x800769C0 */
typedef struct { float m[4][4]; int x; } MtxX;
typedef struct { char pad[0x4D]; unsigned char flags; } Def;
typedef struct {
    char pad0[0xC]; Def *def; void *model; char pad14[8];
    int p0; int p1; int p2; char pad28[2]; unsigned short ang;
    unsigned char idx; unsigned char frame; unsigned char nframes;
} Obj;
extern unsigned char D_8021957C[];
extern int D_8021945C;
extern int func_800AA058(int, void *);
extern void func_8009EFD4(MtxX *, int, int, int, int);
extern void func_800AE4D0(void *, int, MtxX *, int, int, int, int);
void func_800EDDCC(Obj *obj) {
    MtxX m = { { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} }, 0 };
    Obj *o = obj;
    unsigned char mask;
    unsigned char frame;
    int r;
    mask = D_8021957C[o->idx] & (o->def->flags >> 4);
    if (mask == 0) return;
    if (o->frame == 254) {
        frame = D_8021945C % o->nframes;
    } else {
        frame = o->frame;
    }
    r = func_800AA058(o->idx, &o->p0);
    func_8009EFD4(&m, o->p0, o->p2, o->p1, o->ang);
    func_800AE4D0(o->model, r, &m, 0, 0, frame, mask);
}
