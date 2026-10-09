/* GCC 2.7.2 libgcc2 unsigned double-word division wrapper. */
typedef unsigned int UDItype __attribute__((mode(DI)));

extern UDItype __udivmoddi4(UDItype, UDItype, UDItype *);

UDItype __udivdi3(UDItype numerator, UDItype denominator) {
    return __udivmoddi4(numerator, denominator, (UDItype *)0);
}
