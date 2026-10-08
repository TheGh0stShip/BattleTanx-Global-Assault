typedef struct {
    char pad0[0xB];
    unsigned char b11;
    float pos[3];
    unsigned short h24;
    unsigned short h26;
    unsigned char b28;
    unsigned char b29;
    char pad1E;
    unsigned char state;
    unsigned char idx;
    unsigned char alpha;
    char pad22[6];
    unsigned short h40;
    char pad2A[10];
    int w52;
} Obj;

typedef struct {
    float a;
    union {
        int i;
        struct {
            short hi, lo;
        } s;
    } b;
    char pad8[8];
    int c;
    char pad14[0x4C];
} Ent;

typedef struct {
    unsigned char *ptr;
    Obj *obj;
    float timer;
    unsigned short unkC;
    unsigned short flags;
} Seq;

extern float D_80219488;
extern int D_8021945C;
extern unsigned char D_803A6A04, D_803A6A00, D_803A69E2;
extern unsigned short D_803A6A06, D_803A6A02;
extern float D_803A69E8, D_803A69EC, D_803A69F4;
extern unsigned char D_80121CE5, D_803A701C;
extern Ent D_80123BD8[];
extern char D_80115444[], D_801150A4[], D_8011551C[], D_80115868[], D_801159F4[],
    D_80115834[], D_80114EB0[], D_80114EC8[], D_80114EE0[], D_80114EF8[];
unsigned short func_800D12B0(unsigned char *, unsigned short);
void func_80097D14(unsigned char, unsigned char);
void func_800E4530(Obj *, int, int);
void func_800E2F9C(Obj *, float *, unsigned short, unsigned char);
int func_800EC1F8(float *, int, int, int, int, int);
void func_800A2C6C(int, float *, int, int, int, int);
void func_800A5BD8(float *, int, int, float, void *, int);
unsigned short func_800B1898(Obj *, short, short, int, short, short, short, short, short,
                             short, unsigned short, int, unsigned char);
void func_800B22F8(unsigned short);
Obj *func_800E2AEC(unsigned char, float *, unsigned short, int, int, int, int);

void func_800D2B44(Seq *p) {
    unsigned char *s;
    Obj *o;
    unsigned short id;
    unsigned char create;
    unsigned char kind;
    float pos[3];
    int v;

    p->timer -= D_80219488;
    o = p->obj;
    id = 0;
    create = 0;
    kind = 0;
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
                switch (D_803A6A00) {
                case 30:
                    kind = 0;
                    break;
                case 31:
                    kind = 1;
                    break;
                }
                break;
            case 3:
                if (create) {
                    pos[0] = D_803A69E8;
                    id = D_803A6A02;
                    pos[1] = D_803A69F4;
                    pos[2] = D_803A69EC;
                } else if (D_803A69E2) {
                    o->h24 = D_803A6A02;
                }
                break;
            case 9:
                if (o->state == 2) {
                    o->b11 = 100;
                    o->b29 = 1;
                    func_800E2F9C(o, o->pos, o->h24, o->b28);
                }
                break;
            case 10:
                p->flags |= 0x8000;
                break;
            case 11:
                p->flags &= 0x7FFF;
                break;
            case 16: {
                void *fn;
                float *pp;
                int r = 0;
                pp = o->pos;
                switch (D_803A6A00) {
                case 0:
                    r = func_800EC1F8(pp, 0, 0, 100, 2, 0);
                    fn = D_80115444;
                    break;
                case 1:
                    func_800A2C6C(0, pp, 172, 50, 0, 0xE49D0A);
                    fn = D_801150A4;
                    break;
                case 2:
                    fn = D_8011551C;
                    break;
                case 3:
                    fn = D_80115868;
                    break;
                case 4:
                    fn = D_801159F4;
                    break;
                case 5:
                    fn = D_80115834;
                    break;
                case 7:
                    fn = D_80114EB0;
                    break;
                case 8:
                    fn = D_80114EC8;
                    break;
                case 9:
                    fn = D_80114EE0;
                    break;
                case 10:
                    fn = D_80114EF8;
                    break;
                default:
                    fn = 0;
                    break;
                }
                if (fn != 0) {
                    func_800A5BD8(pp, 0, 0, 1.0f, fn, r);
                }
                break;
            }
            case 17:
                if (o->state == 1 && o->idx == 1) {
                    o->state = 4;
                    o->h40 = func_800B1898(o, o->pos[0], o->pos[1], 0,
                                           -D_80123BD8[o->idx].b.i,
                                           D_80123BD8[o->idx].b.s.lo,
                                           -D_80123BD8[o->idx].b.i,
                                           D_80123BD8[o->idx].b.s.lo,
                                           o->pos[2], o->pos[2] + D_80123BD8[o->idx].a,
                                           o->h26, 4096, o->b28);
                }
                break;
            case 18:
                if (o->state == 2 && o->idx == 1) {
                    o->w52 = D_8021945C;
                    o->state = 3;
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
                func_800E4530(o, 10000, 0);
                p->ptr = 0;
                return;
            }
        }
        p->ptr = s;
    }
    if (create) {
        o = func_800E2AEC(kind, pos, id, 0, 0, 0, 0);
        p->obj = o;
        p->flags = 0;
    }
    if (p->flags & 0x8000) {
        if (o->state == 2) {
            o->b11 = 100;
            o->b29 = 1;
            func_800E2F9C(o, o->pos, o->h24, o->b28);
        }
    }
    switch (o->state) {
    case 4:
        v = o->alpha + D_80123BD8[o->idx].c * D_80219488;
        if (v < 255) {
            o->alpha = v;
        } else {
            o->alpha = 255;
            o->state = 2;
        }
        break;
    case 3:
        v = o->alpha - D_80123BD8[o->idx].c * D_80219488;
        if (v <= 0) {
            o->alpha = 0;
            o->state = 1;
            func_800B22F8(o->h40);
            o->h40 = 0xFFFF;
        } else {
            o->alpha = v;
        }
        break;
    }
}
