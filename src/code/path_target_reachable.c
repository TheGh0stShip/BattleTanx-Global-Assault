/* SPAN 0x80082A84 */
/* RODATA_VRAM 0x80071264 */
typedef struct { float x, y; } Pt;
typedef struct { char p0[8]; float pos[2]; char p10[0x128 - 0x10]; Pt goal; } Ent;
int func_8007D628(float *, Pt *, Pt *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define DX (e->pos[0] - p->x)
#define DY (e->pos[1] - p->y)

int func_80082870(Ent *e, Pt *p) {
    Pt *g = &e->goal;

    if (p == 0) return 1;
    if (func_8007D628(e->pos, g, p) != 1) return 1;
    if (MAX(ABS(DX), ABS(DY)) + MIN(ABS(DX), ABS(DY)) * 3.0f / 8.0f <= 10.0f) return 1;
    return 0;
}
