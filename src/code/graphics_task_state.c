#include "types.h"

/* Graphics task state (graphics_task_init.c, graphics_frame_build.c, func_80079FF0.c).
 * Pointers are N64 address tokens (u32); all start null. */
u32 gGfxTaskName = 0;          /* D_801144F0: char * */
u8 gGfxClearRed = 0x64;        /* D_801144F4 */
u8 gGfxClearGreen = 0x7D;      /* D_801144F5 */
u8 gGfxClearBlue = 0x96;       /* D_801144F6 */
u8 gGfxClearAlpha = 0x80;      /* D_801144F7 */
u32 gGfxState = 0;             /* D_801144F8: GfxState * */
