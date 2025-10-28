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

/**
 * @brief deals with the freeing of a block
 *
 * This method is responsible of maintaining the invariant that if a block is free, the next has left_free to true.
 * Then it contains the logic to set next block to left_free.
 *
 * @param block the block to free.
 *
 * @return void
 */
void block_free(Block *block) {
    block->free = 1;
    Block *next_block = block_get_next(block);
    if (next_block) {
        next_block->left_free = 1;
    }
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

/**
 *
 * @brief performs the freeing and merging of a given block
 *
 * Only triggers a block_merge_left if the left block is free.
 * Also to avoid using the O(n/2) complexity function if not useful.
 *
 * Only triggers a block_merge_right if right block is free.
 *
 */

void block_free_and_merge(Block *block) {

    block_free(block);

    Block *next = block_get_next(block);
    if (next && next->free) {
        block_merge_right(block);
    }
    //here we loose ownership of block. Do it at the end only.
    if (block->left_free) {
        block_merge_left(block);
    }
}


/**
 * @brief merges two blocks in the left block.
 *
 * It can be applied to any block. IT
 */

Block* block_merge_right(Block *block) {
    Block *right_block = block_get_next(block);
    if (right_block) {
        block->size += right_block->size;
    }
    return block;
}


/**
 *
 * @brief merge with left block if free
 *
 * Loops through the heap to find the block on the left of the current block and merge with it.
 *
 * This method is safe : it checks if the current block is free and it checks that it is not the first block of the heap.
 * It also checks if we reach the given block during the loop. If we dont, it doesn't merge the block.
 * Maybe contract is not very clear here. 
 * We should precise if the given and the left blocks are supposed to be free or if the method is responsible of the checks.
 *
 * It has a complexity of n/2 with n the number of blocks in the heap because we loop through the heap.
 * It is also not very robust : if there is some left space between two blocks anywhere in the heap before the block, the block could never be found.
 * Maybe we should find a way to keep a direct reference to the left block, by storing left block size.
 *
 * We also could choose to return a reference to the left block
 *
 * @params block pointer to the current block.
 *
 * @return void
 *
 */
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
        block_merge_right(left_block);
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

