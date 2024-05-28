
#ifndef C_LIB_H
#define C_LIB_H

#include <stddef.h>  // size_t

size_t strlen(const char* str)
{
	const char* strCount = str;

	while (*strCount++);
	return strCount - str - 1;
}

int strcmp(const char* a, const char* b)
{
	size_t length = strlen(a);
	for(size_t i = 0; i < length; i++)
	{
		if(a[i] < b[i]){return -1;}
		if(a[i] > b[i]){return 1;}
	}
	return 0;
}

void* memcpy(void* destination, const void* source, size_t num)
{
    for (size_t i = 0; i < num; i++)
        ((uint8_t*) destination)[i] = ((uint8_t*) source)[i];
    return destination;
}

// GCC Version
void *memset (void *ptr, int value, size_t num)
{
  unsigned char *localPtr = (unsigned char*)ptr;
  while (num-- > 0)
    *localPtr++ = value;

  return ptr;
}

#endif  // C_LIB_H
