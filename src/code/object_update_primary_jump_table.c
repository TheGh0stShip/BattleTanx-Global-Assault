/* First symbolic switch table used by func_8009C524. */
#define TARGET(address) extern char D_##address[]
TARGET(8009C6D4); TARGET(8009C83C); TARGET(8009C718); TARGET(8009C814);
TARGET(8009C754); TARGET(8009C7F8);

const void *const jtbl_80072518[] = {
    D_8009C6D4, D_8009C83C, D_8009C718, D_8009C814,
    D_8009C754, D_8009C83C, D_8009C7F8,
};
