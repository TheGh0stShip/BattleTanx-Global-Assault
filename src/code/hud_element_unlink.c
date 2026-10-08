typedef unsigned char u8;
typedef struct { char pad[0x54]; u8 key; u8 next; char pad2[2]; } Node88;
extern Node88 D_803A6A08[];
extern u8 D_80121CD0;

void func_800D05E0(Node88 *n, u8 key) {
    u8 i, prev, cur;
    for (i = 0; i < 16; i++) {
        if (n == &D_803A6A08[i]) break;
    }
    cur = D_80121CD0;
    cur = D_80121CD0;
    prev = 0xFF;
    while (cur != 0xFF && cur != i) {
        prev = cur;
        cur = D_803A6A08[cur].next;
    }
    if (cur != 0xFF) {
        if (prev == 0xFF) {
            D_80121CD0 = D_803A6A08[cur].next;
        } else {
            D_803A6A08[prev].next = D_803A6A08[cur].next;
        }
    }
    D_803A6A08[i].key = key;
    if (D_80121CD0 == 0xFF) {
        D_80121CD0 = i;
        D_803A6A08[i].next = 0xFF;
        return;
    }
    cur = D_80121CD0;
    prev = 0xFF;
    while (cur != 0xFF) {
        if (D_803A6A08[cur].key >= key) break;
        prev = cur;
        cur = D_803A6A08[cur].next;
    }
    if (prev == 0xFF) {
        D_80121CD0 = i;
    } else {
        D_803A6A08[prev].next = i;
    }
    D_803A6A08[i].next = cur;
}
