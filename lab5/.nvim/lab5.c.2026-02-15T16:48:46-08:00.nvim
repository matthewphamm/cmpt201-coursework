#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement first fit
  // Algorithm: Find the first block that is big enough or equal
  //             Traverse the list starting at free_list_ptr
  //             Check if the size is big enough if not go next
  //             until there is enough size
  //             If there is not enough size return -1 (failed to find free
  //             block)

  int current_id = 0;
  uint64_t current_size = 0;
  struct header *current = free_list_ptr;
  current_id = current->id;
  current_size = current->size;

  // Check first block
  if (current_size >= size) {
    return current_id;
  }

  while (current->next != NULL) {
    current = current->next;
    current_size = current->size;
    current_id = current->id;
    if (current_size >= size) {
      return current_id;
    }
  }

  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  // TODO: Implement best fit
  // Algorithm: Find the smallest free block that is big enough
  //             Traverse the entire list
  //             Find a big enough or equal space for size
  //             Calculate the leftover and compare the minimum
  //             Which finds the smallest free block that fits size
  //             If there is not enough size return -1 (failed to find free
  //             block)

  uint64_t leftover = 0;
  uint64_t minimum_remaining = UINT64_MAX;
  uint64_t current_size = 0;
  struct header *current = free_list_ptr;
  current_size = current->size;

  while (current->next != NULL) {
    if (current_size >= size) {
      leftover = current_size - size;
      if (leftover < minimum_remaining) {
        best_fit_id = current->id;
        minimum_remaining = leftover;
      }
    }
    current = current->next;
    current_size = current->size;
  }

  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  // TODO: Implement worst fit
  // Algorithm: Find the largest free block.
  //             Traverse the entire list
  //             Find a big enough or equal space for size
  //             Calculate the leftover and compare maximum
  //             Which finds the largest free block that fits size
  //             If there is not enough size return -1 (failed to find free
  //             block)

  uint64_t leftover = 0;
  uint64_t maximum_remaining = 0;
  uint64_t current_size = 0;
  struct header *current = free_list_ptr;
  current_size = current->size;

  while (current->next != NULL) {
    if (current_size >= size) {
      leftover = current_size - size;
      if (leftover > maximum_remaining) {
        worst_fit_id = current->id;
        maximum_remaining = leftover;
      }
    }
    current = current->next;
    current_size = current->size;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2,
                   1); // means -> free_block1 is the header its
                       // size is 6, its next is free_block2, and
                       // id is 1.
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // TODO: Print out the IDs
  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
 * Coalescing Contiguous Free Blocks Algorithm:
 *      - Once a process terminates and free its memory, we do two checks:
 *          1. Check Behind: If the memory block that is before the newly freed
 *             block is also free. Then merge them into one big block.
 *          2. Check Ahead: If the memory block that is after the newly freed
 * block is also free. Then merge them into one big block.
 *          3. After coalescing contiguous free blocks. Update the list of freed
 * memory to show one large free block.
 *
 * Coalescing Contiguous Free Blocks PseudoCode:
 *
 * function free_memory(block to free):
 *  set block to free's is free to True
 *
 *  if block to free's next is not NULL and block to free next is free then
 *      block to free's size += block to free next's size
 *      temp = block to free's next
 *      block to free next's = temp's next
 *
 *      if temp's next is not NULL then
 *        temp's next prev is block to free
 *
 *  if block to free's prev is not NULL and block to free prev is free then
 *      block to free prev's size += block to free's size
 *      block to free prev's size next = block to free's next
 *
 *      if block to free next is not NULL then
 *        block to free next prev is set to block to free prev
 *
 * DONE
 */
