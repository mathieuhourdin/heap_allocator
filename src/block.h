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

#endif
