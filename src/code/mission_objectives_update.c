typedef unsigned char u8;
typedef struct Ent { int id; int type; short next; } Ent;
typedef struct { void (*fn)(Ent *, int, int, void *, void *); int pad[2]; } Tbl;
typedef struct { Ent *e; int id; int d; } Hit;
typedef struct { float x, y; int r2; int a2; int c; int b; u8 u; } Q;
typedef struct { u8 flag; float x, y, z; } Out;
extern Tbl D_80224B5C[];
extern float D_803A8328[2];
extern int D_803A8334;
extern u8 D_803A8338;
extern Hit D_803A8340[];
extern int D_803A8320;
extern int D_803AA744, D_803AA748, D_803AA74C;

void func_800EC4A8(int r) {
    Q q;
    Out out;
    int i;
    int r2;
    Ent *e;

    q.x = D_803A8328[0];
    q.y = D_803A8328[1];
    q.a2 = D_803A8334;
    q.u = D_803A8338;
    q.b = D_803AA74C;
    q.c = D_803AA744;
    r2 = r * r;
    q.r2 = r2;
    for (i = 0; i < D_803AA748; i++) {
        if (D_803A8340[i].d != -1) {
            break;
        }
        if (D_803A8340[i].e->id == D_803A8340[i].id) {
            out.flag = 0;
            e = D_803A8340[i].e;
            if (D_80224B5C[e->type].fn != 0) {
                D_80224B5C[e->type].fn(e, 0, 5, &q, &out);
            }
            if (out.flag != 0) {
                D_803A8340[i].id = 1;
            }
        }
    }
    for (; D_803A8320 < D_803AA748; D_803A8320++) {
        if (D_803A8340[D_803A8320].d > r2) {
            break;
        }
        if (D_803A8340[D_803A8320].e->id == D_803A8340[D_803A8320].id) {
            out.flag = 0;
            e = D_803A8340[D_803A8320].e;
            if (D_80224B5C[e->type].fn != 0) {
                D_80224B5C[e->type].fn(e, 0, 5, &q, &out);
            }
            if (out.flag != 0) {
                D_803A8340[D_803A8320].id = 1;
            }
        }
    }
}
