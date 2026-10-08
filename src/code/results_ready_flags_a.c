typedef struct { char pad[0x14]; unsigned short id; } Obj;
#define D_803A6638 ((unsigned char *)0x803A6638)
unsigned char func_800CF3E0(Obj *o) { return D_803A6638[o->id]; }
int func_800CF3F8(void) { return D_803A6638[0] && D_803A6638[1]; }
