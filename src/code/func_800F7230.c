/* SPAN 0x800F756C */
/* RODATA_VRAM 0x800772F0 */
typedef struct { unsigned int w0, w1; } Gfx;
typedef struct { char pad[4]; Gfx *dl; } Mdl;
typedef struct { char pad[10]; unsigned char idx; unsigned char n; Mdl *m[1]; } Obj;
extern Gfx D_80125AD0[][15];
extern int D_8021945C;
extern float func_8009D4B0(unsigned short);

void func_800F7230(Obj *o) {
    float t, a, b;
    int i;

    o->idx = (o->idx + 1) % 3;
    t = D_8021945C;
    a = t * 1.1f;
    b = t * 1.6f;
    a += func_8009D4B0((unsigned int)((int)a * 180 / 3.14159 * 182.04444444 / 16.0)) * 4.0f;
    b += func_8009D4B0((unsigned int)((int)b * 180 / 3.14159 * 182.04444444 / 12.0)) * 3.0f;
    { Gfx *_g = &D_80125AD0[o->idx][0]; _g->w0 = 0xF5880880; _g->w1 = 0x01014050; }
    { Gfx *_g = &D_80125AD0[o->idx][1]; _g->w0 = 0xF5880800; _g->w1 = 0x00014050; }
    { Gfx *_g = &D_80125AD0[o->idx][2]; _g->w0 = 0xF2000000 | ((((int)a % 128) & 0xFFF) << 12); _g->w1 = 0x0107C07C; }
    { Gfx *_g = &D_80125AD0[o->idx][3]; _g->w0 = 0xF2000000 | (((int)b % 128) & 0xFFF); _g->w1 = 0x0007C07C; }
    { Gfx *_g = &D_80125AD0[o->idx][9]; _g->w0 = 0xFC6164A0; _g->w1 = 0xF3FCF2FE; }
    for (i = 0; i < o->n; i++) {
        o->m[i]->dl = &D_80125AD0[o->idx][-1];
    }
}
