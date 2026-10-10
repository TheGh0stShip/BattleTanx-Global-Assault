/* Second symbolic switch table used by func_8009C524. */
#define TARGET(address) extern char D_##address[]
TARGET(8009CA60); TARGET(8009CB18); TARGET(8009D0D0); TARGET(8009CBA8);

const void *const jtbl_80072540[] = {
    D_8009CA60, D_8009CB18, D_8009D0D0,
    D_8009CBA8, D_8009D0D0, D_8009D0D0,
};
