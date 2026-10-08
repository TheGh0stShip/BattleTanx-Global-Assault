/* ---- 0x800D8000/f91c0.c ---- */
void func_800D91C0(char *a, char *b, int c, int *out){
  if (*(unsigned char*)(a+0x22)==0 && *(int*)(b+4)==4) {
   char *x = *(char**)(b+0xc);
   unsigned int y = *(unsigned int*)(x+0x9c);
   if (y < 3 ? y == 0 : y == 3) {
    if (*(int*)(*(char**)(x+0x1d0)+0x10) != *(int*)(a+0x2c)) {
     out[0]=6; out[2]=*(int*)(a+0x1c); *(unsigned char*)(a+0x22)=1; return; }
   }
  }
  out[0]=2;
}

/* ---- 0x800D8000/f9244.c ---- */
void func_800D9244(char *a, char *b, int c, int d, int *out){
 if (c == 0) {
  if (*(unsigned char*)(a+0x22)==0 && *(int*)(b+4)==4) {
   char *x = *(char**)(b+0xc);
   unsigned int y = *(unsigned int*)(x+0x9c);
   if (y < 3 ? y == 0 : y == 3) {
    if (*(int*)(*(char**)(x+0x1d0)+0x10) != *(int*)(a+0x2c)) {
     out[0]=6; out[2]=*(int*)(a+0x1c); *(unsigned char*)(a+0x22)=1; return; }
   }
  }
  out[0]=2;
 }
}

