/* ---- 0x800D8000/f8674.c ---- */
void func_800D8674(char *a, int b, char *c, char *d){
 if (*(int*)(c+4)==0 && *(unsigned char*)(a+0x2f)==0 && *(int*)(a+0x24)!=0 && *(unsigned char*)c==*(unsigned char*)(a+0x2e)){
  *d=1; *(float*)(d+4)=*(float*)(a+0x10); *(float*)(d+8)=*(float*)(a+0x14);}
}

/* ---- 0x800D8000/f86c4.c ---- */
int func_800EC72C(int,int);
void func_800D8380(void*,int);
void func_800D86C4(char *a, int b, char *c){
 if (*(unsigned char*)(a+0x2f)==0 && *(int*)(a+0x24)!=0) func_800D8380(a, func_800EC72C(*(int*)(c+0x14),*(int*)(c+8)));
}

/* ---- 0x800D8000/f871c.c ---- */
void func_800D8380(void*,int);
void func_800D871C(char *a, int b, char *c){
 if (*(unsigned char*)(a+0x2f)==0 && *(int*)(a+0x24)!=0) func_800D8380(a, *(int*)(c+0xc));
}

