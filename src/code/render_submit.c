#include "types.h"

typedef struct RenderSubmitItem {
    void* display_list;
    void* texture_1;
    void* texture_2;
    void* matrix;
    s16 pad_10;
    s16 palette;
    struct RenderSubmitItem* next;
} RenderSubmitItem;

typedef struct RenderSubmitNode {
    void* display_list;
    RenderSubmitItem* items[5];
    struct RenderSubmitNode* next;
} RenderSubmitNode;

typedef struct {
    u32 w0;
    u32 w1;
} GfxCommand;

extern u8 D_802194A5;
extern RenderSubmitNode* D_801777E0[][32];
extern u32* func_8007AD94(void);
extern u32* func_8007ADB0(void);
extern void func_8007ACF8(void*);
extern void func_8007AB64(u8*, u16);
extern void func_8007AC34(void*, s32);
extern void func_8007AD40(s32, GfxCommand*);

s32 func_8007B65C(s32 table_index, u8* contexts, void** display_lists,
                  u16* modes) {
    GfxCommand command;
    s32 last_list = -1;
    void* current_matrix = 0;
    s32 list_count;
    s32 bucket;
    u32* allocator;
    u32 limit;
    s32 list_index;
    RenderSubmitItem* item;
    void* texture_1;
    void* texture_2;
    s32 palette;
    RenderSubmitNode* node;
    u32* limits;

    limits = func_8007AD94();
    allocator = func_8007ADB0();
    texture_1 = 0;
    texture_2 = 0;
    list_count = D_802194A5;
    limit = limits[1] + 0xADE0;
    for (bucket = 0; bucket < 32; bucket++) {
        for (node = D_801777E0[table_index][bucket]; node != 0;
             node = node->next) {
            if (node->display_list != 0) {
                texture_1 = 0;
                func_8007ACF8(node->display_list);
            }
            palette = 0;
            for (list_index = 0; list_index < list_count; list_index++) {
                for (item = node->items[list_index]; item != 0;
                     item = item->next) {
                    if (limit < *allocator) {
                        return -1;
                    }
                    if (list_index != last_list) {
                        func_8007ACF8(display_lists[list_index]);
                        last_list = list_index;
                        func_8007AB64(contexts + list_index * 0x44,
                                      modes[list_index]);
                    }
                    if (item->matrix != current_matrix) {
                        current_matrix = item->matrix;
                        func_8007AC34(current_matrix, 2);
                    }
                    if (item->texture_1 != texture_1) {
                        texture_1 = item->texture_1;
                        if (texture_1 != 0) {
                            func_8007ACF8(texture_1);
                            palette = 0;
                        }
                    }
                    if (item->texture_2 != texture_2) {
                        texture_2 = item->texture_2;
                        if (texture_2 != 0) {
                            func_8007ACF8(texture_2);
                        }
                    }
                    if (item->palette != palette) {
                        palette = item->palette;
                        command = ((GfxCommand*)item->texture_1)[1];
                        command.w1 |= (palette & 0xF) << 20;
                        func_8007AD40(1, &command);
                    }
                    func_8007ACF8(item->display_list);
                }
            }
        }
    }
    return 0;
}
