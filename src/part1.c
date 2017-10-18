#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>
#include "headers/util.h"

#define BUFF_SIZE 256*1024
#define BUFF_INCREMENT 32
#define EPOCHS 1000000

// change this if you are not finding your L1 cache size
#define L1_TIMEOUT 30

char buff[BUFF_SIZE] = "";

clock_t getMissTime() {
    int i, j;
    clock_t start, end;
    char temp;
    start = clock();
    for(i = 0; i < EPOCHS; i++) {
        for(j = 0; j < BUFF_SIZE; j += BUFF_INCREMENT) {
            temp = buff[j];
        }
    }
    end = clock();
    return (end-start)/EPOCHS;
}

int getCacheSize() {
    int h,i,j,k;
    int prev_time = 0;
    for (h=8; h<20; h++) {
        int test_size = pow(2,h);
	int avg = 0;
	for (i=0; i<10; i++) {
            clock_t start = clock(), diff;
	    int arr[test_size];
	    for (j=0; j<pow(2,28)/test_size; j++) {
                for (k=0; k<test_size; k+=16) {
                    arr[k] = k;
		}
	    }
	    diff = clock() - start;
	    avg += diff * 1000 / CLOCKS_PER_SEC;
	}
	if (prev_time != 0 && abs(avg/10 - prev_time) > L1_TIMEOUT) {
            printf("Cache size:\t%luB = %gKB\n",
                test_size/2*sizeof(int),
		pow(2,h-11)*sizeof(int));
	    return test_size/2*sizeof(int);
	}
	printf("%lu:\t%d ms\n",
            test_size*sizeof(int), avg/10);
	prev_time = avg/10;
    }
    return -1;
}

int getLineSize() {
    // TODO
    return 0;
}

int getAssociativity() {
    // TODO
    return 0;
}

int main(int argc, char *argv[]) {
    int cacheSize, lineSize, associativity;
    //clock_t missTime = getMissTime();
    cacheSize = getCacheSize();
    printf("Cache size: %d\n", cacheSize);
    lineSize = getLineSize();
    printf("Line size: %d\n", lineSize);
    associativity = getAssociativity();
    printf("Associativity: %d\n", associativity);
}
