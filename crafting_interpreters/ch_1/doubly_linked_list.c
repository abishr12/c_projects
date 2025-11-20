/*
To get some practice with pointers, define a
doubly-linked list of heap-allocated strings. Write functions to insert, find,
and delete items from  it. Test them.
*/
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char *data;        // Pointer to a heap-allocated string
  struct Node *next; // Pointer to the next node
  struct Node *prev; // Pointer to the previous node
} Node;

typedef struct {
  Node *head; // Points to the first node
  Node *tail; // Points to the last node (optional, but makes insertions at the
              // end faster)
} DoublyLinkedList;

Node *create_node(const char *data) {
  // Allocate memory for the node
  Node *new_node = (Node *)malloc(sizeof(Node));
  
  // Always check if malloc succeeded!
  if (new_node == NULL) {
      return NULL;
    }
    
  new_node->data = (char *)malloc(strlen(data) + 1);

  // Second check
  if (new_node->data == NULL) {
    free(new_node); // Clean up the first allocation!
    return NULL;    // Second malloc failed
  }

  strcpy(new_node->data, data);

  // Now allocate and copy the string
  // hint: strlen(data) + 1 for the null terminator
  // hint: use strcpy or strdup

  new_node->next = NULL;
  new_node->prev = NULL;

  return new_node;
}

DoublyLinkedList *create_list(void) {
  // 1. Allocate memory for the list structure
  DoublyLinkedList *new_list = malloc(sizeof(*new_list));

  // 2. Check if malloc succeeded
  if (new_list == NULL) {
    return NULL;
  }

  // 3. Initialize head and tail (what should they be for an empty list?)
  new_list->head = NULL;
  new_list->tail = NULL;


  // 4. Return the list
  return new_list;
}

void insert(DoublyLinkedList *list, const char *data) {
    Node *new_node = create_node(data);

    // Check if node creation failed
    if (new_node == NULL) {
      return; // Can't insert, malloc failed
    }

    // if list is empty insert at head
    // else insert at tail
    if (list->head == NULL) {
      list->head = new_node;
      list->tail = new_node;
    } else {
       Node *old_tail = list->tail;
       old_tail->next = new_node;
       new_node->prev = old_tail;
       list->tail = new_node;
    }
}

Node *find(DoublyLinkedList *list, const char *data) {

    Node *current = list->head;

    while (current != NULL) {
        if(strcmp(current->data, data) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool delete(DoublyLinkedList *list, const char *data) {
  // if list is empty just return false
  if (list->head == NULL) {
    return false;
  }
  Node *node_to_delete = find(list, data);

  if (node_to_delete == NULL) {
    return false; // Not found
  }

  // if node is the only node in the list
  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
  }

  else if (list->head == node_to_delete) {
    list->head = list->head->next;
    list->head->prev = NULL;
  }

  else if (list->tail == node_to_delete) {
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  }
  // node is in the middle
  else {
    node_to_delete->prev->next = node_to_delete->next;
    node_to_delete->next->prev = node_to_delete->prev;
  }
  free(node_to_delete->data); // Free the string first
  free(node_to_delete);
  return true;
}