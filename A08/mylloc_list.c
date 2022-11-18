#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Holds structure for a memory chunk
struct chunk {
  // Holds value for number of memory bytes in chunk
  int size;
  // Holds value for used memory bytes in chunk
  int used;
  // Holds value for next block in the free list
  struct chunk *next;
};
// Holds value for the free list pointer
struct chunk *flist = NULL;

// Allocates memory depending on if memory blocks can be
// reused or not
void *malloc (size_t size) {
  // Checks if there are bytes allocated or not
  if(size == 0) {
    return NULL;
  }
  // Holds value for flist pointer
  struct chunk *next = flist;
  // Sets the previous chunk value to NULL
  struct chunk *prev = NULL;

// Moves through each chunk and checks if the chunk
// from the free list can be reused
  while(next != NULL) {
    // Checks if the allocated memory can fit in this chunk;
    // allocates memory to the block, if so
    if(next->size >= size) {
      next->used = size;
      if(prev != NULL) {
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      // Returns the new chunk
      return (void*)(next + 1);
    // Removes the memory chunk from the free list
    } else {
      prev = next;
      next = next->next;
    }
  }

  // Holds value for the size of a chunk and the allocated memory
  // for a new block
  void *memory = sbrk(size + sizeof(struct chunk));
  if(memory == (void *)-1) {
    return NULL;
  } else {
    // Creates a new memory block if there is no suitable
    // memory block
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    cnk->used = size;
    return (void*)(cnk + 1);
  }
}

// Frees values in memory
void free(void *memory) {
  // Switches pointer to the head of the list provide memory is not null
  if(memory != NULL) {
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}
