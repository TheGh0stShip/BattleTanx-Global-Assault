/* SPAN 0x80098F24 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;

extern u8 D_80217010;
extern void *D_80219200;
extern char D_80216DC0[];
extern OSContStatus D_80216DF0[4];
extern u8 D_80216E00[];
extern int D_80216FA0[4];
extern int D_80216FB0[4];
extern int D_80219208[4];
extern int D_80114800;
extern int D_80114804;

extern int osRecvMesg(void *queue, void *message, int flags);
extern int osSendMesg(void *queue, void *message, int flags);
extern int osContStartQuery(void *mq);
extern void osContGetQuery(OSContStatus *st);
extern int osPfsInitPak(void *mq, void *pfs, int channel);
extern int osMotorInit(void *mq, void *pfs, int channel);

int func_80098CC8(void) {
    int i;
    int r;
    void *pfs;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    while (osRecvMesg(D_80216DC0, 0, 0) == 0) {
    }
    osContStartQuery(D_80216DC0);
    osRecvMesg(D_80216DC0, 0, 1);
    osContGetQuery(D_80216DF0);
    for (i = 0; i < 4; i++) {
        D_80216FA0[i] = -1;
        D_80216FB0[i] = -1;
        D_80219208[i] = -1;
    }
    D_80114804 = 0;
    for (i = 0; i < 4; i++) {
        if (D_80216DF0[i].errno & 8) continue;
        if (!(D_80216DF0[i].type & 4)) continue;
        if (!(D_80216DF0[i].type & 1)) continue;
        D_80219208[D_80114804] = i;
        if (D_80216DF0[i].status & 1) {
            pfs = D_80216E00 + i * 0x68;
            r = osPfsInitPak(D_80216DC0, pfs, i);
            if (r == 2) {
                D_80114800 = r;
            }
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
    osSendMesg(&D_80217010, &D_80219200, 0);
    return D_80114804;
}
