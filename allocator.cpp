//Author: Alexander Barunin 
//E-mail: aabzel@yandex.ru

//This alocator does not perform defragmentation of pool.

#include "stdafx.h"
#include "allocator.h"
#include <stdio.h>
#include <string.h>

//This is a raw pool. The main memory of the system
static tBlock sBlock[SIZE_OF_POOL];
//This is an array of status of each block in pool 
static tPoolStatus sPool[SIZE_OF_POOL];

int init_allocator() {
	for(int i=0; i<SIZE_OF_POOL; i++) {
		sPool[i].blockType = EMPTY_BLOCK;
		sPool[i].amountOfSiblingBlocks=0;
		memset(&sBlock[i], 0x00, sizeof(sBlock[i]));
	}
	return ALLOC_OK;
}

static void release_blocks(int firstBlockForSetBlocks, int ammountOfBlocksToRelease) {
	for(int i=firstBlockForSetBlocks; i<(firstBlockForSetBlocks+ammountOfBlocksToRelease); i++) {
		sPool[i].amountOfSiblingBlocks = 0;
		sPool[i].blockType = EMPTY_BLOCK;
		memset(&sBlock[i], 0x00, sizeof(sBlock[i]));
	}
}

static void occupy_blocks(int firstBlockForSetBlocks, int ammountOfBlocksToAlloc) {
	for(int k=firstBlockForSetBlocks; k<(firstBlockForSetBlocks+ammountOfBlocksToAlloc); k++) {
		sPool[k].amountOfSiblingBlocks = ammountOfBlocksToAlloc; 
		sPool[k].blockType = TRAILING_BLOCK_IN_CHAIN;
	}
	if (1==ammountOfBlocksToAlloc) {
		sPool[firstBlockForSetBlocks].blockType = SINGLE_BLOCK;
	} else if (1<ammountOfBlocksToAlloc) {
		sPool[firstBlockForSetBlocks].blockType = FIRST_BLOCK_IN_CHAIN;
	}
}

static int is_block_exist(tBlock *inblock, int *outBlockIndex) {
	*outBlockIndex =-1;
	for (int i=0; i < SIZE_OF_POOL; i++) {
		if (inblock==&sBlock[i]) {
			*outBlockIndex = i;
			return 1;
		}
	}
	return 0;
}

int allocate_memory_for_blocks(tBlock **outBlock, int ammountOfBlocksToAlloc) {


	if (!ammountOfBlocksToAlloc) {
		return ALLOC_ERR_INPUT;
	}
	// Check up whether or not such a block in a pool.
	int outBlockIndex;
	if ( is_block_exist(*outBlock, &outBlockIndex) ) {
		//such a block already exists in the pool
		return ALLOC_ERR_BLOCK_EXISTS;
	}
	int firstBlockForSetBlocks = -1;
	int continuousBlockCounter = 0;
	int prevBlockBusy = 1;
	for (int i=0; i<SIZE_OF_POOL; i++) {
		if (sPool[i].amountOfSiblingBlocks==0) {
			// Did previous block busy?
			if (prevBlockBusy) {
				firstBlockForSetBlocks=i;
				prevBlockBusy=0;
			}
			continuousBlockCounter++;
			if (continuousBlockCounter == ammountOfBlocksToAlloc) {
				//Enough chunk of space for the chain of blocks has been found.
				occupy_blocks(firstBlockForSetBlocks, ammountOfBlocksToAlloc);
				*outBlock = &sBlock[firstBlockForSetBlocks];

				return ALLOC_OK;
			}
		} else if ( 0 < sPool[i].amountOfSiblingBlocks ) {
			firstBlockForSetBlocks = -1;
			continuousBlockCounter = 0;
			prevBlockBusy=1;
		}
	}
	outBlock = NULL;

	return ALLOC_ERR_LACK_OF_SPACE;
}

int free_memory_from_pool(tBlock *inBlock) {
	if (NULL==inBlock){
		return FREE_ERR_INPUT_ERROR;
	}
	// run through the entire pool and find the block with the address as inblock
	int	desiredBlockIndex=-1;
	if (!is_block_exist(inBlock, &desiredBlockIndex)) {
	    return FREE_ERROR_NOT_FOUND;
	} 

	if ((SINGLE_BLOCK==sPool[desiredBlockIndex].blockType) || 
       (FIRST_BLOCK_IN_CHAIN==sPool[desiredBlockIndex].blockType) ) {
        release_blocks(desiredBlockIndex, sPool[desiredBlockIndex].amountOfSiblingBlocks);
		inBlock = NULL;
	    return FREE_OK;
	} else {
		return FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK;
	}
	return FREE_ERROR_UNKNOWN;
}

void print_pull_status() {
	for(int q=0; q<SIZE_OF_POOL; q++) {
		if (sPool[q].amountOfSiblingBlocks) {
			printf("|%d",sPool[q].amountOfSiblingBlocks);
		}else{
			printf("|_" );
		}
	}
	printf("\n");
}
