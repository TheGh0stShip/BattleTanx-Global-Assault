typedef struct { char pad[0xD]; unsigned char idx; char p2[0x44-0xE]; } EntG;
typedef struct { char pad[0x250]; } SlotG;
typedef struct { char pad[0x42]; short ent; } ObjG;
extern EntG D_80224F00[];
extern SlotG D_80235F00[];
extern void func_800A9B64(void *, int);
static inline SlotG *getSlot(int i) {
    if (i == 127) return 0;
    return &D_80235F00[i];
}
void func_800DE2FC(ObjG *obj) {
    if (obj->ent != -1) {
        func_800A9B64(getSlot(D_80224F00[obj->ent].idx), 9);
    }
}
