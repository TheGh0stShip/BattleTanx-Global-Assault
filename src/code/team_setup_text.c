#include "types.h"

typedef struct TeamSetupText {
    char human[8];
    char computer[12];
    char normal[8];
    char advanced[12];
    char novice[8];
    char off[4];
    char team_blue[12];
    char team_red[12];
    char team_green[12];
    char team_yellow[12];
    char attacker[12];
    char protector[12];
} TeamSetupText;

TeamSetupText gTeamSetupText = {
    "HUMAN", "COMPUTER", "NORMAL", "ADVANCED", "NOVICE", "OFF",
    "TEAM BLUE", "TEAM RED", "TEAM GREEN", "TEAM YELLOW", "ATTACKER",
    "PROTECTOR",
};
