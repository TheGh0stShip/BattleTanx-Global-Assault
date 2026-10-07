#include "types.h"

extern u8* D_80114500;

/* Installs the renderer/framebuffer context used by the helpers in this TU. */
void func_8007A710(u8* context) {
    D_80114500 = context;
}
