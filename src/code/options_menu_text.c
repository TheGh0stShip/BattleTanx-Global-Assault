#include "types.h"

typedef struct OptionsMenuText {
    char sound_fx[12];
    char music[8];
    char difficulty[12];
    char power_ups[12];
    char unlimited_ammo[16];
    char controller_config[20];
    char exit[8];
    char easy[8];
    char normal[8];
    char hard[8];
} OptionsMenuText;

OptionsMenuText gOptionsMenuText = {
    "SOUND FX", "MUSIC", "DIFFICULTY", "POWER UPS", "UNLIMITED AMMO",
    "CONTROLLER CONFIG", "EXIT", "EASY", "NORMAL", "HARD",
};
