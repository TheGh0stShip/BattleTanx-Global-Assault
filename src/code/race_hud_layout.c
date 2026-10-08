#include "types.h"

typedef struct {
    u16 pad0;
    u16 width;
    u16 height;
} HudImage;

typedef struct {
    u8 pad0[2];
    s16 x;
    s16 y;
} HudPos;

typedef struct {
    u8 tag;
    u8 pad1;
    s16 x;
    u8 pad4[4];
    void* target;
    u8 padC[4];
} HudLink;

typedef struct {
    u8 pad0[0x14];
    HudImage* image;
} HudSprite;

typedef struct {
    HudPos* pos;
} HudPosRef;

typedef struct {
    HudLink* links;
} HudLinkRef;

extern s8 D_80117EB0;
extern HudSprite D_803977F0;
extern HudPosRef D_8011DDB0;
extern HudLinkRef D_8011DFE8;
extern u8 D_8011DE38[];
extern s16 D_8011F1F4;
extern u8 D_8011DDAC[];
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);

void func_800C8350(void) {
    HudSprite* sprite = &D_803977F0;
    HudLink* link;

    if (D_80117EB0 == 1) {
        D_8011DDB0.pos->x = 30;
        D_8011DDB0.pos->y = 0xD0 - D_803977F0.image->height;
        for (link = D_8011DFE8.links; link->tag != 0; link++) {
            if (link->target == D_8011DE38) {
                link->x = sprite->image->width + 25;
                break;
            }
        }
    } else {
        if (D_80117EB0 == 2) {
            D_8011DDB0.pos->x = 30;
        } else {
            D_8011DDB0.pos->x = 0xA0 - (D_803977F0.image->width >> 1);
        }
        D_8011DDB0.pos->y = 0x78 - (D_803977F0.image->height >> 1);
    }
    D_8011F1F4 = 0;
    func_800BEBA8(D_8011DDAC, 0, 1);
}
