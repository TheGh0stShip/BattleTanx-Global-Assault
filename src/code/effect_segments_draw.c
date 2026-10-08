typedef struct { float m[4][4]; int x; } MtxX;
typedef struct { float x, y, z; } Vec;
typedef struct Seg {
    char pad[20]; struct Seg *next; float r; char p1c[2]; short w;
    char p20[2]; unsigned short id; void *model; char p28[12]; int state;
} Seg;
typedef struct { float x; float y; char p8[4]; float z; char p10[8]; unsigned short ang; unsigned char b; } Sub;
typedef struct { char pad[12]; Sub s; char p[8]; Seg *list; } Head;
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
extern unsigned char func_800AD14C(float, float, float, unsigned char);
extern unsigned char func_800ACF20(unsigned short, unsigned char);
extern int func_800AA058(unsigned char, Vec *);
extern void func_8009EFD4(MtxX *, float, float, float, unsigned short);
extern void func_800AE4D0(void *, int, MtxX *, int, int, int, unsigned char);
void func_800EF578(Head *hd) {
    Sub *h = &hd->s;
    Seg *s;
    for (s = hd->list; s != 0; s = s->next) {
        if (s->state >= 0) {
            Vec v;
            MtxX m = { { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} }, 0 };
            unsigned char r;
            int k;
            v.x = h->x + (s->r + (float)(s->w / 2)) * func_8009D4B0(h->ang);
            v.y = h->y + (s->r + (float)(s->w / 2)) * func_8009D510(h->ang);
            v.z = h->z;
            if (s->id == 0xFFFF) {
                r = func_800AD14C(v.x, v.y, (float)(s->w / 2), h->b);
            } else {
                r = func_800ACF20(s->id, h->b);
            }
            if (r) {
                k = func_800AA058(h->b, &v);
                func_8009EFD4(&m, v.x, v.z, v.y, h->ang);
                func_800AE4D0(s->model, k, &m, 0, 0, 0, r);
            }
        }
    }
}
