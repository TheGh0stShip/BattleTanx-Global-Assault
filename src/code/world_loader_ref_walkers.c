#include "types.h"

typedef struct WorldBundleSections {
    u8 *groups_header;
    u8 *groups;
    u8 *placements;
    s32 placement_count;
    u8 *object_defs;
    u8 *models;
    u32 model_count;
    s32 model_base;
    u8 *parts;
    u32 part_count;
    s32 part_base;
    u8 *pool_refs;
    s32 pool_ref_count;
    s32 pool_ref_base;
} WorldBundleSections;

typedef struct WorldModel {
    u8 part_count;
    u8 pad1;
    u16 first_part;
    s16 min_x;
    s16 min_y;
    s16 max_x;
    s16 max_y;
    s16 min_z;
    s16 max_z;
} WorldModel;

typedef struct WorldPart {
    u8 reference_count;
    u8 pad1;
    u16 first_reference;
} WorldPart;

extern s32 func_800B9FD4(void *, WorldBundleSections *, s32);

s32 func_800BA1BC(void *state, WorldBundleSections *bundle, s32 part_index) {
    u8 stack_pad[8];
    WorldPart *part;
    s32 i;
    s32 count;

    part = (WorldPart *)bundle->parts + part_index;
    i = 0;
    count = part->reference_count;
    if (count > 0) {
        do {
            if (func_800B9FD4(state, bundle, part->first_reference + i) < 0) {
                return -1;
            }
            i++;
        } while (i < part->reference_count);
    }
    return 0;
}

s32 func_800BA24C(void *state, WorldBundleSections *bundle, s32 model_index) {
    u8 stack_pad[0x10];
    WorldModel *model;
    WorldPart *part;
    s32 i;
    s32 j;
    s32 result;
    s32 count;

    model = (WorldModel *)bundle->models + model_index;
    i = 0;
    count = model->part_count;
    if (count > 0) {
        do {
            part = (WorldPart *)bundle->parts + (model->first_part + i);
            j = 0;
            count = part->reference_count;
            if (count > 0) {
                do {
                    if (func_800B9FD4(state, bundle,
                            part->first_reference + j) < 0) {
                        result = -1;
                        goto part_done;
                    }
                    j++;
                } while (j < part->reference_count);
            }
            result = 0;
part_done:
            i++;
            if (result < 0) {
                return -1;
            }
        } while (i < model->part_count);
    }
    return 0;
}
