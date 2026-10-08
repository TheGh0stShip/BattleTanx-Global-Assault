typedef struct { float x, y, vx, vy, life, dlife; } Flame;
extern unsigned int D_80123B70;
extern unsigned int D_80123B74;
unsigned int func_8009D914(void);
float func_8009D8A0(float);
#define CLAMPADD(o, v) { int t = buf[idx + (o)] + (v); if (t >= 256) buf[idx + (o)] = 255; else buf[idx + (o)] = t; }
void func_800D942C(Flame *s, unsigned char *buf) {
  float a, b, c, d, e;
  int idx, t, n1, n2;
  s->life += s->dlife;
  if (s->life < 0.0f) {
    a = (unsigned int)(func_8009D914() & 63);
    b = func_8009D8A0(2.0f) + -1.0f;
    c = func_8009D8A0(0.6f) + 0.2f;
    d = (unsigned int)(func_8009D914() & 7) + 8.0f;
    e = (unsigned int)(func_8009D914() % D_80123B74 + D_80123B70);
    s->x = a;
    s->vx = b;
    s->life = e;
    s->vy = -c;
    s->y = 29.0f;
    s->dlife = -(e / (d / c));
    return;
  }
  s->x += s->vx;
  if (s->x < 0.0f) s->x += 64.0f;
  if (s->x >= 64.0f) s->x -= 64.0f;
  s->y += s->vy;
  idx = (int)s->x + ((int)s->y << 6);
  t = buf[idx] + s->life;
  if (t >= 256) buf[idx] = 255; else buf[idx] = t;
  n1 = s->life * 3.0f / 64.0f;
  n2 = s->life / 3.0f;
  CLAMPADD(1, n1)
  CLAMPADD(-1, n1)
  CLAMPADD(64, n1)
  CLAMPADD(-64, n1)
  CLAMPADD(2, n2)
  CLAMPADD(-2, n2)
  CLAMPADD(128, n2)
  CLAMPADD(-128, n2)
  CLAMPADD(65, n2)
  CLAMPADD(63, n2)
  CLAMPADD(-63, n2)
  CLAMPADD(-65, n2)
}
