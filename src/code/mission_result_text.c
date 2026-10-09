#include "types.h"

typedef struct MissionResultText {
    char kills[8];
    char rescues[8];
} MissionResultText;

MissionResultText gMissionResultText = { "KILLS", "RESCUES" };
