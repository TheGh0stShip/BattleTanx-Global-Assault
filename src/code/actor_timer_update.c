typedef struct { char pad[0x1E4]; unsigned char r, g, b; } Owner;
typedef struct {
  char pad0[0xC]; int idx; int p10; int p14; char pad18[4];
  unsigned short x1C; char pad1E[2]; Owner *owner; int timer;
  char pad28[4]; unsigned short x2C; unsigned char x2E; char pad2F; unsigned char x30;
} Obj;
typedef struct { int id; char pad[0xCC]; } Ent;
extern int D_80117EB4;
extern Ent D_80122E80[];
extern void *D_803A53A0[];
void func_800D0858(int, unsigned char *, unsigned char *, unsigned char *);
int func_8009D914(void);
void func_80097FB4(int, int, int, float, int);
void func_800DA7D0(void *, int *, int, int, int, int, int, int, int);
void func_800B22F8(int);
void func_800A1BE0(Obj *);
void func_800D8380(Obj *s, int dt) {
  unsigned char r, g, b;
  if (D_80117EB4 == 10 || dt >= s->timer) {
    if (D_80117EB4 == 10) {
      func_800D0858(s->x30, &r, &g, &b);
    } else {
      r = s->owner->r; g = s->owner->g; b = s->owner->b;
    }
    if (!(func_8009D914() & 1))
      func_80097FB4(4, s->p10, s->p14, 1.0f, s->x2E);
    else
      func_80097FB4(40, s->p10, s->p14, 1.0f, s->x2E);
    func_800DA7D0(D_803A53A0[D_80122E80[s->idx].id], &s->p10, s->x1C, s->x2E, 0, 0, r, g, b);
    func_800B22F8(s->x2C);
    func_800A1BE0(s);
  } else {
    s->timer -= dt;
  }
}
