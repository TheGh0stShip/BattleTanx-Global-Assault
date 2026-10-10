#include "types.h"

s32 gFrameBufferIndex = 0;  /* D_80114860: double-buffer index, (x + 1) & 1 in func_80099854 */
u16 gFrameStateSelect = 0;  /* D_80114864: state selector, sh/lhu in 8009C31C..8009D144 */
s32 gFrameStateTimer = 0;   /* D_80114868: counter compared against 20 in 80099FE8 */
/* 0x8011486C-0x80114870: KMC as pads this unit's .data to 16 bytes. */
