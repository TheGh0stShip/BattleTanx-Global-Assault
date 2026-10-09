#include "types.h"

typedef struct MainMenuText {
    char players[8];
    char play_mode[12];
    char options[8];
    char credits[8];
    char input_code[12];
    char load_game[12];
    char start[8];
    char campaign[12];
    char battlelord[12];
    char deathmatch[12];
    char frenzy[8];
    char hold_em[8];
    char family_mode[12];
    char tank_wars[12];
    char convoy[8];
} MainMenuText;

MainMenuText gMainMenuText = {
    "PLAYERS", "PLAY MODE", "OPTIONS", "CREDITS", "INPUT CODE",
    "LOAD GAME", "START", "CAMPAIGN", "BATTLELORD", "DEATHMATCH",
    "FRENZY", "HOLD-EM", "FAMILY MODE", "TANK WARS", "CONVOY",
};
