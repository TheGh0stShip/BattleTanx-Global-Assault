#include "types.h"

typedef struct Func800BFE4CInner {
    u8 pad0[4];
    f32 value;
} Func800BFE4CInner;

typedef struct Func800BFE4COuter {
    u8 pad0[4];
    Func800BFE4CInner *inner;
} Func800BFE4COuter;

typedef struct Func800BFE4CArg {
    u8 pad0[0xC];
    Func800BFE4COuter *outer;
} Func800BFE4CArg;


s32 func_800BFE4C(s32 unused, Func800BFE4CArg *arg) {
    Func800BFE4CInner *inner = arg->outer->inner;

    if (inner->value < 5.0f) {
        inner->value += 1.0f;
        return 0;
    }
    inner->value = 0.0f;
    return 1;
}
