#include "utest.h"

#include "allocator.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

alloc_error_codes_t test_case (void) {
    printf ("\n %s \n", __FUNCTION__);
    alloc_error_codes_t errorCode;

    init_allocator ();

    errorCode = test_case0 ();
    if (errorCode) {
        printf ("test_case0 [%s] failed", to_name_str (errorCode));
        return errorCode;
    }

    errorCode = test_case1 ();
    if (errorCode) {
        printf ("test_case1 [%s] failed", to_name_str (errorCode));
        return errorCode;
    }

    errorCode = test_case2 ();
    if (errorCode) {
        printf ("test_case2 failed. Error %s", to_name_str (errorCode));
        return errorCode;
    }

    errorCode = test_case3 ();
    if (errorCode) {
        printf ("test_case3 failed. Error %s", to_name_str (errorCode));
    }

    errorCode = test_case4 ();
    if (errorCode) {
        printf ("test_case4 failed. Error %s", to_name_str (errorCode));
    }

    errorCode = test_case5 ();
    if (errorCode) {
        printf ("test_case5 failed. Error %s", to_name_str (errorCode));
    }

    errorCode = test_case6 ();
    if (errorCode) {
        printf ("test_case6 failed. Error %s", to_name_str (errorCode));
    }

    errorCode = test_case7 ();
    if (errorCode) {
        printf ("test_case7 failed. Error %s", to_name_str (errorCode));
    }

    errorCode = test_case8 ();
    if (errorCode) {
        printf ("test_case8 failed. Error %s", to_name_str (errorCode));
    }

    return 0;
}

alloc_error_codes_t test_case0 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();

    alloc_error_codes_t errorCode;
    tBlock_t *newBlock1 = NULL;

    errorCode = allocate_memory_for_blocks (&newBlock1, 1);
    if (errorCode) {
        printf ("Unable to allocate. Code: [%s].\n", to_name_str (errorCode));
        return errorCode;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock1);
    if (errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return errorCode;
    }
    print_pull_status ();

    return 0;
}

alloc_error_codes_t test_case1 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();
    print_pull_status ();

    uint8_t errorCode;
    tBlock_t *newBlock2 = NULL;

    errorCode = allocate_memory_for_blocks (&newBlock2, 2);
    if (errorCode) {
        printf ("Unable to allocate. Code: [%s].\n", to_name_str (errorCode));
        return errorCode;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock2);
    if (ALLOC_FREE_OK != errorCode) {
        return errorCode;
    }
    print_pull_status ();

    return 0;
}

alloc_error_codes_t test_case2 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();

    alloc_error_codes_t errorCode;
    tBlock_t *newBlock1 = NULL;
    tBlock_t *newBlock3 = NULL;

    errorCode = allocate_memory_for_blocks (&newBlock1, 1);
    if (errorCode) {
        printf ("Unable to allocate. Code: %s.\n", to_name_str (errorCode));
        return 1;
    }
    print_pull_status ();

    errorCode = allocate_memory_for_blocks (&newBlock3, 3);
    if (errorCode) {
        printf ("Unable to allocate. Code: %s.\n", to_name_str (errorCode));
        return 2;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock1);
    if (ALLOC_FREE_OK != errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return 3;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock3);
    if (errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return 4;
    }
    print_pull_status ();

    return 0;
}

alloc_error_codes_t test_case3 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();

    alloc_error_codes_t errorCode;
    tBlock_t *newBlock1 = NULL;
    tBlock_t *newBlock3 = NULL;
    tBlock_t *newBlock4 = NULL;

    errorCode = allocate_memory_for_blocks (&newBlock1, 1);
    if (errorCode) {
        printf ("Unable to allocate. Code: %s.\n", to_name_str (errorCode));
        return 1;
    }
    print_pull_status ();

    errorCode = allocate_memory_for_blocks (&newBlock3, 3);
    if (errorCode) {
        printf ("Unable to allocate. Code: %s.\n", to_name_str (errorCode));
        return 2;
    }
    print_pull_status ();

    errorCode = allocate_memory_for_blocks (&newBlock4, 4);
    if (errorCode) {
        printf ("Unable to allocate. Code: %s.\n", to_name_str (errorCode));
        return 2;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock1);
    if (errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return 3;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock4);
    if (errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return 4;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock3);
    if (errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return 4;
    }
    print_pull_status ();

    return 0;
}

alloc_error_codes_t test_case4 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();

    alloc_error_codes_t errorCode;
    tBlock_t *newBlock1 = NULL;

    errorCode = allocate_memory_for_blocks (&newBlock1, 1);
    if (errorCode) {
        printf ("Unable to allocate. Code: %s.\n", to_name_str (errorCode));
        return 1;
    }
    print_pull_status ();

    errorCode = allocate_memory_for_blocks (&newBlock1, 6);
    if (ALLOC_ERR_BLOCK_EXISTS != errorCode) {
        return ALLOC_ERR_BLOCK_EXISTS;
    }
    print_pull_status ();

    errorCode = free_memory_from_pool (newBlock1);
    if (errorCode) {
        printf ("Unable to release block %s\n", to_name_str (errorCode));
        return 3;
    }
    print_pull_status ();

    return 0;
}

alloc_error_codes_t test_case5 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();
    alloc_error_codes_t errorCode;

    srand (time (0));

    for (int i = 0; i < 20; i++) {
        printf ("\n");
        int sizeOfBlock;
        tBlock_t *newBlock1 = NULL;
        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock1, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                errorCode = free_memory_from_pool (newBlock1);
                if (errorCode) {
                    printf ("Unable to delete block %s\n", to_name_str (errorCode));
                    return errorCode;
                }
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return errorCode;
        }

    }
    return 0;
}

alloc_error_codes_t test_case6 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();
    alloc_error_codes_t errorCode;

    srand (time (0));

    for (int i = 0; i < 100; i++) {
        printf ("\n");
        int sizeOfBlock;
        tBlock_t *newBlock1 = NULL;
        tBlock_t *newBlock2 = NULL;

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock1, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock2, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return errorCode;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock1);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return errorCode;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock2);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return errorCode;
        }
        print_pull_status ();
    }
    return 0;
}

alloc_error_codes_t test_case7 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();
    alloc_error_codes_t errorCode;

    srand (time (0));

    for (int i = 0; i < 100; i++) {
        printf ("\n");
        int sizeOfBlock;
        tBlock_t *newBlock1 = NULL;
        tBlock_t *newBlock2 = NULL;
        tBlock_t *newBlock3 = NULL;

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock1, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock2, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock3, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock1);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return 2;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock3);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return 2;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock2);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return 2;
        }
        print_pull_status ();
    }
    return 0;
}

alloc_error_codes_t test_case8 (void) {
    printf ("\n %s \n", __FUNCTION__);
    init_allocator ();
    alloc_error_codes_t errorCode;

    srand (time (0));

    for (int i = 0; i < 100; i++) {
        printf ("\n");
        int sizeOfBlock;
        tBlock_t *newBlock1 = NULL;
        tBlock_t *newBlock2 = NULL;
        tBlock_t *newBlock3 = NULL;

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock1, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock2, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock1);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return 2;
        }
        print_pull_status ();

        sizeOfBlock = 1 + rand () % 8;
        errorCode = allocate_memory_for_blocks (&newBlock3, sizeOfBlock);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case ALLOC_ERR_LACK_OF_SPACE:
                break;
            default:
                printf ("alloc err: %s", to_name_str (errorCode));
                return 1;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock3);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return 2;
        }
        print_pull_status ();

        errorCode = free_memory_from_pool (newBlock2);
        switch (errorCode) {
            case ALLOC_FREE_OK:
                break;
            case FREE_ERR_INPUT_ERROR:
                break;
            default:
                printf ("delete err: %s", to_name_str (errorCode));
                return 2;
        }
        print_pull_status ();
    }
    return 0;
}
