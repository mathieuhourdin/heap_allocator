#include<stdlib.h>
#include<stdio.h>

#include "block.h"
#include "heap.h"


void *heap = NULL;

void my_heap_init(size_t size) {
    heap = malloc(size);
    block_new(heap, (int)size - sizeof(Block));
    block_new((char *)heap + (int)size - sizeof(Block), sizeof(Block));
}

void* heap_get_next_matching_block(size_t nbytes) {
    Block *block = heap;
    while (block) {
        if (block->free && (size_t)block->size > (size_t)nbytes + sizeof(Block)) {
            return block;
        }
        block = block_get_next(block);
    }
    return NULL;
}

void my_heap_dump(void) {
    Block *block = heap;
    int offset = 0;
    printf("Headers size : %d\n", (int)sizeof(Block));
    int counter = 0;
    while (block && counter < 10) {
        printf(
                "Block at address %p (offset %d) is %d free and of size %d and next is : %p\n",
                block, offset, block->free, block->size, block_get_next(block)
                );
        offset += block->size; 
        block = block_get_next(block);
        counter++;
    }
    printf("Final offset : %d\n", offset);
};
void my_heap_destroy(void) {
    free(heap);
};

