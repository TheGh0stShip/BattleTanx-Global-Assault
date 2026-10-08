#include "pfs.h"

s32 osPfsFindFile(OSPfs *pfs, u16 company_code, u32 game_code,
                  u8 *game_name, u8 *ext_name, s32 *file_no)
{
    s32 j;
    int i;
    __OSDir dir;
    s32 ret = 0;
    int fail;

    PFS_CHECK_ID;
    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel,
                              pfs->dir_table + j, (u8 *)&dir));
        if ((dir.company_code == company_code) &&
            (dir.game_code == game_code)) {
            fail = 0;
            if (game_name != 0) {
                for (i = 0; i < PFS_FILE_NAME_LEN; i++) {
                    if (dir.game_name[i] != game_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if ((ext_name != 0) && (fail == 0)) {
                for (i = 0; i < PFS_FILE_EXT_LEN; i++) {
                    if (dir.ext_name[i] != ext_name[i]) {
                        fail = 1;
                        break;
                    }
                }
            }
            if (fail == 0) {
                *file_no = j;
                return ret;
            }
        }
    }
    *file_no = -1;
    return PFS_ERR_INVALID;
}
