#include "types.h"

typedef struct RenderCommand {
    u32 word0;
    u32 word1;
} RenderCommand;

typedef struct RendererGlobalTables {
    u8 primary_state_remap[8];
    u8 secondary_state_remap[8];
    u32 pool_pointer_token;
    u32 reserved14[3];
    f32 epsilon;
    u32 reserved24;
    f32 scales[12];
    u32 reserved58[3];
    u8 mode_lifetimes[20];
    RenderCommand combine_templates[2];
    u32 reserved88[2];
} RendererGlobalTables;

/* Mutable renderer globals and their initialized lookup tables. */
RendererGlobalTables gRendererGlobalTables = {
    { 0, 3, 4, 2, 1, 0, 0, 0 },
    { 0, 4, 3, 1, 2, 0, 0, 0 },
    0,
    { 0, 0, 0 },
    0.0000599999985f,
    0,
    { 1.0f, 0.6f, 1.5f, 1.0f, 0.6f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 0, 0, 0 },
    { 0x00, 0x00, 0x1E, 0x1E, 0x5A, 0x1E, 0x0A, 0x1E, 0x1E, 0x1E,
      0x1E, 0x1E, 0x02, 0x1E, 0x1E, 0x0A, 0x1E, 0x1E, 0x00, 0x00 },
    {
        { 0xFC127FFF, 0xFFFFF238 },
        { 0xFC121BFF, 0xFFFFFE38 },
    },
    { 0, 0 },
};
