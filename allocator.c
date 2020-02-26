/*This allocator does not perform defragmentation of pool.*/

#include "allocator.h"

#include "low_level.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*This is a raw pool. The main memory of the system*/
static tBlock_t sBlock [SIZE_OF_POOL];
/*This is an array of status of each block in pool */
static tPoolStatus_t sPool [SIZE_OF_POOL];

static void get_suitable_continuously_spare_block (uint32_t * const outIndexOfFirstBlock, uint32_t demandedSize);
static void get_list_of_spare_regions (void);
static uint32_t get_amount_of_spare_regions (void);
static uint32_t count_amount_of_spare_blocks (void);
static uint32_t count_amount_of_continuously_spare_blocks (uint32_t * const outIndexOfFirstMaxBlock);
static uint8_t is_block_exist (tBlock_t * const inblock, int32_t * const outBlockIndex);
static void release_blocks (uint32_t firstBlockForSetBlocks, uint32_t ammountOfBlocksToRelease);
static void occupy_blocks (uint32_t firstBlockForSetBlocks, uint32_t ammountOfBlocksToAlloc);
static void print_block_address (void);

void init_allocator (void) {
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        sPool [i].blockType = EMPTY_BLOCK;
        sPool [i].amountOfSiblingBlocks = 0;
        memset (&sBlock [i], 0x00, sizeof(sBlock [i]));
    }
#if 0
    uint32_t firstBlock = 0;
    uint32_t numOfBlock = count_amount_of_continuously_spare_blocks (&firstBlock);
    printf ("\n numOfBlock %d firstBlock %d\n ", numOfBlock, firstBlock);
#endif
}

alloc_error_codes_t allocate_memory_for_blocks (tBlock_t **outBlock, uint32_t ammountOfBlocksToAlloc) {
    disable_interrupts ();

#if DEBUG_ALLOC
    printf ("\n SizeOfBlockToAlloc: %u", ammountOfBlocksToAlloc);
#endif
    uint8_t errorCode = ALLOC_FREE_OK;
    if (0 == ammountOfBlocksToAlloc) {
        errorCode = ALLOC_ERR_INPUT;
    }

    if (ALLOC_FREE_OK == errorCode) {
        uint32_t numOfContBlocks = 0;
        uint32_t indexOfFirsrMaxBlock = 0;
        numOfContBlocks = count_amount_of_continuously_spare_blocks (&indexOfFirsrMaxBlock);
        if (ammountOfBlocksToAlloc < numOfContBlocks) {

            uint32_t indexOfFirsrBlock = 0xFFFFFFFF;
            get_suitable_continuously_spare_block (&indexOfFirsrBlock, ammountOfBlocksToAlloc);
            if (indexOfFirsrBlock < SIZE_OF_POOL) {
                /*Enough chunk of space for the chain of blocks has been found.*/
                occupy_blocks (indexOfFirsrBlock, ammountOfBlocksToAlloc);
                *outBlock = &sBlock [indexOfFirsrBlock];
                errorCode = ALLOC_FREE_OK;
            } else {
                errorCode = ALLOC_ERR_LACK_OF_SPACE;
            }

        } else {
            errorCode = ALLOC_ERR_LACK_OF_SPACE;
        }
    }

    enable_interrupts ();
    return errorCode;
}

alloc_error_codes_t free_memory_from_pool (tBlock_t * inBlock) {
    disable_interrupts ();
#if DEBUG_FREE
    printf ("\nAddress block to del %p\n", inBlock);
#endif /*DEBUG_FREE*/
    int desiredBlockIndex = -1;
    alloc_error_codes_t errCode = ALLOC_FREE_OK;
    if (NULL == inBlock) {
        errCode = FREE_ERR_INPUT_ERROR;
    }

    if (ALLOC_FREE_OK == errCode) {
        /* run through the entire pool and find the block with the address as inblock*/
        if (0 == is_block_exist (inBlock, &desiredBlockIndex)) {
            errCode = FREE_ERROR_NOT_FOUND;
        }
        printf ("\ndesiredBlockIndex to del %d\n", desiredBlockIndex);
    }

    if ((ALLOC_FREE_OK == errCode) && ((0 <= desiredBlockIndex) && (desiredBlockIndex < SIZE_OF_POOL))) {
        if ((SINGLE_BLOCK == sPool [desiredBlockIndex].blockType) || (FIRST_BLOCK_IN_CHAIN == sPool [desiredBlockIndex].blockType)) {
            release_blocks (desiredBlockIndex, sPool [desiredBlockIndex].amountOfSiblingBlocks);
            errCode = ALLOC_FREE_OK;
        } else {
            errCode = FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK;
        }
    }
    enable_interrupts ();
    return errCode;
}

char* to_name_str (alloc_error_codes_t errorCode) {
    static char *nameStr = "FINE";
    switch (errorCode) {
        case FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK:
            nameStr = "FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK";
            break;
        case FREE_ERROR_UNKNOWN:
            nameStr = "FREE_ERROR_UNKNOWN";
            break;
        case FREE_ERROR_NOT_FOUND:
            nameStr = "FREE_ERROR_NOT_FOUND";
            break;
        case FREE_ERR_INPUT_ERROR:
            nameStr = "FREE_ERR_INPUT_ERROR";
            break;
        case ALLOC_BUSY:
            nameStr = "ALLOC_BUSY";
            break;
        case ALLOC_ERR_BLOCK_EXISTS:
            nameStr = "ALLOC_ERR_BLOCK_EXISTS";
            break;
        case ALLOC_ERR_LACK_OF_SPACE:
            nameStr = "ALLOC_ERR_LACK_OF_SPACE";
            break;
        case ALLOC_ERR_INPUT:
            nameStr = "ALLOC_ERR_INPUT";
            break;
        case ALLOC_FREE_OK:
            nameStr = "ALLOC_FREE_OK";
            break;
        default:
            nameStr = "UNKNOWN_ERROR";
            break;
    }
    return nameStr;
}

void print_pull_status (void) {
    printf ("\n");
    for (int q = 0; q < SIZE_OF_POOL; q++) {
        if (EMPTY_BLOCK != sPool [q].blockType) {
            printf ("|%d", sPool [q].amountOfSiblingBlocks);
        } else {
            printf ("|_");
        }
    }
    printf ("|");
    /*print_block_address ();*/
    uint32_t numOfSrateReg = get_amount_of_spare_regions ();
    uint32_t outIndexOfFirstMaxBlock = 0;
    uint32_t numSpareBlock = count_amount_of_spare_blocks ();
    uint32_t numContSpareBlock = count_amount_of_continuously_spare_blocks (&outIndexOfFirstMaxBlock);
    printf (
        "\nnumSpareBlock %u numContSpareBlock %u IndexOfFirstMaxBlock %u numOfSrateReg %u",
        numSpareBlock,
        numContSpareBlock,
        outIndexOfFirstMaxBlock,
        numOfSrateReg);
    get_list_of_spare_regions ();
}

static uint32_t count_amount_of_spare_blocks (void) {
    uint32_t amountOfspareBlocks = 0;
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        if (EMPTY_BLOCK == sPool [i].blockType) {
            amountOfspareBlocks++;
        }
    }
    return amountOfspareBlocks;
}

static void get_suitable_continuously_spare_block (uint32_t * const outIndexOfFirstBlock, uint32_t demandedSize) {
    uint32_t amountOfContinuouslySpareBlocks = 0;
    uint32_t indexOfFirstBlock = 0;

    block_types_t prevBlockType = UNDEF_BLOCK;
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        if (EMPTY_BLOCK == sPool [i].blockType) {
            if (EMPTY_BLOCK == prevBlockType) {
                amountOfContinuouslySpareBlocks++;
            } else {
                indexOfFirstBlock = i;
                amountOfContinuouslySpareBlocks = 1;
            }
            if ((SIZE_OF_POOL - 1) == i) {
                if (demandedSize <= amountOfContinuouslySpareBlocks) {
                    *outIndexOfFirstBlock = indexOfFirstBlock;
                    return;
                }
            }
            prevBlockType = EMPTY_BLOCK;
        } else {
            if (EMPTY_BLOCK == prevBlockType) {
                if (demandedSize <= amountOfContinuouslySpareBlocks) {
                    *outIndexOfFirstBlock = indexOfFirstBlock;
                    return;
                }
            }
            prevBlockType = sPool [i].blockType;
        }
    }
}

static uint32_t count_amount_of_continuously_spare_blocks (uint32_t * const outIndexOfFirstMaxBlock) {
    uint32_t amountOfContinuouslySpareBlocks = 0;
    uint32_t maxAmountOfContinuouslySpareBlocks = 0;
    uint32_t indexOfFirstBlock = 0;

    block_types_t prevBlockType = UNDEF_BLOCK;
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        if (EMPTY_BLOCK == sPool [i].blockType) {
            if (EMPTY_BLOCK == prevBlockType) {
                amountOfContinuouslySpareBlocks++;
            } else {
                indexOfFirstBlock = i;
                amountOfContinuouslySpareBlocks = 1;
            }
            if ((SIZE_OF_POOL - 1) == i) {
                if (maxAmountOfContinuouslySpareBlocks < amountOfContinuouslySpareBlocks) {
                    maxAmountOfContinuouslySpareBlocks = amountOfContinuouslySpareBlocks;
                    *outIndexOfFirstMaxBlock = indexOfFirstBlock;
                }
            }
            prevBlockType = EMPTY_BLOCK;
        } else {
            if (EMPTY_BLOCK == prevBlockType) {
                if (maxAmountOfContinuouslySpareBlocks < amountOfContinuouslySpareBlocks) {
                    maxAmountOfContinuouslySpareBlocks = amountOfContinuouslySpareBlocks;
                    *outIndexOfFirstMaxBlock = indexOfFirstBlock;
                }
            }
            prevBlockType = sPool [i].blockType;
        }
    }
    return maxAmountOfContinuouslySpareBlocks;
}

static uint8_t is_block_exist (tBlock_t * const inblock, int32_t * const outBlockIndex) {
    disable_interrupts ();
    *outBlockIndex = -1;
    uint8_t res = 0;
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        if (inblock == &sBlock [i]) {
            *outBlockIndex = i;
            res = 1;
            break;
        }
    }
    enable_interrupts ();
    return res;
}

static void release_blocks (uint32_t firstBlockForSetBlocks, uint32_t ammountOfBlocksToRelease) {
#if DEBUG_FREE
    printf ("\n%s first block: [%d] amount: [%d]  ", __FUNCTION__, firstBlockForSetBlocks, ammountOfBlocksToRelease);
#endif /*DEBUG_FREE*/

    for (int i = firstBlockForSetBlocks; i < (firstBlockForSetBlocks + ammountOfBlocksToRelease); i++) {
        sPool [i].amountOfSiblingBlocks = 0;
        sPool [i].blockType = EMPTY_BLOCK;
        memset (&sBlock [i], 0x00, sizeof(sBlock [i]));
    }
}

static void occupy_blocks (uint32_t firstBlockForSetBlocks, uint32_t ammountOfBlocksToAlloc) {
    for (int k = firstBlockForSetBlocks; k < (firstBlockForSetBlocks + ammountOfBlocksToAlloc); k++) {
        sPool [k].amountOfSiblingBlocks = ammountOfBlocksToAlloc;
        sPool [k].blockType = TRAILING_BLOCK_IN_CHAIN;
    }
    if (1 == ammountOfBlocksToAlloc) {
        sPool [firstBlockForSetBlocks].blockType = SINGLE_BLOCK;
    } else if (1 < ammountOfBlocksToAlloc) {
        sPool [firstBlockForSetBlocks].blockType = FIRST_BLOCK_IN_CHAIN;
    }
}

static uint32_t get_amount_of_spare_regions (void) {
    uint32_t amountOfContinuouslySpareRegions = 0;
    block_types_t prevBlockType = UNDEF_BLOCK;
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        if (EMPTY_BLOCK == sPool [i].blockType) {
            if (EMPTY_BLOCK != prevBlockType) {
                amountOfContinuouslySpareRegions++;
            }
            prevBlockType = EMPTY_BLOCK;
        } else {
            prevBlockType = sPool [i].blockType;
        }
    }
    return amountOfContinuouslySpareRegions;
}

/*For debug*/
static void get_list_of_spare_regions (void) {
    uint32_t amountOfContinuouslySpareBlocks = 0;
    printf ("\n [");
    block_types_t prevBlockType = UNDEF_BLOCK;
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        if (EMPTY_BLOCK == sPool [i].blockType) {
            if (EMPTY_BLOCK == prevBlockType) {
                amountOfContinuouslySpareBlocks++;
            } else {
                amountOfContinuouslySpareBlocks = 1;
            }
            if ((SIZE_OF_POOL - 1) == i) {
                printf (" %u ", amountOfContinuouslySpareBlocks);

            }
            prevBlockType = EMPTY_BLOCK;
        } else {
            if (EMPTY_BLOCK == prevBlockType) {
                printf (" %u", amountOfContinuouslySpareBlocks);
            }
            prevBlockType = sPool [i].blockType;
        }
    }
    printf ("] ");
}

/*For debug*/
static void print_block_address (void) {
    for (int i = 0; i < SIZE_OF_POOL; i++) {
        printf ("\naddress block[%d]:%p", i, &sBlock [i]);
    }
}
