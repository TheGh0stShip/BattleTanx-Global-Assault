#include "types.h"

typedef struct ControllerBindingText {
    char fire_1[8];
    char fire_2[8];
    char weapon[8];
    char change_view[12];
    char change_tank[12];
    char off_rail[12];
    char rail_right[12];
    char rail_left[12];
    char strafe[8];
    char forward[8];
    char backward[12];
    char right[8];
    char left[8];
    char turret_right[12];
    char turret_left[12];
    char done[8];
    char easy[8];
    char arcade[8];
    char one_button[12];
    char driver[8];
} ControllerBindingText;

ControllerBindingText gControllerBindingText = {
    "Fire 1", "Fire 2", "Weapon", "Chg View", "Chg Tank", "Off Rail",
    "Rail Right", "Rail Left", "Strafe", "Forward", "Backward", "Right",
    "Left", "Tur Right", "Tur Left", "Done", "Easy", "Arcade",
    "OneButton", "Driver",
};
