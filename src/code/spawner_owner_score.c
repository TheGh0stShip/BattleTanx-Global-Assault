typedef struct { char pad[0xD]; unsigned char idx; char p2[0x44-0xE]; } EntG;
typedef struct { char pad[0x250]; } SlotG;
typedef struct { char p0[0x1B]; unsigned char hp; char p1[0x42-0x1C]; short ent; } ObjG;
typedef struct { int a[3]; int dmg; } ArgG;
extern EntG D_80224F00[];
extern SlotG D_80235F00[];
extern void func_800A9B64(void *, int);
static inline SlotG *getSlot(int i) {
    if (i == 127) return 0;
    return &D_80235F00[i];
}
void func_800DE748(ObjG *obj, void *msg, ArgG *arg) {
    if (obj->ent != -1) {
        func_800A9B64(getSlot(D_80224F00[obj->ent].idx), 9);
    }
    obj->hp -= arg->dmg;
}
