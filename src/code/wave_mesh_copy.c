/* SPAN 0x800F1B24 */
/* RODATA_VRAM 0x80076F98 */
typedef struct { unsigned int w0; unsigned int w1; } Gfx;
typedef struct { short x, y, z, f; int a, b; } Vtx;
typedef struct { unsigned short ang; char p2[2]; float div; float off; float amp; } Wave;
extern unsigned int func_80079570(void);
extern Gfx *func_8007B190(unsigned int);
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
Gfx *func_800F1900(Gfx *src, Wave *p) {
    Gfx *dst;
    int i = 0;
    dst = func_8007B190(func_80079570() >> 3);
    if (dst == 0) return 0;
    while (src[i].w0 != 0xDF000000) {
        dst[i] = src[i];
        if (*(unsigned char *)&src[i] == 1) {
            Vtx *d, *s;
            int j, n;
            dst[i].w1 = src[i].w1 - (unsigned int)src + (unsigned int)dst;
            d = (Vtx *)dst[i].w1;
            s = (Vtx *)src[i].w1;
            n = (src[i].w0 >> 12) & 0xFF;
            for (j = 0; j < n; j++) {
                float t = func_8009D4B0(p->ang) * s[j].x + func_8009D510(p->ang) * s[j].z;
                t += p->off;
                t /= p->div;
                t *= 65535.0f;
                d[j] = s[j];
                d[j].y += (short)(p->amp * func_8009D4B0((unsigned int)t));
            }
        }
        i++;
    }
    dst[i] = src[i];
    return dst;
}
