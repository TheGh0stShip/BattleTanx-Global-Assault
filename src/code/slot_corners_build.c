typedef float MtxF[4][4];
extern void func_8009EEE0(MtxF *);
extern void func_8009EFD4(MtxF *, float, float, float, int);
extern void func_8009F288(MtxF *, float *, float *);
extern void _bzero(void *, int);
extern void guMtxF2L(MtxF *, void *);
void func_800DF0D0(short *r, short *o, float x, float y, float z, unsigned short rot, void *dst) {
    MtxF m;
    float in[4][3];
    float out[4][3];
    int i;
    func_8009EEE0(&m);
    func_8009EFD4(&m, x, y, z, rot);
    _bzero(in, sizeof(in));
    _bzero(out, sizeof(out));
    in[0][0] = r[0]; in[0][1] = r[1];
    in[1][0] = r[0]; in[1][1] = r[3];
    in[2][0] = r[2]; in[2][1] = r[3];
    in[3][0] = r[2]; in[3][1] = r[1];
    for (i = 0; i < 4; i++) {
        func_8009F288(&m, in[i], out[i]);
    }
    o[0] = out[0][0];
    o[2] = out[0][0];
    o[1] = out[0][1];
    o[3] = out[0][1];
    for (i = 1; i < 4; i++) {
        if (out[i][0] < o[0]) o[0] = out[i][0];
        else if (o[2] < out[i][0]) o[2] = out[i][0];
        if (out[i][1] < o[1]) o[1] = out[i][1];
        else if (o[3] < out[i][1]) o[3] = out[i][1];
    }
    guMtxF2L(&m, dst);
}
