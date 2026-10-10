/* SPAN 0x8009D4AC */
typedef unsigned long long u64;
typedef struct { int unk0; unsigned int buttons; } Pad;

extern int osTvType;
extern int D_802195C0;
extern int D_802195C4;
extern int D_802195C8;
extern int D_802195CC;
extern int D_802195D0;
extern int D_802195D4;
extern int D_802195D8;
extern int D_802195DC;
extern unsigned char D_802195B9;

void func_80097560(void);
void func_80079CB8(void);
void func_800985A0(void);
void func_800A1150(int a, int b, int c, int d, int e);
void func_8009DA34(unsigned int seed);
u64 osGetTime(void);
void func_80098B2C(void);
void func_80098B40(void);
Pad *func_80098250(int n);
void func_800C1BC8(void);
void func_800CD730(int a, int b, int c);
void func_80079FF0(void);
void func_8009C524(void);
void func_800BF80C(void);
void func_8007A0A0(void);

void func_8009D270(void) {
    Pad *pad;

    D_802195D8 = 0;
    D_802195C8 = 0;
    D_802195DC = 2;
    D_802195CC = 2;
    D_802195D0 = 2;
    D_802195C0 = 2;
    D_802195D4 = 0;
    D_802195C4 = 0;
    func_80097560();
    func_80079CB8();
    func_800985A0();
    switch (osTvType) {
    case 0:
        func_800A1150(3, 30, 16, 1, 2);
        break;
    case 2:
        func_800A1150(3, 30, 30, 1, 2);
        break;
    default:
        func_800A1150(3, 30, 2, 1, 2);
        break;
    }
    func_8009DA34(osGetTime());
    func_80098B2C();
    func_80098B40();
    pad = func_80098250(0);
    func_800C1BC8();
    if (pad->buttons & 0x1000) {
        func_800CD730(3, D_802195D8, D_802195C8);
    }
    D_802195B9 = 0;
}

void func_8009D3A4(void) {
    func_8009D270();
    while (1) {
        if (D_802195DC != D_802195D8) {
            D_802195D0 = D_802195DC;
            D_802195DC = D_802195D8;
            D_802195D4 = 0;
        } else {
            D_802195D4++;
        }
        if (D_802195CC != D_802195C8) {
            D_802195C0 = D_802195CC;
            D_802195CC = D_802195C8;
            D_802195C4 = 0;
        } else {
            D_802195C4++;
        }
        func_80079FF0();
        if (D_802195DC != 0) {
            if (D_802195DC == 1) {
                func_8009C524();
            }
        } else {
            func_800BF80C();
        }
        func_8007A0A0();
    }
}
