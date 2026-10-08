/* ---- 0x800D8000/c/f97ec.c ---- */
typedef struct { char b[2048]; } Buf2K;
typedef struct { char pad[24]; } Ent24;
void _bzero(void *, int);
void func_800D942C(Ent24 *, Buf2K *);
extern int D_80123B78;
#define gCount D_80123B78
extern Ent24 D_803A7050[];
#define gEnts D_803A7050
void func_800D97EC(char *a0){
  Buf2K buf;
  Buf2K *dst = *(Buf2K **)(a0 + 0xc);
  int i;
  _bzero(&buf, 2048);
  for (i = 0; i < gCount; i++) func_800D942C(&gEnts[i], &buf);
  *dst = buf;
}

