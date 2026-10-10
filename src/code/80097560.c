/* SPAN 0x80097660 */
typedef unsigned char u8;

typedef struct {
    unsigned long control_flag;
    int channels;
    void *sched;
    int thread_priority;
    u8 *heap;
    int heap_length;
    u8 *ptr;
    u8 *wbk;
    void *default_fxbank;
    int fifo_length;
    int syn_updates;
    int syn_output_rate;
    int syn_rsp_cmds;
    int syn_retraceCount;
    int syn_num_dma_bufs;
    int syn_dma_buf_size;
    void *diskrom_handle;
} musConfig;

typedef struct { char p[0x20]; } MusStream;

extern char D_801B4560[];
extern u8 D_801B45D8[];
extern char D_801147EC[];
extern MusStream D_801B4540;
extern int D_801B4490;

void alHeapInit(void *hp, u8 *base, int len);
void func_800FBDBC(void *p);
void func_800FAFBC(musConfig *c);
void func_8009790C(void);
void func_800979A0(void);
void func_80097C84(MusStream *s);
void MusSetMasterVolume(int flags, int vol);
void func_80097C1C(int vol);

void func_80097560(void) {
    musConfig c;

    alHeapInit(D_801B4560, D_801B45D8, 0x3EBC0);
    func_800FBDBC(D_801147EC);
    c.control_flag = 0;
    c.channels = 42;
    c.sched = 0;
    c.thread_priority = 20;
    c.heap = D_801B45D8;
    c.heap_length = 0x3EBC0;
    c.ptr = 0;
    c.wbk = 0;
    c.default_fxbank = 0;
    c.fifo_length = 64;
    c.syn_updates = 256;
    c.syn_output_rate = 22050;
    c.syn_rsp_cmds = 0x1000;
    c.syn_retraceCount = 2;
    c.syn_num_dma_bufs = 58;
    c.syn_dma_buf_size = 0x800;
    func_800FAFBC(&c);
    func_8009790C();
    func_800979A0();
    func_80097C84(&D_801B4540);
    MusSetMasterVolume(1, 0x502D);
    func_80097C1C(0x313B);
    D_801B4490 = 0;
}
