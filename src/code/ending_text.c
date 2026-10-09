#include "types.h"

typedef struct EndingText {
    char world_safe[32];
    char done[8];
    char rating[8];
    char secret_level_code[28];
    char password[8];
    char tank_warrior[16];
    char tank_commander[16];
    char battlelord[12];
    char supreme_warlord[16];
    char ultimate_warlord[20];
} EndingText;

EndingText gEndingText = {
    "The World Is Safe... For Now!", "Done", "Rating:",
    "Secret Level Cheat Code:", "WRDRB", "Tank Warrior", "Tank Commander",
    "Battlelord", "Supreme Warlord", "Ultimate Warlord",
};
