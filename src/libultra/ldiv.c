#include "types.h"

typedef struct {
    s64 quot;
    s64 rem;
} lldiv_t;

typedef struct {
    s32 quot;
    s32 rem;
} ldiv_t;

lldiv_t lldiv(s64 numerator, s64 denominator)
{
    lldiv_t result;

    result.quot = numerator / denominator;
    result.rem = numerator - denominator * result.quot;
    if (result.quot < 0 && result.rem > 0) {
        result.quot++;
        result.rem -= denominator;
    }
    return result;
}

ldiv_t ldiv(s32 numerator, s32 denominator)
{
    ldiv_t result;

    result.quot = numerator / denominator;
    result.rem = numerator - denominator * result.quot;
    if (result.quot < 0 && result.rem > 0) {
        result.quot++;
        result.rem -= denominator;
    }
    return result;
}
