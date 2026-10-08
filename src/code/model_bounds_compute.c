typedef struct { short v[3]; short pad; int w[2]; } V;
void func_800D9B50(float *mx, float *mn, V *v, int n) {
  int i;
  for (i = 0; i < n; i++) {
    short *p = v[i].v;
    if (p[0] < mn[0]) mn[0] = p[0];
    if (p[1] < mn[2]) mn[2] = p[1];
    if (p[2] < mn[1]) mn[1] = p[2];
    if (mx[0] < p[0]) mx[0] = p[0];
    if (mx[2] < p[1]) mx[2] = p[1];
    if (mx[1] < p[2]) mx[1] = p[2];
  }
}
