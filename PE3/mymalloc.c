#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int has_initialized = 0;

// our memory area we can allocate from, here 64 kB
#define MEM_SIZE (64*1024)
uint8_t heap[MEM_SIZE];

// start and end of our own heap memory area
void *managed_memory_start; 

// this block is stored at the start of each free and used block
// Has a pointer to the next mem_control_block
struct mem_control_block {
  int size;
  struct mem_control_block *next;
};

// pointer to start of our free list
struct mem_control_block *free_list_start;      

void mymalloc_init() { 

  // our memory starts at the start of the heap array
  managed_memory_start = &heap;

  // allocate and initialize our memory control block 
  // for the first (and at the moment only) free block
  struct mem_control_block *m = (struct mem_control_block *)managed_memory_start;
  m->size = MEM_SIZE - sizeof(struct mem_control_block);

  // no next free block
  m->next = (struct mem_control_block *)0;

  // initialize the start of the free list
  free_list_start = m;

  // We're initialized and ready to go
  has_initialized = 1;
  printf("Memory has been initialized\n");
}

void *mymalloc(long numbytes) {
  if (has_initialized == 0) {
     mymalloc_init();
  }

  struct mem_control_block *current = free_list_start;
  struct mem_control_block *previous = (void*)0;
  void *result;

  //We round the number of bytes to a multiple of 8 as specified in the
  // assignment specification. 
  if (numbytes % 8 != 0) {
      numbytes = numbytes + (8 - numbytes % 8);
  }

  //size of the block we want to allocate
  long block = numbytes + sizeof(struct mem_control_block);

 
  //this loop runs while the current free-block is smaller than the block we want to allocate
  while (current && (current->size < block)) {
      previous = current;
      current = current->next;
      //If the current is NULL, we have reached the end of the free list. 
      if (current == (void*)0) {
        printf("We reached the end of the free list\n");
        return (void*)0;
      }
      printf("One iteration of checking blocks\n");
  }
  //if the block we want to allocate fits exact in the free-memory-area
  if (current->size == block) {
    if(current == free_list_start) {
        free_list_start = (void*)0;
    }
    if(previous != (void*)0) {
        previous->next = current->next;
    }
    printf("We found an exact fitting block, and allocated this at address %p\n", current);
    result = (void*)(++current);  // result is the first memory allocation after the mem_control_block
    return result;
  }

  else if(current->size > block) {
      //create new free-space after the allocated block
      struct mem_control_block *new = (void*)((void*)current + block);
      new->size = current->size - block;
      new->next = current->next;
      //if we allocate memory at the start at our memory-segment, we have to update
      //the free-list start
      if(previous == (void*)0) {
        free_list_start = new;
      }
      else {
        previous->next = new;
      }
      result = (void*)(++current);
      printf("We found a fitting block, splitted this, and allocated memory at address %p\n", current);
      return result;
  }

  else {
      return (void*)0;
  }
}

void myfree(void *firstbyte) {

  struct mem_control_block *current, *previous;
  current = firstbyte - sizeof(struct mem_control_block);

  //This would be the case if all the memory addresses is allocated
  if (free_list_start == NULL) {
    free_list_start = current;
    current->next = (struct mem_control_block*)0;
  }

  //If the block we want to free is located before the start of the free-list in memory
  else if (free_list_start > current) {
    //we copy the mem_control_block of the start, to make calculations on it before we
    //make the change the free_list_start to our current mem_control_block
    struct mem_control_block *dupe = free_list_start;
    free_list_start = current;
    //If this is the case, the two blocks is adjacent, and we merge
    if (current + current->size == dupe) {
      current->size += dupe->size;
      current->next = dupe->next;
      printf("Freed memoryblock with address %p\n", firstbyte);
    }
    //We have two non-adjacent blocks, so we just need to set the currents next to 
    //our next, which is our duplicated mem_control_block
    else {
      current->next = dupe;
      printf("Freed memoryblock with address %p\n", firstbyte);
  }
}
  //if the block we want to free is located after the start of the free-list in memory
  else if (free_list_start < current) {
    struct mem_control_block *left, *right;
    left = free_list_start;
    right = left->next;
  
    //We want to find two free memory control blocks surrounding our current memory control block
    //This while loop will iterate so that we make this happen
    while (right != NULL && right < current) {
      left = left->next;
      right = left->next;
    }
    //We now how to check for three different scenarios

    //First scenario. Our current memory control block is adjacent to the right
    //And we merge our current with the right block
    if ((current + current->size == right) && (left + left->size != current)) {
      left->next = current;
      current->next = right->next;
      current->size += right->size;
      printf("Freed memoryblock with address %p\n", firstbyte);
    }
    //Second scenario. Our current memory control block is adjacent to the left
    // and we merge our current with the right block
    else if (left + left->size == current) {
      left->size += current->size;
      printf("Freed memoryblock with address %p\n", firstbyte);
    }
    //Third scenario. The memory control block is adjacent to both the left and the right.
    else if ((left + left->size == current) && (current + current->size == right)) {
        left->next = right->next;
        left->size += current->size + right->size;
        current = left;
        printf("Freed memoryblock with address %p\n", firstbyte);
    }
    //Final scenario. Our current memory control block is not adjacent to any free blocks, 
    //but placed in between. 
    else {
      left->next = current;
      current->next = right;
      printf("Freed memoryblock with address %p\n", firstbyte);
    }
  }
  //If something funny has happened
  else {
      return (void)0;
  }
}

int main(int argc, char **argv) {


  printf("Test 1: Try to allocate more bytes than available\n\n");
  mymalloc(64*1024);
  printf("\n");

  printf("Test 2: Try to add multiple blocks of data\n\n");
  mymalloc_init();
  mymalloc(8);
  mymalloc(128);
  mymalloc(1024);
  printf("\n");

  printf("Test 3: Add multiple blocks of data, then add a block which is too large\n\n");
  mymalloc_init();
  mymalloc(1024);
  mymalloc(128);
  mymalloc(1024);
  mymalloc(64*1024);
  printf("\n");
 
  printf("Test 4: Add multiple blocks of data that fits perfectly\n\n");
  mymalloc_init();
  mymalloc(64*1024-32);
  printf("\n");


  printf("Test 5: Remove a block to adjecent the left of the free list\n\n");
  mymalloc_init();
  mymalloc(1024);
  mymalloc(128);
  void*(v) = mymalloc(1024);
  myfree(v);
  printf("\n");

  printf("Test 6: Remove a block adjecent to the right of the free list\n\n");
  mymalloc_init();
  void*(a) = mymalloc(1024);
  void*(b) = mymalloc(128);
  void*(c) = mymalloc(1024);
  myfree(b);
  myfree(c);
  mymalloc(8);
  printf("\n");

  printf("Test 7: Remove a block adjecent to the free list at both left and right\n\n");
  mymalloc_init();
  mymalloc(64);
  void*(d) = mymalloc(1024);
  void*(e) = mymalloc(128);
  void*(f) = mymalloc(1024);
  myfree(d);
  myfree(f);
  myfree(e);
  mymalloc(8);
  printf("\n");

  printf("Test 8: Remove a block which is non-adjecent to the free list\n\n");
  mymalloc_init();
  mymalloc(64);
  void*(h) = mymalloc(1024);
  mymalloc(128);
  void*(i) = mymalloc(128);
  mymalloc(128);
  void*(j) = mymalloc(1024);
  myfree(j);
  myfree(h);
  myfree(i);
  mymalloc(1024);
  printf("\n");
 
}