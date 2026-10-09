#include "types.h"

typedef struct RenderCommand {
    u32 word0;
    u32 word1;
} RenderCommand;

/* Baseline RDP/RSP state lists selected by the renderer. */
RenderCommand gRenderStateOpaque[7] = {
    { 0xE7000000, 0x00000000 },
    { 0xE3000A01, 0x00000000 },
    { 0xE3001801, 0x000000C0 },
    { 0xE3001A01, 0x00000030 },
    { 0xE200001C, 0x00504240 },
    { 0xFCFFFFFF, 0xFFFDF6FB },
    { 0xDF000000, 0x00000000 },
};

RenderCommand gRenderStateTextured[11] = {
    { 0xE7000000, 0x00000000 },
    { 0xE3000A01, 0x00000000 },
    { 0xE3001801, 0x000000C0 },
    { 0xE3001A01, 0x00000030 },
    { 0xE200001C, 0x00504240 },
    { 0xFCFFFFFF, 0xFFFCF279 },
    { 0xD7000002, 0x10001000 },
    { 0xE3000C00, 0x00000000 },
    { 0xE3000F00, 0x00000000 },
    { 0xE3000D01, 0x00000000 },
    { 0xDF000000, 0x00000000 },
};

RenderCommand gRenderStateTranslucent[12] = {
    { 0xE7000000, 0x00000000 },
    { 0xE3000A01, 0x00000000 },
    { 0xE3001801, 0x000000C0 },
    { 0xE3001A01, 0x00000030 },
    { 0xE200001C, 0x00504240 },
    { 0xFC119623, 0xFF2FFFFF },
    { 0xD7000002, 0x10001000 },
    { 0xE3000C00, 0x00000000 },
    { 0xE3000F00, 0x00000000 },
    { 0xE3000D01, 0x00000000 },
    { 0xE3001001, 0x00000000 },
    { 0xDF000000, 0x00000000 },
};

RenderCommand gRenderStateModulated[11] = {
    { 0xE7000000, 0x00000000 },
    { 0xE3000A01, 0x00000000 },
    { 0xE3001801, 0x000000C0 },
    { 0xE3001A01, 0x00000030 },
    { 0xE200001C, 0x00504240 },
    { 0xFCFF97FF, 0xFF2CFE7F },
    { 0xD7000002, 0x10001000 },
    { 0xE3000C00, 0x00000000 },
    { 0xE3000F00, 0x00000000 },
    { 0xE3000D01, 0x00000000 },
    { 0xDF000000, 0x00000000 },
};
