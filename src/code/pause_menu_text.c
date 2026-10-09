#include "types.h"

typedef struct PauseMenuText {
    char continue_game[12];
    char restart[8];
    char quit[8];
} PauseMenuText;

PauseMenuText gPauseMenuText = { "CONTINUE", "RESTART", "QUIT" };
