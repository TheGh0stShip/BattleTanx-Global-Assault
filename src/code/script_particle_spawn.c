typedef struct {
    char pad0[0xA];
    unsigned short a;
    float x, y, z;
    float b[2];
    float c[2];
    unsigned char d;
    unsigned char e;
    unsigned char f;
    unsigned char g;
    int h;
    int i;
    int j;
    float k;
    float l;
    unsigned char m;
} Part;
extern void *func_800A18D0(int, int);
extern float func_8009D8A0(float);

void func_800D5C80(float *pos, unsigned char g, float *vel, float *acc, unsigned short a,
                   unsigned char d, float k, int f, int h, unsigned char e, int i, float l)
{
    Part *p;
    float r;

    p = func_800A18D0(37, 68);
    if (p == 0)
        return;
    p->e = e;
    p->f = f;
    p->x = pos[0];
    p->z = pos[2];
    p->y = pos[1];
    p->b[0] = vel[0];
    p->b[1] = vel[1];
    p->c[0] = acc[0];
    p->c[1] = acc[1];
    p->g = g;
    if (d != 0)
        p->d = d;
    else
        p->d = 1;
    p->a = a;
    p->m = 0;
    p->j = 0;
    p->i = i;
    p->k = k;
    p->h = h;
    p->l = l;
    if (p->f == 1) {
        r = func_8009D8A0(1.0f);
        p->x += r * vel[0] + func_8009D8A0(6.0f) - 3.0f;
        p->y += r * vel[1] + func_8009D8A0(6.0f) - 3.0f;
    }
}
