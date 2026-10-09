/* SPAN 0x80089D98 */
typedef unsigned short u16;
typedef struct Tank { char p0[0xA4]; int bit; int seen; int seenBy; char pb0[0x1D0 - 0xB0]; struct { char p[0x10]; int team; } *owner; } Tank;
typedef struct { unsigned int count; Tank *list[25]; } TankList;
void func_80089E84(TankList *, int, int, int, int);

void func_80089C90(Tank *t) {
    TankList l;
    int mask = ~t->bit;
    int team = t->owner->team;
    u16 i;

    func_80089E84(&l, t->seenBy, team, 1, 0);
    for (i = 0; i < l.count; i++) {
        l.list[i]->seenBy &= mask;
    }
    func_80089E84(&l, t->seen, team, 0, 1);
    for (i = 0; i < l.count; i++) {
        l.list[i]->seen &= mask;
    }
    t->seenBy = 0;
    t->seen = 0;
}
