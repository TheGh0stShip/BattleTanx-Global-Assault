/* SPAN 0x80079FF0 */
/* CFLAGS -O0 -G0 -mips3 -mgp32 -mfp32 */
/* LDSYM D_801293E0=0x801293E0 */
/* LDSYM D_80000400=0x80000400 */
/* LDSYM D_801294B8=0x801294B8 */
/* LDSYM D_801144F8=0x801144F8 */
/* LDSYM rspbootTextEnd=0x800F8E80 */
/* LDSYM gspF3DEX_fifoTextStart=0x800F8E80 */
/* LDSYM gspF3DEX_fifoDataStart=0x80125EC0 */
/* LDSYM D_801294C0=0x801294C0 */
/* LDSYM D_80158080=0x80158080 */
/* LDSYM D_80168080=0x80168080 */
/* LDSYM D_801287E0=0x801287E0 */
/* LDSYM D_80157E80=0x80157E80 */
/* LDSYM D_801420C0=0x801420C0 */
/* LDSYM D_801418C0=0x801418C0 */
/* LDSYM D_801298C0=0x801298C0 */
/* LDSYM D_B00B7E30=0xB00B7E30 */
/* LDSYM D_B00B8230=0xB00B8230 */
/* LDSYM D_803B17B0=0x803B17B0 */
/* LDSYM D_803D88B0=0x803D88B0 */
/* LDSYM D_801144F0=0x801144F0 */
extern unsigned char D_801144F4, D_801144F5, D_801144F6;
typedef struct {
    unsigned int type, flags;
    unsigned long long *ucode_boot;
    unsigned int ucode_boot_size;
    unsigned long long *ucode;
    unsigned int ucode_size;
    unsigned long long *ucode_data;
    unsigned int ucode_data_size;
    unsigned long long *dram_stack;
    unsigned int dram_stack_size;
    unsigned long long *output_buff;
    unsigned long long *output_buff_size;
    unsigned long long *data_ptr;
    unsigned int data_size;
    unsigned long long *yield_data_ptr;
    unsigned int yield_data_size;
} OSTask_t;
typedef union { OSTask_t t; long long force_structure_alignment; } OSTask;
typedef struct { OSTask task; int pad[2]; } TaskSlot;
typedef struct { unsigned char *a; unsigned char *b; unsigned char *c; unsigned char *d; } Bufs;
typedef struct {
    TaskSlot slot[2];
    Bufs bufs[2];
    char pad[0xB0 - 0xB0];
    short id[4];
    int fbsize;
    void *fb;
    short cnt;
    int unkC4;
} GfxState;
extern GfxState D_801293E0;
extern char D_80000400[];
extern unsigned char D_801294B8;
extern GfxState *D_801144F8;
extern unsigned long long rspbootTextStart[], rspbootTextEnd[];
extern unsigned long long gspF3DEX_fifoTextStart[], gspF3DEX_fifoDataStart[];
extern unsigned long long D_801294C0[], D_80158080[], D_80168080[], D_801287E0[];
extern unsigned char D_80157E80[][0x100];
extern unsigned char D_801420C0[][0xAEE0];
extern unsigned char D_801418C0[][0x400];
extern unsigned char D_801298C0[][0xC000];
extern char D_B00B7E30[], D_B00B8230[], D_803B17B0[], D_803D88B0[];
extern char *D_801144F0;
extern void _bzero(void *, int);
extern void func_8007A710(GfxState *);
extern void guMtxIdent(void *);
extern void func_8009ED00(char *, char *, unsigned int);

void func_80079C00(unsigned char r, unsigned char g, unsigned char b) {
    D_801144F4 = r;
    D_801144F5 = g;
    D_801144F6 = b;
}

void func_80079C5C(unsigned char *r, unsigned char *g, unsigned char *b) {
    *r = D_801144F4;
    *g = D_801144F5;
    *b = D_801144F6;
}

void func_80079CB8(void) {
    unsigned short i;
    unsigned int len;
    OSTask *task;
    OSTask_t *t;
    Bufs *b;
    GfxState *gs = &D_801293E0;

    _bzero(D_80000400, 0x70800);
    D_801294B8 = 1;
    D_801144F8 = gs;
    func_8007A710(gs);
    for (i = 0; i < 2; i++) {
        task = &gs->slot[i].task;
        t = &task->t;
        t->type = 1;
        t->flags = 0;
        t->ucode_boot = rspbootTextStart;
        t->ucode_boot_size = (unsigned int)rspbootTextEnd - (unsigned int)rspbootTextStart;
        t->ucode = gspF3DEX_fifoTextStart;
        t->ucode_data = gspF3DEX_fifoDataStart;
        t->ucode_size = 4096;
        t->ucode_data_size = 2048;
        t->dram_stack = D_801294C0;
        t->dram_stack_size = 1024;
        t->output_buff = D_80158080;
        t->output_buff_size = D_80168080;
        t->yield_data_ptr = D_801287E0;
        t->yield_data_size = 3072;
        b = &gs->bufs[i];
        b->a = D_80157E80[i];
        b->b = D_801420C0[i];
        b->c = D_801418C0[i];
        b->d = D_801298C0[i];
        guMtxIdent(b->d);
    }
    gs->fb = D_80000400;
    gs->fbsize = 0x12C00;
    gs->id[0] = -1;
    gs->id[1] = -1;
    gs->id[2] = -1;
    gs->id[3] = -1;
    gs->cnt = 0;
    gs->unkC4 = 0;
    len = D_B00B8230 - D_B00B7E30;
    if (len >= (unsigned int)(D_803D88B0 - D_803B17B0)) {
    }
    D_801144F0 = D_803B17B0;
    func_8009ED00(D_B00B7E30, D_801144F0, len);
}
