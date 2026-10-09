/* GCC 2.7.2 libgcc2 unsigned double-word remainder wrapper. */
typedef unsigned int UDItype __attribute__((mode(DI)));

extern UDItype __udivmoddi4(UDItype, UDItype, UDItype *);

UDItype __umoddi3(UDItype numerator, UDItype denominator) {
    UDItype remainder;

    __udivmoddi4(numerator, denominator, &remainder);
    return remainder;
}
