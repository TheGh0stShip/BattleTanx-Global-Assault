#include "types.h"

/*
 * Race HUD per-player state, zero-initialized .data (retail 0x8011DDF8-0x8011DE9C).
 * Record shape {s32 value; s16 kind;} comes from race_player_message.c
 * (e->p = p; e->n = 17). The first two groups use the same 8-byte stride in
 * race_hud_init / race_player_reset / race_player_set_vehicle_label /
 * race_player_set_layout; their field typing is inferred from that stride.
 */
typedef struct RaceHudSlot {
    s32 value;
    s16 kind;
} RaceHudSlot;

RaceHudSlot gRaceHudLabelSlots[4] = { 0 };   /* D_8011DDF8..D_8011DE10 */
RaceHudSlot gRaceHudLayoutSlots[4] = { 0 };  /* D_8011DE18..D_8011DE30 */
RaceHudSlot gRaceHudMessageSlots[4] = { 0 }; /* D_8011DE38..D_8011DE50: race_player_message */
s32 gRaceHudLabelValues[4] = { 0 };          /* D_8011DE58..D_8011DE64 */
s32 gRaceHudLayoutValues[4] = { 0 };         /* D_8011DE68..D_8011DE74 */
s32 gRaceHudModeIcons[4] = { 0 };            /* D_8011DE78..D_8011DE84: race_player_set_mode_icon */
u32 gRaceHudModeIconNodes[4] = { 0 };        /* D_8011DE88..D_8011DE94: N64 pointer tokens */
s32 gRaceHudDisplayValue = 0;                /* D_8011DE98: race_display_value_set */
