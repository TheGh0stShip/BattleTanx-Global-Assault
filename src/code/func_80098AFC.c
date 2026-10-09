#include "types.h"

extern u8 D_80217030[];
extern s32 osSendMesg(void *queue, void *message, s32 flags);

void func_80098AFC(void) {
    void *message = 0;

    osSendMesg(D_80217030, &message, 0);
}
