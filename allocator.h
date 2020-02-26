#ifndef __BLOCK_ALLOCATOR_H
#define __BLOCK_ALLOCATOR_H

#include <stdint.h>

#define DEBUG_FREE 0
#define DEBUG_ALLOC 1

#define SIZE_OF_BLOCK 1
#define SIZE_OF_POOL 40

typedef enum {
    ALLOC_FREE_OK = 0,
    ALLOC_ERR_INPUT = 1,
    ALLOC_ERR_LACK_OF_SPACE = 2,
    ALLOC_ERR_BLOCK_EXISTS = 3,
    ALLOC_BUSY = 4,
    FREE_ERR_INPUT_ERROR = 5,
    FREE_ERROR_NOT_FOUND = 6,
    FREE_ERROR_UNKNOWN = 7,
    FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK = 8
} alloc_error_codes_t;

typedef enum {
    EMPTY_BLOCK = 0,
    SINGLE_BLOCK = 1,
    FIRST_BLOCK_IN_CHAIN = 2,
    TRAILING_BLOCK_IN_CHAIN = 3,
    UNDEF_BLOCK = 4
} block_types_t;

typedef struct {
    // there can be anything: structure, variables, arrays etc...
    unsigned char data [SIZE_OF_BLOCK];
} tBlock_t;

typedef struct {
    block_types_t blockType;
    int amountOfSiblingBlocks; /*The number of blocks in the piece into which this block enters*/
} tPoolStatus_t;

void init_allocator (void);
alloc_error_codes_t allocate_memory_for_blocks (tBlock_t **outBlock, uint32_t ammountOfBlocksToAlloc);
alloc_error_codes_t free_memory_from_pool (tBlock_t * inBlock);
char* to_name_str (alloc_error_codes_t errorCode);


void print_pull_status (void);

#endif /* __BLOCK_ALLOCATOR_H */
