#include "types.h"

/* D_80114700: set to 1 by func_800BBDC0 when the HUD scale record has a
 * negative flip (mirrored draw), else 0; also cleared by later HUD paths. */
u16 gHudFlipActive = 0;
/* 0x80114702-0x80114710: KMC as pads this unit's .data to 16 bytes. */
