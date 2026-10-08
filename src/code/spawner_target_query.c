/* ---- 0x800DE000/b/e.c ---- */
typedef struct { char pad[0xC]; float x; float y; char p2[0x1A-0x14]; unsigned char id; char p3[0x3D-0x1B]; unsigned char v; } ObjE;
typedef struct { unsigned char id; char pad[3]; int busy; } QE;
typedef struct { unsigned char flag; char pad[3]; float x; float y; } OutE;
void func_800DE4DC(ObjE *obj, int unused, QE *q, OutE *out) {
    int v;
    if (q->busy == 0 && q->id == obj->id && (v = obj->v) < 6 && v >= 0) {
        out->flag = 1; out->x = obj->x; out->y = obj->y;
    }
}

