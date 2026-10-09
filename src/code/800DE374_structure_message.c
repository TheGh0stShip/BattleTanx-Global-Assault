/* Normalizer-assisted: two redundant byte extensions become retail moves. */
/* RODATA_VRAM 0x80075740 */
typedef struct { char pad[0xD]; unsigned char idx; char p2[0x44-0xE]; } EntG;
typedef struct { char pad[0x250]; } SlotG;
typedef struct { char p0[0x1B]; unsigned char hp; char p1[0x3D-0x1C]; unsigned char kind; char p2[0x42-0x3E]; short ent; } ObjG;
typedef struct { int a; int type; int b; int c; } MsgG;
typedef struct { int a[4]; int dmg; } ArgG;
extern EntG D_80224F00[];
extern EntG D_80224EF0[];
extern SlotG D_80235F00[];
extern void func_800A9B64(void *, int);
extern void func_800E82AC(int, void *);
void func_800DE374(ObjG *obj, MsgG *msg, ArgG *arg, int *out) {
    int i; int k;
    SlotG *p;
    switch (msg->type) {
    case 38:
        *out = 5;
        break;
    case 35:
        obj->hp -= arg->dmg;
        if (obj->ent == -1) goto one;
        i = D_80224F00[obj->ent].idx;
        k = (unsigned char)i;
        if (i == 127) goto null;
        goto slot;
    case 11: case 37: case 50:
        obj->hp -= arg->dmg;
        if (obj->ent == -1) goto one;
        i = D_80224F00[obj->ent].idx;
        k = (unsigned char)i;
        if (i != 127) goto slot;
    null:
        p = 0;
        goto call;
    slot:
        p = &D_80235F00[k];
    call:
        func_800A9B64(p, 9);
    one:
        *out = 1;
        break;
    case 28: case 66:
        *out = 1;
        break;
    case 4:
        if (obj->ent != -1) func_800E82AC(msg->c, &D_80224EF0[obj->ent]);
        if (obj->kind != 7) *out = 1;
        break;
    }
}
