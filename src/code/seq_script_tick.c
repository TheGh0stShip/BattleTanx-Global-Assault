typedef float MtxF[4][4];

typedef struct {
    char pad0[0x24];
    float at[3];
    float eye[3];
    float up[3];
    MtxF mtx;
} Camera;

typedef struct {
    unsigned char *ptr;
    Camera *obj;
    float timer;
    unsigned short unkC;
} Seq;

extern float D_80219488;
extern unsigned char *D_80219498;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern float D_803A69E8, D_803A69EC, D_803A69F4, D_803A69F8;
extern unsigned char D_803A7030, D_803A7031, D_803A7032, D_803A7033;
extern unsigned char D_803A7025, D_803A6FC8, D_803A6672;
extern unsigned char D_80121CE5;
extern unsigned char D_803A701C;
extern char D_80235F00[];
extern Camera *D_803A66B8;
extern float D_803A6668;
extern float D_80074568, D_8007456C, D_80074570, D_80074580, D_80074584;
extern double D_80074578;
extern float D_80121CD4, D_80121CD8;
extern char *D_80121CDC;
extern float D_80236068;
extern int D_8023A060;
extern unsigned short D_803A66BE;
extern float D_803A6678[4][4];
extern unsigned short D_803A701E;
extern char D_01000150[], D_01000168[], D_010001E0[];
unsigned short func_800D12B0(unsigned char *, unsigned short);
void func_80097D14(unsigned char, unsigned char);
int func_800B0444(void);
void guLookAtF(float mf[4][4], float xEye, float yEye, float zEye, float xAt, float yAt,
               float zAt, float xUp, float yUp, float zUp);
void guPerspectiveF(float mf[4][4], unsigned short *perspNorm, float fovy, float aspect,
                    float near, float far, float scale);

void func_800D25E0(Seq *p) {
    unsigned char *s;
    Camera *o;
    unsigned char changed;
    unsigned char proj;
    float f;
    int far;
    char *base;

    p->timer -= D_80219488;
    o = p->obj;
    if (p->timer <= 0.0f) {
        proj = 0;
        s = p->ptr;
        changed = 0;
        while (p->timer <= 0.0f) {
            s += func_800D12B0(s, p->unkC);
            switch (D_803A6A04) {
            case 1:
            case 2:
                p->timer += D_803A6A06;
                break;
            case 8:
                base = D_80235F00;
                o = (Camera *)(base + 120);
                D_803A66B8 = o;
                D_803A7033 = 0;
                D_80121CE5 = 0;
                p->unkC = D_803A6A00;
                o->at[0] = o->at[1] = o->at[2] = o->eye[0] = o->eye[1] = o->eye[2] = 0.0f;
                o->up[0] = o->up[1] = o->up[2] = 0.0f;
                p->obj = o;
                D_803A6668 = D_80074568;
                break;
            case 3:
                o->eye[0] += D_803A69E8;
                o->eye[1] += D_803A69F4;
                o->eye[2] += D_803A69EC;
                changed = 1;
                break;
            case 4:
                o->at[0] += D_803A69E8;
                o->at[1] += D_803A69F4;
                o->at[2] += D_803A69EC;
                changed = 1;
                break;
            case 5:
                if (D_803A7030) {
                    o->up[0] = D_803A69E8;
                    changed = 1;
                }
                if (D_803A7032) {
                    o->up[1] = D_803A69F4;
                    changed = 1;
                }
                if (D_803A7031) {
                    o->up[2] = D_803A69EC;
                    changed = 1;
                }
                break;
            case 6:
            case 43:
                D_80121CD4 = D_803A69F8;
                f = D_803A69F8 * D_8007456C + D_80074570;
                if (D_80121CDC == D_01000150 || D_80121CDC == D_01000168 ||
                    D_80121CDC == D_010001E0) {
                    f = f * D_80074578;
                }
                D_80236068 = f;
                proj = 1;
                break;
            case 44:
                D_803A7033 = 1;
                D_8023A060 = D_803A66BE;
                break;
            case 45:
                D_8023A060 = 5000;
                D_803A7033 = 0;
                break;
            case 14:
                D_80219498[632] = D_803A6FC8;
                D_80219498[633] = D_803A6672;
                D_80219498[634] = D_803A7025;
                break;
            case 13:
                D_80219498[28] = D_803A6FC8;
                D_80219498[29] = D_803A6672;
                D_80219498[30] = D_803A7025;
                break;
            case 48:
                D_80219498[638] = D_803A6FC8;
                D_80219498[639] = D_803A6672;
                D_80219498[640] = D_803A7025;
                break;
            case 49:
                D_80219498[635] = D_803A6FC8;
                D_80219498[636] = D_803A6672;
                D_80219498[637] = D_803A7025;
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
        if (changed) {
            guLookAtF(o->mtx, o->eye[0], o->eye[2], o->eye[1], o->at[0], o->at[2], o->at[1],
                      o->up[0], o->up[2], o->up[1]);
        }
        if (!D_803A7033 && !func_800B0444()) {
            far = 4096;
        } else {
            far = D_8023A060;
        }
        if (proj || D_8023A060 != far) {
            guPerspectiveF(D_803A6678, &D_803A701E, D_80121CD4, D_80121CD8, D_80074580, far,
                           D_80074584);
        }
    }
}
