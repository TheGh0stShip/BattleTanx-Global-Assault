/* GCC 2.7.2 libgcc2 signed double-word comparison helper. */
typedef int SItype __attribute__((mode(SI)));
typedef unsigned int USItype __attribute__((mode(SI)));
typedef int DItype __attribute__((mode(DI)));

typedef union {
    struct {
        SItype high;
        SItype low;
    } s;
    DItype ll;
} DIunion;

int __cmpdi2(DItype first, DItype second) {
    DIunion a;
    DIunion b;

    a.ll = first;
    b.ll = second;
    if (a.s.high < b.s.high)
        return 0;
    if (a.s.high > b.s.high)
        return 2;
    if ((USItype)a.s.low < (USItype)b.s.low)
        return 0;
    if ((USItype)a.s.low > (USItype)b.s.low)
        return 2;
    return 1;
}
