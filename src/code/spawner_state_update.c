typedef struct { char pad[0xD]; unsigned char idx; char p2[0x44-0xE]; } EntG;
typedef struct { char pad[0x250]; } SlotG;
typedef struct {
    char p0[0xA]; unsigned char flags; char p0b; int pos[3];
    unsigned short f18; unsigned char f1a; char p1[0x30-0x1B];
    void *f30; void *f34; char p2[0x3C-0x38];
    unsigned char f3c; unsigned char state; char p3[0x42-0x3E]; short ent;
} ObjG;
typedef struct { int a[2]; int f8; int c[2]; int f14; } ArgG;
extern EntG D_80224F00[];
extern SlotG D_80235F00[];
extern int D_80117EB4;
extern void func_800A9B64(void *, int);
extern int func_8009E9C8(ArgG *, int *);
extern int func_800EC72C(int, int);
extern unsigned int func_8009D914(void);
extern void func_800DA4F0(void *, int *, int, int, int, int);
static inline SlotG *getSlot(int i) {
    if (i == 127) return 0;
    return &D_80235F00[i];
}
void func_800DE52C(ObjG *obj, void *msg, ArgG *arg) {
    int s1 = func_8009E9C8(arg, obj->pos);
    int chance;
    switch (obj->state) {
    case 0: case 1: case 2: case 3:
        chance = func_800EC72C(arg->f14, arg->f8);
        if (obj->ent != -1) {
            func_800A9B64(getSlot(D_80224F00[obj->ent].idx), 9);
        }
        if ((int)(func_8009D914() % 110) < chance || D_80117EB4 == 10) {
            func_800DA4F0(obj->f30, obj->pos, obj->f18, obj->f1a, (unsigned short)s1, 0);
            func_800DA4F0(obj->f34, obj->pos, obj->f18, obj->f1a, (unsigned short)s1, 0);
            obj->state = 6; obj->f3c = 255; obj->flags &= ~1;
            return;
        }
        obj->state = 3;
        return;
    case 5: case 6:
        if (obj->ent != -1) {
            func_800A9B64(getSlot(D_80224F00[obj->ent].idx), 9);
        }
        func_800DA4F0(obj->f34, obj->pos, obj->f18, obj->f1a, (unsigned short)s1, 0);
        obj->state = 6; obj->f3c = 255; obj->flags &= ~1;
        return;
    default:
        return;
    }

}
