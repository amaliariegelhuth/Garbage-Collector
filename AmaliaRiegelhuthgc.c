#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memorymgr.h"

void gc(); // perform garbage collection on the heap
void mark(void *p);
void sweep();
void *stackBottom();

void gc() {
	int *i;
	int **ptr = &i;
	int **bottom = (int**)stackBottom();
	while(*ptr != *bottom) {
		mark(*ptr);
		ptr = ptr + 1;
	}
	sweep();
	return;
}

void mark(void *p) {
	int *ptr = (int*)p;
	int *first = firstBlock();
	int *last = lastBlock();
	if ((ptr < first) || (ptr > last)) {
		return;
	}
	int *b = nextBlock(ptr);
	if((*(b-1)% 2) == 1) {
		*(b-1) = *(b-1) + 2;
		int *ptr2;
		int length = *(b-1)/8 * 8;
		int i;
		for (i = 0; i < length/4 - 1; i++) {
			ptr2 = b + i;
			if((*ptr2 >= *first) && (*ptr2 <= *last)) {
			printf("hi");
			mark(ptr2);
			}
		}
	}

	return;
}

void sweep() {
	int *ptr = firstBlock();
	while(ptr != lastBlock()) {
		if (((*ptr) % 4) == 1) {
			*ptr = *ptr - 1;
		}		
		ptr = nextBlock(ptr);
	}
	coalesce();
	return;
}

void *stackBottom() {
	unsigned long bottom;
    FILE *statfp = fopen("/proc/self/stat", "r");
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*u %*u %*u %*u %*u %*u %*d %*d "
           "%*d %*d %*d %*d %*u %*u %*d "
           "%*u %*u %*u %lu", &bottom);
    fclose(statfp);
	return (void *) bottom;
}















