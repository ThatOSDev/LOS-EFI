
#ifndef C_LIB_H
#define C_LIB_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#if defined(_WIN64)
   typedef unsigned __int64 size_t;
#else
   typedef unsigned int size_t;
#endif

typedef uint_least16_t char16_t;

size_t strlen(const char* str);
int    strcmp(const char* a, const char* b);
void   itoa(uint64_t n, uint64_t* buffer, uint32_t basenumber);
void*  memcpy(void* destination, const void* source, size_t num);
void*  memset (void *ptr, int value, size_t num);

#endif  // C_LIB_H