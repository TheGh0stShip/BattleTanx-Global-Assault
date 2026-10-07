#include "types.h"

typedef struct OSMesgQueue OSMesgQueue;

extern OSMesgQueue D_8021C0A0;
extern void* D_8021C080[];
extern void osCreateMesgQueue(OSMesgQueue* queue, void* messages, s32 count);
extern void func_8009D3A4(void);

void func_8009EEA0(void) {
    osCreateMesgQueue(&D_8021C0A0, D_8021C080, 8);
    func_8009D3A4();
}
