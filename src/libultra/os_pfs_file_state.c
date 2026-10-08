#include "pfs.h"

typedef struct {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
} OSPfsState;

s32 osPfsFileState(OSPfs *pfs, s32 file_no, OSPfsState *state)
{
    s32 ret;
    int pages;
    __OSInode inode;
    __OSDir dir;
    __OSInodeUnit page;
    int j;
    u8 bank;
    u8 startpage;

    if (file_no >= pfs->dir_size || file_no < 0)
        return PFS_ERR_INVALID;
    PFS_CHECK_STATUS;
    PFS_CHECK_ID;
    SET_ACTIVEBANK_TO_ZERO;
    ERRCK(__osContRamRead(pfs->queue, pfs->channel,
                          pfs->dir_table + file_no, (u8 *)&dir));
    if ((dir.company_code == 0) || (dir.game_code == 0))
        return PFS_ERR_INVALID;
    if (dir.start_page.ipage < pfs->inode_start_page)
        return PFS_ERR_INCONSISTENT;
    pages = 0;
    startpage = dir.start_page.inode_t.page;
    for (bank = dir.start_page.inode_t.bank; bank < pfs->banks;) {
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_READ, bank));
        page = inode.inode_page[startpage];
        pages++;
        while (page.ipage >= pfs->inode_start_page) {
            pages++;
            page = inode.inode_page[page.inode_t.page];
            if (page.inode_t.bank != bank) {
                bank = page.inode_t.bank;
                startpage = page.inode_t.page;
                break;
            }
        }
        if (page.ipage == PFS_EOF)
            break;
    }
    if (page.ipage != PFS_EOF)
        return PFS_ERR_INCONSISTENT;
    state->file_size = pages * (PFS_ONE_PAGE * BLOCKSIZE);
    state->company_code = dir.company_code;
    state->game_code = dir.game_code;
    for (j = 0; j < PFS_FILE_NAME_LEN; j++)
        state->game_name[j] = dir.game_name[j];
    for (j = 0; j < PFS_FILE_EXT_LEN; j++)
        state->ext_name[j] = dir.ext_name[j];
    return 0;
}
