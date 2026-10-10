#include "types.h"

typedef struct CopyChunk {
    u32 words[4];
} CopyChunk;

typedef struct CopyTail {
    u32 words[2];
} CopyTail;

typedef union TemplateRecord {
    struct {
        CopyChunk chunks[32];
        u32 tail[2];
    } copy;
    struct {
        u8 pad00[0x1F0];
        u16 kind;
        u8 pad1F2[2];
        u16 valid;
        u8 pad1F6[6];
        s32 pointer;
        s32 tail;
    } fields;
} TemplateRecord;

extern TemplateRecord D_80222930;

void func_800A1384(void) {
    CopyChunk *base = D_80222930.copy.chunks;
    CopyChunk *end = base + 32;
    TemplateRecord record;
    CopyChunk *source;
    CopyChunk *dest;

copy_record:
    dest = record.copy.chunks;
reset_source:
    source = base;
copy_chunks:
    do {
        *dest++ = *source++;
    } while (source != end);
    *(CopyTail *)dest = *(CopyTail *)source;

    dest = record.copy.chunks;
    if (record.fields.valid != 0) {
        goto reset_source;
    }
    if (record.fields.pointer != 0) {
        goto reset_source;
    }
    if (record.fields.kind < 5) {
        source = base;
        goto copy_chunks;
    }
}
