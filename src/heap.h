#ifndef HEAP_H
#define HEAP_H
#include<stdlib.h>

extern void* heap;

void my_heap_init(size_t size);
void* heap_get_next_matching_block(size_t nbytes);
void my_heap_dump(void);
void my_heap_destroy(void);

#endif
