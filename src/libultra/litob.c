#include "types.h"

typedef u8 fmt_type;

typedef struct {
    union {
        s64 s64;
        u64 u64;
        f64 f64;
        u32 u32;
        u16 u16;
    } value;
    char *buff;
    s32 part1_len;
    s32 num_leading_zeros;
    s32 part2_len;
    s32 num_mid_zeros;
    s32 part3_len;
    s32 num_trailing_zeros;
    s32 precision;
    s32 width;
    u32 size;
    u32 flags;
    fmt_type length;
} printf_struct;

typedef struct {
    s64 quot;
    s64 rem;
} lldiv_t;

#define FLAGS_MINUS 4
#define FLAGS_ZERO 16
#define BUFF_LEN 0x18

/* DATA_VRAM 0x80127E00 */
static u8 ldigs[] = "0123456789abcdef";
static u8 udigs[] = "0123456789ABCDEF";

extern lldiv_t lldiv(s64 numerator, s64 denominator);
extern void *memcpy(void *dst, const void *src, u32 length);

void _Litob(printf_struct *args, fmt_type type)
{
    u8 buff[BUFF_LEN];
    const u8 *numMap;
    s32 base;
    s32 index;
    u64 num;
    lldiv_t result;

    if (type == 'X') {
        numMap = udigs;
    } else {
        numMap = ldigs;
    }

    base = type == 'o' ? 8 : (type != 'x' && type != 'X' ? 10 : 16);
    index = BUFF_LEN;
    num = args->value.s64;

    if ((type == 'd' || type == 'i') && args->value.s64 < 0) {
        num = -num;
    }

    if (num != 0 || args->precision != 0) {
        buff[--index] = numMap[num % base];
    }

    args->value.s64 = num / base;
    while (args->value.s64 > 0 && index > 0) {
        result = lldiv(args->value.s64, base);
        args->value.s64 = result.quot;
        buff[--index] = numMap[result.rem];
    }

    args->part2_len = BUFF_LEN - index;
    memcpy(args->buff, buff + index, args->part2_len);

    if (args->part2_len < args->precision) {
        args->num_leading_zeros = args->precision - args->part2_len;
    }

    if (args->precision < 0 &&
        (args->flags & (FLAGS_ZERO | FLAGS_MINUS)) == FLAGS_ZERO) {
        index = args->width - args->part1_len - args->num_leading_zeros -
                args->part2_len;
        if (index > 0) {
            args->num_leading_zeros += index;
        }
    }
}
