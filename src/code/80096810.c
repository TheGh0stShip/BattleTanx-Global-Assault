/* SPAN 0x80096A54 */
/* RODATA_VRAM 0x800722A0 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u16 u, v;
    u16 w;
} Glyph;

typedef struct {
    u16 h;              /* 0x00 */
    u16 fixedw;         /* 0x02 */
    u16 spacew;         /* 0x04 */
    u16 unk6;
    int tex;            /* 0x08 */
    Glyph *glyphs;      /* 0x0C */
} Font;

typedef struct {
    char pad[0x1E6];
    u16 h;              /* 0x1E6 */
    u16 fixedw;         /* 0x1E8 */
    u16 spacew;         /* 0x1EA */
} FontFile;

extern Font D_801B4450[];
extern u16 D_80114700;

void *func_800ACEB4(int size);
void func_8009ED00(void *src, void *dst, int size);
void func_8007C364(void **gfx, int tex, s16 x, s16 y, int u0, int v0, int u1, int v1, float sx, float sy);
s16 func_80096F48(void *ctx, u8 *text, u16 font, s16 x, s16 y, float sx, float sy);
s16 func_800973E0(u8 *s, u16 font, float scale);



s16 func_80096A54(void **ctx, u8 c, u16 font, s16 x, s16 y, float sx, float sy);

s16 func_80096BDC(void *ctx, int val, u16 font, s16 x, s16 y, float sx, float sy, u16 align);

s16 func_80096E14(void *ctx, int val, u16 font, s16 x, s16 y, float sx, float sy, u16 align);


void func_80096810(u8 *start, u8 *end, int tex, u16 slot) {
    int n = end - start;
    FontFile *data = func_800ACEB4(n);

    func_8009ED00(start, data, n);
    D_801B4450[slot].h = data->h;
    D_801B4450[slot].fixedw = data->fixedw;
    D_801B4450[slot].spacew = data->spacew;
    D_801B4450[slot].glyphs = (Glyph *)data;
    D_801B4450[slot].tex = tex;
    D_801B4450[slot].fixedw = (float)D_801B4450[slot].fixedw * 0.75;
}

u16 func_80096928(u8 c) {
    if ((u8)(c - 'A') < 26) return c - 'A';
    if ((u8)(c - 'a') < 26) return c - 'a' + 26;
    if ((u8)(c - '0') < 10) return c - '0' + 52;
    switch (c) {
    case '.': return 0x3E;
    case ',': return 0x3F;
    case '!': return 0x40;
    case '?': return 0x41;
    case '+': return 0x42;
    case '-': return 0x43;
    case '*': return 0x44;
    case '/': return 0x45;
    case '=': return 0x46;
    case '"': return 0x47;
    case 0x27: return 0x48;
    case '@': return 0x49;
    case '%': return 0x4A;
    case '(': return 0x4B;
    case ')': return 0x4C;
    case '^': return 0x4D;
    case '~': return 0x4E;
    case '#': return 0x4F;
    case ':': return 0x50;
    }
    return 0xFFFF;
}
