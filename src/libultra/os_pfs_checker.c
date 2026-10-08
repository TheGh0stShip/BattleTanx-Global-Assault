#include "pfs.h"

#define PFS_BANK_LAPPED_BY 8
#define CHECK_IPAGE(p, pfs)                                     \
    (((p).ipage >= (pfs).inode_start_page) &&                   \
     ((p).inode_t.bank < (pfs).banks) &&                        \
     ((p).inode_t.page > 0) &&                                  \
     ((p).inode_t.page < PFS_INODE_SIZE_PER_PAGE))

typedef struct {
    __OSInode inode;
    u8 bank;
    u8 map[256];
} __OSInodeCache;

s32 corrupted_init(OSPfs *pfs, __OSInodeCache *cache);
s32 corrupted(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache);

s32 osPfsChecker(OSPfs *pfs)
{
    int j;
    s32 ret;
    __OSInodeUnit next_page;
    __OSInode checked_inode;
    __OSInode tmp_inode;
    __OSDir tmp_dir;
    __OSInodeUnit file_next_node[16];
    __OSInodeCache cache;
    int fixed = 0;
    u8 bank;
    s32 cc;
    s32 cl;
    int offset;

    ret = __osCheckId(pfs);
    if (ret == PFS_ERR_NEW_PACK)
        ret = __osGetId(pfs);
    if (ret != 0)
        return ret;

    ERRCK(corrupted_init(pfs, &cache));

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel,
                              pfs->dir_table + j, (u8 *)&tmp_dir));

        if ((tmp_dir.company_code != 0) && (tmp_dir.game_code != 0)) {
            next_page = tmp_dir.start_page;
            cc = 0;
            cl = 0;
            bank = 255;

            while (CHECK_IPAGE(next_page, *pfs)) {
                if (bank != next_page.inode_t.bank) {
                    bank = next_page.inode_t.bank;
                    ret = __osPfsRWInode(pfs, &tmp_inode, PFS_READ, bank);
                    if ((ret != 0) && (ret != PFS_ERR_INCONSISTENT))
                        return ret;
                }
                if ((cc = corrupted(pfs, next_page, &cache) - cl) != 0)
                    break;
                cl = 1;
                next_page = tmp_inode.inode_page[next_page.inode_t.page];
            }

            if ((cc != 0) || (next_page.ipage != PFS_EOF)) {
                tmp_dir.company_code = 0;
                tmp_dir.game_code = 0;
                tmp_dir.start_page.ipage = 0;
                tmp_dir.status = 0;
                tmp_dir.data_sum = 0;
                SET_ACTIVEBANK_TO_ZERO;
                ERRCK(__osContRamWrite(pfs->queue, pfs->channel,
                                       pfs->dir_table + j,
                                       (u8 *)&tmp_dir, 0));
                fixed++;
            }
        } else if ((tmp_dir.company_code != 0) ||
                   (tmp_dir.game_code != 0)) {
            tmp_dir.company_code = 0;
            tmp_dir.game_code = 0;
            tmp_dir.start_page.ipage = 0;
            tmp_dir.status = 0;
            tmp_dir.data_sum = 0;
            SET_ACTIVEBANK_TO_ZERO;
            ERRCK(__osContRamWrite(pfs->queue, pfs->channel,
                                   pfs->dir_table + j, (u8 *)&tmp_dir, 0));
            fixed++;
        }
    }

    for (j = 0; j < pfs->dir_size; j++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel,
                              pfs->dir_table + j, (u8 *)&tmp_dir));
        if ((tmp_dir.company_code != 0) && (tmp_dir.game_code != 0) &&
            (tmp_dir.start_page.ipage >= ((u16)pfs->inode_start_page)))
            file_next_node[j].ipage = tmp_dir.start_page.ipage;
        else
            file_next_node[j].ipage = 0;
    }

    for (bank = 0; bank < pfs->banks; bank++) {
        ret = __osPfsRWInode(pfs, &tmp_inode, PFS_READ, bank);
        if ((ret != 0) && (ret != PFS_ERR_INCONSISTENT))
            return ret;
        if (bank > 0)
            offset = 1;
        else
            offset = pfs->inode_start_page;
        for (j = 0; j < offset; j++)
            checked_inode.inode_page[j].ipage =
                tmp_inode.inode_page[j].ipage;
        for (; j < PFS_INODE_SIZE_PER_PAGE; j++)
            checked_inode.inode_page[j].ipage = PFS_PAGE_NOT_USED;
        for (j = 0; j < pfs->dir_size; j++) {
            while ((file_next_node[j].inode_t.bank == bank) &&
                   (file_next_node[j].ipage >=
                    ((u16)pfs->inode_start_page))) {
                u8 pp = file_next_node[j].inode_t.page;

                file_next_node[j] = checked_inode.inode_page[pp] =
                    tmp_inode.inode_page[pp];
            }
        }
        ERRCK(__osPfsRWInode(pfs, &checked_inode, PFS_WRITE, bank));
    }

    if (fixed)
        pfs->status |= PFS_CORRUPTED;
    else
        pfs->status &= ~PFS_CORRUPTED;
    return 0;
}

s32 corrupted_init(OSPfs *pfs, __OSInodeCache *cache)
{
    int i;
    int n;
    int offset;
    u8 bank;
    __OSInodeUnit tpage;
    __OSInode tmp_inode;
    s32 ret;

    for (i = 0; i < 256; i++)
        cache->map[i] = 0;
    cache->bank = 255;

    for (bank = 0; bank < pfs->banks; bank++) {
        if (bank > 0)
            offset = 1;
        else
            offset = pfs->inode_start_page;
        ret = __osPfsRWInode(pfs, &tmp_inode, PFS_READ, bank);
        if ((ret != 0) && (ret != PFS_ERR_INCONSISTENT))
            return ret;
        for (i = offset; i < PFS_INODE_SIZE_PER_PAGE; i++) {
            tpage = tmp_inode.inode_page[i];
            if ((tpage.ipage >= pfs->inode_start_page) &&
                (tpage.inode_t.bank != bank)) {
                n = (tpage.inode_t.page / 4) +
                    ((tpage.inode_t.bank % PFS_BANK_LAPPED_BY) * BLOCKSIZE);
                cache->map[n] |= 1 << (bank % PFS_BANK_LAPPED_BY);
            }
        }
    }
    return 0;
}

s32 corrupted(OSPfs *pfs, __OSInodeUnit fpage, __OSInodeCache *cache)
{
    int j;
    int n;
    int hit = 0;
    u8 bank;
    int offset;
    s32 ret = 0;

    n = (fpage.inode_t.page / 4) +
        (fpage.inode_t.bank % PFS_BANK_LAPPED_BY) * BLOCKSIZE;
    for (bank = 0; bank < pfs->banks; bank++) {
        if (bank > 0)
            offset = 1;
        else
            offset = pfs->inode_start_page;
        if ((bank == fpage.inode_t.bank) ||
            (cache->map[n] & (1 << (bank % PFS_BANK_LAPPED_BY)))) {
            if (bank != cache->bank) {
                ret = __osPfsRWInode(pfs, &cache->inode, PFS_READ, bank);
                if ((ret != 0) && (ret != PFS_ERR_INCONSISTENT))
                    return ret;
                cache->bank = bank;
            }
            for (j = offset;
                 (hit < 2) && (j < PFS_INODE_SIZE_PER_PAGE); j++) {
                if (cache->inode.inode_page[j].ipage == fpage.ipage)
                    hit++;
            }
            if (1 < hit)
                return PFS_ERR_NEW_PACK;
        }
    }
    return hit;
}
