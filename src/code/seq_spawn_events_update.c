typedef struct { short x, y; } Pt;
typedef struct {
    char pad0[0x10];
    float x;
    float y;
    float z;
    unsigned short a;
    unsigned short b;
} Obj;
typedef struct {
    unsigned char *script;
    Obj *obj;
    float time;
    unsigned short type;
    union {
        unsigned short w;
        struct { unsigned char hi, idx; } b;
    } u;
} Ev;
extern float D_80219488;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern float D_803A69E8, D_803A69F4, D_803A69EC;
extern unsigned short D_803A6A02, D_803A69E0;
extern unsigned char D_803A69E2;
extern unsigned char D_803A66BC;
extern unsigned char D_80121CE5;
extern unsigned char D_803A701C;
extern unsigned char D_80121CD1;
extern Pt D_803A6F88[];
extern unsigned short func_800D12B0(unsigned char *, unsigned short);
extern void func_80097D14(int, int);
extern Obj *func_800A1A28(Obj *, int);
extern Obj *func_800A1A80(Obj *, int);

void func_800D404C(Ev *e)
{
    Obj *o;
    unsigned char *s;
    unsigned short a;
    unsigned short b;
    unsigned char found;
    float pos[3];
    Obj *p;
    Obj *q;
    float d, min;
    float px, py;
    short sx, sy;
    unsigned char k;
    int i;

    o = e->obj;
    a = 0;
    b = 0;
    found = 0;
    e->time -= D_80219488;
    if (e->time <= 0.0f) {
        s = e->script;
        while (e->time <= 0.0f) {
            s += func_800D12B0(s, e->type);
            switch (D_803A6A04) {
            case 1:
            case 2:
                e->time += D_803A6A06;
                break;
            case 8:
                e->type = D_803A6A00;
                found = 1;
                break;
            case 3:
                if (found) {
                    pos[0] = D_803A69E8;
                    a = D_803A6A02;
                    b = D_803A69E0;
                    pos[1] = D_803A69F4;
                    pos[2] = D_803A69EC;
                } else {
                    o->x += D_803A69E8;
                    o->y += D_803A69F4;
                    o->z += D_803A69EC;
                    if (D_803A69E2)
                        o->a = D_803A6A02;
                    if (D_803A66BC)
                        o->b = D_803A69E0;
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
                i = e->u.b.idx;
                o->x = D_803A6F88[i].x;
                o->y = D_803A6F88[i].y;
                e->script = 0;
                return;
            }
        }
        e->script = s;
    }
    if (found) {
        o = 0;
        min = 1e12f;
        px = pos[0];
        py = pos[1];
        p = func_800A1A28(0, 31);
        if (p != 0) do {
            d = (p->x - px) * (p->x - px) + (p->y - py) * (p->y - py);
            if (d < min) {
                min = d;
                o = p;
            }
            p = func_800A1A28(p, 31);
        } while (p != 0);
        for (p = func_800A1A80(0, 31); p; p = func_800A1A80(p, 31)) {
        d = (p->x - px) * (p->x - px) + (p->y - py) * (p->y - py);
        if (d < min) {
            min = d;
            o = p;
        }
        }
        sx = o->x;
        sy = o->y;
        D_803A6F88[D_80121CD1].x = sx;
        D_803A6F88[D_80121CD1].y = sy;
        k = D_80121CD1 + 1;
        e->u.w |= (unsigned char)k - 1;
        o->x = pos[0];
        o->y = pos[1];
        D_80121CD1 = k;
        o->z = pos[2];
        o->a = a;
        o->b = b;
        e->obj = o;
    }
}
