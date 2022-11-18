#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

// Prints out stats for memory bytes and memory blocks
void memstats(struct chunk* freelist, void* buffer[], int len) {
  // Holds the value for total memory blocks
  int totalblocks = 0;
  // Holds value for free memory blocks
  int freeblocks = 0;
  // Holds value for used memory blocks
  int usedblocks = 0;

  // Holds value for total memory bytes
  int totalmem = 0;
  // Holds value for free memory bytes
  int freemem = 0;
  // Holds value for used memory bytes
  int usedmem = 0;

  // Goes through each value in the buffer
  for(int i = 0; i < len; i++) {
    if (buffer[i] != NULL) {
      // Calculation for total memory blocks
      totalblocks++;
      // Calculation for used memory blocks
      usedblocks++;
      // Calculation for used memory
      usedmem = usedmem + *(int*)buffer[i];
    }
  }

  // Checks that freelist exists
  while (freelist != NULL) {
    // Calculation for total memory blocks
    totalblocks++;
    // Calculation for free memory blocks
    freeblocks++;
    // Calculation for free memory
    freemem = freemem + freelist->size;
    // Calculation for used memory
    usedmem = usedmem + freelist->used + freelist->size;
    // Calculation for total memory
    totalmem = freemem + usedmem;
    // Moves to next item in the free list
    freelist = freelist->next;
  }

  // Calculates for underutilized memory
  int underMem = usedmem / totalmem;

  printf("Total Blocks: %d Free Blocks %d Used Blocks: %d\n", totalblocks, freeblocks, usedblocks);
  printf("Total Memory Allocated: %d Free Memory: %d Used Memory: %d\n", totalmem, freemem, usedmem);
  printf("Underutilized Memory: %d\n", underMem);

}

int main (int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
