typedef struct { unsigned char type; char pad[7]; int val; int pad2; } Entry;
typedef struct { int pad; Entry *list; } Owner;
extern unsigned int func_800C17C8(int);
void func_800CB1E4(Owner *o, unsigned char *dst) {
    unsigned char *out;
    Entry *e = o->list;
    unsigned char i;
    unsigned char r;
    while (e->type != 4) e++;
    for (out = dst, i = 0; i < 9; i++) {
        e++;
        switch (func_800C17C8(e->val)) {
        case 0x8000: r = 1; break;
        case 0x4000: r = 2; break;
        case 0x2000: r = 3; break;
        case 0x20: r = 4; break;
        case 0x10: r = 5; break;
        case 8: r = 6; break;
        case 4: r = 7; break;
        case 2: r = 8; break;
        case 1: r = 9; break;
        default: r = 0; break;
        }
        *out++ = r;
        e++;
    }
}
