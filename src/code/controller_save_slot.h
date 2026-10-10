#ifndef CONTROLLER_SAVE_SLOT_H
#define CONTROLLER_SAVE_SLOT_H

typedef struct {
    int a;
    int b;
    unsigned short c;
    char ext[4];
    unsigned char name[16];
    char pad[2];
} ControllerSaveSlot;

#endif
