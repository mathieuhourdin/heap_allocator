#ifndef BLOCK_H
#define BLOCK_H
#include<stdbool.h>
#include<stdlib.h>

typedef struct Block {
    int size;
    bool free;
} Block;


Block* block_get_next(Block *block);
void block_split(Block *block, size_t nbytes);
Block* block_new(char *position, int size);
Block* block_merge_right(Block *first_block, Block *second_block);

#endif
