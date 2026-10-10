typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    float m[4][4];
    u8 grid;
} Frame;

typedef struct {
    int active;
    int type;
    char pad08[4];
    u16 angle;
    float x;
    float y;
    u8 unit;
    u8 grid;
} Spotter;

void func_8009EEE0(Frame *frame);
void func_8009EFD4(Frame *frame, float x, float z, float y, u16 angle);

void func_800A6FD0(Spotter *spotter, void *from, int kind, Frame *frame) {
    func_8009EEE0(frame);
    func_8009EFD4(frame, spotter->x, 0.0f, spotter->y, spotter->angle);
    frame->grid = spotter->grid;
}
