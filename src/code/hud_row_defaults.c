typedef struct { char pad[4]; unsigned char *p; } S;
void func_800C425C(S *a) {
    unsigned char *p = a->p;
    if (p != 0) {
        p[0x30] = 23; p[0x40] = 1; p[0x50] = 1; p[0x60] = 1; p[0x70] = 1; p[0x80] = 1;
        p[0x90] = 1; p[0xA0] = 1; p[0xB0] = 1; p[0xC0] = 1; p[0xD0] = 1;
    }
}
