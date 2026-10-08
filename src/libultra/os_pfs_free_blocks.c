#include "pfs.h"

s32 osPfsFreeBlocks(OSPfs *pfs, s32 *bytes_not_used)
{
    int j;
    int pages = 0;
    __OSInode inode;
    s32 ret = 0;
    u8 bank;
    int offset;

    PFS_CHECK_STATUS;
    PFS_CHECK_ID;
    for (bank = 0; bank < pfs->banks; bank++) {
        ERRCK(__osPfsRWInode(pfs, &inode, PFS_READ, bank));
        if (bank > 0)
            offset = 1;
        else
            offset = pfs->inode_start_page;
        for (j = offset; j < PFS_INODE_SIZE_PER_PAGE; j++) {
            if (inode.inode_page[j].ipage == PFS_PAGE_NOT_USED)
                pages++;
        }
    }
    *bytes_not_used = pages * PFS_ONE_PAGE * BLOCKSIZE;
    return 0;
}
