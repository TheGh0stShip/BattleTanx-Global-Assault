/* SPAN 0x80097508 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    s16 unk0, unk2;
    u16 w;
} Glyph;

typedef struct {
    u16 h;              /* 0x00 */
    u16 fixedw;         /* 0x02 */
    u16 spacew;         /* 0x04 */
    u16 unk6;
    int unk8;
    Glyph *glyphs;      /* 0x0C */
} Font;

extern Font D_801B4450[];
extern u16 D_80114700;

u16 func_80096928(u8 c);
s16 func_80096A54(void *ctx, u8 c, u16 font, s16 x, s16 y, float sx, float sy);



s16 func_800973E0(u8 *str, u16 font, float scale) {
    int w = 0;
    u8 *s = str;
    Font *f = &D_801B4450[font];
    u16 g;

    while (*s != 0) {
        if (D_80114700) {
            w += f->fixedw;
            s++;
        } else if (*s == ' ') {
            w += (int)(f->spacew * scale);
            s++;
        } else {
            g = func_80096928(*s++);
            if (g != 0xFFFF && f->glyphs[g].w != 0) {
                w += f->glyphs[g].w;
            }
        }
    }
    return (int)((s16)w * scale) + 1;
}
