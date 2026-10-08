typedef struct {
  char pad0[0xC]; float x; float y; float z; char pad18[2]; unsigned char x1A; char pad1B;
  int x1C; unsigned short x20; unsigned char x22; char pad23; int x24; int x28;
  char pad2C[5]; unsigned char x31; char pad32[2]; int x34;
} Obj;
extern int D_8021945C;
extern unsigned int D_803A7040;
extern unsigned char D_80123B04[];
int func_8009D144(void);
unsigned int func_8009D914(void);
void func_800B22F8(int);
unsigned short func_800B1898(Obj *, short, short, int, int, int, int, int, short, short, int, int, int);
void func_800D8F48(Obj *s, int *done) {
  int r, i, k; unsigned char *p;
  if (s->x22 && func_8009D144() && s->x34 != 1) {
    *done = 1;
    func_800B22F8(s->x20);
    return;
  }
  if (s->x28 != 0) {
    if (s->x22 || D_8021945C >= s->x28) {
      *done = 1;
      func_800B22F8(s->x20);
    }
    return;
  }
  if (s->x22) {
    s->x22 = 0;
    func_800B22F8(s->x20);
    r = func_8009D914() % 3600 + 1800;
    s->x24 = D_8021945C + r;
    return;
  }
  if (s->x24 == 0 || s->x24 >= D_8021945C) return;
  s->x24 = 0;
  if (s->x31) {
    i = 0;
    k = func_8009D914() % D_803A7040;
    if (k >= D_80123B04[i]) {
      p = D_80123B04;
      do {
        i++;
        k -= *p++;
      } while (k >= D_80123B04[i]);
    }
    if (i == 5) {
      D_80123B04[i]--;
      D_803A7040--;
    }
    s->x1C = i;
  }
  s->x20 = func_800B1898(s, s->x, s->y, 0, -35, 35, -35, 35,
                         s->z + -35.0f, s->z + 35.0f, 0, 16, s->x1A);
}
