typedef struct { char pad[0xD]; unsigned char idx; char p2[0x44-0xE]; } EntG;
typedef struct { char pad[0x250]; } SlotG;
typedef struct { char p0[0xC]; float x; float y; char p1[0x1A-0x14]; unsigned char team; unsigned char hp; char p2[0x3D-0x1C]; unsigned char kind; char p3[0x42-0x3E]; short ent; } ObjG;
typedef struct { unsigned char team; char p[3]; int busy; int c; int dmg; } ArgG;
typedef struct { unsigned char flag; char p[3]; float x; float y; } OutG;
extern EntG D_80224F00[];
extern SlotG D_80235F00[];
extern void func_800A9B64(void *, int);
extern void func_800DE374(ObjG *, void *, ArgG *, void *);
extern void func_800DE52C(ObjG *, void *, ArgG *, void *);
static inline SlotG *getSlot(int i) {
    if (i == 127) return 0;
    return &D_80235F00[i];
}
void func_800DE7E8(ObjG *obj, void *msg, unsigned int type, ArgG *arg, OutG *out) {
    switch (type) {
    case 0:
        func_800DE374(obj, msg, arg, out);
        break;
    case 4:
        if (arg->busy == 0 && arg->team == obj->team) {
            int k = obj->kind;
            if (k < 6) if (k >= 0) {
            out->flag = 1;
            out->x = obj->x;
            out->y = obj->y;
        }}
        break;
    case 5:
        func_800DE52C(obj, msg, arg, out);
        break;
    case 3:
        if (obj->ent != -1) {
            func_800A9B64(getSlot(D_80224F00[obj->ent].idx), 9);
        }
        obj->hp -= arg->dmg;
        break;
    }
}
