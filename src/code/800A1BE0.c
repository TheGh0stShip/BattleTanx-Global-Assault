/* SPAN 0x800A2154 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    int active;         /* 0x00 */
    int type;           /* 0x04 */
    s16 next;           /* 0x08 */
    char pA[0x44 - 0xA];
} Task;

typedef struct {
    void (*update)(Task *t, int *result);
    void (*msg)(Task *t, int a, int b, int c, int d);
    void (*draw)(Task *t);
} TaskClass;

extern u16 D_80224B50;
extern TaskClass D_80224B58[];
extern s16 D_80224E68[];
extern Task D_80224EF0[];
extern s16 D_80235EF0;
extern u8 D_80114CB4[][65];

static inline void taskFlushPending(void) {
    s16 i;
    int next;

    i = D_80235EF0;
    if (i != -1) {
        do {
            next = D_80224EF0[i].next;
            D_80235EF0 = next;
            D_80224EF0[i].next = D_80224E68[D_80224EF0[i].type];
            D_80224E68[D_80224EF0[i].type] = i;
            i = next;
        } while (next != -1);
    }
}

static inline void taskUnlink(s16 *pp) {
    s16 i;

    D_80224EF0[*pp].active = 0;
    i = *pp;
    *pp = D_80224EF0[i].next;
    D_80224EF0[i].next = D_80224B50;
    D_80224B50 = i;
}

static inline s16 *taskFind(s16 *pp, Task *t) {
    while (*pp != -1 && &D_80224EF0[*pp] != t) {
        pp = &D_80224EF0[*pp].next;
    }
    return pp;
}

void func_800A1BE0(Task *t) {
    s16 *pp = &D_80224E68[t->type];

    while (*pp != -1 && &D_80224EF0[*pp] != t) {
        pp = &D_80224EF0[*pp].next;
    }
    if (*pp == -1) {
        pp = &D_80235EF0;
        while (*pp != -1 && &D_80224EF0[*pp] != t) {
            pp = &D_80224EF0[*pp].next;
        }
    }
    if (*pp != -1) {
        taskUnlink(pp);
    }
}

void func_800A1D24(int mode) {
    s16 *pp;
    int result;
    int i;

    taskFlushPending();
    for (i = 0; i < 65; i++) {
        if (D_80224B58[i].update != 0 && D_80114CB4[mode][i] != 0) {
            pp = &D_80224E68[i];
            while (*pp != -1) {
                Task *t;

                result = 0;
                t = &D_80224EF0[*pp];
                D_80224B58[t->type].update(t, &result);
                if (result != 0) {
                    if (result == 1) {
                        taskUnlink(pp);
                    }
                } else {
                    pp = &D_80224EF0[*pp].next;
                }
            }
        }
    }
    taskFlushPending();
}

void func_800A1FDC(void) {
    Task *t;
    int i;
    int j;

    taskFlushPending();
    for (j = 0; j < 65; j++) {
        if (D_80224B58[j].draw != 0) {
            for (i = D_80224E68[j]; i != -1; i = D_80224EF0[i].next) {
                t = &D_80224EF0[i];
                D_80224B58[t->type].draw(t);
            }
        }
    }
}
