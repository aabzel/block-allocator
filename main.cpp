// allocator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "allocator.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <time.h>


int test_case1() {
	printf("\n test_case 1\n");
	init_allocator();

	int errorCode;
	tBlock *newBlock2=NULL;

	errorCode = allocate_memory_for_blocks(&newBlock2, 2);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 1;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock2+1);
	if(FREE_ERROR_UNAEBLE_TO_DELETE_TRAILING_BLOCK!=errorCode) {
		return 2;
	}
	print_pull_status();
	
	return 0;
}

int test_case2() {
	printf("\n test_case 2\n");
	init_allocator();

	int errorCode;
	tBlock *newBlock1=NULL;
	tBlock *newBlock3=NULL;

	errorCode = allocate_memory_for_blocks(&newBlock1, 1);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 1;
	}
	print_pull_status();
    
	errorCode = allocate_memory_for_blocks(&newBlock3, 3);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 2;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock1);
	if(errorCode) {
		printf("Unable to release block %d\n", errorCode);
		return 3;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock3);
	if(errorCode) {
		printf("Unable to release block %d\n", errorCode);
		return 4;
	}
	print_pull_status();
	
	return 0;
}

int test_case3() {
	printf("\n test_case 3\n");
	init_allocator();

	int errorCode;
	tBlock *newBlock1=NULL;
	tBlock *newBlock3=NULL;
	tBlock *newBlock4=NULL;

	errorCode = allocate_memory_for_blocks(&newBlock1, 1);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 1;
	}
	print_pull_status();
    
	errorCode = allocate_memory_for_blocks(&newBlock3, 3);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 2;
	}
	print_pull_status();

	errorCode = allocate_memory_for_blocks(&newBlock4, 4);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 2;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock1);
	if(errorCode) {
		printf("Unable to release block %d\n", errorCode);
		return 3;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock4);
	if(errorCode) {
		printf("Unable to release block %d\n", errorCode);
		return 4;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock3);
	if(errorCode) {
		printf("Unable to release block %d\n", errorCode);
		return 4;
	}
	print_pull_status();
	
	return 0;
}

int test_case4() {
	printf("\n test_case 4\n");
	init_allocator();

	int errorCode;
	tBlock *newBlock1=NULL;

	errorCode = allocate_memory_for_blocks(&newBlock1, 1);
	if (errorCode ) {
		printf("Unable to allocate. Code: %d.\n", errorCode );
		return 1;
	}
	print_pull_status();

	errorCode = allocate_memory_for_blocks(&newBlock1, 6);
	if ( 2!=errorCode ) {
		return 2;
	}
	print_pull_status();

	errorCode = free_memory_from_pool(newBlock1);
	if(errorCode) {
		printf("Unable to release block %d\n", errorCode);
		return 3;
	}
	print_pull_status();
	
	return 0;
}

int test_case5() {
	printf("\n test_case 5\n");
	init_allocator();
	int errorCode;

	srand(time(0));

	for(int i=0; i<20; i++) {
		printf("\n");
		int sizeOfBlock;
		tBlock *newBlock1=NULL;

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock1, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();
		
		errorCode = free_memory_from_pool(newBlock1);
		if(errorCode) {
		    printf("Unable to delete block %d\n", errorCode);
		    return 1;
	    }
		print_pull_status();

	}
	return 0;
}


int test_case6() {
	printf("\n test_case 6\n");
	init_allocator();
	int errorCode;

	srand(time(0));

	for(int i=0; i<100; i++) {
		printf("\n");
		int sizeOfBlock;
		tBlock *newBlock1 = NULL;
		tBlock *newBlock2 = NULL;

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock1, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();
		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock2, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();
		
		errorCode = free_memory_from_pool(newBlock1);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();

		errorCode = free_memory_from_pool(newBlock2);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();
	}
	return 0;
}

int test_case7() {
	printf("\n test_case 7\n");
	init_allocator();
	int errorCode;

	srand(time(0));

	for(int i=0; i<100; i++) {
		printf("\n");
		int sizeOfBlock;
		tBlock *newBlock1 = NULL;
		tBlock *newBlock2 = NULL;
		tBlock *newBlock3 = NULL;

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock1, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock2, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock3, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();
		
		errorCode = free_memory_from_pool(newBlock1);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();

		errorCode = free_memory_from_pool(newBlock3);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();

		errorCode = free_memory_from_pool(newBlock2);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();
	}
	return 0;
}


int test_case8() {
	printf("\n test_case 8\n");
	init_allocator();
	int errorCode;

	srand(time(0));

	for(int i=0; i<100; i++) {
		printf("\n");
		int sizeOfBlock;
		tBlock *newBlock1 = NULL;
		tBlock *newBlock2 = NULL;
		tBlock *newBlock3 = NULL;

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock1, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock2, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();

		errorCode = free_memory_from_pool(newBlock1);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();

		sizeOfBlock =  1+rand()%8;
		errorCode = allocate_memory_for_blocks(&newBlock3, sizeOfBlock);
		switch(errorCode) {
          case ALLOC_OK: break;
		  case ALLOC_ERR_LACK_OF_SPACE: break;
		  default: printf("alloc err: %d", errorCode ); return 1;
	    }
		print_pull_status();
		

		errorCode = free_memory_from_pool(newBlock3);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();

		errorCode = free_memory_from_pool(newBlock2);
		switch(errorCode) {
		  case FREE_OK: break;
		  case FREE_ERR_INPUT_ERROR: break;
		  default: printf("delete err: %d", errorCode ); return 2;
	    }
		print_pull_status();
	}
	return 0;
}




int main()
{
	int errorCode;
	
	errorCode = test_case1();
	if(errorCode ) {
		printf("test_case1 %d failed", errorCode);
	}
	

	errorCode = test_case2();
	if (errorCode) {
		printf("test_case2 failed. Stage %d", errorCode);
	}

	errorCode = test_case3();
	if (errorCode) {
		printf("test_case3 failed. Stage %d", errorCode);
	}

	errorCode = test_case4();
	if (errorCode) {
		printf("test_case4 failed. Stage %d", errorCode);
	}
	
	
	errorCode = test_case8();
	if (errorCode) {
		printf("test_case8 failed. Stage %d", errorCode);
	}

	getchar();
	return 0;
}

