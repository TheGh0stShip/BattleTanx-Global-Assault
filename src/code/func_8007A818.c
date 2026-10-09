#include "types.h"

typedef struct RendererContext8007A818 {
    u8 pad00[0xB0];
    s16 work_slot;
    s16 reserved_slot[3];
    s32 slot_stride;
    void *slot_base;
} RendererContext8007A818;

extern RendererContext8007A818 *D_80114500;

/* Exact via the first-search allocation/branch rewrite documented in
 * docs/NORMALIZER_ASSISTED.md; the second search compiles directly. */
static __inline__ s16 find_free_slot(RendererContext8007A818 *context) {
    s16 index;

    for (index = 0; index < 3; index++) {
        if ((index != context->reserved_slot[2]) &&
            (index != context->reserved_slot[1]) &&
            (index != context->reserved_slot[0])) {
            return index;
        }
    }
    return -1;
}

void func_8007A818(void) {
    RendererContext8007A818 *context = D_80114500;
    s16 candidate;

    do {
    } while (context->work_slot != -1);

    do {
        candidate = find_free_slot(context);
    } while (candidate == -1);

    candidate = find_free_slot(D_80114500);
    D_80114500->work_slot = candidate;
}
