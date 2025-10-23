#include <stdio.h>

#include "block.h"

Block* block_get_next(Block *block) {
    if (block->size == sizeof(Block)) {
        printf("Reached the end of the heap");
        return NULL;
    }
    Block *next = (Block*)(char *)block + block->size;
    return next;
}

void block_split(Block *block, size_t nbytes) {
    int remaining_memory_for_new_block = block->size - sizeof(Block) - nbytes;
    if ((size_t)remaining_memory_for_new_block > (size_t)2 * sizeof(Block)) {                        //If overflow is too small, dont split
        int new_size = nbytes + 2 * sizeof(Block) - nbytes % sizeof(Block);
        Block *new_block = (Block*)(char *)block + new_size;
        new_block->size = block->size - new_size;
        new_block->free = 1;

        block->size = new_size; 
    }
}

