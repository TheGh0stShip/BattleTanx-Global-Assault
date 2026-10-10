/* SPAN 0x8009B0F0 */
/* RODATA_VRAM 0x800724C8 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned long long u64;

extern int D_80117EB4;
extern s8 D_80117EB0;
extern int D_80117F44;
extern u8 D_80121CE0;
extern int D_802195C8;
extern int D_802195CC;
extern u64 D_80219478;
extern u64 D_80219250;
extern u64 D_80219480;
extern int D_8021948C;
extern int D_8021945C;
extern float D_80219488;
extern int D_8023A060;
extern u8 D_802194A4, D_802194A5, D_802194A6, D_802194A7, D_802194A8, D_802194A9, D_802194AA, D_802194AB;
extern int D_802194A0;
extern int D_802194B0;
extern int D_8021949C;
extern int D_80114680;

int *func_800E9424(void);
void func_800D520C(int a, int b);
void func_800A1384(void);
void func_800C180C(void);
u64 osGetTime(void);
void func_8008A1A4(int mode);
void func_8009A6F8(void);
void func_8009A95C(void);
void func_80097B44(int a, int b);
void func_800A22CC(void);
void func_800A87BC(void);
void func_800AF43C(void);
void func_800A7D70(void);
void func_800D4DE0(void);
void func_8008A8C4(void);
void func_80098B2C(void);
void func_800A9D50(void);
void func_800B04E0(void);
void func_800F1770(void);

void func_8009AE38(void) {
    int *p;
    u64 t;
    int mode;
    int m;

    mode = D_80117EB4;
    if (mode == 1 && *func_800E9424() == mode) {
        p = func_800E9424();
        D_802195C8 = 3;
        D_802195CC = 3;
        D_80121CE0 = 0;
        func_800D520C(p[1], p[2]);
    }
    func_800A1384();
    func_800C180C();
    t = osGetTime();
    D_80219478 = t;
    D_80219250 = t;
    D_80219480 = t;
    D_8021948C = 20;
    D_8021945C = 0;
    D_80219488 = 1.0f;
    m = D_80117EB4;
    if (m == 10) {
        D_8023A060 = 3400;
    } else {
        switch (D_80117EB0) {
        case 1:
            D_8023A060 = 3400;
            break;
        case 2:
            D_8023A060 = 2866;
            break;
        default:
            D_8023A060 = 1800;
            break;
        }
    }
    func_8008A1A4(m);
    if (D_80117EB4 == 1) {
        func_8009A6F8();
    } else if (D_80117EB4 == 10) {
        D_802194A4 = 1;
        D_802194A5 = 1;
        D_802194A6 = 4;
        D_802194A7 = 4;
        D_802194A8 = 0;
        D_802194A9 = 1;
        D_802194AA = 2;
        D_802194AB = 3;
        D_802194A0 = 0;
        D_802194B0 = 1200;
        D_8021949C = D_80117F44;
        func_80097B44(0, D_80117F44);
        func_800A22CC();
        func_800A87BC();
        func_800AF43C();
        func_800A7D70();
        func_800D4DE0();
    } else {
        func_8009A95C();
    }
    if (D_80117EB4 == 10) {
        D_80114680 = 0;
    } else {
        func_8008A8C4();
    }
    func_80098B2C();
    func_800A9D50();
    func_800B04E0();
    func_800F1770();
    t = osGetTime();
    D_80219478 = t;
    D_80219250 = t;
    D_80219480 = t;
    D_8021945C = 0;
    D_8021948C = 20;
    D_80219488 = 1.0f;
}
