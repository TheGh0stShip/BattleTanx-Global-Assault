/* SPAN 0x800996C4 */
typedef unsigned char u8;
typedef unsigned int u32;

extern u8 D_80217010;
extern void *D_80219200;
typedef struct { char d[0x68]; } OSPfs;
extern OSPfs D_80216D98[];

extern int osRecvMesg(void *queue, void *message, int flags);
extern int osSendMesg(void *queue, void *message, int flags);
extern int osPfsReadWriteFile(void *pfs, int file_number, int mode, int offset, int size, void *buffer);
extern u32 func_80099758(void *buffer);

inline int func_80099464(int controller, int file_number, void *buffer) {
    int result;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller--;
    result = osPfsReadWriteFile(&D_80216D98[controller + 1], file_number, 0, 0, 0x100, buffer);
    if (result == 0 && func_80099758(buffer) != *(u32 *)buffer) {
        result = -0x45;
    }
    osSendMesg(&D_80217010, &D_80219200, 0);
    return result;
}
int func_80099534(int controller, int file_number, u32 *buffer) {
    u32 sum;
    int result;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    sum = func_80099758(buffer);
    *buffer = sum;
    result = osPfsReadWriteFile(&D_80216D98[controller], file_number, 1, 0, 0x100, buffer);
    osSendMesg(&D_80217010, &D_80219200, 0);
    if (result != 0) return result;
    result = func_80099464(controller, file_number, buffer);
    if (result != 0) return result;
    if (func_80099758(buffer) != sum) {
        result = -99;
    }
    return result;
}
