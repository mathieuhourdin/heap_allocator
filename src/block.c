#include <stdio.h>

#include "block.h"
#include "heap.h"

Block* block_new(char *position, int size) {
    Block *block = (Block*)position;
    block->size = size;
    block->free = 1;
    block->left_free = 0;
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

void block_free(Block *block) {
    Block *next = block_get_next(block);
    if (next->free) {
        block_merge_right(block, next);
    } else {
        next->left_free = 1;
    }
    block->free = 1;
    //here we loose ownership of block. Do it at the end only.
    if (block->left_free) {
        block_merge_left(block);
    }
}

Block* block_merge_right(Block *first_block, Block *second_block) {
    first_block->size += second_block->size;
    return first_block;
}

void block_merge_left(Block *block) {
    if (block->free == 0) {
        return;
    }
    Block *left_block = (Block *)heap;
    if (left_block == block) {
        return;
    }
    Block *test_block = block_get_next(left_block);
    while (test_block != NULL && test_block != block) {
        left_block = test_block;
        test_block = block_get_next(left_block);
    }
    if (test_block == block && left_block->free == 1) {
        block_merge_right(left_block, block);
    }
}

void block_split(Block *block, size_t nbytes) {
    int remaining_memory_for_new_block = block->size - sizeof(Block) - nbytes;
    if ((size_t)remaining_memory_for_new_block > (size_t)2 * sizeof(Block)) {                        //If overflow is too small, dont split
        int new_size = nbytes + 2 * sizeof(Block) - nbytes % sizeof(Block);
        block_new((char *)block + new_size, block->size - new_size);
        block->size = new_size; 
    }
}

