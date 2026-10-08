typedef struct { char pad[0x14]; unsigned short id; } Obj;
#define D_803A65FC ((unsigned char *)0x803A65FC)
unsigned char func_800CF58C(Obj *o) { return D_803A65FC[o->id]; }
int func_800CF5A4(void) { return D_803A65FC[0] && D_803A65FC[1]; }
