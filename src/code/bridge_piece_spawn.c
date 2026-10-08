typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 a;
    float p1[3];
    float p2[3];
    Vec3 b;
    unsigned char c;
} Obj603C;
extern Obj603C *func_800A18D0(int, int);
extern float func_8009D8A0(float);

void func_800E6040(Vec3 *a, Vec3 *b, unsigned char c)
{
    Obj603C *o = func_800A18D0(18, 64);
    if (o != 0) {
        o->a = *a;
        o->b = *b;
        o->p1[0] = a->x + (b->x - a->x) / 2.0f + func_8009D8A0(200.0f) - 100.0f;
        o->p1[2] = a->z + (b->z - a->z) / 2.0f + func_8009D8A0(100.0f);
        o->p1[1] = a->y + (b->y - a->y) / 2.0f + func_8009D8A0(200.0f) - 100.0f;
        o->p2[0] = a->x + (b->x - a->x) / 2.0f + func_8009D8A0(200.0f) - 100.0f;
        o->p2[2] = a->z + (b->z - a->z) / 2.0f + func_8009D8A0(100.0f);
        o->p2[1] = a->y + (b->y - a->y) / 2.0f + func_8009D8A0(200.0f) - 100.0f;
        o->c = c;
    }
}
