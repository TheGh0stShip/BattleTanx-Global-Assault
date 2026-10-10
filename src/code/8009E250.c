/* SPAN 0x8009E948 */
typedef unsigned char u8;
typedef unsigned long long u64;

extern u8 D_80114C84[5][6];

static inline u64 lfsrStep(u64 v, u8 *taps) {
    u8 c;
    int i;
    u8 p;

    p = 1;
    c = v >> 63;
    v <<= 1;
    v |= c;
    for (i = 0; i < 6; i++) {
        if ((v >> taps[i]) & 1) {
            p ^= 1;
        }
    }
    return v ^ p;
}

u64 func_8009E250(u64 v) {
    int r;
    int k;
    int n;

    for (r = 0; r < 5; r++) {
        n = D_80114C84[r][5] + 128;
        for (k = 0; k < n; k++) {
            v = lfsrStep(v, D_80114C84[r]);
        }
    }
    return v;
}

static inline u64 lfsrUnstep(u64 v, u8 *taps) {
    unsigned int c;
    int i;
    u8 p;

    p = 1;
    for (i = 0; i < 6; i++) {
        if ((v >> taps[i]) & 1) {
            p ^= 1;
        }
    }
    v ^= p;
    c = v & 1;
    v >>= 1;
    return v | ((u64)c << 63);
}

u64 func_8009E374(u64 v) {
    int r;
    int k;
    int n;

    for (r = 4; r >= 0; r--) {
        n = D_80114C84[r][5] + 128;
        for (k = 0; k < n; k++) {
            v = lfsrUnstep(v, D_80114C84[r]);
        }
    }
    return v;
}

inline u64 func_8009E49C(u64 v, u8 *taps) {
    u8 c;
    int i;
    u8 p;

    p = 1;
    c = v >> 49;
    v = (v << 1) & 0x3FFFFFFFFFFFFULL;
    v |= c;
    for (i = 0; i < 6; i++) {
        if ((v >> (u8)(taps[i] % 50)) & 1) {
            p ^= 1;
        }
    }
    return v ^ p;
}

inline u64 func_8009E58C(u64 v, u8 *taps) {
    unsigned int c;
    int i;
    u8 p;

    p = 1;
    for (i = 0; i < 6; i++) {
        if ((v >> (u8)(taps[i] % 50)) & 1) {
            p ^= 1;
        }
    }
    v ^= p;
    c = v & 1;
    v >>= 1;
    return v | ((u64)c << 49);
}

u64 func_8009E668(u64 v) {
    int r;
    int k;
    int n;

    for (r = 0; r < 5; r++) {
        n = D_80114C84[r][5] + 128;
        for (k = 0; k < n; k++) {
            v = func_8009E49C(v, D_80114C84[r]);
        }
    }
    return v;
}

u64 func_8009E7F0(u64 v) {
    int r;
    int k;
    int n;

    for (r = 4; r >= 0; r--) {
        n = D_80114C84[r][5] + 128;
        for (k = 0; k < n; k++) {
            v = func_8009E58C(v, D_80114C84[r]);
        }
    }
    return v;
}
