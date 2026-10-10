/* RODATA_VRAM 0x80072448 */
typedef unsigned char u8;
typedef short s16;

typedef struct {
    float x, y;
    char p8[0x1D];
    u8 type;
    char p26[2];
} Listener;

extern u8 D_802194A5;
extern Listener D_802194B4[];
extern u8 D_801147E0;
extern char D_801FE598[];
extern char D_801F3198[];

void func_800FBB70(void *bank);
void func_800FBD94(void *bank);
int func_800FB570(int snd, int vol, int pan, int a3, int a4);
void func_8009813C(int h);

int func_80097FB4(int snd, float x, float y, float vol, u8 type) {
    float s = 0.0f;
    int none = -1;
    float dx, dy, inf;
    int i;
    int h;
    int v;
    u8 n;

    n = D_802194A5;
    for (i = 0; i < n; i++) {
        if (D_802194B4[i].type == type) {
            dx = x - D_802194B4[i].x;
            dy = y - D_802194B4[i].y;
            inf = 1.0 / ((dx * dx + dy * dy) * (0.2 / 1000000.0) + 1.0);
            if (s < inf) {
                s = inf;
            }
        }
        if (s > 0.2) {
            v = (s16)(vol * s * 255.0f);
            D_801147E0++;
            func_800FBB70(D_801FE598);
            func_800FBD94(D_801F3198);
            h = func_800FB570(snd, v, 128, 0, none);
            if (h != 0) {
                func_8009813C(h);
            }
            return h;
        }
    }
    return -1;
}
