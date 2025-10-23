#include <stdio.h>

#include "block.h"

Block* block_new(void *position, int size) {
    Block *block = (Block*)position;
    block->size = size;
    block->free = 0;
    return block;
}

Block* block_get_next(Block *block) {
    if (block->size == sizeof(Block)) {
        printf("Reached the end of the heap");
        return NULL;
    }
    Block *next = (Block*)(char *)block + block->size;
    return next;
}

Block* block_merge_right(Block *first_block, Block *second_block) {
    first_block->size = second_block->size;
    return first_block;
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

