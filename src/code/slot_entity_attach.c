typedef struct { char pad[0x18]; char *items; char p2[4]; } GrpH;
typedef struct {
    char pad[0x9B7C];
    int counts[10];
    GrpH *groups;
} CtxH;
typedef struct {
    char pad[0x40];
    char sub[8];
    void *src;
    char c4c, c4d, c4e;
    char pad2[1];
} EntH;
extern void func_800DF0D0(char *, char *, float, float, float, int, EntH *);
extern void func_80102F10(void *, int);
EntH *func_800DF558(CtxH *ctx, char *src, float *pos, unsigned short flags,
                    unsigned char idx, int c, unsigned char d, unsigned char e) {
    EntH *ent = (EntH *)(((GrpH *)((char *)ctx->groups + (idx << 5)))->items + ctx->counts[idx]++ * 80);
    ent->src = src;
    if (src) {
        func_800DF0D0(src + 4, ent->sub, pos[0], pos[2], pos[1], flags, ent);
        ent->c4e = e;
        ent->c4c = c;
        ent->c4d = d;
    } else {
        func_80102F10(ent, 80);
    }
    return ent;
}

