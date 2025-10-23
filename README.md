## Memory allocator design document

This memory allocator manages memory for C programs.

I create a contiguous allocation system. All memory is covered by the blocs. They dont have fixed sizes.
first block is always at the start of the heap.

The heap is divided in different segments called blocks. The blocks are a partition of the heap.
A block has two part : the header that stores metadata and the part allocated to user data.
The heap always ends with a block of user data length of zero.

Block have no fixed size, but they have to be multiple of the header size (8bytes currently).
Each block of data has a header with metadata : size of the block including header, status of use.

### Invariants : 
 - Each byte of the heap belongs to one and only one block;
 - The sum of the size of all blocks including the headers equals the size of the heap;
 - The address of each block sorted in the linked list should be increasing.
 - Blocks are contiguous : a new block starts when the previous ends

### Operations on the heap :
- Split operation (block split) preserves those invariants, by creating two blocks which sizes summed equals the originary block size and exactly separated by length bytes on the heap.
- Merge right operation (block merge right) preserves the invariants, by assigning a size of the remaining block equals to the previous size plus merged block size. Merged block bytes are not erased but the block is removed from block geometrical list by assigning a bigger length to the previous one.

Block are allocated on first match based on the way we loop through the array.
Once a matching block is found, if usefull we split it to the upper mulptiple of header size and create a new free block if needed.

to free a block mostly sets its free field to true, and it merges it with the next block on the right side if it is free too. 
One more advanced implementation should also try to merge blocks on the left, maybe by referencing the previous block too.
