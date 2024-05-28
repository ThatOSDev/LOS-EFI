
#include "clib.h"

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

void itoa(uint64_t n, uint64_t* buffer, uint32_t basenumber)
{
	unsigned long int hold;
	int i, j;
	hold = n;
	i = 0;
	
	do{
		hold = n % basenumber;
		buffer[i++] = (hold < 10) ? (hold + '0') : (hold + 'a' - 10);
	} while(n /= basenumber);
	buffer[i--] = 0;
	
	for(j = 0; j < i; j++, i--)
	{
		hold = buffer[j];
		buffer[j] = buffer[i];
		buffer[i] = hold;
	}
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


