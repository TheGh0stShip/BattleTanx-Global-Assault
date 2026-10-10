#include "types.h"

/* Race state scalars (retail 0x8011F1F4-0x8011F230), typed from their extern uses. */
u16 gRaceEventCount = 0;          /* D_8011F1F4: race_event_push, race_hud_layout, race_map_draw */
u16 gRaceStateUnknownF1F6 = 0;    /* D_8011F1F6: no consumer found */
s16 gRacePlayerTimes[9] = { 0 };  /* D_8011F1F8..D_8011F208: race_state_*, race_object_timestamps (D_8011F206 is linked from race_widget_layout) */
/* D_8011F20A..D_8011F20D: race_flag_queries, race_player_widget. Separate
 * scalars: KMC GCC word-aligns char arrays, which would move them to 0x8011F20C. */
u8 gRacePlayerFlag0 = 1;
u8 gRacePlayerFlag1 = 1;
u8 gRacePlayerFlag2 = 1;
u8 gRacePlayerFlag3 = 1;
u8 gRaceSlotState[4] = { 0 };     /* D_8011F210: race_slot_clear, race_player_icons_draw, race_timer_update */
f32 gRaceTimer = 0.0f;            /* D_8011F214: race_map_draw, race_timer_update */
u16 gRaceMode = 0;                /* D_8011F218: race_assets_load and many code asm users */
u16 gRaceModeAux = 0;             /* D_8011F21A: race_assets_load, race_event_push */
u8 gRacePlayerColors[6][3] = {    /* D_8011F21C: race_map_draw, race_timer_update */
    { 0xC8, 0xC8, 0xC8 },
    { 0x00, 0x00, 0xC8 },
    { 0x64, 0x64, 0xC8 },
    { 0x00, 0xC8, 0x00 },
    { 0xC8, 0xC8, 0x00 },
    { 0xC8, 0x00, 0x00 },
};
s32 gRaceStateUnknownF22C = 0;    /* D_8011F22C: no consumer found */
