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
}

void *mymalloc(long numbytes) {
  if (has_initialized == 0) {
     mymalloc_init();
  }

  /* add your code here! */
  struct mem_control_block *current, *previous;
  current = free_list_start;
  void *result;

  while(current && (current->size) < numbytes) { //must add statement to check if we are at the end of the freelist
      previous = current;
      current = current->next;
      printf("One block checked\n")
  }

  if(current->size == numbytes) { //allocate exact fitting memory block
      result = (void*)(++current);
      previous->next = current->next;
      printf("We found an exact fitting block, and allocated this!");
      return result;
  }

  else if(current->size > (numbytes + sizeof(struct mem_control_block))) { //split the block into two pieces
      if((numbytes % 8) != 0) {
          numbytes = numbytes + 8 - (numbytes % 8)
      }
      struct mem_control_block *new = (void*)((void*)current+numbytes+sizeof(struct mem_control_block));
      new->size = current->size-numbytes-sizeof(struct mem_control_block);
      new->next = current->next;
      previous->next = new;
      result = (void*)(++current);
      printf("We found a splitting block, splitted this and allocated memory");
      return result;
  }

  else {
      result = (void*)0;
      printf("No memory to allocate");
      return result;
  }

}

void myfree(void *firstbyte) {

  /* add your code here! */

}

int main(int argc, char **argv) {

  /* add your test cases here! */

}