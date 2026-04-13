#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define DEFAULT_ALIGNMENT 8

typedef struct Stack Stack;
struct Stack {
    unsigned char *buf;
    size_t buf_len;
    size_t offset;
    size_t prev_offset;
};

typedef struct Stack_Allocation_Header Stack_Allocation_Header;
struct Stack_Allocation_Header {
    uint8_t padding;
    size_t prev_offset;
};

void stack_init(Stack *s, void *backing_buffer, size_t backing_buffer_length) {
    s->buf = (unsigned char *)backing_buffer;
    s->buf_len = backing_buffer_length;
    s->offset = 0;
}

bool is_power_of_two(uintptr_t x)
{
    return (x & (x-1)) == 0;
}

size_t calc_padding_with_header(uintptr_t ptr, uintptr_t alignment, size_t header_size) {
    uintptr_t p, a, modulo, padding, needed_space;

    assert(is_power_of_two(alignment));

    p = ptr;
    a = alignment;
    // p%a as it assumes alignment is power of two
    modulo = p & (a-1); 
    
    padding = 0;
    needed_space = 0;

    if (modulo != 0) {
        padding = a - modulo;        
    }

    needed_space = (uintptr_t)header_size;

    if (padding < needed_space) {
        needed_space -= padding;

        if ((needed_space & (a-1)) != 0) {
            padding += a * (1+(needed_space/a));
        } else {
            padding += a * (needed_space/a);
        }
    }

    return (size_t)padding;
}

void *stack_alloc_align(Stack *s, size_t size, size_t alignment) {
    uintptr_t curr_addr, next_addr;
    size_t padding;
    Stack_Allocation_Header *header;

    assert(is_power_of_two(alignment));

    if (alignment > 128) {
        // Largest alignment can only be 128 because padding is 
        // 8 bits (1 byte)
        alignment = 128;
    }

    curr_addr = (uintptr_t)s->buf + (uintptr_t)s->offset;
    padding = calc_padding_with_header(curr_addr, (uintptr_t)alignment, sizeof(Stack_Allocation_Header));
    if (s->offset + padding + size > s->buf_len) {
        // Stack allocator has run out of memory
        return NULL;
    }
    s->prev_offset = s->offset;
    s->offset += padding;

    next_addr = curr_addr + (uintptr_t)padding;
    header = (Stack_Allocation_Header *)(next_addr - sizeof(Stack_Allocation_Header));
    header->padding = (uint8_t)padding;
    // store the previous offset in header
    header->prev_offset = s->prev_offset;

    s->offset += size;

    return memset((void *)next_addr, 0, size);
}

void stack_free(Stack *s, void *ptr) {
    if (ptr != NULL) {
        uintptr_t start, end, curr_addr;
        Stack_Allocation_Header *header;
        size_t prev_offset;

        start = (uintptr_t)s->buf;
        end = start + (uintptr_t)s->buf_len;
        curr_addr = (uintptr_t)ptr;

        if(!(start <= curr_addr && curr_addr < end)) {
            assert(0 && "Out of bounds memory address passed to stack allcator (free)");
            return;
        }

        if (curr_addr >= start+(uintptr_t)s->offset) {
            // Allow double frees
            return;
        }

        header = (Stack_Allocation_Header *)(curr_addr - sizeof(Stack_Allocation_Header));
        prev_offset = (size_t)(curr_addr - (uintptr_t)header->padding - start);

        if(prev_offset != header->prev_offset) {
            assert(0 && "Out of order stack allocator free");
            return;
        }
        // Rest offsets to the previous allocation
        s->offset = s->prev_offset;
        s->prev_offset = header->prev_offset;
    }
}

void *stack_resize_align (Stack *s, void *ptr, size_t old_size, size_t new_size, size_t alignment)
{
    if (ptr == NULL) {
        return stack_alloc_align(s, new_size, alignment);
    } else if (new_size == 0) {
        stack_free(s, ptr);
        return NULL;
    } else {
        uintptr_t start, end, curr_addr;
        size_t min_size = old_size < new_size ? old_size: new_size;
        void *new_ptr;

        start = (uintptr_t)s->buf;
        end = start + (uintptr_t)s->buf_len;
        curr_addr = (uintptr_t)ptr;
        if(!(start <= curr_addr && curr_addr < end)) {
            assert(0 && "Out of bounds memory address passed to stack allocator (resize)");
            return NULL;
        }

        if (curr_addr >= start + (uintptr_t)s->offset) {
            // treat as a double free
            return NULL;
        }

        if (old_size == new_size) {
            return ptr;
        }

        new_ptr = stack_alloc_align(s, new_size, alignment);
        memmove(new_ptr, ptr, min_size);
        return new_ptr;
    }
}

void *stack_resize(Stack *s, void *ptr, size_t old_size, size_t new_size) {
	return stack_resize_align(s, ptr, old_size, new_size, DEFAULT_ALIGNMENT);
}

void stack_free_all(Stack *s) {
    s->offset = 0;
}
