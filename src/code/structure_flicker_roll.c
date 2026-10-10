typedef float f32; typedef unsigned char u8;
extern f32 func_8009D8A0(f32);
typedef struct { char pad[10]; u8 f; } B;
void func_800DD6F4(B *b) {
    if (b->f & 6) {
        if (func_8009D8A0(1.0f) < 0.25f) b->f |= 1;
    }
}
