#include "types.h"

typedef struct ControllerPakText {
    char used[8];
    char free[8];
    char recheck[28];
    char add_save[36];
    char delete[8];
    char no_space[140];
    char select_delete[92];
    char cancel[8];
    char play_without_saving[28];
    char no_pak[56];
    char still_no_pak[60];
    char read_error[48];
    char pak_changed[56];
    char corrupt[32];
    char attempt_repair[36];
    char add_new_save[64];
    char no_save[96];
    char save_loaded[32];
    char main_menu[24];
    char repair_success[40];
    char repair_failed[40];
    char continue_label[12];
    char overwrite[44];
} ControllerPakText;

ControllerPakText gControllerPakText = {
    "Used:",
    "Free:",
    "Re-check for Controller Pak",
    "Add new save file to Controller Pak",
    "Delete",
    "Not enough space for BattleTanx GA save game. Select Controller Pak file to delete. BattleTanx GA requires an empty note and 1 free page.",
    "Select Controller Pak file to delete.\nBattleTanx GA requires an empty note and 1 free page.",
    "Cancel",
    "Play Game Without Saving",
    "No Controller Pak found.\nYou will not be able to save",
    "Still no Controller Pak found.\nYou will not be able to save",
    "Error reading Controller Pak.\nPlease re-insert.",
    "Controller Pak has changed.\nUse current Controller Pak?",
    "Corrupt Controller Pak detected",
    "Attempt Repair (data may be lost)",
    "Add New BattleTanx GA save game file to this Controller Pak?",
    "No BattleTanx GA Save Game on this Controller Pak.\nPlease insert a different Controller Pak.",
    "BattleTanx GA Save Game Loaded.",
    "Go Back to Main Menu",
    "Controller Pak repaired successfully.",
    "Controller Pak could not be repaired.",
    "Continue",
    "Overwrite previous Battletanx GA save game?",
};
