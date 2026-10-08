typedef struct {
    float x;
    float y;
    float z;
    unsigned short unkC;
    unsigned short flags;
} SeqSlot;

typedef struct {
    unsigned char *ptr;
    SeqSlot *obj;
    float timer;
    unsigned short unkC;
} Seq;

extern float D_80219488;
extern unsigned char D_803A6A04;
extern unsigned short D_803A6A06;
extern unsigned char D_803A6A00;
extern unsigned short D_803A6A02;
extern unsigned char D_803A69E2;
extern float D_803A69E8, D_803A69EC, D_803A69F4;
extern SeqSlot D_803A6FD8[];
extern unsigned char D_80121CE5;
extern unsigned char D_803A701C;
extern char D_80115444[], D_801150A4[], D_8011551C[], D_80115868[], D_801159F4[];
extern char D_80115834[], D_80114EB0[], D_80114EC8[], D_80114EE0[], D_80114EF8[];
unsigned short func_800D12B0(unsigned char *, unsigned short);
void func_80097D14(unsigned char, unsigned char);
int func_800EC1F8(SeqSlot *, int, int, int, int, int);
void func_800A2C6C(int, SeqSlot *, int, int, int, int);
void func_800A5BD8(SeqSlot *, int, int, float, char *, int);

void func_800D31DC(Seq *p) {
    unsigned char *s;
    SeqSlot *o;
    unsigned short i;
    int h;
    char *anim;

    p->timer -= D_80219488;
    o = p->obj;
    if (p->timer <= 0.0f) {
        s = p->ptr;
        while (p->timer <= 0.0f) {
            s += func_800D12B0(s, p->unkC);
            switch (D_803A6A04) {
            case 1:
            case 2:
                p->timer += D_803A6A06;
                break;
            case 8:
                p->unkC = D_803A6A00;
                for (i = 0; i < 4; i++) {
                    if (!(D_803A6FD8[i].flags & 0x8000)) {
                        break;
                    }
                }
                if (i >= 4) {
                    o = 0;
                } else {
                    D_803A6FD8[i].flags = 0x8000;
                    o = &D_803A6FD8[i];
                }
                o->x = 0;
                o->y = 0;
                o->z = 0;
                p->obj = o;
                break;
            case 3:
                o->x += D_803A69E8;
                o->y += D_803A69F4;
                o->z += D_803A69EC;
                if (D_803A69E2) {
                    o->unkC = D_803A6A02;
                }
                break;
            case 16:
                h = 0;
                switch (D_803A6A00) {
                case 0:
                    h = func_800EC1F8(o, 0, 0, 100, 2, 0);
                    anim = D_80115444;
                    break;
                case 1:
                    func_800A2C6C(0, o, 172, 50, 0, 0xE49D0A);
                    anim = D_801150A4;
                    break;
                case 2:
                    anim = D_8011551C;
                    break;
                case 3:
                    anim = D_80115868;
                    break;
                case 4:
                    anim = D_801159F4;
                    break;
                case 5:
                    anim = D_80115834;
                    break;
                case 7:
                    anim = D_80114EB0;
                    break;
                case 8:
                    anim = D_80114EC8;
                    break;
                case 9:
                    anim = D_80114EE0;
                    break;
                case 10:
                    anim = D_80114EF8;
                    break;
                default:
                    anim = 0;
                    break;
                }
                if (anim) {
                    func_800A5BD8(o, 0, 0, 1.0f, anim, h);
                }
                break;
            case 47:
                if (D_80121CE5 == 0) {
                    D_803A701C = 1;
                    D_80121CE5 = 1;
                } else {
                    D_803A701C = 20;
                }
                func_80097D14(D_803A6A00, D_803A701C);
                break;
            case 0:
                o->flags &= 0x7FFF;
                p->ptr = 0;
                return;
            }
        }
        p->ptr = s;
    }
}
