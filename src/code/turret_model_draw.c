typedef struct { float w[17]; } M44;
typedef struct { char pad[0x10]; int id; char pad2[0x250 - 0x14]; } Plr;
typedef struct {
  char pad0[0xC]; float x, y, z; unsigned short rot; unsigned char x1A; char pad1B;
  int x1C; char pad20[4]; int x24; char pad28[4]; int x2C; unsigned char x30;
} Obj;
static const M44 D_80075210 = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };
extern unsigned char D_802194A5[];
extern Plr D_80235F00[];
extern int D_80123B1C[];
extern void *D_803A53A0[];
unsigned char func_800AD14C(float, float, float, int);
void *func_800AA058(int, float *);
void func_8009EFD4(M44 *, float, float, float, int);
void func_800AE4D0(void *, void *, M44 *, int, int, int, int);
static inline Plr *getPlr(int i) { if (i == 127) return 0; return &D_80235F00[i]; }
void func_800D8A80(Obj *arg) {
  M44 m = D_80075210;
  Obj *s = arg;
  unsigned short i;
  Plr *p;
  unsigned char r;
  unsigned char *cnt;
  void *q;
  s->rot += 0x600;
  if (s->x24 != 0) return;
  if (s->x30 == 0xFF) {
    if (s->x2C == 0) {
      s->x30 = 15;
    } else {
      s->x30 = 0;
      
      for (i = 0; i < D_802194A5[0]; i++) {
        p = getPlr(i);
        if (p->id != s->x2C) s->x30 |= 1 << i;
      }
    }
  }
  r = func_800AD14C(s->x, s->y, 30.0f, s->x1A);
  if (r) {
    q = func_800AA058(s->x1A, &s->x);
    func_8009EFD4(&m, s->x, s->z, s->y, s->rot);
    func_800AE4D0(D_803A53A0[D_80123B1C[s->x1C]], q, &m, 0, 0, 0, r & s->x30);
  }
}
