typedef struct { unsigned short f0; unsigned short w; unsigned short h; char pad[0x16]; } Spr;
extern Spr D_801175B8, D_801175D4, D_801175F0, D_8011760C;
extern Spr D_80117628, D_80117644, D_80117660, D_8011767C;
extern void func_8007BDFC(void *, unsigned short);
extern void func_8007C9B8(void *, Spr *, short, short, float, float);

void func_800C7650(void *ctx, short x, short y, short w, short h) {
    short i;
    short end;
    Spr *p;

    func_8007BDFC(ctx, 1);
    p = &D_801175B8;
    end = x + w - D_801175B8.w;
    for (i = x; i < end; i += p->w) {
        func_8007C9B8(ctx, &D_801175B8, i, y - 2, 1.0f, 1.0f);
    }
    func_8007C9B8(ctx, &D_801175B8, end, y - 2, 1.0f, 1.0f);
    p = &D_801175D4;
    end = x + w - D_801175D4.w;
    for (i = x; i < end; i += p->w) {
        func_8007C9B8(ctx, &D_801175D4, i, y + h, 1.0f, 1.0f);
    }
    func_8007C9B8(ctx, &D_801175D4, end, y + h, 1.0f, 1.0f);
    p = &D_801175F0;
    end = y + h - D_801175F0.h;
    for (i = y; i < end; i += p->h) {
        func_8007C9B8(ctx, &D_801175F0, x - 2, i, 1.0f, 1.0f);
    }
    func_8007C9B8(ctx, &D_801175F0, x - 2, end, 1.0f, 1.0f);
    p = &D_8011760C;
    end = y + h - D_8011760C.h;
    for (i = y; i < end; i += p->h) {
        func_8007C9B8(ctx, &D_8011760C, x + w, i, 1.0f, 1.0f);
    }
    func_8007C9B8(ctx, &D_8011760C, x + w, end, 1.0f, 1.0f);
    func_8007C9B8(ctx, &D_80117660, x - 3, y - 4, 1.0f, 1.0f);
    func_8007C9B8(ctx, &D_80117628, x + w, y - 3, 1.0f, 1.0f);
    func_8007C9B8(ctx, &D_8011767C, x - 2, y + h, 1.0f, 1.0f);
    func_8007C9B8(ctx, &D_80117644, x + w, y + h, 1.0f, 1.0f);
}
