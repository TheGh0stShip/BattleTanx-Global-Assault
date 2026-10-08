/* ---- 0x800D4000/func_800D4D00.c ---- */
typedef struct {
    int used;
    char pad[0x18];
} Slot;
extern unsigned char D_80121CE4;
extern void *D_803A6FCC;
extern void *D_803A6FD0;
extern Slot D_8011686C[4];
extern Slot D_80117880[14];
extern void *func_800ACEB4(int);

void func_800D4D00(void)
{
    if (D_80121CE4 == 0) {
        D_803A6FCC = func_800ACEB4(18400);
        D_803A6FD0 = func_800ACEB4(18400);
        D_8011686C[0].used = 0;
        D_8011686C[1].used = 0;
        D_8011686C[2].used = 0;
        D_8011686C[3].used = 0;
        D_80117880[3].used = 0;
        D_80117880[0].used = 0;
        D_80117880[1].used = 0;
        D_80117880[2].used = 0;
        D_80117880[4].used = 0;
        D_80117880[5].used = 0;
        D_80117880[6].used = 0;
        D_80117880[7].used = 0;
        D_80117880[8].used = 0;
        D_80117880[9].used = 0;
        D_80117880[10].used = 0;
        D_80117880[11].used = 0;
        D_80117880[12].used = 0;
        D_80117880[13].used = 0;
        D_80121CE4 = 1;
    }
}

