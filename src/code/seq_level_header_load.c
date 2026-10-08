extern unsigned char D_80121CE0;
extern unsigned char D_80121CE3;
extern unsigned char D_80121CE4;
extern int D_80117F44;
extern int D_80117EB4;
extern int D_803A702C;
extern int D_803A69FC;
extern short D_803A7020;
extern int D_803A6664;
extern int D_802195D8;
extern int D_802195C8;
extern unsigned char *func_800ACEB4(int);
extern void func_8009ED00(int, unsigned char *, int);

void func_800D520C(int a0, int a1)
{
    unsigned char *buf;

    D_80121CE4 = 0;
    buf = func_800ACEB4(64);
    func_8009ED00(a0, buf, 64);
    if (buf[0] == 255)
        return;
    switch (buf[0]) {
    case 0: D_80117F44 = 15; break;
    case 1: D_80117F44 = 0; break;
    case 2: D_80117F44 = 16; break;
    case 3: D_80117F44 = 1; break;
    case 4: D_80117F44 = 2; break;
    case 5: D_80117F44 = 3; break;
    case 6: D_80117F44 = 5; break;
    case 7: D_80117F44 = 4; break;
    case 8: D_80117F44 = 8; break;
    case 9: D_80117F44 = 6; break;
    case 10: D_80117F44 = 7; break;
    case 11: D_80117F44 = 9; break;
    case 12: D_80117F44 = 11; break;
    case 13: D_80117F44 = 10; break;
    case 14: D_80117F44 = 13; break;
    case 15: D_80117F44 = 14; break;
    case 16: D_80117F44 = 12; break;
    case 17: D_80117F44 = 17; break;
    case 18: D_80117F44 = 18; break;
    case 19: D_80117F44 = 19; break;
    case 20: D_80117F44 = 20; break;
    case 21: D_80117F44 = 21; break;
    case 22: D_80117F44 = 22; break;
    case 23: D_80117F44 = 23; break;
    case 24: D_80117F44 = 25; break;
    }
    D_803A702C = a0;
    D_803A69FC = a1;
    D_803A7020 = D_80117F44;
    D_803A6664 = D_80117EB4;
    D_80117EB4 = 10;
    if (D_80121CE0 != 0) {
        D_802195D8 = 1;
        D_802195C8 = 3;
    }
    D_80121CE3 = 1;
}
