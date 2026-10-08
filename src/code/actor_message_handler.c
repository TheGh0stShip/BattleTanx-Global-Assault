typedef struct { char pad[0x30]; float scale; char pad34[0xC]; unsigned short x40; char pad42[0x98 - 0x42]; void *x98; char pad9C[0x1E0 - 0x9C]; int flags; } Ent;
typedef struct { int x0; int type; int x8; Ent *ent; } Msg;
typedef struct { char pad[0x12]; unsigned short x12; } Arg;
typedef struct { char pad0[4]; int x4; char pad8[0x2C - 8]; unsigned short x2C; char pad2E; unsigned char x2F; } Obj;
typedef struct { unsigned int w0; char pad[36]; } Rec;
extern Rec D_803978E0[];
int func_80095B50(void *, int);
void func_800D8380(Obj *, int);
void func_800D84DC(Obj *s, Msg *m, Arg *a, int *out) {
  float f;
  Ent *e;
  Rec *p;
  switch (m->type) {
  case 11: case 35: case 37: case 38: case 50:
    if (s->x2F) { *out = 2; return; }
    func_800D8380(s, a->x12);
    *out = 1;
    return;
  case 4:
    if (s->x2F) { *out = 9; return; }
    if (func_80095B50(m->ent->x98, s->x4)) {
      s->x2F = 1;
      p = &D_803978E0[s->x2C];
      p->w0 &= 0x051A2280;
      *out = 10;
      return;
    }
    e = m->ent;
    if (e->flags & 2) {
      f = e->x40 * e->scale;
      if (f > 800.0f) {
        f = (f - 800.0f) * 0.04f;
        func_800D8380(s, (unsigned short)(unsigned int)f);
      } else {
        *out = 10;
        return;
      }
    } else {
      func_800D8380(s, 30000);
    }
    *out = 10;
    return;
  default:
    *out = 1;
    return;
  }
}
