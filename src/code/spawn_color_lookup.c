#include "types.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Rgb8;

typedef struct {
    u8 pad0[0x14];
    Rgb8 color;
} ColorSource;

extern Rgb8 D_80116580[];
extern Rgb8 D_8011658C[];
extern ColorSource* func_800D69E0(s32 index);

void func_800D0858(u32 index, u8* r, u8* g, u8* b) {
    index &= 0x3F;
    if (index < 12) {
        ColorSource* src = func_800D69E0(index);

        *r = src->color.r;
        *g = src->color.g;
        *b = src->color.b;
    } else if (index < 16) {
        *r = D_80116580[index - 12].r;
        *g = D_80116580[index - 12].g;
        *b = D_80116580[index - 12].b;
    } else if (index < 18) {
        *r = D_8011658C[index - 14].r;
        *g = D_8011658C[index - 14].g;
        *b = D_8011658C[index - 14].b;
    } else {
        *r = 0;
        *g = 0;
        *b = 0;
    }
}
