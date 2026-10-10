typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct Task {
    int active;
    int type;
    s16 next;
    char padA[0x3A];
} Task;

typedef struct {
    void (*update)(Task *task, int *result);
    void (*message)(Task *task, int arg1, int arg2, int arg3, int arg4);
    void (*draw)(Task *task);
} TaskClass;

extern u16 D_80224B50;
extern TaskClass D_80224B58[];
extern s16 D_80224E68[];
extern Task D_80224EF0[];
extern s16 D_80235EF0;
extern u8 D_80114CB4[][65];

static inline void task_flush_pending(void) {
    s16 index;
    int next;

    index = D_80235EF0;
    if (index != -1) {
        do {
            next = D_80224EF0[index].next;
            D_80235EF0 = next;
            D_80224EF0[index].next = D_80224E68[D_80224EF0[index].type];
            D_80224E68[D_80224EF0[index].type] = index;
            index = next;
        } while (next != -1);
    }
}

void func_800A2154(void) {
    Task *task;
    int index;
    int type;
    int next;

    task_flush_pending();
    for (type = 0; type < 65; type++) {
        index = D_80224E68[type];
        while (index != -1) {
            task = &D_80224EF0[index];
            next = D_80224EF0[index].next;
            if (D_80224B58[task->type].message != 0) {
                D_80224B58[task->type].message(task, 0, 2, 0, 0);
            }
            index = next;
        }
    }
}
