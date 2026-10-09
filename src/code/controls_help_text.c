#include "types.h"

typedef struct ControlsHelpText {
    char select_action[40];
    char choose_controller[36];
    char easy[28];
    char arcade[32];
    char one_button[40];
    char driver[36];
    char done[28];
} ControlsHelpText;

ControlsHelpText gControlsHelpText = {
    "Select action and hit desired button",
    "Choose Controller type at bottom",
    "EASY... No Turret Control",
    "ARCADE... Body Follows Turret",
    "ONE BUTTON... One Button to Fire All",
    "DRIVER... Buttons Control Turret",
    "Select DONE when finished",
};
