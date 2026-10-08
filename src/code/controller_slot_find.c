typedef struct { int a; int b; unsigned short c; char ext[4]; unsigned char name[16]; char pad[2]; } Save;
extern Save D_803A6360[16];
extern short D_8011F230, D_8011F232;
extern unsigned short D_8011F234, D_8011F236, D_8011F238, D_8011481C, D_803A6560, D_803A5970;
extern unsigned char D_8011F23A;
extern int D_80114820;
extern int D_803A62A8[];
extern char D_803A61A0[];
extern unsigned short func_800CC574(int, unsigned short, int);
extern unsigned short func_800CC0B0(void);
extern unsigned short func_80099028(void);
extern short func_80099534(unsigned short, int, void *);
extern short func_80099464(unsigned short, int, void *);
extern void func_800CBE40(void);

static inline int find_slot(void) {
    unsigned short i;
    if (D_8011F236 == 0 && func_800CC0B0() != 0) return -1;
    for (i = 0; i < 16; i++) {
        if (D_803A6360[i].c == D_8011481C && D_803A6360[i].b == D_80114820) return i;
    }
    return -1;
}

static inline unsigned short slot_used(unsigned short s) {
    if (D_8011F236 == 0 && func_800CC0B0() != 0) return 0;
    if (D_803A6360[s].b != 0) return 1;
    if (D_803A6360[s].c != 0) return 1;
    if (D_803A6360[s].a != 0) return 1;
    return 0;
}

static inline void push(int x) {
    if (D_8011F230 < 10) { D_8011F230++; D_803A62A8[D_8011F230] = x; }
}

int func_800CCEC4(int arg) {
    int state;
    int idx;

    if (D_8011F230 < 0) {
        goto done;
    busy:
        return 1;
    }
    D_8011F234 = func_80099028();
    if (D_8011F234 == 0) {
        D_8011F232 = 1;
        push(6);
    }
    for (;;) {
        state = D_803A62A8[D_8011F230];
        switch (state) {
        case 0:
            break;
        case 6:
            if (D_8011F232 != 0) {
                if (D_8011F23A) goto busy;
                D_8011F230 = 1;
                D_803A62A8[1] = 0;
                func_800CC574(6, D_8011F232, arg);
            }
            break;
        case 8:
        case 9:
            D_8011F232 = func_800CC574(D_803A62A8[D_8011F230], 0, arg);
            break;
        case 1:
            idx = find_slot();
            if (idx >= 0) {
                if (D_8011F23A) return 0;
                D_8011F230--;
                break;
            }
            if (D_8011F23A) goto busy;
            if ((D_8011F232 = func_800CC574(1, 0, arg)) != 0) {
                push(6);
            } else {
                push(0);
            }
            break;
        case 3:
            if (D_8011F23A) goto busy;
            if ((D_8011F232 = func_800CC574(3, 0, arg)) != 0) {
                push(6);
            } else {
                push(0);
            }
            break;
        case 4:
            if (!slot_used(D_8011F238)) {
                D_8011F230--;
                break;
            }
            if ((D_8011F232 = func_800CC574(4, 0, arg)) != 0) {
                push(6);
            } else {
                push(0);
            }
            break;
        case 10:
            if ((D_8011F232 = func_800CC574(10, 0, arg)) != 0) {
                push(6);
            } else {
                push(0);
            }
            break;
        case 2:
            idx = find_slot();
            if (idx < 0) {
                if (D_8011F23A) goto busy;
                push(1);
            } else if (D_803A6560 == 0) {
                if (D_8011F23A) goto busy;
                D_803A6560 = 1;
                push(10);
            } else {
                if ((D_8011F232 = func_80099534(D_8011F234, idx, D_803A61A0)) != 0) {
                    push(6);
                } else {
                    D_8011F230--;
                }
            }
            break;
        case 5:
            idx = find_slot();
            if (idx < 0) {
                D_8011F232 = -1;
                push(6);
            } else if ((D_8011F232 = func_80099464(D_8011F234, idx, D_803A61A0)) != 0) {
                push(6);
            } else {
                func_800CBE40();
                D_8011F230--;
                push(7);
            }
            break;
        case 7:
            if (D_8011F23A) goto busy;
            D_8011F232 = func_800CC574(7, 0, arg);
            break;
        }
        if (D_8011F230 < 0) break;
        if (state == D_803A62A8[D_8011F230]) return 0;
    }
done:
    D_803A5970 = 1;
    return 0;
}
