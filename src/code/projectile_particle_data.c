#include "types.h"

typedef struct ProjectileParticleData {
    s32 projectile_model_ids[3];
    u32 projectile_speed_bits[3];
    u32 projectile_vertical_speed_bits[3];
    u32 effect_definition_tokens[3];
    u16 shell_trace_heights[4];
    u32 particle_scale_bits[3];
    s32 particle_lifetimes[3];
    u32 empty_display_list[2];
    u32 impact_position_bits[3];
    u32 emitter_pool_state[3];
    u32 interpolation_curve_bits[16];
    u32 texture_rotation_scratch;
    u32 texture_coordinate_bits[8];
    u32 alignment_pad;
} ProjectileParticleData;

/*
 * Float values remain as bit patterns until all users agree on field names.
 * Address fields are 32-bit N64 tokens, never native host pointers.
 */
ProjectileParticleData gProjectileParticleData = {
    { 0xA0, 0xA1, 0xAB },
    { 0x41900000, 0x41F00000, 0x41F00000 },
    { 0x41600000, 0x41200000, 0x41600000 },
    { 0x801150A4, 0x80115444, 0x00000000 },
    { 5, 30, 20, 0 },
    { 0x40800000, 0x41500000, 0x41200000 },
    { 200, 240, 300 },
    { 0xDF000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000 },
    {
        0x3DC5E0B5, 0x3DC5E0B5, 0x3DF75643, 0x3E37079E,
        0x3E6D71F3, 0x3EA0C5EB, 0x3ED23E18, 0x3F0809D5,
        0x3F20C5EB, 0x3F384578, 0x3F51018E, 0x3F64CAD5,
        0x3F761B09, 0x3F7D86EC, 0x3F800000, 0x3F800000,
    },
    0x00000000,
    {
        0xBED1EB85, 0xBED1EB85, 0x3ED1EB85, 0xBED1EB85,
        0xBED1EB85, 0x3ED1EB85, 0x3ED1EB85, 0x3ED1EB85,
    },
    0x00000000,
};
