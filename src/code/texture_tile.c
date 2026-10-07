#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} GfxCommand;

typedef struct {
    u8 format;
    u8 size;
    u16 width;
    u8 pad_4[6];
    u16 palette;
    void* image;
} TextureInfo;

#define SHIFTL(value, shift, width) \
    ((u32)(((u32)(value) & ((1 << (width)) - 1)) << (shift)))

#define SET_TEXTURE_IMAGE(packet, format, size, width, image) { \
    GfxCommand* command = (GfxCommand*)(packet); \
    command->w0 = SHIFTL(0xFD, 24, 8) | SHIFTL(format, 21, 3) | \
                  SHIFTL(size, 19, 2) | SHIFTL((width) - 1, 0, 12); \
    command->w1 = (u32)(image); \
}

#define SET_TILE(packet, format, size, line, tmem, tile, palette, cmt, maskt, \
                 shiftt, cms, masks, shifts) { \
    GfxCommand* command = (GfxCommand*)(packet); \
    command->w0 = SHIFTL(0xF5, 24, 8) | SHIFTL(format, 21, 3) | \
                  SHIFTL(size, 19, 2) | SHIFTL(line, 9, 9) | \
                  SHIFTL(tmem, 0, 9); \
    command->w1 = SHIFTL(tile, 24, 3) | SHIFTL(palette, 20, 4) | \
                  SHIFTL(cmt, 18, 2) | SHIFTL(maskt, 14, 4) | \
                  SHIFTL(shiftt, 10, 4) | SHIFTL(cms, 8, 2) | \
                  SHIFTL(masks, 4, 4) | SHIFTL(shifts, 0, 4); \
}

#define NO_PARAM(packet, opcode) { \
    GfxCommand* command = (GfxCommand*)(packet); \
    command->w0 = SHIFTL(opcode, 24, 8); \
    command->w1 = 0; \
}

#define TILE_RECT(packet, opcode, tile, uls, ult, lrs, lrt) { \
    GfxCommand* command = (GfxCommand*)(packet); \
    command->w0 = SHIFTL(opcode, 24, 8) | SHIFTL(uls, 12, 12) | \
                  SHIFTL(ult, 0, 12); \
    command->w1 = SHIFTL(tile, 24, 3) | SHIFTL(lrs, 12, 12) | \
                  SHIFTL(lrt, 0, 12); \
}

#define LOAD_TILE(commands, texture, image_size, load_size, line_bytes, uls, \
                  ult, lrs, lrt) { \
    SET_TEXTURE_IMAGE(commands++, texture->format, load_size, \
                      texture->width, texture->image); \
    SET_TILE(commands++, texture->format, load_size, \
             (((lrs) - (uls) + 1) * line_bytes + 7) >> 3, 0, 7, 0, \
             2, 0, 0, 2, 0, 0); \
    NO_PARAM(commands++, 0xE6); \
    TILE_RECT(commands++, 0xF4, 7, (uls) << 2, (ult) << 2, \
              (lrs) << 2, (lrt) << 2); \
    NO_PARAM(commands++, 0xE7); \
    SET_TILE(commands++, texture->format, image_size, \
             (((lrs) - (uls) + 1) * line_bytes + 7) >> 3, 0, 0, \
             texture->palette, 2, 0, 0, 2, 0, 0); \
    TILE_RECT(commands++, 0xF2, 0, (uls) << 2, (ult) << 2, \
              (lrs) << 2, (lrt) << 2); \
}

void func_8007BEF0(GfxCommand** command_pointer, TextureInfo* texture,
                   u16 uls, u16 ult, u16 lrs, u16 lrt) {
    GfxCommand* commands = *command_pointer;

    switch (texture->size) {
        case 0:
            SET_TEXTURE_IMAGE(commands++, texture->format, 1,
                              texture->width >> 1, texture->image);
            SET_TILE(commands++, texture->format, 1,
                     (((lrs - uls + 1) >> 1) + 7) >> 3, 0, 7, 0,
                     2, 0, 0, 2, 0, 0);
            NO_PARAM(commands++, 0xE6);
            TILE_RECT(commands++, 0xF4, 7, uls << 1, ult << 2,
                      lrs << 1, lrt << 2);
            NO_PARAM(commands++, 0xE7);
            SET_TILE(commands++, texture->format, 0,
                     (((lrs - uls + 1) >> 1) + 7) >> 3, 0, 0,
                     texture->palette, 2, 0, 0, 2, 0, 0);
            TILE_RECT(commands++, 0xF2, 0, uls << 2, ult << 2,
                      lrs << 2, lrt << 2);
            break;
        case 1:
            LOAD_TILE(commands, texture, 1, 1, 1, uls, ult, lrs, lrt);
            break;
        case 2:
            LOAD_TILE(commands, texture, 2, 2, 2, uls, ult, lrs, lrt);
            break;
        case 3:
            LOAD_TILE(commands, texture, 3, 3, 2, uls, ult, lrs, lrt);
            break;
    }
    *command_pointer = commands;
}
