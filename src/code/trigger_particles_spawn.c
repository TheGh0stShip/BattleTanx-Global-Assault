extern unsigned int D_80123B70, D_80123B74;
unsigned int func_8009D914(void);
float func_8009D8A0(float);
void func_800D92E0(float *out) {
  float a, b, c, d, e;
  a = func_8009D914() & 0x3f;
  b = func_8009D8A0(2.0f) + -1.0f;
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
