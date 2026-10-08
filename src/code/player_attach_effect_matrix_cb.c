/* ---- 0x800D6000/f7d60.c ---- */
typedef struct Info { int a; int b; unsigned short c; unsigned char d; } Info;
typedef struct Obj { char pad[0x1E8]; Info *info; } Obj;
typedef struct Wrap { char pad[0xC]; Obj *obj; } Wrap;
typedef struct Spr { char pad[0x40]; unsigned char unk40; } Spr;
extern void func_8009EEE0(Spr *);
extern void func_8009EFD4(Spr *, int, int, int, int);

void func_800D7D60(Wrap *w, int a1, int a2, int a3, Spr *s) {
    if (a2 == 1) {
        func_8009EEE0(s);
        func_8009EFD4(s, w->obj->info->a, 0, w->obj->info->b, w->obj->info->c);
        s->unk40 = w->obj->info->d;
    }
}

