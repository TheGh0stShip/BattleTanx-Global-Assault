/* SPAN 0x8009A4C8 */
/* RODATA_VRAM 0x80072484 */
typedef unsigned char u8;
typedef unsigned long long u64;

#define USEC(c) ((c) * 1000000 / D_80126E50)

extern u64 D_80126E50;
extern int D_80114868;
extern int D_80117EB4;
extern u8 D_803A7033;
extern int D_8023A060;
extern u64 D_80219250;
extern u64 D_80219478;
extern u64 D_80219480;
extern float D_80219488;
extern int D_8021948C;
extern unsigned int D_802195CC;
extern unsigned int D_8021945C;

u64 osGetTime(void);
void func_800A7664(void);

void func_80099FE8(void) {
    u64 now = osGetTime();
    u64 d;

    if (USEC(D_80219478) / 1000000 < USEC(now) / 1000000) {
        D_8021948C = D_80114868;
        if (D_80117EB4 != 10 || D_803A7033 == 0) {
            if (D_80114868 < 20) {
                if (D_8023A060 > 1800) {
                    D_8023A060 -= 50;
                    func_800A7664();
                }
            } else if (D_8023A060 < 5000) {
                D_8023A060 += 50;
                func_800A7664();
            }
        }
        D_80114868 = 0;
    } else {
        D_80114868++;
    }
    switch (D_802195CC) {
    case 5:
    case 6:
    case 7:
        break;
    default:
        d = now - D_80219478;
        D_80219250 += d;
        D_80219488 = USEC(d) * 30.0f / 1000000.0f;
        break;
    }
    D_80219478 = now;
    D_8021945C = USEC(D_80219250 - D_80219480) * 30 / 1000000;
}
