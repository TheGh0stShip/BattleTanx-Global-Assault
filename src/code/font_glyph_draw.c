typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u16 u;
    u16 v;
    u16 width;
} Glyph;

typedef struct {
    u16 height;
    u16 fixed_width;
    u16 space_width;
    u16 unk06;
    int texture;
    Glyph *glyphs;
} Font;

extern Font D_801B4450[];
extern u16 D_80114700;

u16 func_80096928(u8 character);
void func_8007C364(
    void **gfx, int texture, s16 x, s16 y, int u0, int v0, int u1, int v1,
    float scale_x, float scale_y
);

s16 func_80096A54(
    void **ctx, u8 character, u16 font_index, s16 x, s16 y,
    float scale_x_arg, float scale_y
) {
    float scale_x = scale_x_arg;
    Font *font = &D_801B4450[font_index];
    u16 glyph_index;
    Glyph *glyph;
    void *gfx;
    u16 width;
    u16 u;
    u16 v;
    u16 u1;
    u16 v1;

    if (character == ' ') {
        if (!D_80114700) {
            width = font->space_width;
        } else {
            width = font->fixed_width;
        }
    } else {
        glyph_index = func_80096928(character);
        if (glyph_index == 0xFFFF || font->glyphs[glyph_index].width == 0) {
            return 0;
        }
        gfx = *ctx;
        glyph = &font->glyphs[glyph_index];
        u = glyph->u;
        u1 = u + glyph->width;
        v = glyph->v;
        v1 = v + font->height;
        func_8007C364(
            &gfx, font->texture, x, y, u, v, u1, v1, scale_x, scale_y
        );
        *ctx = gfx;
        if (!D_80114700) {
            width = font->glyphs[glyph_index].width;
        } else {
            width = font->fixed_width;
        }
    }
    return width * scale_x;
}
