/* SPAN 0x80097EE4 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    void *rom;
    int size;
} Track;

typedef struct {
    u16 state;          /* 0x00 */
    u16 cur;            /* 0x02 */
    int handle;         /* 0x04 */
    void *buf[2];       /* 0x08 */
    int queued;         /* 0x10 */
} MusStream;

extern MusStream D_801B4540;
extern Track D_80114710[];
extern u8 D_801147E0;

void func_8009ED00(void *rom, void *dst, int size);
void func_800FB7D4(int handle, int fade);
int func_800FB888(int handle);
int MusStartSong(void *song);
void func_80098180(void);

void func_80097D14(int track, int fade);



void func_80097DF8(void) {
    MusStream *m = &D_801B4540;

    func_80098180();
    if (m->state == 2) {
        if (m->handle != 0) {
            if (func_800FB888(m->handle) != 0) return;
            m->handle = 0;
            if (m->queued == 0) {
                func_8009ED00(D_80114710[5].rom, m->buf[(m->cur + 1) & 1], D_80114710[5].size);
            }
        }
        if (m->queued > 0) {
            m->cur = (m->cur + 1) & 1;
            m->handle = MusStartSong(m->buf[m->cur]);
            m->state = 1;
        }
    }
    D_801147E0 = 0;
}
