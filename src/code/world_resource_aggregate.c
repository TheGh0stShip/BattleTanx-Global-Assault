#include "types.h"

typedef struct WorldBundleSections {
    u8 *groups_header;
    u8 *groups;
    u8 *placements;
    s32 placement_count;
    u8 *object_defs;
    u8 *models;
    s32 model_count;
    s32 model_base;
    u8 *parts;
    s32 part_count;
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
    s32 group_object_bytes[12];
    s32 object_bytes;
    s32 model_count;
    s32 part_count;
    s32 pool_ref_count;
} WorldBuildState;

extern s32 func_800DF3B8(u8 *, WorldBuildState *, s32);
extern s32 func_800B9FD4(WorldBuildState *, WorldBundleSections *, s32);

s32 func_800BA330(WorldBuildState *state) {
    s32 bundle_index;
    s32 placement_index;
    s32 group_index;
    s32 group_count;
    s32 part_index;
    s32 ref_index;
    s32 result;
    s32 ref_status;
    s32 model_result;
    s32 object_size;
    WorldBundleSections *bundle;
    u8 *group;
    u8 *model;
    u8 *part;

    for (bundle_index = 0; bundle_index < state->bundle_count; bundle_index++) {
        bundle = &state->bundles[bundle_index];
        group_count = *(s32 *)bundle->groups_header;
        if (group_count > state->max_groups) {
            state->max_groups = group_count;
        }
        for (group_index = 0; group_index < group_count; group_index++) {
            group = bundle->groups + group_index * 0x10;
            for (placement_index = *(u16 *)(group + 2);
                 placement_index < *(u16 *)(group + 2) + *(u16 *)group;
                 placement_index++) {
                object_size = func_800DF3B8(
                    bundle->object_defs +
                        *(s32 *)(bundle->placements + placement_index * 0xC + 8),
                    state, bundle_index);
                state->group_object_bytes[group_index] += object_size;
                state->object_bytes += object_size;
            }
        }
        for (group_index = 0; group_index < bundle->model_count; group_index++) {
            model = bundle->models + group_index * 0x10;
            part_index = 0;
            while (part_index < *(u8 *)model) {
                model_result = -1;
                part = bundle->parts + (*(u16 *)(model + 2) + part_index) * 4;
                for (ref_index = 0; ref_index < *(u8 *)part; ref_index++) {
                    if (func_800B9FD4(state, bundle,
                            *(u16 *)(part + 2) + ref_index) < 0) {
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
                part_index++;
            }
            model_result = 0;
model_done:
            if (model_result < 0) {
                return -1;
            }
        }
        state->model_count += bundle->model_count;
        state->part_count += bundle->part_count;
        state->pool_ref_count += bundle->pool_ref_count;
    }

    result = 0;
    for (group_index = 0; group_index < state->material_count; group_index++) {
        state->materials[group_index].offset = result;
        result += state->materials[group_index].size;
        result = (result + 7) & ~7;
    }
    state->material_pool_size = result;

    result = 0;
    for (group_index = 0; group_index < state->vertex_count; group_index++) {
        state->vertices[group_index].offset = result;
        result += state->vertices[group_index].size;
        result = (result + 7) & ~7;
    }
    state->vertex_pool_size = result;

    result = 0;
    for (group_index = 0; group_index < state->texture_count; group_index++) {
        state->textures[group_index].offset = result;
        result += state->textures[group_index].size;
        result = (result + 7) & ~7;
    }
    state->texture_pool_size = result;
    return 0;
}
