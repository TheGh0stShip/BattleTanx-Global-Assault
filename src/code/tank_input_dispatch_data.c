/* Numeric constants and weapon-category switch used by func_80091768. */
#define TARGET(address) extern char D_##address[]
TARGET(80092058); TARGET(800920C4); TARGET(80092070);
TARGET(800920A0); TARGET(80092088);

const float D_80071EE0[] = {
    0.1f, 1.0f, -1.0f, 1024.0f, 80.0f, 2147483648.0f,
    80.0f, 1024.0f, 2147483648.0f, 65536.0f,
    2147483648.0f, 2147483648.0f, 1.5f, 2147483648.0f,
    80.0f, 512.0f, 2147483648.0f, 512.0f,
    2147483648.0f, 80.0f, 2147483648.0f, 2.0f,
};

const void *const jtbl_80071F38[] = {
    D_80092058, D_80092058, D_80092058, D_800920C4, D_80092070,
    D_800920C4, D_80092058, D_80092058, D_800920C4, D_800920C4,
    D_800920A0, D_80092088, D_80092058, D_80092070, D_80092088,
};

const float D_80071F74[] = {
    80.0f, 8192.0f, -0.6f, 0.65f, 2.0f, 5.0f, 600.0f,
};
