/* RODATA_VRAM 0x800715FC */
typedef struct AngleArc {
    unsigned short start;
    char pad2[2];
    float span;
    float scale;
    float base;
} AngleArc;

extern int func_8009D6DC(unsigned short, unsigned short);
extern int func_8009D81C(unsigned short, unsigned short);
extern float func_8009D4B0(unsigned short);

void func_80087824(AngleArc *arc, unsigned short from, unsigned short to) {
    int from_side = (unsigned short)(from - 0x4000) > 0x7FFF;
    int to_side = (unsigned short)(to - 0x4000) > 0x7FFF;
    float low;
    float high;

    if (from_side == to_side) {
        unsigned short first = from;

        if ((unsigned short)func_8009D6DC(to, from) <= 0x7FFF) {
            if (from_side) {
                low = func_8009D4B0(first);
                high = func_8009D4B0(to);
            } else {
                high = func_8009D4B0(first);
                low = func_8009D4B0(to);
            }
        } else {
            high = 1.0f;
            low = 0.0f;
        }
    } else if (from_side) {
        unsigned short from_distance;
        unsigned short to_distance;

        high = 1.0f;
        from_distance = func_8009D81C(from, 0xC000);
        to_distance = func_8009D81C(to, 0xC000);
        if (from_distance < to_distance) {
            low = func_8009D4B0(from);
        } else {
            low = func_8009D4B0(to);
        }
    } else {
        unsigned short from_distance;
        unsigned short to_distance;

        low = 0.0f;
        from_distance = func_8009D81C(from, 0x4000);
        to_distance = func_8009D81C(to, 0x4000);
        if (from_distance < to_distance) {
            high = func_8009D4B0(from);
        } else {
            high = func_8009D4B0(to);
        }
    }

    arc->start = from;
    arc->span = (unsigned short)func_8009D6DC(to, from);
    arc->base = -low;
    arc->scale = 1.0f / (high - low);
}
