typedef struct { char pad[4]; unsigned char *p; } S;
void func_800C420C(S *a) {
    unsigned char *p;
    if ((p = a->p) != 0) { p[0x30] = 1; p[0x40] = 20; p[0x50] = 22; p[0x60] = 21; p[0x70] = 21; p[0x80] = 21; p[0x90] = 21; p[0xA0] = 21; p[0xB0] = 22; p[0xC0] = 14; p[0xD0] = 14; }
}
