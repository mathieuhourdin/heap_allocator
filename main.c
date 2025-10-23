#include<stdio.h>
#include<stdbool.h>

#include "src/block.h"
#include "src/heap.h"

#define HEAP_SIZE 1024


void* my_alloc(size_t nbytes) {
    Block *matching_block = heap_get_next_matching_block(nbytes);
    block_split(matching_block, nbytes);
    matching_block->free = 0;
    void *user_ptr = ((char *)matching_block + sizeof(Block));
    return user_ptr;
};

void my_free(void* ptr) {
    Block *block = (Block*)((char *)ptr - sizeof(Block));
    Block *next = block_get_next(block);
    if (next->free) {
        block->size += next->size;
    }
    block->free = 1;
};

int main() {

    printf("Sizeof Block : %d\n", (int)sizeof(Block));
    my_heap_init(HEAP_SIZE);
    my_heap_dump();
    void *ptr_1 = my_alloc(24);
    void *ptr_2 = my_alloc(45);
    my_heap_dump();
    printf("Ptr_1 : %p\n", ptr_1);
    printf("Ptr_2 : %p\n", ptr_2);
    my_heap_dump();
    my_free(ptr_2);
    my_heap_dump();
    my_heap_destroy();

}
