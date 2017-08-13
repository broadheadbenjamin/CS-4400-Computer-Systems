/*
 * mm-naive.c - The least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by allocating a
 * new page as needed.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "mm.h"
#include "memlib.h"

/* Macros */
#define ALIGNMENT           16                                                          // always use 16-byte alignment
#define ALIGN(size)         (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))                 // rounds up to the nearest multiple of ALIGNMENT
#define PAGE_ALIGN(size)    (((size) + (mem_pagesize()-1)) & ~(mem_pagesize()-1))       // rounds up to the nearest multiple of mem_pagesize()
#define CHUNK_SIZE          (1 << 14)                                                   // Chunk size
#define CHUNK_ALIGN(size)   (((size)+(CHUNK_SIZE-1)) & ~(CHUNK_SIZE-1))                 // Alligns chunks to the correct size
#define OVERHEAD            (sizeof(block_header)+sizeof(block_footer))                 // Overhead for a block_header
#define HDRP(bp)            ((char *)(bp) - sizeof(block_header))                       // Macro for getting the header from a payload pointer
#define GET_SIZE(p)         ((block_header *)(p))->size                                 // Gets the the size of a payload
#define GET_ALLOC(p)        ((block_header *)(p))->allocated                            // Gets to allocated info for a payload
#define NEXT_BLKP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)))                         // Macro for getting the next block's payload
#define PREV_BLKP(bp)       ((char *)(bp)-GET_SIZE((char *)(bp)-OVERHEAD))              // Points to the previous block payload
#define FTRP(bp)            ((char *)(bp)+GET_SIZE(HDRP(bp))-OVERHEAD)                  // Points to the footer for a given payload


/* Global variables */
void *current_avail = NULL;
int current_avail_size = 0;


/* Struct definitions */
typedef struct {
    size_t size;
    char allocated;
} block_header;

typedef struct {
    size_t size;
    int filler;
} block_footer;


/* Main Functions */
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void){
  current_avail = NULL;
  current_avail_size = 0;
  
  return 0;
}

/* 
 * mm_malloc - Allocate a block by using bytes from current_avail,
 *     grabbing a new page if necessary.
 */
void *mm_malloc(size_t size){
  int newsize = ALIGN(size);
  void *p;
  
  if (current_avail_size < newsize) {
    current_avail_size = PAGE_ALIGN(newsize);
    current_avail = mem_map(current_avail_size);
    if (current_avail == NULL)
      return NULL;
  }

  p = current_avail;
  current_avail += newsize;
  current_avail_size -= newsize;
  
  return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr){

}