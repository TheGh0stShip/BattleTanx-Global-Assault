/* ---- 0x800D8000/f9a88.c ---- */
void func_800D9900(int);
void func_800D9A88(unsigned int *p){
 while (*p != 0xDF000000) { if (*(unsigned char*)p == 0xFD) { func_800D9900(p[1]); return; } p += 2; }
}

/* ---- 0x800D8000/r2/f9ae0.c ---- */
void func_800D9900(int);
void func_800D9AE0(unsigned int ****a){
 unsigned int *q = **a[0];
 int i;
 for (i = 0; q[i*2] != 0xDF000000; i++) { if (*(unsigned char*)&q[i*2] == 0xFD) { func_800D9900(q[i*2+1]); return; } }
}

