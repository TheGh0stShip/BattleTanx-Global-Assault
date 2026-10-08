#include "n_audio_private.h"

void func_80101D90(ALPlayer *client)
{
    u32 mask = osSetIntMask(1);

    client->samplesLeft = n_syn->curSamples;
    client->next = n_syn->head;
    n_syn->head = client;
    osSetIntMask(mask);
}
