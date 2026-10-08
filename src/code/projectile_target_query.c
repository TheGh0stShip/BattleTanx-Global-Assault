/* ---- 0x800E9000/b/ed730.c ---- */
typedef struct { char pad[24]; float f24, f28; char p[12]; unsigned char b44, b45; } Obj;
typedef struct { unsigned char b0; char p[3]; int w4; } Msg;
typedef struct { unsigned char b0; char p[3]; float f4, f8; } Out;
void func_800ED730(Obj *a0, int a1, Msg *a2, Out *a3) {
    if (a2->w4 == 0 && a0->b45 == 0 && a0->b44 == a2->b0) {
        a3->b0 = 1;
        a3->f4 = a0->f24;
        a3->f8 = a0->f28;
    }
}

