
#ifndef PAGING_H
#define PAGING_H

void FreePage(void* memoryAddress);
void LockPage(void* memoryAddress);

void ReservePage(void* memoryAddress);
void UnReservePage(void* memoryAddress);

#endif // PAGING_H