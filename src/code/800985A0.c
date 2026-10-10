/* SPAN 0x800988E8 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned long long u64;

typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;

typedef struct {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;

typedef struct { OSContPad pad[4]; } PadSet;
typedef struct { char d[0x68]; } OSPfs;

extern u8 D_80217010[];
extern void *D_80217028[];
extern u8 D_80217030[];
extern void *D_80217048[];
extern char D_80217050[];
extern void *D_80219200;
extern char D_80216DC0[];
extern void *D_80216DE0[];
extern OSContStatus D_80216DF0[4];
extern OSPfs D_80216E00[];
extern int D_80216FA0[4];
extern int D_80216FB0[4];
extern int D_80216FC0[4];
extern int D_80216FD0[4];
extern int D_80216FE0[4];
extern int D_80216FF0[4];
extern int D_80217000[4];
extern int D_80219208[4];
extern PadSet D_80219218;
extern int D_80219230;
extern PadSet D_80219238;
extern int D_80114804;
extern int osResetType;

extern int osRecvMesg(void *queue, void *message, int flags);
extern int osSendMesg(void *queue, void *message, int flags);
extern void osCreateMesgQueue(void *mq, void **msg, int count);
extern void osCreateThread(void *t, int id, void (*entry)(void), void *arg, void *sp, int pri);
extern void osStartThread(void *t);
extern void osSetEventMesg(int e, void *mq, void *msg);
extern u64 osGetTime(void);
extern int osContInit(void *mq, u8 *bitpattern, OSContStatus *st);
extern int osContStartReadData(void *mq);
extern void osContGetReadData(OSContPad *pad);
extern int osPfsInitPak(void *mq, OSPfs *pfs, int channel);
extern int osMotorInit(void *mq, OSPfs *pfs, int channel);
extern int osMotorStart(OSPfs *pfs);
extern int osMotorStop(OSPfs *pfs);
extern void func_80102F10(void *p, int n);

void func_800988E8(void);

int func_800985A0(void) {
    u8 bits;
    int i;
    int r;
    OSPfs *pfs;

    osCreateMesgQueue(D_80217010, D_80217028, 1);
    osCreateMesgQueue(D_80217030, D_80217048, 2);
    osCreateThread(D_80217050, 5, func_800988E8, 0, &D_80219200, 16);
    osStartThread(D_80217050);
    osCreateMesgQueue(D_80216DC0, D_80216DE0, 4);
    osSetEventMesg(5, D_80216DC0, (void *)1);
    if (osResetType != 0) {
        while (osGetTime() < 0x2000000) {
        }
    }
    osContInit(D_80216DC0, &bits, D_80216DF0);
    func_80102F10(&D_80219218, sizeof(PadSet));
    func_80102F10(&D_80219238, sizeof(PadSet));
    for (i = 0; i < 4; i++) {
        D_80216FA0[i] = -1;
        D_80216FC0[i] = 1;
        D_80216FD0[i] = 0;
        D_80216FB0[i] = -1;
        D_80219208[i] = -1;
    }
    D_80114804 = 0;
    for (i = 0; i < 4; i++) {
        if (!((bits >> i) & 1)) continue;
        if (D_80216DF0[i].errno & 8) continue;
        if (!(D_80216DF0[i].type & 4)) continue;
        if (!(D_80216DF0[i].type & 1)) continue;
        D_80219208[D_80114804] = i;
        if (D_80216DF0[i].status & 1) {
            pfs = &D_80216E00[i];
            r = osPfsInitPak(D_80216DC0, pfs, i);
            if (r == 10) {
                if (osMotorInit(D_80216DC0, pfs, i) == 0) {
                    D_80216FA0[D_80114804] = i;
                } else {
                    D_80216FB0[D_80114804] = i;
                }
            } else if (r != 11) {
                D_80216FB0[D_80114804] = i;
            }
        }
        D_80114804++;
    }
    while (osRecvMesg(D_80216DC0, 0, 0) == 0) {
    }
    osSendMesg(D_80217010, &D_80219200, 0);
    return D_80114804;
}
