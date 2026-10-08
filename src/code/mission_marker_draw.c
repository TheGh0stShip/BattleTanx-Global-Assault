typedef struct { float m[4][4]; float w; } Mtx17;
typedef struct { char pad[72]; void *p72; char p2[1]; unsigned char b77; } Model;
typedef struct { char pad[12]; float x, y, z; unsigned short h24; unsigned char b26, b27; void *p28; void *p32; void *p36; void *p40; Model *p44; void *p48; unsigned char b52, b53; } Obj;
extern unsigned char D_8021957C[];
int func_800AA058(int, float *);
void func_8009EFD4(Mtx17 *, float, float, float, int);
void func_800AD9A8(void *, int, Mtx17 *, int, int, int, unsigned int *, int);
void func_80079968(void *, void *, int, int, Mtx17 *, int, int, int, int, int);
void func_800AE4D0(void *, int, Mtx17 *, int, int, int, int);

void func_800EA714(Obj *arg0) {
    Mtx17 mtx = { { { 1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, 0.0f };
    unsigned int gfx[6];
    Obj *o = arg0;
    int mask;
    int r;

    mask = D_8021957C[o->b26] & (o->p44->b77 >> 4);
    if (mask == 0) return;
    r = func_800AA058(o->b26, &o->x);
    func_8009EFD4(&mtx, o->x, o->z, o->y, o->h24);
    switch (o->b53) {
    case 1: case 3:
        gfx[0] = 0xFB000000;
        gfx[1] = 0xFAFA00FF;
        gfx[2] = 0xFC147E04;
        gfx[3] = 0x5FFEFDFE;
        gfx[4] = 0xE200001C;
        gfx[5] = 0xC8112078;
        func_800AD9A8(o->p44->p72, r, &mtx, 0, 0, mask, gfx, 3);
        break;
    case 0: case 2:
        func_800AE4D0(o->p44->p72, r, &mtx, 0, 0, 0, mask);
        break;
    case 4:
        if (o->b52 < 255) {
            func_80079968(o->p28, o->p32, o->b52, r, &mtx, 0, 0, mask, 0, 0);
        }
        func_800AE4D0(o->p36, r, &mtx, 0, 0, 0, mask);
        break;
    }
}
