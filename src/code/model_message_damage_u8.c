typedef struct { char pad[27]; unsigned char hp; } A;
typedef struct { char pad[4]; int type; } B;
typedef struct { char pad[16]; int dmg; } C;
void func_800EA8F4(A *a, B *b, C *c, int *out) {
    switch (b->type) {
    case 38:
        *out = 5;
        break;
    case 35:
        a->hp -= c->dmg;
        *out = 1;
        break;
    case 11: case 37: case 50:
        a->hp -= c->dmg;
        *out = 1;
        break;
    case 4: case 28: case 66:
        *out = 1;
        break;
    }
}
