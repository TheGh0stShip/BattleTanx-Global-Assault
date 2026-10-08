typedef struct { char pad[0x14]; unsigned short id; } Obj;
#define D_803A65B4 ((unsigned char *)0x803A65B4)
#define D_803A65D4 ((unsigned char *)0x803A65D4)
unsigned char func_800CF738(Obj *o) { return D_803A65B4[o->id]; }
int func_800CF750(void) { return D_803A65B4[0] && D_803A65B4[1]; }
int func_800CF774(Obj *o) { D_803A65D4[o->id] = 1; return 1; }
unsigned char func_800CF790(Obj *o) { return D_803A65B4[o->id]; }
