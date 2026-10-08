typedef struct { short x, y, z, f; int w[2]; } V;
typedef struct { unsigned short ang; float div; float off; float amp; } P;
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
void func_800F17B0(V *in, V *out, int n, P *p) {
    int i;
    float s, c, t;
    for (i = 0; i < n; i++) {
        s = func_8009D4B0(p->ang);
        c = func_8009D510(p->ang);
        t = (s * in[i].x + c * in[i].z + p->off) / p->div * 65535.0f;
        out[i] = in[i];
        out[i].y += (short)(p->amp * func_8009D4B0((unsigned int)t));
    }
}
