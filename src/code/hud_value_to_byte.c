extern float D_803A5948;
extern float D_80073CE4;
typedef struct { char pad[0xC]; unsigned short v; } S;
typedef struct { char pad[8]; S *s; } A;
int func_800C68AC(S *a0, A *a1) {
 S *s = a1->s; unsigned int t; a0 = s; t = D_803A5948 * D_80073CE4; s->v = a0->v + t;
 return 0;
}
