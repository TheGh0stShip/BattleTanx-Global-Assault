/* ---- 0x800E9000/b/ed774.c ---- */
typedef struct { char pad[24]; float v[3]; char p[8]; unsigned char b44, b45; } Obj;
extern int func_8009E9C8(void *, void *);
extern void func_800ED4F4(Obj *, int, int);
void func_800ED774(Obj *a0, int a1, void *a2) {
    if (a0->b45 == 0) {
        func_800ED4F4(a0, func_8009E9C8(a2, a0->v) & 0xFFFF, 1);
    }
}

