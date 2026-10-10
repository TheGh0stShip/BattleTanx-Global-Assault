/* Data reconstruction: decompals/ultralib e24c836796df4bf520ff8b11a5c9d2cea3a66cbd src/audio/sl.c: ALGlobals *alGlobals=0; */
#include "types.h"

typedef struct ALLink {
    struct ALLink* next;
    struct ALLink* previous;
} ALLink;

void* alGlobals_80126ED0 = 0; /* ALGlobals *alGlobals */
extern void alSynDelete_80110750(void* globals);
extern void alSynNew(void* globals);

void alUnlink(ALLink* element) {
    if (element->next != 0) {
        element->next->previous = element->previous;
    }
    if (element->previous != 0) {
        element->previous->next = element->next;
    }
}

void alLink(ALLink* element, ALLink* after) {
    element->next = after->next;
    element->previous = after;
    if (after->next != 0) {
        after->next->previous = element;
    }
    after->next = element;
}

void alClose(void* globals) {
    if (alGlobals_80126ED0 != 0) {
        alSynDelete_80110750(globals);
        alGlobals_80126ED0 = 0;
    }
}

void alInit(void* globals) {
    if (alGlobals_80126ED0 == 0) {
        alGlobals_80126ED0 = globals;
        alSynNew(globals);
    }
}
