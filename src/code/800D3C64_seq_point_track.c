/* Unit 0x800D3C64..0x800D404C (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/r3/d3c64_d12b0/func_800D3C64.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800D404C */
/* RODATA_VRAM 0x800749B0 */

typedef struct
{
  char pad0[0xC];
  float x;
  float y;
  float z;
  unsigned short unk18;
  char pad1A[0x26];
  int unk40;
} SeqObj_800D3C64;
typedef struct
{
  unsigned char *ptr;
  SeqObj_800D3C64 *obj;
  float timer;
  unsigned short unkC;
  unsigned short unkE;
} Seq_800D3C64;
typedef struct
{
  short x;
  short y;
} SeqPt_800D3C64;
extern float D_80219488;
extern float D_80074A70;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern unsigned short D_803A6A02;
extern unsigned char D_803A69E2;
extern float D_803A69E8;
extern float D_803A69EC;
extern float D_803A69F4;
extern unsigned char D_80121CE5;
extern unsigned char D_80121CD1;
extern unsigned char D_803A701C;
extern SeqPt_800D3C64 D_803A6F88[];
unsigned short func_800D12B0(unsigned char *, unsigned long);
void func_80097D14(unsigned char, unsigned char);
SeqObj_800D3C64 *func_800A1A28(SeqObj_800D3C64 *, int);
SeqObj_800D3C64 *func_800A1A80(SeqObj_800D3C64 *, int);
void func_800D3C64(Seq_800D3C64 *p)
{
  float vec[3];
  unsigned char *s;
  unsigned char *new_var;
  SeqObj_800D3C64 *o;
  unsigned short u18;
  unsigned char u40;
  unsigned char create;
  o = p->obj;
  u18 = 0;
  u40 = 0;
  create = 0;
  p->timer -= D_80219488;
  if (p->timer <= 0.0f)
  {
    new_var = p->ptr;
    s = new_var;
    while (p->timer <= 0.0f)
    {
      s += func_800D12B0(s, p->unkC);
      switch (D_803A6A04)
      {
        case 1:

        case 2:
          p->timer += D_803A6A06;
          break;

        case 8:
          p->unkC = D_803A6A00;
          create = 1;
          break;

        case 3:
          if (create)
        {
          vec[0] = D_803A69E8;
          u18 = D_803A6A02;
          vec[1] = D_803A69F4;
          vec[2] = D_803A69EC;
        }
        else
        {
          o->x += D_803A69E8;
          o->y += D_803A69F4;
          o->z += D_803A69EC;
          if (D_803A69E2)
          {
            o->unk18 = D_803A6A02;
          }
        }
          break;

        case 12:
          if (create)
        {
          u40 = D_803A6A00;
        }
        else
        {
          o->unk40 &= ~0x3F;
          o->unk40 |= D_803A6A00;
        }
          break;

        case 47:
          if (D_80121CE5 == 0)
        {
          D_803A701C = 1;
          D_80121CE5 = 1;
        }
        else
        {
          D_803A701C = 20;
        }
          func_80097D14(D_803A6A00, D_803A701C);
          break;

        case 0:
        {
          int i = ((unsigned char *) (&p->unkE))[1];
          o->x = D_803A6F88[i].x;
          o->y = D_803A6F88[i].y;
        }
          p->ptr = 0;
          return;

      }

    }

    p->ptr = s;
  }
  if (create)
  {
    SeqObj_800D3C64 *best = 0;
    SeqObj_800D3C64 *v;
    float min = D_80074A70;
    unsigned char idx;
    unsigned char n;
    float vx;
    float vy;
    short tx;
    short ty;
    vx = vec[0];
    vy = vec[1];
    for (v = func_800A1A28(0, 28); v != 0; v = func_800A1A28(v, 28))
    {
      float dx = v->x - vx;
      float dy = v->y - vy;
      float d = (dx * dx) + (dy * dy);
      if (d < min)
      {
        min = d;
        best = v;
      }
    }

    for (v = func_800A1A80(0, 28); v != 0; v = func_800A1A80(v, 28))
    {
      float dx = v->x - vx;
      float dy = v->y - vy;
      float d = (dx * dx) + (dy * dy);
      if (d < min)
      {
        min = d;
        best = v;
      }
    }

    tx = best->x;
    ty = best->y;
    idx = D_80121CD1;
    D_803A6F88[idx].x = tx;
    D_803A6F88[idx].y = ty;
    n = idx + 1;
    o = best;
    D_80121CD1 = n;
    p->unkE |= n - 1;
    o->x = vec[0];
    o->y = vec[1];
    o->z = vec[2];
    o->unk18 = u18;
    o->unk40 = u40;
    p->obj = o;
  }
}
