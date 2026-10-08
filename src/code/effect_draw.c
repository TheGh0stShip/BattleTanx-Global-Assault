typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct Mtx68 { int w[17]; } Mtx68;
typedef struct Eff {
    char pad0[0xA];
    unsigned short unkA;
    Vec3 pos;
    float unk18;
    float unk1C;
    char pad20[0x2A - 0x20];
    unsigned char idx;
    unsigned char unk2B;
    char pad2C[0x34 - 0x2C];
    float unk34;
    int unk38;
    float unk3C;
} Eff;
extern int D_80121D5C[];
extern float D_80074FE4;
extern float D_80219488;
extern Mtx68 D_80074F94;
extern void *D_803A53A0[];
extern int func_800AD14C(float, float, float, int);
extern void func_800D6508(Vec3 *, int, int, int, int, int, float);
extern void func_8009EFD4(Mtx68 *, float, float, float, int);
extern void func_800AE4D0(void *, int, Mtx68 *, int, int, int, int);

void func_800D67F0(Eff *e) {
    Vec3 v;
    float d[2];
    Mtx68 m;
    int r;
    int alpha;
    int i;

    if (D_80121D5C[e->idx] == -1) {
        return;
    }
    r = func_800AD14C(e->pos.x, e->pos.y, 30.0f, e->unk2B);
    if ((r & 0xFF) == 0) {
        return;
    }
    if (e->idx == 0) {
        v = e->pos;
        d[0] = e->unk18 * D_80074FE4 * D_80219488;
        d[1] = e->unk1C * D_80074FE4 * D_80219488;
        alpha = 255;
        for (i = 0; i < 3; i++) {
            func_800D6508(&v, e->unkA, e->unk2B, e->unk38, alpha & 0xFF, r & 0xFF, e->unk3C * e->unk34);
            v.x += d[0];
            v.y += d[1];
            alpha -= 96;
        }
    } else {
        m = D_80074F94;
        func_8009EFD4(&m, e->pos.x, e->pos.z, e->pos.y, e->unkA);
        func_800AE4D0(D_803A53A0[D_80121D5C[e->idx]], 0, &m, 0, 0, 0, r & 0xFF);
    }
}
