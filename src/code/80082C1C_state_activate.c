#include "types.h"

typedef struct StateOwner82C1C {
    u8 pad00[0x10];
    s32 value;
} StateOwner82C1C;

typedef struct StateObject82C1C {
    u8 pad00[0xAC];
    s32 query_arg;
    u8 padB0[0x158 - 0xB0];
    u32 state;
    s32 timer;
    u8 pad160[0x1D0 - 0x160];
    StateOwner82C1C *owner;
} StateObject82C1C;

typedef struct StateQuery82C1C {
    u32 count;
    StateObject82C1C *objects[25];
} StateQuery82C1C;

extern s32 D_8021945C;
extern void func_80089E84(StateQuery82C1C *, s32, s32, s32, s32);

s32 func_80082C1C(StateObject82C1C *arg) {
    StateQuery82C1C query;
    u16 i;
    s32 found;
    s32 owner_value;
    StateObject82C1C *object;
    s32 activated;

    object = arg;
    activated = 0;
    if (object->state == 2) {
        goto done;
    }
    if (object->state >= 3) {
        goto done;
    }
    if (object->state != 1) {
        goto done;
    }

    owner_value = object->owner->value;
    found = 0;
    func_80089E84(&query, object->query_arg, owner_value, 1, found);
    for (i = 0; i < query.count; i++) {
        if (query.objects[i]->state == 2) {
            found = 1;
            break;
        }
    }
    if (!found) {
        object->state = 2;
        activated = 1;
        object->timer = D_8021945C + 120;
    }

done:
    return activated;
}
