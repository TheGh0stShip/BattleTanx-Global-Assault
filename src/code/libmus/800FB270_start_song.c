#include "types.h"

s32 player_text_329C(void *song);
s32 MusHandleUnPause(s32 handle);

s32 MusStartSong(void *song) {
    s32 handle = player_text_329C(song);

    MusHandleUnPause(handle);
    return handle;
}
