#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100
#define INVALID_PAGE_NUM UINT32_MAX

typedef enum { NODE_INTERNAL, NODE_LEAF } NodeType;

const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET = INTERNAL_NODE_NUM_KEYS_OFFSET + INTERNAL_NODE_NUM_KEYS_SIZE;
const uint32_t INTERNAL_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + INTERNAL_NODE_NUM_KEYS_SIZE + INTERNAL_NODE_RIGHT_CHILD_SIZE;
const uint32_t INTERNAL_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CELL_SIZE = INTERNAL_NODE_CHILD_SIZE + INTERNAL_NODE_KEY_SIZE;

const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;

NodeType get_node_type(void* node) {
  return (NodeType)*((uint8_t*)(node + NODE_TYPE_OFFSET));
}

uint32_t* internal_node_num_keys(void* node) {
  return node + INTERNAL_NODE_NUM_KEYS_OFFSET;
}

uint32_t* internal_node_right_child(void* node) {
  return node + INTERNAL_NODE_RIGHT_CHILD_OFFSET;
}

uint32_t* internal_node_cell(void* node, uint32_t cell_num) {
  return node + INTERNAL_NODE_HEADER_SIZE + cell_num * INTERNAL_NODE_CELL_SIZE;
}

uint32_t* leaf_node_num_cells(void* node) {
  return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s <db_file>\n", argv[0]);
    return 1;
  }
  
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
  
  void* pages[10];
  for (int i = 0; i < 10; i++) {
    pages[i] = malloc(PAGE_SIZE);
    ssize_t r = pread(fd, pages[i], PAGE_SIZE, i * PAGE_SIZE);
    if (r <= 0) break;
    
    void* node = pages[i];
    NodeType type = get_node_type(node);
    
    printf("Page %d: ", i);
    if (type == NODE_LEAF) {
      printf("LEAF, num_cells=%u\n", *leaf_node_num_cells(node));
    } else {
      uint32_t num_keys = *internal_node_num_keys(node);
      uint32_t right_child = *internal_node_right_child(node);
      printf("INTERNAL, num_keys=%u, right_child=%u, children: ", num_keys, right_child);
      for (uint32_t j = 0; j < num_keys; j++) {
        uint32_t* cell = internal_node_cell(node, j);
        printf("%u ", *cell);
      }
      printf("\n");
    }
  }
  
  close(fd);
  return 0;
}
