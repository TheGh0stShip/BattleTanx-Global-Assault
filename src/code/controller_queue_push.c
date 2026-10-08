typedef struct { int a; int b; unsigned short c; short pad; int rest[5]; } Slot;
extern short D_8011F230;
extern short D_8011F232;
extern unsigned short D_8011F234, D_8011F236, D_8011F238;
extern int D_803A62A8[];
extern int D_803A6358;
extern short D_803A6350;
extern short D_803A6560;
extern Slot D_803A6360[];
extern int *func_8009836C(unsigned short);
extern short func_80099784(unsigned short, int *);
extern short func_800992E0(unsigned short, int *);
extern short func_800993B0(unsigned short, Slot *);
extern short func_800996C4(unsigned short);
extern void func_80098CC8(void);
extern unsigned short func_800CC0B0(void);
extern void func_800CD85C(int, int, int, int);

static inline void push(int x) {
    if (D_8011F230 < 10) { D_8011F230++; D_803A62A8[D_8011F230] = x; }
}
#define PUSH(x) push(x)

static inline unsigned short slot_used(unsigned short s) {
    if (D_8011F236 == 0 && func_800CC0B0() != 0) return 0;
    if (D_803A6360[s].b != 0) return 1;
    if (D_803A6360[s].c != 0) return 1;
    if (D_803A6360[s].a != 0) return 1;
    return 0;
}

int func_800CCA6C(unsigned short *arg) {
    int buf[2];
    unsigned int btn;
    unsigned short ok;
    unsigned short i;
    int st;
    int next;

    btn = func_8009836C(arg[10])[1];
    if (btn & 0x9000) {
        switch (D_803A6358) {
        case 6:
            func_80098CC8();
            break;
        case 1:
            if ((D_8011F232 = func_80099784(D_8011F234, &buf[0])) != 0) {
                PUSH(6);
                return 1;
            }
            if (buf[0] >= 256) {
                D_8011F232 = func_800992E0(D_8011F234, &buf[1]);
                D_8011F236 = 0;
                if (D_8011F232 == 8) {
                    D_8011F230--; push(3); return 1;
                }
                if (D_8011F232 != 0) {
                    PUSH(6);
                    return 1;
                }
                D_8011F230--;
                if (D_803A62A8[0] != 2) {
                    func_800CD85C(0, 0, 0, 50);
                    PUSH(2);
                }
                D_803A6560 = 1;
                return 1;
            }
            D_8011F230--; push(3); return 1;
        case 3:
            D_8011F230--; push(4); return 1;
        case 4:
            i = D_8011F238;
            if (slot_used(i)) {
                if ((D_8011F232 = func_800993B0(D_8011F234, &D_803A6360[D_8011F238])) != 0) {
                    PUSH(6);
                    return 1;
                }
                D_8011F236 = 0;
            }
        case 7:
        case 10:
            D_8011F230--;
            break;
        }
        D_8011F230--;
        return 1;
    }
    if (btn & 0x2000) {
        st = D_803A6358;
        if (st != 6) return 0;
        if (D_803A6350 == 10 || D_803A6350 == 5) {
            D_8011F232 = func_800996C4(D_8011F234);
            if (D_8011F232 == 0) {
                func_80098CC8();
                D_8011F230--; push(8); return 1;
            }
            if (D_8011F232 == 10) {
                D_8011F230--;
                push(9);
                return 1;
            }
            D_8011F230--;
            PUSH(st);
            return 1;
        }
        return 0;
    }
    switch ((unsigned int)D_803A6358) {
    case 3:
    case 4:
        D_8011F230 -= 2;
        break;
    case 8:
    case 9:
        break;
    default:
        D_8011F230 = -1;
        break;
    }
    return 1;
}
