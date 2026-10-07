#include "types.h"

typedef struct {
    s32 field_0;
    s32 duration;
} TimingData;

typedef struct {
    u8 pad_0[0xC];
    TimingData* timing;
    s32 start_time;
} TimedObject;

extern s32 D_8021945C;

void func_800A4098(TimedObject* object, s32* complete) {
    if ((D_8021945C - object->start_time) >= object->timing->duration) {
        *complete = 1;
    }
}
