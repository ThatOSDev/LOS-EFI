
#include <stdint.h>

void FreePage(void* memoryAddress)
{
	uint64_t index = (uint64_t)memoryAddress / 4096;
	//if(check_page_bitmap) == 0) return;
	
}

void LockPage(void* memoryAddress)
{
}

void ReservePage(void* memoryAddress)
{
}

void UnReservePage(void* memoryAddress)
{
}


