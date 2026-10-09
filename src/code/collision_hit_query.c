/* SPAN 0x80081FD8 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { int id; int type; } Hit;
typedef struct { char p0[0x18]; u16 id; } Info;
typedef struct { int a, b; } Buf;
typedef struct { int b0; char p4[4]; float pos[3]; char p14[0x94 - 0x14]; u8 b94; char p95[0x120 - 0x95]; int b120; char p124[0x1D0 - 0x124]; struct { char p[0x14]; int base; } *res; } Ent;
extern short D_80397650;
void *func_80082834();
u16 func_8007D69C(Ent *, int);
u16 func_800B49E0(float *, void *, int, int, int, int, Hit **);
int func_8007D484(int);
void func_8007D558(Buf *, Hit **);
int func_80081C78(Ent *, Buf *);

int func_80081EA4(Ent *e) {
    Hit *hit;
    int base = e->res->base;
    int r = 0;
    void *m;
    int k;
    u16 t;
    int b0;

    m = func_80082834();
    if (m != 0) {
        k = e->b94;
        t = func_8007D69C(e, 1);
        b0 = e->b0;
        D_80397650 = 0;
        if (func_800B49E0(e->pos, m, base + 0x2C700F, k, t, b0, &hit) == 0) {
            r = 1;
        } else if (e->b120 != 0 && hit != 0 && e->b120 == hit->id) {
            r = 1;
        } else {
            Info info;
            if (func_8007D484(info.id) != 0 && hit != 0 && hit->type == 4) {
                Buf buf;
                func_8007D558(&buf, &hit);
                r = func_80081C78(e, &buf);
            }
        }
    }
    return r;
}
