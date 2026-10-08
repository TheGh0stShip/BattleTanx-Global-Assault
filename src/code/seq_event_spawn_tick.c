typedef struct {
    char pad0[0xC];
    float x;
    float y;
    float z;
    char pad18[4];
    float unk1C;
    unsigned char unk20;
    unsigned char unk21;
} SeqObj_800D3554;

typedef struct {
    unsigned char *ptr;
    SeqObj_800D3554 *obj;
    float timer;
    unsigned short unkC;
} Seq_800D3554;

extern float D_80219488;
extern float D_80074764;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern float D_803A69E8, D_803A69EC, D_803A69F4, D_803A69F8;
extern unsigned char D_80121CE5;
extern unsigned char D_803A701C;
unsigned short func_800D12B0(unsigned char *, unsigned short);
void func_80097D14(unsigned char, unsigned char);
SeqObj_800D3554 *func_800F7BA4(float *, int, float);

void func_800D3554(Seq_800D3554 *p) {
    float vec[3];
    unsigned char *s;
    SeqObj_800D3554 *o = p->obj;
    float scale = D_80074764;
    unsigned char create = 0;
    unsigned char flag = 0;

    p->timer -= D_80219488;
    if (p->timer <= 0.0f) {
        s = p->ptr;
        while (p->timer <= 0.0f) {
            s += func_800D12B0(s, p->unkC);
            switch (D_803A6A04) {
            case 1:
            case 2:
                p->timer += D_803A6A06;
                break;
            case 8:
                p->unkC = D_803A6A00;
                create = 1;
                break;
            case 3:
                if (create) {
                    vec[0] = D_803A69E8;
                    vec[1] = D_803A69F4;
                    vec[2] = D_803A69EC;
                } else {
                    o->x += D_803A69E8;
                    o->y += D_803A69F4;
                    o->z += D_803A69EC;
                }
                break;
            case 42:
                if (create) {
                    scale = D_803A69F8;
                } else {
                    o->unk1C = D_803A69F8;
                }
                break;
            case 29:
                if (create) {
                    flag = 0;
                } else {
                    o->unk20 = 0;
                }
                break;
            case 30:
                if (create) {
                    flag = 1;
                } else {
                    o->unk20 = 1;
                }
                break;
            case 47:
                if (D_80121CE5 == 0) {
                    D_803A701C = 1;
                    D_80121CE5 = 1;
                } else {
                    D_803A701C = 20;
                }
                func_80097D14(D_803A6A00, D_803A701C);
                break;
            case 0:
                o->unk21 = 1;
                p->ptr = 0;
                return;
            }
        }
        p->ptr = s;
    }
    if (create) {
        o = func_800F7BA4(vec, 0, scale);
        o->unk20 = flag;
        p->obj = o;
    }
}

typedef struct {
    unsigned char *ptr;
    int unk4;
    float timer;
    unsigned short unkC;
} Seq_800D37D0;

extern float D_80219488;
extern unsigned char *D_80219498;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern unsigned char D_803A7030, D_803A7031, D_803A7032;
extern unsigned char D_803A7022, D_803A7023, D_803A7024, D_803A7025;
extern unsigned char D_803A6FC8, D_803A6672;
extern unsigned char D_80121CE5;
extern unsigned char D_803A701C;
unsigned short func_800D12B0(unsigned char *, unsigned short);
void func_80097D14(unsigned char, unsigned char);

void func_800D37D0(Seq_800D37D0 *p) {
    unsigned char *s;
    unsigned char *v;

    p->timer -= D_80219488;
    if (p->timer <= 0.0f) {
    s = p->ptr;
    while (p->timer <= 0.0f) {
        s += func_800D12B0(s, p->unkC);
        switch (D_803A6A04) {
        case 1:
        case 2:
            p->timer += D_803A6A06;
            break;
        case 8:
            p->unkC = D_803A6A00;
            break;
        case 7:
            if (p->unkC == 1) {
                v = D_80219498 + 623;
            } else {
                v = D_80219498 + 629;
            }
            if (D_803A7030) v[0] = D_803A7022;
            if (D_803A7032) v[2] = D_803A7024;
            if (D_803A7031) v[1] = D_803A7023;
            break;
        case 15:
            if (p->unkC == 1) {
                v = D_80219498 + 620;
            } else {
                v = D_80219498 + 626;
            }
            v[0] = D_803A6FC8;
            v[1] = D_803A6672;
            v[2] = D_803A7025;
            break;
        case 47:
            if (D_80121CE5 == 0) {
                D_803A701C = 1;
                D_80121CE5 = 1;
            } else {
                D_803A701C = 20;
            }
            func_80097D14(D_803A6A00, D_803A701C);
            break;
        case 0:
            p->ptr = 0;
            return;
        }
    }
    p->ptr = s;
    }
}

typedef struct {
    char pad0[0xC];
    float x;
    float y;
    char pad14[0x18];
    float unk2C;
} SeqObj_800D39F8;

typedef struct {
    unsigned char *ptr;
    SeqObj_800D39F8 *obj;
    float timer;
    unsigned short unkC;
} Seq_800D39F8;

extern float D_80219488;
extern float D_800749A8;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern float D_803A69E8, D_803A69F4, D_803A7028;
extern unsigned char D_80121CE5;
extern unsigned char D_803A701C;
unsigned short func_800D12B0(unsigned char *, unsigned short);
void func_80097D14(unsigned char, unsigned char);
void func_800EE8C0(SeqObj_800D39F8 *, float);
SeqObj_800D39F8 *func_800A1A28(SeqObj_800D39F8 *, int);
SeqObj_800D39F8 *func_800A1A80(SeqObj_800D39F8 *, int);

void func_800D39F8(Seq_800D39F8 *p) {
    unsigned char *s;
    SeqObj_800D39F8 *o;
    SeqObj_800D39F8 *e;
    float best, d, px, py;

    p->timer -= D_80219488;
    o = p->obj;
    if (p->timer <= 0.0f) {
        s = p->ptr;
        while (p->timer <= 0.0f) {
            s += func_800D12B0(s, p->unkC);
            switch (D_803A6A04) {
            case 1:
            case 2:
                p->timer += D_803A6A06;
                break;
            case 8:
                p->unkC = D_803A6A00;
                break;
            case 19:
                func_800EE8C0(o, o->unk2C + D_803A7028);
                break;
            case 20:
                o = 0;
                best = D_800749A8;
                px = D_803A69E8;
                py = D_803A69F4;
                for (e = func_800A1A28(0, 1); e != 0; e = func_800A1A28(e, 1)) {
                    d = (e->x - px) * (e->x - px) + (e->y - py) * (e->y - py);
                    if (d < best) {
                        best = d;
                        o = e;
                    }
                }
                for (e = func_800A1A80(0, 1); e != 0; e = func_800A1A80(e, 1)) {
                    d = (e->x - px) * (e->x - px) + (e->y - py) * (e->y - py);
                    if (d < best) {
                        best = d;
                        o = e;
                    }
                }
                o->unk2C = 0.0f;
                p->obj = o;
                break;
            case 47:
                if (D_80121CE5 == 0) {
                    D_803A701C = 1;
                    D_80121CE5 = 1;
                } else {
                    D_803A701C = 20;
                }
                func_80097D14(D_803A6A00, D_803A701C);
                break;
            case 0:
                p->ptr = 0;
                return;
            }
        }
        p->ptr = s;
    }
}
