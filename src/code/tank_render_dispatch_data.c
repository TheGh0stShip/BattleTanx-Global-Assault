/* Aggregate initializers, numeric constants, and part-type switch used by
 * func_80092C38.  Its text remains assembly. */
#define TARGET(address) extern char D_##address[]
TARGET(80093C90); TARGET(80093B00); TARGET(80093B60); TARGET(80093DF8);
TARGET(80093BC0); TARGET(80093FA0); TARGET(80093A48); TARGET(80093D44);
TARGET(80093CC8); TARGET(80094100); TARGET(80094184); TARGET(80094204);
TARGET(8009407C); TARGET(80094280); TARGET(800942E8);

struct TankRenderInitializers {
    float identity[16];
    int identity_flag;
    float zero_vector[3];
    unsigned char color_a[4];
    unsigned char color_b[4];
    float twenty;
};

const struct TankRenderInitializers D_80071FDC = {
    { 1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f },
    0,
    { 0.0f, 0.0f, 0.0f },
    { 0xFF, 0xFD, 0x4C, 0xFF },
    { 0x4D, 0xA5, 0xFF, 0xFF },
    20.0f,
};

/* This standalone slice begins four bytes after the original 8-byte-aligned
 * translation-unit pool.  Preserve the retail absolute placement by keeping
 * these already-aligned doubles at four-byte object alignment. */
const double D_80072038 __attribute__((aligned(4))) = 1.0;
const float D_80072040[] = { 255.0f, 2147483648.0f };
const double D_80072048 __attribute__((aligned(4))) = 182.04444444;
const float D_80072050 = 28.0f;
const unsigned int D_80072054 = 0;
const double D_80072058 __attribute__((aligned(4))) = 182.04444444;
const float D_80072060[] = {
    28.0f, 0.0f, 1.0f, 20.0f, 20.0f,
    0.66666f, 0.666666f, 0.3333333f, 1.0f, 0.9f, 0.1f,
    0.333333f, 0.33333334f, 0.3333333f, 1.0f, 0.9f, 0.1f,
    0.3333333f, 1.0f, 0.9f, 0.1f, 1.0f, 2.0f,
    -24.5f, 40.0f, 0.5f, 20.0f, 1.0f, 20.0f, 0.5f,
    8.0f, 140.0f, 1.0f, 256.0f, 20.0f, 1.0f, 192.0f,
    2147483648.0f,
};

const void *const jtbl_800720F8[] = {
    D_80093C90, D_80093B00, D_80093B60, D_80093DF8, D_80093DF8,
    D_80093DF8, D_80093BC0, D_80093FA0, D_80093FA0, D_80093A48,
    D_80093D44, D_80093CC8, D_80094100, D_80094184, D_80094204,
    D_8009407C, D_80094280, D_800942E8,
};

const float D_80072140[] = { 1.0f, 64.0f, 1.0f, 64.0f, 1.0f, 35.0f };
