#include <stdio.h>

#include "block.h"

Block* block_new(char *position, int size) {
    Block *block = (Block*)position;
    block->size = size;
    block->free = 1;
    return block;
}

Block* block_get_next(Block *block) {
    char *intermediate = (char *)block + (size_t)block->size;
    Block *test_block = (Block *)intermediate;
    if (test_block->size == sizeof(Block)) {
        printf("Reached the end of the heap");
        return NULL;
    }
    //Block *next = (Block*)(char *)block + block->size;
    return test_block;
}

Block* block_merge_right(Block *first_block, Block *second_block) {
    first_block->size += second_block->size;
    return first_block;
}

void block_split(Block *block, size_t nbytes) {
    int remaining_memory_for_new_block = block->size - sizeof(Block) - nbytes;
    if ((size_t)remaining_memory_for_new_block > (size_t)2 * sizeof(Block)) {                        //If overflow is too small, dont split
        int new_size = nbytes + 2 * sizeof(Block) - nbytes % sizeof(Block);
        block_new((char *)block + new_size, block->size - new_size);
        block->size = new_size; 
    }
}

