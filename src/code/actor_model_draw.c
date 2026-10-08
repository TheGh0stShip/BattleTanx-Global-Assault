typedef struct { char pad[0x1E4]; unsigned char r, g, b; } Owner;
typedef struct {
  char pad0[0xC]; int idx; float x, y, z; unsigned short x1C; char pad1E[2];
  Owner *owner; char pad24[4]; int timer; char pad2C[2];
  unsigned char x2E; unsigned char x2F; unsigned char x30; char pad31;
  unsigned short x32; unsigned short x34;
} Obj;
typedef struct { int m[17]; } Mtx68;
typedef struct { float m[16]; } Mtx;
typedef struct { int id; int id2; int id3; char pad[0xC4]; } Ent;
extern Mtx68 D_800750E0;
extern int D_80117EB4;
extern int D_8021945C;
extern Ent D_80122E80[];
extern void *D_803A53A0[];
void func_800D0858(int, unsigned char *, unsigned char *, unsigned char *);
unsigned char func_800AD14C(float, float, float, int);
void func_8009EEE0(Mtx *);
void func_8009EF30(Mtx *, float *);
void func_8009FB68(Mtx *, Mtx68 *, int);
void func_8009F8A0(Mtx68 *, Mtx *, int);
void func_8009FA04(Mtx *, Mtx68 *, int);
int func_800AA058(int, float *);
void func_800AD9A8(void *, int, Mtx68 *, int, int, int, unsigned int *, int);
void func_800D8088(Obj *arg) {
  Obj *s;
  unsigned int gfx[4];
  Mtx68 m1;
  Mtx m2;
  unsigned char c[3];
  unsigned int gfx2[4];
  unsigned char vis;
  int lod, d, a;
  m1 = D_800750E0;
  s = arg;
  vis = func_800AD14C(s->x, s->y, 90.0f, s->x2E);
  if (vis == 0) return;
  func_8009EEE0(&m2);
  func_8009EF30(&m2, &s->x);
  if (s->x32 || s->x34) {
    func_8009FB68(&m2, &m1, s->x32);
    func_8009F8A0(&m1, &m2, s->x34);
  }
  func_8009FA04(&m2, &m1, s->x1C);
  lod = func_800AA058(s->x2E, &s->x);
  if (D_80117EB4 == 10) {
    func_800D0858(s->x30, &c[0], &c[1], &c[2]);
    gfx[0] = 0xFB000000;
    gfx[1] = (c[0] << 24) | (c[1] << 16) | (c[2] << 8) | 0xFF;
  } else {
    gfx[0] = 0xFB000000;
    gfx[1] = (s->owner->r << 24) | (s->owner->g << 16) | (s->owner->b << 8) | 0xFF;
  }
  func_800AD9A8(D_803A53A0[s->x2F ? D_80122E80[s->idx].id3 : D_80122E80[s->idx].id],
                lod, &m1, 0, 0, vis, gfx, 1);
  if (s->x2F) return;
  d = D_8021945C - s->timer;
  if (d >= 256) return;
  if (d < 128) a = 255; else a = ~(d * 2);
  gfx2[0] = 0xFA000000;
  gfx2[1] = (a & 0xFF) | 0xFF000000;
  gfx2[2] = 0xFB000000;
  gfx2[3] = 0xFFFF0000;
  func_800AD9A8(D_803A53A0[D_80122E80[s->idx].id2], 0, &m1, 1, 0, vis, gfx2, 2);
}
