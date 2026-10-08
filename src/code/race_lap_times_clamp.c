typedef struct {
    unsigned short count;
    float t[4];
    unsigned char text[1];
} MsgList;
extern float D_803A5948;
int func_800CA2BC(MsgList *s) {
    unsigned short i, j;
    unsigned char *p, *q;
    for (i = 0; i < s->count; i++) {
        s->t[i] -= D_803A5948;
        if (s->t[i] < 0.0f) {
            p = s->text;
            for (j = 0; j < i; j++) {
                while (*p != '<') p++;
            }
            for (j = i + 1; j < s->count; j++) {
                s->t[j - 1] = s->t[j];
            }
            q = p + 1;
            while (*q != '<') q++;
            q++;
            while (*q) *p++ = *q++;
            *p = 0;
            s->count--;
        }
    }
    return s->count == 0;
}
