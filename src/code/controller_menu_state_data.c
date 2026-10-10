#include "types.h"

/*
 * Controller-menu state (retail 0x8011F230-0x8011F23C, typed from controller_*
 * externs) followed by an unreferenced menu-descriptor tail (0x8011F23C-0x8011F298)
 * with the same shape as the other front-end descriptors: button masks,
 * callback address tokens (0x800CD57C, 0x800CD628, 0x800CCA6C, 0x800CCEC4) and
 * the shared RGBA palette.
 */
s16 gControllerMenuCursor = 0;   /* D_8011F230 */
s16 gControllerMenuPage = 0;     /* D_8011F232 */
u16 gControllerMenuSlot = 0;     /* D_8011F234 */
u16 gControllerMenuPort = 0;     /* D_8011F236 */
u16 gControllerMenuBank = 0;     /* D_8011F238 */
u8 gControllerMenuMode = 0;      /* D_8011F23A */
u8 gControllerMenuBusy = 0;      /* D_8011F23B */

/* 0x8011F23C-0x8011F298: no code consumer found; field meaning inferred from sibling descriptors. */
u32 gControllerMenuDescriptor[23] = {
    0x00000808, 0x00000404, 0x0000F000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x800CD57C, 0x800CD628, 0x00000000, 0x800CCA6C,
    0x800CCEC4, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x7B1802FF, 0xAAAA00FF, 0x4040FFE1,
    0x00FF00FF, 0x9B470EFF, 0xE1E1E1FF,
};
