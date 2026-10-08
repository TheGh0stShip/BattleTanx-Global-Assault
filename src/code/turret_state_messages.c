typedef struct { char pad[0x1E4]; unsigned char r, g, b; } Owner;
typedef struct {
  char pad0[0xC]; int idx; float p10; float p14; char pad18[4];
  unsigned short x1C; char pad1E[2]; Owner *owner; int timer;
  char pad28[4]; unsigned short x2C; unsigned char x2E; unsigned char x2F;
} Obj;
typedef struct { unsigned char b0; char pad1[3]; int i4; int i8; int iC; int i10; int i14; } Msg;
typedef struct { unsigned char b0; char pad[3]; float x; float y; } Out;
typedef struct { int id; char pad[0xCC]; } Ent;
extern Ent D_80122E80[];
extern void *D_803A53A0[];
void func_800DA7D0(void *, float *, int, int, int, int, int, int, int);
void func_800B22F8(int);
void func_800A1BE0(Obj *);
void func_800D84DC(Obj *, int, Msg *, Out *);
int func_800EC72C(int, int);
void func_800D8380(Obj *, int);
void func_800D8758(Obj *s, int a1, int msg, Msg *m, Out *o) {
  switch (msg) {
  case 0:
    func_800D84DC(s, a1, m, o);
    break;
  case 2:
    func_800DA7D0(D_803A53A0[D_80122E80[s->idx].id], &s->p10, s->x1C, s->x2E, 0, 0,
                  s->owner->r, s->owner->g, s->owner->b);
    func_800B22F8(s->x2C);
    func_800A1BE0(s);
    break;
  case 4:
    if (m->i4 == 0 && s->x2F == 0 && s->timer != 0 && m->b0 == s->x2E) {
      o->b0 = 1;
      o->x = s->p10;
      o->y = s->p14;
    }
    break;
  case 5:
    if (s->x2F == 0 && s->timer != 0)
      func_800D8380(s, func_800EC72C(m->i14, m->i8));
    break;
  case 3:
    if (s->x2F == 0 && s->timer != 0)
      func_800D8380(s, m->iC);
    break;
  }
}
