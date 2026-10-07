#include "types.h"

typedef struct {
    u8 type;
    u8 flag;
    u8 pad[10];
    u8* data;
    u8* base;
    u8* rom_start;
    u8* rom_end;
} AssetDescriptor;

extern s32 D_80224B40;
extern u8* func_800ACEB4(s32);
extern void func_8009ED00(void*, void*, s32);
extern s32* func_8007B020(void);
extern void func_800A0750(s32, void*, s32, void*);

void func_8007BCF0(AssetDescriptor* asset, u16 compressed) {
    s32 size;
    u8* buffer;
    s32* temporary;

    size = asset->rom_end - asset->rom_start;
    size += size & 1;
    if (compressed == 0) {
        buffer = func_800ACEB4(size);
        func_8009ED00(asset->rom_start, buffer, size);
    } else {
        temporary = func_8007B020();
        func_8009ED00(asset->rom_start, temporary, size);
        size = *temporary;
        buffer = func_800ACEB4(size);
        func_800A0750(asset->rom_end - asset->rom_start, temporary, size,
                      buffer);
    }
    if (asset->type == 2) {
        asset->base = buffer;
        if (asset->flag == 0) {
            asset->data = buffer + 0x20;
        } else {
            asset->data = buffer + 0x200;
        }
    } else {
        asset->data = buffer;
    }
    if (D_80224B40 != 0) {
        while (1) {
        }
    }
}
