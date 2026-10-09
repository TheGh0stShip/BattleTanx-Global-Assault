/* SPAN 0x80078830 */
/* RODATA_VRAM 0x80071040 */
/* LDSYM D_80127E40=0x80127E40 */
extern char D_80127E40[];
typedef struct { unsigned int limit; unsigned int flags; } MemRegion;
extern MemRegion D_801144A0[];
extern unsigned int func_80077CE0(unsigned int);
extern void func_80077DA4(unsigned int, unsigned int, unsigned int);
extern void func_80077EBC(unsigned int, unsigned int, unsigned int);
extern void func_80078048(unsigned int, unsigned int, unsigned int);
extern void func_80078134(unsigned int, unsigned int, unsigned int);
extern void func_80078200(unsigned int, unsigned int, unsigned int);
extern void func_80077CA8(unsigned int, unsigned int);
extern void func_80077D1C(int);
extern int func_80077C40(void);
extern void func_80077C78(int);
extern void func_80078D50(void);
extern unsigned char D_80128160, D_80128162;
extern unsigned int D_801144A8, D_801144B8;
#define osMemSize (*(unsigned int *)0x80000318)
extern void func_80078274(int);
extern void func_80078424(void);
extern unsigned int __osGetSR(void);
extern void func_80078E40(unsigned int);
extern unsigned int D_80127FB8;

void func_80078274(int dir) {
    unsigned int dst = 0xB1FE0000;
    unsigned int addr = func_80077CE0(0xB1FFFFF4);
    unsigned int len = func_80077CE0(0xB1FFFFF8);
    unsigned int n;
    unsigned int flags;
    int i;

    if (addr > 0xEFFFFFFF) {
        addr += (unsigned int)D_80127E40 - 0xF1000000;
    }
    while (len != 0) {
        for (i = 0; D_801144A0[i].limit < addr; i++) {
        }
        n = D_801144A0[i].limit + 1 - addr;
        if (len < n) {
            n = len;
        }
        flags = D_801144A0[i].flags;
        if (!(flags & dir)) {
            if (dir == 1) {
                func_80078134(dst, 0, n);
            }
        } else if (flags & 4) {
            if (dir == 1) {
                func_80078200(dst, addr, n);
            } else {
                func_80078200(addr, dst, n);
            }
        } else if (dir == 1) {
                func_80078048(dst, addr, n);
        } else if (flags & 8) {
            func_80077EBC(dst, addr, n);
        } else {
            func_80077DA4(dst, addr, n);
        }
        dst += n;
        len -= n;
        addr += n;
    }
}

void func_80078424(void) {
    unsigned int src = func_80077CE0(0xB1FFFFF4) & 0xB1FFFFFC;
    unsigned int len = func_80077CE0(0xB1FFFFF8) & 0x1FFFFFC;
    unsigned int i;

    func_80077CA8(0xB1FFFFFC, 0);
    while (func_80077CE0(0xB0000010) == 0) {
        func_80077D1C(500);
    }
    for (i = 0; i < len >> 2; i++) {
        func_80077CA8((src & 0xB07FFFFF) + i * 4, func_80077CE0(src + i * 4));
    }
    func_80077D1C(2000);
    func_80077CA8(0xB1FFFFF4, 0);
}

void func_80078524(void) {
    int mask = func_80077C40();
    unsigned int size;
    func_80078D50();
    func_80077CA8(0xB1FFFFF0, 0);
    func_80077CA8(0xB1FFFFFC, 0);
    size = osMemSize;
    D_80128160 = 1;
    D_80128162 = 0xFF;
    if (size > 0x3FFFFF) {
        if (!(size & 0x1FFFF) & (size <= 0x2000000)) {
            size--;
            D_801144A8 = size - 0x80000000;
            D_801144B8 = size + 0xA0000000;
        }
    }
    func_80077C78(mask);
}

void func_800785F0(void) {
    int cmd;
    int keep;

    do {
        keep = 1;
        while ((cmd = func_80077CE0(0xB1FFFFF0)) == 0) {
            func_80077D1C(1000);
        }
        if (cmd == 2) {
            func_80077CA8(0xB1FFFFFC, 0);
            func_80077D1C(1000);
        } else {
            func_80077CA8(0xB1FFFFFC, 0x101);
            while (func_80077CE0(0xB1FFFFF0) == cmd) {
                func_80077D1C(500);
            }
            if (func_80077CE0(0xB1FFFFF0) == 2) {
                func_80077CA8(0xB1FFFFFC, 0);
            } else {
                switch (cmd) {
                case 16:
                    break;
                case 17:
                    keep = 0;
                    break;
                case 18:
                    D_80128160 = 0;
                    D_80128162 = 0;
                    D_80127FB8 &= ~1;
                    func_80078E40(__osGetSR() & ~1);
                    break;
                case 19:
                    D_80128162 = 0;
                    D_80128160 = 0;
                    break;
                case 20:
                    D_80128162 = 0xFF;
                    break;
                case 21:
                    D_80128162 = 0;
                    break;
                case 22:
                    func_80077CA8(0xB1FE0000, (D_80128162 << 24) | (D_80128160 << 16));
                    break;
                case 23:
                    func_80078274(2);
                    break;
                case 24:
                    func_80078274(1);
                    break;
                case 25:
                    func_80078424();
                    break;
                }
                func_80077CA8(0xB1FFFFFC, 0);
                while (func_80077CE0(0xB1FFFFF0) == 1) {
                    func_80077D1C(500);
                }
            }
        }
    } while (keep || D_80128162 == 0);
}
