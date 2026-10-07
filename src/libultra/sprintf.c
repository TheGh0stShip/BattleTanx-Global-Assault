#include "types.h"

typedef char* va_list;

#define va_start(list, last) ((list) = (char*)&(last) + sizeof(last))
#define va_end(list) ((void)(list))

extern void* memcpy(void* destination, const void* source, u32 length);
extern s32 _Printf(void* (*output)(void*, const char*, u32), void* destination,
                   const char* format, va_list arguments);

void* proutSprintf(void* destination, const char* source, u32 length) {
    return (u8*)memcpy(destination, source, length) + length;
}

s32 sprintf(char* destination, const char* format, ...) {
    s32 result;
    va_list arguments;

    va_start(arguments, format);
    result = _Printf(proutSprintf, destination, format, arguments);
    if (result >= 0) {
        destination[result] = '\0';
    }
    va_end(arguments);
    return result;
}
