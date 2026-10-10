/* Symbolic switch table used by func_800A77A8. */
#define TARGET(address) extern char D_##address[]
TARGET(800A7A4C); TARGET(800A7AA8); TARGET(800A79D8); TARGET(800A7B1C);
TARGET(800A7968);

const void *const jtbl_80072CE0[] = {
    D_800A7A4C, D_800A7AA8, D_800A79D8, D_800A7B1C,
    D_800A7AA8, D_800A7A4C, D_800A7B1C, D_800A7968,
};
