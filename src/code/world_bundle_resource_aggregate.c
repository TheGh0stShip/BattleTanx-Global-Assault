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

typedef struct SizeEntry {
    s32 id;
    s32 size;
    s32 offset;
} SizeEntry;

typedef struct WorldBuildState {
    s32 bundle_count;
    WorldBundleSections *bundles;
    s32 texture_count;
    SizeEntry textures[0x100];
    s32 texture_pool_size;
    s32 material_count;
    SizeEntry materials[0x100];
    s32 material_pool_size;
    s32 vertex_count;
    SizeEntry vertices[0xAEE];
    u8 pad_9B44[0x18];
    s32 vertex_pool_size;
    s32 max_groups;
    s32 object_bytes;
    u8 pad_9B68[0x2C];
    s32 object_count;
    s32 model_count;
    s32 part_count;
    s32 pool_ref_count;
    u8 pad_9BA4[8];
} WorldBuildState;

extern void *D_803A53A0[0x107];
extern s32 func_800B9FD4(WorldBuildState *, WorldBundleSections *, s32);

s32 func_800BB2A0(WorldBuildState *state) {
    WorldBundleSections *bundle;
    void **slot;
    u16 *model_index;
    s32 i;
    s32 j;
    s32 k;
    s32 result;
    s32 ref_status;
    s32 model_status;
    u8 *model;
    u8 *part;

    bundle = state->bundles;
    model_index = (u16 *)(bundle->object_defs + *(s32 *)(bundle->placements + 8) + 2);
    for (i = 0; i < 0x107; model_index++, i++) {
        slot = &D_803A53A0[i];
        if (*slot != 0) {
            continue;
        }
        if (*model_index == 0xFFFF) {
            continue;
        }

        model = bundle->models + *model_index * 0x10;
        j = 0;
        while (j < *(u8 *)model) {
            model_status = -1;
            part = bundle->parts + (*(u16 *)(model + 2) + j) * 4;
            for (k = 0; k < *(u8 *)part; k++) {
                if (func_800B9FD4(state, bundle, *(u16 *)(part + 2) + k) < 0) {
                    goto ref_failed;
                }
            }
            ref_status = 0;
check_result:
            if (ref_status >= 0) {
                goto ref_success;
            }
            goto model_done;
ref_failed:
            ref_status = -1;
            goto check_result;
ref_success:
            j++;
        }
        model_status = 0;
model_done:
        if (model_status < 0) {
            return -1;
        }
    }

    state->model_count += bundle->model_count;
    state->part_count += bundle->part_count;
    state->pool_ref_count += bundle->pool_ref_count;

    result = 0;
    for (i = 0; i < state->material_count; i++) {
        state->materials[i].offset = result;
        result += state->materials[i].size;
        result = (result + 7) & ~7;
    }
    state->material_pool_size = result;

    result = 0;
    for (i = 0; i < state->vertex_count; i++) {
        state->vertices[i].offset = result;
        result += state->vertices[i].size;
        result = (result + 7) & ~7;
    }
    state->vertex_pool_size = result;

    result = 0;
    for (i = 0; i < state->texture_count; i++) {
        state->textures[i].offset = result;
        result += state->textures[i].size;
        result = (result + 7) & ~7;
    }
    state->texture_pool_size = result;
    return 0;
}
