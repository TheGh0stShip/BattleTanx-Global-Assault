const float D_800752E8_pad = 4.0f; /* stand-in for preceding TU rodata */
extern unsigned int D_80123B70, D_80123B74;
extern float D_803A7050[][6];
unsigned int func_8009D914(void);
float func_8009D8A0(float);
void *func_800A18D0(int, int);
void func_800D9900(int arg) {
  int i;
  float two;
  float *out;
  float a, b, c, d, e;
  void *p = func_800A18D0(9, 16);
  if (p == 0) return;
  two = 2.0f;
  *(int *)((char *)p + 12) = arg;
  for (i = 0; i < 200; i++) {
    out = D_803A7050[i];
    a = func_8009D914() & 0x3f;
    b = func_8009D8A0(two) + -1.0f;
    c = func_8009D8A0(0.6f) + 0.2f;
    d = (float)(func_8009D914() & 7) + 8.0f;
    e = func_8009D914() % D_80123B74 + D_80123B70;
    out[0] = a;
    out[2] = b;
    out[4] = e;
    out[3] = -c;
    out[1] = 29.0f;
    out[5] = -(e / (d / c));
  }
}
