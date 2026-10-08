typedef struct { void (*fn)(void *, void *, int, int, int); int a; int b; } Handler;
typedef struct { void *p; int pad[8]; } Entry;
typedef struct { int pad; int type; } Obj;
extern Handler D_80224B5C[];
extern short D_80397650;
extern unsigned short func_800B3748(int, int, Entry *, int, int);
void func_800F5470(unsigned short *arg0) {
    Entry buf[32];
    unsigned short n;
    int i;
    Entry *e;
    D_80397650 = 0;
    n = func_800B3748(arg0[5], 0x10000, e = buf, 0, 0);
    if (n != 0) {
        for (i = 0; i < n; i++) {
            if (e[i].p != 0) {
                if (D_80224B5C[((Obj *)e[i].p)->type].fn != 0) {
                    D_80224B5C[((Obj *)e[i].p)->type].fn(e[i].p, arg0, 0, 0, 0);
                }
            }
        }
    }
}
