/* ---- 0x800D8000/c/f9c38.c ---- */
typedef struct { float x, y, z; } Vec3f;
typedef struct { unsigned int w0, w1; } Gfx;
typedef struct { int a, b; Gfx *dl; } Model;
static const Vec3f D_80075310 = { 1e+06f, 1e+06f, 1e+06f };
static const Vec3f D_8007531C = { -1e+06f, -1e+06f, -1e+06f };
void func_800D9B50(Vec3f *, Vec3f *, unsigned int, unsigned int);
void func_800D9C38(Model *m, float *out){
  Vec3f mn = D_80075310;
  Vec3f mx = D_8007531C;
  Gfx *dl = m->dl;
  Gfx *p;
  float d;
  int i;
  for (i = 0; dl[i].w0 != 0xDF000000; i++) {
    p = &dl[i];
    if (*(unsigned char *)p == 1)
      func_800D9B50(&mx, &mn, p->w1, (p->w0 >> 12) & 0xff);
  }
  d = 2.0f;
  out[0] = (mn.x + mx.x) / d;
  out[2] = (mn.z + mx.z) / d;
  out[1] = (mn.y + mx.y) / d;
}

