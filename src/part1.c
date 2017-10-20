#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>
#include "headers/util.h"

#define BUFF_SIZE 256*1024
#define BUFF_INCREMENT 32
#define EPOCHS 1000000

// change this if you are not finding your L1 cache size
#define CACHE_SIZE_TIMEOUT 30
#define LINE_SIZE_TIMEOUT 50

#define CACHE_H_START 8
#define CACHE_H_STOP 20

char buff[BUFF_SIZE] = "";
char cacheSizeLogFile[] = "cacheSize.log";

int getCacheSize() {
    int h,i,j,k, timeIndex = 0, *aveTimes = (int *) malloc(sizeof(int)*CACHE_H_STOP-CACHE_H_START);
    int prev_time = 0;
    int retval = -1;
    for (h=CACHE_H_START; h<CACHE_H_STOP; h++) {
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
        if (prev_time != 0 && abs(avg/10 - prev_time) > CACHE_SIZE_TIMEOUT) {
            printf("Cache size:\t%luB = %gKB\n",
                test_size/2*sizeof(int),
                pow(2,h-11)*sizeof(int));
            retval = test_size/2*sizeof(int);
            break;
        }
        prev_time = avg/10;
        printf("%lu:\t%d ms\n",
            test_size*sizeof(int), prev_time);
        aveTimes[timeIndex++] = prev_time;
    }
    
    char temp[BUFF_INCREMENT] = ""; 
    sprintf(temp, "%d", retval);
    logStrings(cacheSizeLogFile, 3, "\nCache size ", temp, " Timings:");
    logInts(cacheSizeLogFile, aveTimes, h-CACHE_H_START);
    printf("Logged cache size timings to %s\n", cacheSizeLogFile);
    return retval;
}

int getLineSize(int cacheSize) {
    int h,i,j,k;
    int prev_time = 0;
    for (h=0; h<8; h++) {
        int lineSize = pow(2,h);
        int avg = 0;
        for (i=0; i<10; i++) {
            int arr[cacheSize*2];
	    clock_t start = clock(), diff;
	    for (j=0; j<1000; j++) {
                for (k=0; k<cacheSize*2/lineSize; k++) {
                    arr[k*lineSize] = k;
                }
            }
            diff = clock() - start;
            avg += diff * 1000 / CLOCKS_PER_SEC;
        }
        if (prev_time != 0 && abs(avg*lineSize/10 - prev_time) > LINE_SIZE_TIMEOUT) {
            printf("Line size:\t%luB\n",lineSize*sizeof(int));
            return lineSize*sizeof(int);
        }
        printf("%lu:\t%d\n",lineSize*sizeof(int),avg*lineSize/10);
        prev_time = avg*lineSize/10;
    }
    return 0;
}

int getAssociativity(int cacheSize) {
    // TODO
    int h,i,j,k;
    int prev_time = 0;
    int temp;
    for (h=8; h>0; h/=2) {
        int avg = 0;
        for (i=0; i<10; i++) {
            int arr[cacheSize*2];
            clock_t start = clock(), diff;
            for (j=0; j<40000000; j++) {
                for (k=0; k<h+1; k++) {
                    arr[k*cacheSize/h] = k;
                }
            }
            diff = clock() - start;
            avg += diff * 1000 / CLOCKS_PER_SEC;
        }
        printf("%d:\t%d\n",h,avg/10/(h+1));
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int cacheSize, lineSize, associativity;
    //clock_t missTime = getMissTime();
    cacheSize = getCacheSize();
    printf("Cache size: %d\n", cacheSize);
    lineSize = getLineSize(cacheSize);
    printf("Line size: %d\n", lineSize);
    associativity = getAssociativity(cacheSize);
    printf("Associativity: %d\n", associativity);
    return 0;
}
