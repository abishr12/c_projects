#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define DEFAULT_ALIGNMENT 8

static unsigned char *arena_buffer;
static size_t arena_buffer_length;
static size_t arena_offset;

void *arena_alloc(size_t size)
{
    // Check to see if the backing memory has space left
    if (arena_offset+size <= arena_buffer_length) {
        void *ptr = &arena_buffer[arena_offset];
        arena_offset += size;
        // Zero new memory by default
        memset(ptr, 0, size);
        return ptr;
    }
    // Return Null if arena is out of memory
    return NULL;
}

bool is_power_of_two(uintptr_t x)
{
    return (x & (x-1)) == 0;
}

uintptr_t align_forward(uintptr_t ptr, size_t align)
{
    uintptr_t p, a, modulo;

    assert(is_power_of_two(align));

    p = ptr;
    a = (uintptr_t)align;

    // Same as p % a but faster as 'a' is a power of two 
    modulo = p & (a-1);

    if (modulo != 0) {
        // if p address is not aligned, push the address to the 
        // next value which is aligned
        p += a - modulo;
    }
}


typedef struct Arena Arena;

struct Arena {
    unsigned char *buf;
    size_t buf_len;
    size_t prev_offset;
    size_t curr_offset;
    
};

void *arena_alloc_align(Arena *a, size_t size, size_t align)
{
    // Align curr_offset forward to the specified alignment
    uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
    uintptr_t offset = align_forward(curr_ptr, align);
    offset -= (uintptr_t)a->buf;

}

void arena_init (Arena *a, void *backing_buffer, size_t backing_buffer_length)
{
    a->buf = (unsigned char *)backing_buffer;
    a->buf_len = backing_buffer_length;
    a->curr_offset = 0;
    a->prev_offset = 0;
}

void arena_free(Arena *a, void *ptr)
{
    // Do nothing - just decoration
}

void *arena_resize_align (Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align)
{
    unsigned char *old_mem = (unsigned char *)old_memory;

    assert(is_power_of_two(align));

    if (old_mem == NULL || old_size == 0) {
        return arena_alloc_align(a,new_size, align);
    } else if (a->buf <= old_mem && old_mem < a->buf+a->buf_len) {
        if (a->buf + a->prev_offset == old_mem ) {
            a->curr_offset = a->prev_offset + new_size;

            if (new_size > old_size) {
                // Zero new mem by default
                memset(&a->buf[a->curr_offset], 0, new_size-old_size);
            }
            return old_memory;
        } else {
            void *new_memory = arena_alloc_align(a, new_size, align);
            size_t copy_size = old_size < new_size ? old_size : new_size;
            // Copy across old memory to new memory
            memmove(new_memory, old_memory, copy_size);
            return new_memory;
        }
    } else {
        assert(0 && "Memory is out of bounds of the bufffer in this arena");
        return NULL;
    }

}

// Because C does not have default params
void *arena_resize(Arena *a, void *old_memory, size_t old_size, size_t new_size) {
    return arena_resize_align(a,  old_memory, old_size, new_size, DEFAULT_ALIGNMENT);
}

void arena_free_all(Arena *a) {
    a->curr_offset = 0;
    a->prev_offset = 0;
}
