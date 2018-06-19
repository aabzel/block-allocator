//Author: Alexander Barunin 
//E-mail: aabzel@yandex.ru

#ifndef __block_allocator_H
#define __block_allocator_H

#define SIZE_OF_BLOCK 1
#define SIZE_OF_POOL 10

enum alloc_access {
	ALLOCATE_UNLOCK = 0,
	ALLOCATE_LOCK = 1
};

enum alloc_error_codes {
	ALLOC_OK = 0,
	ALLOC_ERR_INPUT = 1,
	ALLOC_ERR_LACK_OF_SPACE = 2,
	ALLOC_ERR_BLOCK_EXISTS = 3, 
	ALLOC_BUSY = 4 
};

enum free_error_codes { 
  FREE_OK = 0,
  FREE_ERR_INPUT_ERROR = 1,
  FREE_ERROR_NOT_FOUND = 2,
  FREE_ERROR_UNKNOWN = 3,
  FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK = 4
};

enum block_types {
  EMPTY_BLOCK = 0,
  SINGLE_BLOCK = 1, 
  FIRST_BLOCK_IN_CHAIN = 2,   
  TRAILING_BLOCK_IN_CHAIN = 3
};

struct tBlock {
	// there can be anything: structure, variables, arrays etc...
	unsigned char data[SIZE_OF_BLOCK];
};

struct tPoolStatus {
	int blockType; // 
	int amountOfSiblingBlocks; // The number of blocks in the piece into which this block enters
};

int init_allocator();
int allocate_memory_for_blocks(tBlock **outBlock, int ammountOfBlocksToAlloc);
int free_memory_from_pool(tBlock *inBlock) ;

void print_pull_status() ;

#endif /* __block_allocator_H */