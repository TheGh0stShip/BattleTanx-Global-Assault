/* SPAN 0x800EC4A8 */
typedef unsigned char u8;
typedef struct Ent { int id; int type; short next; char pad[0x44 - 10]; } Ent;
typedef struct { void (*fn)(Ent *, int, int, void *, void *); int pad[2]; } Tbl;
typedef struct { Ent *e; int id; int d; } Hit;
typedef struct { u8 b; int x; } Req;
typedef struct { u8 flag; float x, y; } Out;
extern Tbl D_80224B5C[];
extern short D_80224E68[];
extern Ent D_80224EF0[];
extern float D_803A8328[2];
extern u8 D_803A8330;
extern int D_803A8334;
extern u8 D_803A8338;
extern Hit D_803A8340[];
extern int D_803A8320;
extern int D_803AA740, D_803AA744, D_803AA748, D_803AA74C;
extern void func_800EBF80(Hit *, int);

int func_800EC1F8(float *pos, u8 a1, int a2, int a3, u8 a4, int a5) {
    Out out;
    Req req;
    int i;
    int k;
    Ent *e;
    int dx, dy;

    if (D_803A8330 >= a4) {
        return 0;
    }
    D_803A8328[0] = pos[0];
    D_803A8328[1] = pos[1];
    D_803A8338 = a1;
    D_803AA748 = 0;
    D_803AA740 = 0;
    D_803AA74C = a3;
    D_803A8334 = a2;
    D_803AA744 = a5;
    D_803A8330 = a4;
    req.b = a1;
    req.x = a5;
    for (i = 0; i < 65; i++) {
        if (D_80224B5C[i].fn == 0) {
            continue;
        }
        for (k = D_80224E68[i]; k != -1; k = e->next) {
            e = &D_80224EF0[k];
            out.flag = 0;
            if (D_80224B5C[e->type].fn != 0) {
                D_80224B5C[e->type].fn(e, 0, 4, &req, &out);
            }
            if (out.flag != 0 && D_803AA748 != 768) {
                D_803A8340[D_803AA748].e = e;
                D_803A8340[D_803AA748].id = e->id;
                if (out.flag == 2) {
                    D_803A8340[D_803AA748].d = -1;
                } else {
                    dx = D_803A8328[0] - out.x;
                    dy = D_803A8328[1] - out.y;
                    D_803A8340[D_803AA748].d = dx * dx + dy * dy;
                }
                D_803AA748++;
            }
        }
    }
    func_800EBF80(D_803A8340, D_803AA748);
    for (D_803A8320 = 0; D_803A8320 < D_803AA748; D_803A8320++) {
        if (D_803A8340[D_803A8320].d != -1) {
            break;
        }
    }
    return 1;
}
