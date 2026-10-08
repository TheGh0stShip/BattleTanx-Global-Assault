typedef struct {
  char pad0[0xC]; float x; float y; float z; char pad18[2]; unsigned char x1A; char pad1B;
  int x1C; unsigned short x20; unsigned char x22; char pad23; int x24; int x28;
  int x2C; unsigned char x30; unsigned char x31; char pad32[2]; int x34;
} Obj;
extern int D_8021945C;
extern int D_802194A0;
extern int D_80117EC4;
extern unsigned int D_803A7040;
extern unsigned char D_80123B04[];
extern unsigned char D_80123AB0[];
int func_8009D144(void);
unsigned int func_8009D914(void);
Obj *func_800A18D0(int, int);
unsigned short func_800B1898(Obj *, short, short, int, int, int, int, int, short, short, int, int, int);
int func_800D8C60(float *pos, unsigned char kind, int mode, int timer, int a4, int a5) {
  Obj *s;
  int r, j;
  unsigned char *p;
  int i;
  if (func_8009D144() == 0 && D_80117EC4 == 0) return -1;
  s = func_800A18D0(8, 56);
  if (s == 0) return -1;
  if (mode == 20) {
    s->x31 = 1;
    r = func_8009D914() % D_803A7040;
    mode = 0;
    if (r >= D_80123B04[mode]) {
      p = D_80123B04;
      do {
        mode++;
        r -= *p++;
      } while (r >= D_80123B04[mode]);
    }
    if (mode == 5) {
      D_80123B04[mode]--;
      D_803A7040--;
    }
  } else {
    s->x31 = 0;
  }
  if (D_802194A0 == 6) {
    j = mode;
    if (D_80123AB0[j] == 0) do {
      j = func_8009D914() % 19 + 1;
    } while (D_80123AB0[j] == 0);
    mode = j;
  }
  s->x1C = mode;
  s->x22 = 0;
  s->x = pos[0];
  s->z = pos[2];
  s->y = pos[1];
  s->x1A = kind;
  s->x20 = func_800B1898(s, s->x, s->y, 0, -35, 35, -35, 35,
                         s->z + -35.0f, s->z + 35.0f, 0, 16, kind);
  if (timer > 0) s->x28 = D_8021945C + timer; else s->x28 = 0;
  s->x24 = 0;
  s->x2C = a4;
  s->x30 = 255;
  s->x34 = a5;
  return 0;
}
