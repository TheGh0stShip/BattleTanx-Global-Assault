/* SPAN 0x80087570 */
/* RODATA_VRAM 0x800715E0 */
typedef unsigned char u8;
typedef struct {
    char p0[0xD8]; int nextFire; int nextPick; char pe0[0xE6 - 0xE0]; u8 weapon; u8 streak;
    char pe8[0x1B8 - 0xE8]; float skill; char p1bc[0x224 - 0x1BC]; int lastFire;
} Ai;
extern int D_8021945C;
int func_80089200(Ai *);
int func_80087DD4(Ai *, int);
float func_8009D8A0(float);
int func_8008E620(Ai *, int);
void func_8008C5D8(Ai *, int, int);

void func_800873FC(Ai *a) {
    int pick;
    int fire;
    int w;

    if (a->nextFire <= D_8021945C) {
        if (a->nextPick > D_8021945C) return;
        w = func_80089200(a);
        if (func_80087DD4(a, w) != 0) {
            pick = (int)func_8009D8A0(15.0f) + 75;
            fire = pick - (int)(pick * (a->skill * 0.25f));
        } else {
            pick = func_8008E620(a, w) * a->skill;
            fire = 0;
        }
        a->nextPick = D_8021945C + pick;
        a->nextFire = D_8021945C + fire;
        if (w == 0) return;
        if (w == a->weapon) {
            a->streak++;
        } else {
            a->weapon = w;
            a->streak = 1;
        }
        a->lastFire = D_8021945C;
        func_8008C5D8(a, w, 0);
    } else {
        if (a->lastFire + func_8008E620(a, a->weapon) < D_8021945C) {
            a->lastFire = D_8021945C;
            func_8008C5D8(a, a->weapon, 0);
        }
    }
}
