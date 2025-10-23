#include<stdlib.h>
#include<stdio.h>

#define HEAP_SIZE 1024

static const int sizes[] = {16, 32, 64, 128, 256, 512};


typedef struct Block {
    int size;
    int free;
    struct Block* next;
} Block;

typedef struct HeapMetadata {
    Block* block_types[6];
    size_t left_size;
} HeapMetadata;

void* my_heap_init(size_t size) {
    void *heap = malloc(size);
    HeapMetadata *heap_metadata = heap;
    for (int i = 0; i < 6; i++) {
        heap_metadata->block_types[i] = NULL;
    }
    heap_metadata->left_size = size - sizeof(HeapMetadata);

    int block_size = heap_metadata->left_size / 2;
    Block* block_head = heap + sizeof(HeapMetadata);

    for (int i = 5; i >= 0; i--) {
        while (heap_metadata->left_size >= sizes[i]) {
            Block *new_block = block_head;
            new_block->size = sizes[i];
            new_block->free = 1;
            new_block->next = NULL;
            if (heap_metadata->block_types[i]) {
                new_block->next = heap_metadata->block_types[i];
            }
            heap_metadata->block_types[i] = new_block;
            block_head = block_head + sizes[i];
            heap_metadata->left_size -= sizes[i];
        }
    }
    return heap;
}

void my_heap_dump(void* heap) {
    HeapMetadata *heap_metadata = (HeapMetadata*)heap;
    printf("Size of heap metadatas : %d\n", (int)sizeof(HeapMetadata));
    printf("Size of blocks metadatas : %d\n", (int)sizeof(Block));
    printf("Heap left size : %d\n", (int)heap_metadata->left_size);
    for (int i = 0; i < 6; i++) {
        Block *next_block = heap_metadata->block_types[i];
        while (next_block) {
            printf("Block of size %d at address %p is free\n", sizes[i], next_block);
            next_block = next_block->next;
        }
        
    }
};

Block* get_block_of_size(HeapMetadata *heap_metadata, int index) {
    if (index < 0) {
        return NULL;
    }
    if (heap_metadata->block_types[index]) {
        return heap_metadata->block_types[index];
    }
    if (index > 4) {
        return NULL;
    }
    Block* next_level_block = get_block_of_size(heap_metadata, index + 1);
    if (next_level_block) {
        //split the found block in two blocks
        heap_metadata->block_types[index + 1] = next_level_block->next;

        Block *new_block_1 = next_level_block;
        Block *new_block_2 = (Block*)((char *)next_level_block + sizes[index]);
        new_block_1->free = 1;
        new_block_1->size = sizes[index];
        new_block_1->next = new_block_2;
        new_block_2->free = 1;
        new_block_2->size = sizes[index];
        new_block_2->next = heap_metadata->block_types[index];
        heap_metadata->block_types[index] = new_block_1;
        return new_block_1;
    }
    return NULL;
}

void* my_alloc(void* heap, size_t nbytes) {
    HeapMetadata *heap_metadata = (HeapMetadata*)heap;
    int i = 0;
    int block_min_size = nbytes + sizeof(Block);
    while (i < 6) {
        //try different block sizes until a size sufficient for block metadata + nbytes is found
        //then find a block
        if (block_min_size < sizes[i]) {
            Block *found_block = get_block_of_size(heap_metadata, i);

            //my_heap_dump(heap);

            if (found_block) {
                heap_metadata->block_types[i] = found_block->next;
                found_block->free = 0;
                printf("Choosed block : %p\n", found_block);
                void *ptr = (char *)found_block + sizeof(Block);
                return ptr;
            } else {
                //if no block is found, exit (should send a message that not enougth memory is left)"
                fprintf(stderr, "Error : no block left with sufficient size\n");
                exit(1);
            }
        }
        i++;
    }
    //if no sufficient size is found, exit (should send an error message saying that requested size to allocate is too big)"
    fprintf(stderr, "Error : size is too big for the memory allocated to this process\n");
    exit(1);
};

void   my_free(void* heap, void* ptr) {
    HeapMetadata *heap_metadata = heap;

    void *block_void = (char *)ptr - sizeof(Block);
    printf("Block : %p\n", block_void);

    Block *block = (Block*)block_void;

    int size = block->size;
    printf("Size : %d\n", size);
    for (int i = 0; i < 6; i++) {
        if (size == sizes[i]) {
            block->free = 1;
            block->next = heap_metadata->block_types[i];
            heap_metadata->block_types[i] = block;
            return;
        }
    }
    exit(1);
};


void   my_heap_destroy(void* heap) {
    free(heap);
}


int main() {
    void* heap = my_heap_init(HEAP_SIZE);

    my_heap_dump(heap);
    printf("before alloc\n");
    void *ptr_1 = my_alloc(heap, 10);
    void *ptr_2 = my_alloc(heap, 5);
    void *ptr_3 = my_alloc(heap, 20);
    void *ptr_4 = my_alloc(heap, 22);
    void *ptr_5 = my_alloc(heap, 5);
    void *ptr_6 = my_alloc(heap, 6);
    void *ptr_7 = my_alloc(heap, 7);
    void *ptr_8 = my_alloc(heap, 8);
    void *ptr_9 = my_alloc(heap, 9);
    printf("9\n");
    void *ptr_10 = my_alloc(heap, 10);
    void *ptr_11 = my_alloc(heap, 11);
    void *ptr_12 = my_alloc(heap, 12);
    printf("Last dump before crash \n");
    my_heap_dump(heap);
    printf("After Last dump before crash \n");
    void *ptr_13 = my_alloc(heap, 10);
    printf("After alloc\n");
    my_heap_dump(heap);
    printf("After dump\n");
    my_free(heap, ptr_1);
    my_heap_dump(heap);
    my_free(heap, ptr_2);
    my_heap_dump(heap);
    my_free(heap, ptr_3);
    my_heap_dump(heap);
    my_free(heap, ptr_4);
    my_heap_dump(heap);
    my_free(heap, ptr_5);
    my_heap_dump(heap);
    my_free(heap, ptr_6);
    my_heap_dump(heap);
    my_free(heap, ptr_7);
    my_heap_dump(heap);
    my_free(heap, ptr_8);
    my_free(heap, ptr_9);
    my_free(heap, ptr_10);
    my_free(heap, ptr_11);
    my_free(heap, ptr_12);
    my_free(heap, ptr_13);
    my_heap_dump(heap);
    void *ptr_14 = my_alloc(heap, 300);
    printf("last pointer");
    my_heap_destroy(heap);

    return 0;
}
