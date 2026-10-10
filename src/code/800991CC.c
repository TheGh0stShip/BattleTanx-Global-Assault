/* SPAN 0x800992E0 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216E00[];

extern int osRecvMesg(void *queue, void *message, int flags);
extern int osSendMesg(void *queue, void *message, int flags);
extern int osPfsFileState(void *pfs, int file_no, OSPfsState *state);

int func_800991CC(int controller, OSPfsState *st, int *count) {
    int i;
    int r;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller--;
    *count = 0;
    for (i = 0; i < 16; i++) {
        r = osPfsFileState(D_80216E00 + controller * 0x68, i, &st[i]);
        if (r == 0) {
            (*count)++;
        } else if (r == 5) {
            r = 0;
            st[i].file_size = 0;
            st[i].game_name[0] = 0;
            st[i].company_code = 0;
            st[i].game_code = 0;
        } else {
            break;
        }
    }
    osSendMesg(&D_80217010, &D_80219200, 0);
    return r;
}
