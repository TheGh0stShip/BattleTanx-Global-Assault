/* ---- 0x800D6000/d/f7cf4.c ---- */
typedef struct Info { int a; int b; unsigned short c; unsigned char d; } Info;
typedef struct Obj { char pad[0x1E8]; Info *info; } Obj;
typedef struct Wrap { char pad[0xC]; Obj *obj; } Wrap;
typedef struct Spr { char pad[0x40]; unsigned char unk40; } Spr;
extern void func_8009EEE0(Spr *);
extern void func_8009EFD4(Spr *, int, int, int, int);

void func_800D7CF4(Wrap *w, int a1, int a2, Spr *sp) {
    func_8009EEE0(sp);
    func_8009EFD4(sp, w->obj->info->a, 0, w->obj->info->b, w->obj->info->c);
    sp->unk40 = w->obj->info->d;
}

