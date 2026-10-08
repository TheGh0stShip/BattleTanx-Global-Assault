typedef struct { float x, y, z; } V3;

void func_800E58D0(V3 *p, float *t, V3 *out) {
    V3 a, b;
    float s0 = t[1] / t[0] / 3.0f;
    float s1 = t[1] / t[2] / 3.0f;
    a.x = p[1].x + (p[1].x - p[0].x) * s0;
    a.z = p[1].z + (p[1].z - p[0].z) * s0;
    a.y = p[1].y + (p[1].y - p[0].y) * s0;
    b.x = p[2].x + (p[2].x - p[3].x) * s1;
    b.z = p[2].z + (p[2].z - p[3].z) * s1;
    b.y = p[2].y + (p[2].y - p[3].y) * s1;
    out->x = (a.x + b.x) / 2.0f;
    out->z = (a.z + b.z) / 2.0f;
    out->y = (a.y + b.y) / 2.0f;
}

void func_800E59D0(V3 *p, float *t, V3 *out) {
    V3 a, b;
    float s;
    s = t[0] / t[1] / 3.0f;
    a.x = (p[0].x + p[0].x + p[1].x) / 3.0f;
    a.z = (p[0].z + p[0].z + p[1].z) / 3.0f;
    a.y = (p[0].y + p[0].y + p[1].y) / 3.0f;
    b.x = p[1].x + (p[1].x - p[2].x) * s;
    b.z = p[1].z + (p[1].z - p[2].z) * s;
    b.y = p[1].y + (p[1].y - p[2].y) * s;
    out->x = (a.x + b.x) / 2.0f;
    out->z = (a.z + b.z) / 2.0f;
    out->y = (a.y + b.y) / 2.0f;
}

void func_800E5AC4(V3 *p, float *t, V3 *out) {
    V3 a, b;
    float s = t[1] / t[0] / 3.0f;
    a.x = p[1].x + (p[1].x - p[0].x) * s;
    a.z = p[1].z + (p[1].z - p[0].z) * s;
    a.y = p[1].y + (p[1].y - p[0].y) * s;
    b.x = (p[1].x + (p[2].x + p[2].x)) / 3.0f;
    b.z = (p[1].z + (p[2].z + p[2].z)) / 3.0f;
    b.y = (p[1].y + (p[2].y + p[2].y)) / 3.0f;
    out->x = (a.x + b.x) / 2.0f;
    out->z = (a.z + b.z) / 2.0f;
    out->y = (a.y + b.y) / 2.0f;
}
