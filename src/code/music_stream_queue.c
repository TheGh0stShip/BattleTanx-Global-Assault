typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    void *rom;
    int size;
} Track;

typedef struct {
    u16 state;
    u16 current;
    int handle;
    void *buffers[2];
    int queued;
} MusicStream;

extern MusicStream D_801B4540;
extern Track D_80114710[];

void func_8009ED00(void *rom, void *destination, int size);
void func_800FB7D4(int handle, int fade);

void func_80097D14(int track, int fade) {
    MusicStream *stream = &D_801B4540;

    switch (stream->state) {
    case 0:
        func_8009ED00(
            D_80114710[track].rom,
            stream->buffers[0],
            D_80114710[track].size
        );
        break;
    case 1:
        func_8009ED00(
            D_80114710[track].rom,
            stream->buffers[(stream->current + 1) & 1],
            D_80114710[track].size
        );
        if (fade != -1) {
            func_800FB7D4(stream->handle, fade);
        }
        break;
    }
    stream->state = 2;
}
