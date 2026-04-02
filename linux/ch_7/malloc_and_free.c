#include <stdlib.h>
#include <unistd.h>

typedef struct block_header {
    size_t size; // Size of usable memory (sans header)
    struct block_header *next;            // Pointer to the next free block (only used when free)
} block_header;

static block_header *free_list = NULL;

void *my_malloc(size_t size){

    block_header *current = free_list;
    block_header *previous = NULL;


    // Search free list for block that fits
    while (current != NULL) {
        if (current->size >= size) {
            // found one - remove from free list
            if (previous == NULL) {
                // it was the head
                free_list = current->next;
            } else {
                // Skip over it
                previous->next = current->next;
            }
            // return pointer after header
            return (void *)(current + 1);
        }
        previous = current;
        current = current->next;
    }

    // No free block found - ask kernel for memory
    size_t total = sizeof(block_header) + size;
    block_header *new_block = sbrk(total);

    if (new_block == (void *)-1){
        // sbrk failed
        return NULL; 
    }

    new_block->size = size;
    new_block->next = NULL;

    return (void *)(new_block+1);
}


void my_free(void *ptr){
    
    if (ptr == NULL) {
        return;
    }

    // Step back to find block_header
    block_header *block = (block_header *)ptr - 1;

    block->next = free_list;
    free_list = block;

}
