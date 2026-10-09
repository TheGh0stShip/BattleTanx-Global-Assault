#include "types.h"

typedef struct ResultsText {
    char victory[8];
    char defeat[8];
    char score[8];
    char player_one_score[16];
    char player_two_score[16];
    char kills[8];
    char tanks_lost[12];
    char par_time[12];
    char your_time[12];
    char tank_bucks[12];
    char continue_label[12];
    char save_and_continue[20];
    char replay_level[16];
    char quit[8];
    char password[12];
} ResultsText;

ResultsText gResultsText = {
    "VICTORY", "DEFEAT", "Score", "Player 1 Score", "Player 2 Score",
    "Kills", "Tanks Lost", "Par Time", "Your Time", "Tank Bucks",
    "Continue", "Save And Continue", "Replay Level", "Quit", "Password:",
};
