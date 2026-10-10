/* RODATA_VRAM 0x80072598 */
typedef unsigned short u16;

extern float D_80114970[];

u16 func_8009D5B4(float value) {
    u16 low;
    u16 step;
    int trial;

    if (value == -1.0f) {
        low = 128;
    } else {
        low = 0;
        step = 64;
        while (value < D_80114970[low + 1] && step != 0) {
            trial = low ^ step;
            low = trial;
            if (D_80114970[trial] < value) {
                low = step ^ trial;
            }
            step >>= 1;
        }
    }
    return (low << 8) +
           (unsigned int)((D_80114970[low] - value) * 256.0 /
                          (D_80114970[low] - D_80114970[low + 1]));
}
